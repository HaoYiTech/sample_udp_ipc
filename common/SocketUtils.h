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
 * SocketUtils  => 套接字工具类，对套接字外围相关API进行了封装。
 ******************************************************/

#ifndef	_SOCKET_UTILS_H
#define	_SOCKET_UTILS_H

#include "StrPtrLen.h"

//#include <string>
//#include <map>
//
//typedef map<int, string>  MapIPString;

class SocketUtils
{
public:
	
	// Call initialize before using any socket functions.
	// (pass true for lookupDNSName if you want the hostname
	// looked up via DNS during initialization -- %%sfu)
	static void Initialize(Bool16 lookupDNSName = false);
	static void UnInitialize();
	
	//static utility routines
	static Bool16	IsMulticastIPAddr(UINT inAddress);
	static Bool16	IsLocalIPAddr(UINT inAddress);
	static Bool16	IsLocalIPAddr(LPCTSTR lpszAddr);
	
	//This function converts an integer IP address to a dotted-decimal string.
	//This function is NOT THREAD SAFE!!!
	static void   ConvertAddrToString(const struct in_addr& theAddr, StrPtrLen * outAddr);
	static string ConvertAddrToString(DWORD dwHostAddr);
	
	static int  FormatAddress(SOCKADDR *sa, int salen, char * addrbuf,int addrbuflen, int * iport );

	// This function converts a dotted-decimal string IP address to a UInt32
	static UINT ConvertStringToAddr(char* inAddr);
	
	//You can get at all the IP addrs and DNS names on this machine this way
	static UINT	GetNumIPAddrs() { return sNumIPAddrs; }
	static inline UINT			GetIPAddr(UINT inAddrIndex);
	static inline StrPtrLen *	GetIPAddrStr(UINT inAddrIndex);
	static inline StrPtrLen *	GetDNSNameStr(UINT inDNSIndex);

	static UINT				GetNumIPAddrsV6() { return sNumIPV6Addrs; }
	static inline char *	GetIPAddrStrV6(UINT inAddrIndex);
private:
	static void GetIPV4List(Bool16 lookupDNSName = false);
	static int  GetIPV6List();
	static void ReleaseIPV4Ary();
	static void ReleaseIPV6Ary();
	//
	// For storing relevent information about each IP interface
	//
	struct IPAddrInfo
	{
		UINT 		fIPAddr;
		StrPtrLen	fIPAddrStr;
		StrPtrLen	fDNSNameStr;
	};
	
	static IPAddrInfo *		sIPAddrInfoArray;

	//static MapIPString	sIPV6StringArray;		
	static char **			sIPV6StringArray;	
	static UINT				sNumIPV6Addrs;
	static UINT				sNumIPAddrs;
};

inline UINT	SocketUtils::GetIPAddr(UINT inAddrIndex)
{
	ASSERT(sIPAddrInfoArray != NULL);
	ASSERT(inAddrIndex < sNumIPAddrs);
	return sIPAddrInfoArray[inAddrIndex].fIPAddr;
}

inline StrPtrLen *	SocketUtils::GetIPAddrStr(UINT inAddrIndex)
{
	ASSERT(sIPAddrInfoArray != NULL);
	ASSERT(inAddrIndex < sNumIPAddrs);
	return &sIPAddrInfoArray[inAddrIndex].fIPAddrStr;
}

inline StrPtrLen *	SocketUtils::GetDNSNameStr(UINT inDNSIndex)
{
	ASSERT(sIPAddrInfoArray != NULL);
	ASSERT(inDNSIndex < sNumIPAddrs);
	return &sIPAddrInfoArray[inDNSIndex].fDNSNameStr;
}

inline char *	SocketUtils::GetIPAddrStrV6(UINT inAddrIndex)
{
	ASSERT(sIPV6StringArray != NULL);
	ASSERT(inAddrIndex < sNumIPV6Addrs);
	return sIPV6StringArray[inAddrIndex];
}


#endif	// _SOCKET_UTILS_H