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
 * Socket   => 基础套接字类，对套接字API进行了封装。
 ******************************************************/

#pragma once

class Socket
{
public:
	enum { kBufferSize = 4096 };
protected:
	Socket();
	~Socket();
public:
	GM_Error 		Open(int theType, BOOL bIsIPV6 = false);
	GM_Error		Close();
	
	SOCKET			GetSocket()			{ return m_hSocket; }
	UInt32			GetLocalAddrV4()	{ return ntohl(m_LocalAddrV4.sin_addr.s_addr); }
	UInt16			GetLocalPortV4()	{ return ntohs(m_LocalAddrV4.sin_port); }
	int				ReGetLocalPotrV4();
	UInt32			GetRemoteAddrV4() { return ntohl(m_RemoteAddrV4.sin_addr.s_addr); }
	UInt16			GetRemotePortV4() { return ntohs(m_RemoteAddrV4.sin_port); }
	LPCTSTR			GetRemoteAddrStrV4();

	void			Broadcast();
	void 			NonBlocking();
	void			ReuseAddr();
	void			NoDelay();
	void			KeepAlive();
	void			Linger(int nTime);
	void			SetSocketSendBufSize(UInt32 inNewSize);
	void			SetSocketRcvBufSize(UInt32 inNewSize);
	
	GM_Error 		Bind(UInt32 addr, UInt16 port);
	GM_Error		AsyncSelect(HWND hWnd, UINT uMsg, long lEvent=FD_READ|FD_CONNECT|FD_CLOSE);

	GM_Error 		Send(const char* inData, const UInt32 inLength, UInt32* outLengthSent);
	GM_Error 		Read(char *buffer, const UInt32 length, UInt32 *outRecvLenP);

	GM_Error		Send(const string & inStr, UInt32 * outLengthSent);
	GM_Error		Read(string & outStr, UInt32 * outRecvLenP);
public: /*-- For Event IO --*/
	WSAEVENT		GetWSAEvent()	{ return m_hEvent; }
	BOOL			IsUseEvent()	{ return (m_hEvent != NULL) ? TRUE : FALSE; }			// Query Socket Event is Used?
	GM_Error		CreateEvent(int inWorkID = FD_READ | FD_CLOSE);							// Create Socket binded event.
	GM_Error		ResetEvent(int inWorkID = FD_READ | FD_CLOSE);							// Resignal socket event sate.
	void			CloseEvent();															// Close Event.
	void			SignalEvent();															// Change socket event sate.
protected:
	WSAEVENT 		m_hEvent;
	SOCKET			m_hSocket;
	string			m_RemoteStrV4;			// 远程地址IPV4
	sockaddr_in 	m_RemoteAddrV4;			// 远程地址IPV4
	sockaddr_in		m_LocalAddrV4;			// 本地地址IPV4
	BOOL			m_bIsIPV6;				// 是否是IPV6的链接地址
};