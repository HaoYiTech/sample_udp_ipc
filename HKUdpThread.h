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

#pragma once

#include "OSThread.h"
#include "UDPSocket.h"

#define DEF_MCAST_ADDRV4	"239.255.255.250"	// 组播地址
#define	DEF_MCAST_PORT		37020				// 组播端口
#define	DEF_MCAST_STEP		30					// 间隔时间(秒)
#define DEF_MCAST_TIME		5000				// 默认组播事件线程等待时间(毫秒)

class CHKUdpThread : public OSThread
{
public:
	CHKUdpThread();
	~CHKUdpThread();
public:
	GM_Error		InitMulticast();
private:
	virtual void	Entry();
private:
	GM_Error		ForRead();						// 响应网络读取事件
	GM_Error		SendCmdQuiry();					// 发送查询指令 - 组播命令
	GM_Error		WaitAndProcMulticast();			// 等待并处理组播数据
	GM_Error		ProcessEvent(int eventBits);	// 处理网络事件
private:
	UDPSocket		m_UDPSocket;					// UDP对象
};
