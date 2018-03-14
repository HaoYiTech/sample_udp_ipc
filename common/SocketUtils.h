/******************************************************
 * ��һ�Ƽ����ṩ��¼�����Ƽ�ص�ȫƽ̨�޲�����������
 * ֧�ְ���ֱ������㲼�أ��ֲ�ʽ�����洢����̬���ݣ�
 * ֧��΢��ɨ���¼��ȫƽ̨�ʺ�ͳһ������΢��С����
 * ֧�ֶ����������룺����ͷIPC��rtmp��rtsp��MP4�ļ���
 * ֧��ȫʵʱ��ȫ��̬��ȫ��ҳ������ҳǰ��̨����IE8��
 * ֧�ֶ��ն��޲������Ӧ���ţ�flvjs/hls/rtmp�Զ����䣻
 ******************************************************
 * �ٷ���վ => https://myhaoyi.com
 * �������� => http://blog.csdn.net/haoyitech
 * ��Դ���� => https://github.com/HaoYiTech/
 * �Ƽ��Ⱥ => 483663026��QQȺ��
 * ��¼��Ⱥ => 630379661��QQȺ��
 ******************************************************
 * SocketUtils  => �׽��ֹ����࣬���׽�����Χ���API�����˷�װ��
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