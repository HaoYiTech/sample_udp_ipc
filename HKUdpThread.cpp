/******************************************************
 * 浩一科技，提供云录播、云监控的全平台无插件解决方案。
 * 支持按需直播，多点布控，分布式海量存储，动态扩容；
 * 支持微信扫码登录，全平台帐号统一，关联微信小程序；
 * 支持多种数据输入：摄像头IPC、rtmp、rtsp、MP4文件；
 * 支持全实时、全动态、全网页管理，网页前后台兼容IE8；
 * 支持多终端无插件自适应播放，flvjs/hls/rtmp自动适配；
 ******************************************************
 * 官方网站 => https://myhaoyi.com
 * 技术博客 => http://blog.csdn.net/haoyitech
 * 开源代码 => https://github.com/HaoYiTech/
 * 云监控群 => 483663026（QQ群）
 * 云录播群 => 630379661（QQ群）
 ******************************************************
 * CHKUdpThread => 海康(IPC)网络摄像机自动探测类。
 * 注意：本类从线程基础类中继承，并定义了探测参数。
 ******************************************************/

#include "stdafx.h"
#include "HKUdpThread.h"
#include "SocketUtils.h"
#include "UtilTool.h"
#include "tinyxml.h"

CHKUdpThread::CHKUdpThread()
{
}

CHKUdpThread::~CHKUdpThread()
{
	// 先关闭UDPSocket对象，会快速退出...
	m_UDPSocket.Close();
	// 停止线程，等待退出...
	this->StopAndWaitForThread();
}

GM_Error CHKUdpThread::InitMulticast()
{
	// 建立UDP,接收组播...
	GM_Error theErr = GM_NoErr;
	theErr = m_UDPSocket.Open();
	if( theErr != GM_NoErr ) {
		MsgLogGM(theErr);
		return theErr;
	}
	// 设置重复使用端口...
	m_UDPSocket.ReuseAddr();
	// 必须直接绑定组播端口...
	theErr = m_UDPSocket.Bind(INADDR_ANY, DEF_MCAST_PORT);
	if( theErr != GM_NoErr ) {
		MsgLogGM(theErr);
	}
	// 创建事件对象...
	theErr = m_UDPSocket.CreateEvent();
	if( theErr != GM_NoErr ) {
		MsgLogGM(theErr);
		return theErr;
	}
	// 加入组播组，接收数据...
	theErr = m_UDPSocket.JoinMulticastForRecv(inet_addr(DEF_MCAST_ADDRV4), INADDR_ANY);
	if( theErr != GM_NoErr ) {
		MsgLogGM(theErr);
		return theErr;
	}
	// 加入组播组，使用第一IP地址发送数据包...
	ASSERT(SocketUtils::GetNumIPAddrs() >= 1 );
	theErr = m_UDPSocket.JoinMulticastForSend(inet_addr(DEF_MCAST_ADDRV4), htonl(SocketUtils::GetIPAddr(0)));
	if( theErr != GM_NoErr ) {
		MsgLogGM(theErr);
		return theErr;
	}
	// 设置TTL和组播地址，简化SendTo参数......
	m_UDPSocket.SetTtl(32);
	m_UDPSocket.SetRemoteAddr(DEF_MCAST_ADDRV4, DEF_MCAST_PORT);
	// 启动组播接收线程...
	this->Start();
	return theErr;
}

void CHKUdpThread::Entry()
{
	// 首先，发送一个查询数据包...
	GM_Error theErr = GM_NoErr;
	theErr = this->SendCmdQuiry();

	// 开始线程循环过程...
	DWORD dwStartTimeMS = ::GetTickCount();
	while( !this->IsStopRequested() ) {
		// 等待并处理组播事件,没有错误,继续处理...
		theErr = this->WaitAndProcMulticast();
		if( theErr != GM_NoErr ) {
			MsgLogGM(theErr);
			return;
		}
		// 每隔 XX 秒发送一次查询数据包指令...
		if( (::GetTickCount() - dwStartTimeMS) >= (DEF_MCAST_STEP * 1000) ) {
			dwStartTimeMS = ::GetTickCount();
			theErr = this->SendCmdQuiry();
		}
	}
}
//
// 发送查询指令 - 组播命令...
GM_Error CHKUdpThread::SendCmdQuiry()
{
	GM_Error theErr = GM_NoErr;
	GUID     theGuid = {0};
	string   strGuid;
	// 准备摄像机查询命令需要的数据...
	HRESULT  hr = ::CoCreateGuid(&theGuid);
	ASSERT( theGuid != GUID_NULL );
	strGuid = CUtilTool::GUIDToStr(theGuid);
	// 组合字符串，直接发送xml格式的命令...
	TCHAR szCmd[MAX_PATH] = {0};
	sprintf(szCmd, "%s\r\n<Probe><Uuid>%s</Uuid><Types>inquiry</Types></Probe>", XML_DECLARE_UTF8, strGuid.c_str());
	theErr = m_UDPSocket.SendTo(szCmd, strlen(szCmd));
	// 发送数据包失败的处理...
	if( theErr != GM_NoErr ) {
		MsgLogGM(theErr);
		return theErr;
	}
	return GM_NoErr;
}
//
// 等待并处理组播数据...
GM_Error CHKUdpThread::WaitAndProcMulticast()
{
	DWORD		dwIndex = 0;
	GM_Error	theErr = GM_NoErr;
	WSAEVENT	theEvent = m_UDPSocket.GetWSAEvent();
	SOCKET		theSocket = m_UDPSocket.GetSocket();
	WSANETWORKEVENTS theNetwork = {0};
	ASSERT( theEvent != NULL && theSocket != INVALID_SOCKET );
	// 等待网络事件到达...
	dwIndex = WaitForSingleObject(theEvent, DEF_MCAST_TIME);
	if( this->IsStopRequested() ) {
		theErr = GM_Thread_Close;
		MsgLogGM(theErr);
		return theErr;
	}
	// 如果发生超时错误,直接返回正确...
	if( dwIndex == WSA_WAIT_FAILED || dwIndex == WSA_WAIT_TIMEOUT || dwIndex == WAIT_IO_COMPLETION ) {
		//theErr = GM_Err_Timeout;
		//MsgLogGM(theErr);
		return GM_NoErr;
	}
	// 是有效的通知，进一步查询网络事件，对错误信息进行必要加工处理...
	ASSERT( dwIndex == WSA_WAIT_EVENT_0 );
	if( SOCKET_ERROR == WSAEnumNetworkEvents(theSocket, theEvent, &theNetwork) ) {
		theErr = WSAGetLastError();
		theErr = (theErr == GM_NoErr) ? GM_NetClose : theErr;
		MsgLogGM(theErr);
		return theErr;
	}
	// 没有网络事件，空转一次...
	if( theNetwork.lNetworkEvents == 0 )
		return GM_NoErr;
	// 对网络事件进行移位操作，目的得到错误号...
	int nEventBit = 0;
	int nEventTemp = theNetwork.lNetworkEvents >> 1;
	while( nEventTemp > 0 ) {
		++nEventBit;
		nEventTemp >>= 1;
	}
	ASSERT( nEventBit >= 0 );
	// 网络层发生了FD_CLOSE事件...
	if( theNetwork.lNetworkEvents & FD_CLOSE ) {
		theErr = GM_NetClose;
		MsgLogGM(theErr);
		return theErr;
	}
	// 网络事件有错误信息...
	if( theNetwork.iErrorCode[nEventBit] > 0 ) {
		theErr = theNetwork.iErrorCode[nEventBit];
		MsgLogGM(theErr);
		return theErr;
	}
	// 执行没有错误的网络事件...
	ASSERT( theErr == GM_NoErr );
	return this->ProcessEvent(theNetwork.lNetworkEvents);
}
//
// 处理网络事件...
GM_Error CHKUdpThread::ProcessEvent(int eventBits)
{
	GM_Error theErr = GM_NoErr;
	// 处理读取事件通知...
	if( eventBits & FD_READ ) {
		theErr = this->ForRead();
		if( theErr != GM_NoErr ) {
			MsgLogGM(theErr);
			return theErr;
		}
		eventBits &= ~ FD_READ;
	}
	// 不肯能再有其它事件通知...
	ASSERT( eventBits == 0 );
	return GM_NoErr;
}
//
// 处理读取事件通知...
GM_Error CHKUdpThread::ForRead()
{
	UInt32 uRomAddr = 0;
	UInt16 uRomPort = 0;
	UInt32 uLenReturn = 0;	
	char   szBuffer[2048] = {0};
	// 获取网络组播数据内容...
	ASSERT(SocketUtils::GetNumIPAddrs() >= 1 );
	UInt32 iRemoteAddr = SocketUtils::GetIPAddr(0);
	GM_Error theErr = m_UDPSocket.RecvFrom(&uRomAddr, &uRomPort, szBuffer, 2048, &uLenReturn);
	if( theErr != GM_NoErr ) {
		MsgLogGM(theErr);
		return theErr;
	}
	// 打印获取的数据信息...
	TRACE("\r%s\r\n", szBuffer);
	// 扔掉来自本机的数据包...
	if( iRemoteAddr == uRomAddr )
		return GM_NoErr;
	ASSERT( iRemoteAddr != uRomAddr );
	// 解析获取到的数据包 => xml...
	TiXmlNode    * lpNode     = NULL;
	TiXmlElement * lpDataElem = NULL;
	TiXmlElement * lpRootElem = NULL;
	TiXmlDocument  theXDoc;
	theXDoc.Parse(szBuffer);
	lpRootElem = theXDoc.RootElement();
	// 解析xml节点错误...
	if( lpRootElem == NULL ) {
		theErr = GM_No_Xml_Node;
		MsgLogGM(theErr);
		return theErr;
	}
	// 遍历xml节点数据内容...
	LPCTSTR		lpszText = NULL;
	LPCTSTR		lpszValue = NULL;
	lpNode = lpRootElem->FirstChild();
	while( lpNode != NULL ) {
		lpDataElem = lpNode->ToElement();
		lpszValue = lpDataElem->Value();
		lpszText = lpDataElem->GetText();
		ASSERT( lpszValue != NULL && lpszText != NULL );
		// 去掉 Uuid 这个节点数据...
		if( stricmp(lpszValue, "Uuid") != 0 ) {
			//theMapData[lpszValue] = lpszText;
		}
		// 打印获取到的网络摄像机相关节点信息...
		TRACE("%s: %s\n", lpszValue, lpszText);
		// 继续下一个节点...
		lpNode = lpNode->NextSibling();
	}
	// 通知管理层，发现一个摄像头，需要进行事件处理...
	return GM_NoErr;
}