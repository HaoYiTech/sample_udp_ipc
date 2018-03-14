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

#include "StdAfx.h"
#include "SocketUtils.h"

UINT							SocketUtils::sNumIPAddrs = 0;
SocketUtils::IPAddrInfo *		SocketUtils::sIPAddrInfoArray = NULL;
char **							SocketUtils::sIPV6StringArray = NULL;	
UINT							SocketUtils::sNumIPV6Addrs = 0;
void SocketUtils::Initialize(Bool16 lookupDNSName /* = false */)
{
	SocketUtils::GetIPV4List(lookupDNSName);
	SocketUtils::GetIPV6List();
}

Bool16 SocketUtils::IsMulticastIPAddr(UINT inAddress)
{
	return ((inAddress>>8) & 0x00f00000) == 0x00e00000;	//	multicast addresses == "class D" == 0xExxxxxxx == 1,1,1,0,<28 bits>
}

Bool16 SocketUtils::IsLocalIPAddr(UINT inAddress)
{
	for (UINT x = 0; x < sNumIPAddrs; x++)
	{
		if (sIPAddrInfoArray[x].fIPAddr == inAddress)
			return TRUE;
	}
	return FALSE;
}

Bool16 SocketUtils::IsLocalIPAddr(LPCTSTR lpszAddr)
{
	if( lpszAddr == NULL )
		return FALSE;
	for( UINT x = 0; x < sNumIPAddrs; x++ )
	{
		if( sIPAddrInfoArray[x].fIPAddrStr.EqualIgnoreCase(lpszAddr, strlen(lpszAddr)) )
			return TRUE;
	}
	return FALSE;
}
//  Input the string of address , IPV4 or IPV6
//  得到字符串IP
int  SocketUtils::FormatAddress(SOCKADDR *sa, int salen, char * addrbuf,int addrbuflen, int * iport )
{
	//	ASSERT( sa != NULL && addrbuf != NULL && iport != NULL );
	char    host[NI_MAXHOST],		serv[NI_MAXSERV];
	int     hostlen = NI_MAXHOST,	servlen = NI_MAXSERV,	err = NO_ERROR;

	err = getnameinfo(
		sa,
		salen,
		host,
		hostlen,
		serv,
		servlen,
		NI_NUMERICHOST | NI_NUMERICSERV
		);
	if (err != NO_ERROR){		
		return err;
	}
	if( (strlen(host) + strlen(serv) + 1) > (unsigned)addrbuflen)
		return WSAEFAULT;
	if (sa->sa_family == AF_INET || sa->sa_family == AF_INET6 ){
		sprintf(addrbuf, "%s", host);
		*iport = atoi(serv);
	}
	else
		addrbuf[0] = '\0';
	return NO_ERROR;
}

void SocketUtils::ConvertAddrToString(const struct in_addr& theAddr, StrPtrLen* ioStr)
{
	char* addr = inet_ntoa(theAddr);
	strcpy(ioStr->Ptr, addr);
	ioStr->Len = ::strlen(ioStr->Ptr);
}

string SocketUtils::ConvertAddrToString(DWORD dwHostAddr)
{
	struct sockaddr_in dAddr;
	memset((void *)&dAddr, 0, sizeof(dAddr));
	dAddr.sin_addr.s_addr = htonl(dwHostAddr);
	return inet_ntoa(dAddr.sin_addr);
}

UINT SocketUtils::ConvertStringToAddr(char* inAddrStr)
{
	return ntohl(::inet_addr(inAddrStr));
}

void SocketUtils::UnInitialize()
{
	SocketUtils::ReleaseIPV4Ary();
	SocketUtils::ReleaseIPV6Ary();
}

void SocketUtils::ReleaseIPV4Ary()
{
	if(sIPAddrInfoArray == NULL)
		return;
	for(UInt32 i = 0; i < sNumIPAddrs; i++)
	{
		if(sIPAddrInfoArray[i].fIPAddrStr.Ptr != NULL)
		{
			delete [] sIPAddrInfoArray[i].fIPAddrStr.Ptr;
			sIPAddrInfoArray[i].fIPAddrStr.Ptr = NULL;
		}
		if(sIPAddrInfoArray[i].fDNSNameStr.Ptr != NULL)
		{
			delete [] sIPAddrInfoArray[i].fDNSNameStr.Ptr;
			sIPAddrInfoArray[i].fDNSNameStr.Ptr = NULL;
		}
	}
	delete [] (UInt8*)sIPAddrInfoArray;
	sIPAddrInfoArray = NULL;
	sNumIPAddrs = 0;
}

void SocketUtils::ReleaseIPV6Ary()
{
	if(sIPV6StringArray == NULL)
		return;
	for(UInt32 i = 0; i < sNumIPV6Addrs; i++)
	{
		if(sIPV6StringArray[i] != NULL)
		{
			delete [] sIPV6StringArray[i];
			sIPV6StringArray[i] = NULL;
		}
	}
	delete [] (char*)sIPV6StringArray;
	sIPV6StringArray = NULL;
	sNumIPV6Addrs = 0;
}

void SocketUtils::GetIPV4List(Bool16 lookupDNSName /* = false */)
{
	if( sNumIPAddrs > 0 )
		return;
	ASSERT( sNumIPAddrs <= 0 );
	int tempSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (tempSocket == -1)
		return;

	static const UINT kMaxAddrBufferSize = 2048;
	char	inBuffer[kMaxAddrBufferSize] = {0};
	char	outBuffer[kMaxAddrBufferSize] = {0};
	ULONG	theReturnedSize = 0;
	
	//
	// Use the WSAIoctl function call to get a list of IP addresses
	//
	int theErr = ::WSAIoctl(	tempSocket, SIO_GET_INTERFACE_LIST, 
								inBuffer, kMaxAddrBufferSize,
								outBuffer, kMaxAddrBufferSize,
								&theReturnedSize,
								NULL,
								NULL);
	ASSERT(theErr == 0);
	if (theErr != 0){
		int ierr = GetLastError();
		return;
	}
	
	ASSERT((theReturnedSize % sizeof(INTERFACE_INFO)) == 0);	
	LPINTERFACE_INFO addrListP = (LPINTERFACE_INFO)&outBuffer[0];
	
	sNumIPAddrs = theReturnedSize / sizeof(INTERFACE_INFO);
	//
	// allocate the IPAddrInfo array. Unfortunately we can't allocate this
	// array the proper way due to a GCC bug
	//
	BYTE * addrInfoMem = new BYTE[sizeof(IPAddrInfo) * sNumIPAddrs];
	::memset(addrInfoMem, 0, sizeof(IPAddrInfo) * sNumIPAddrs);
	sIPAddrInfoArray = (IPAddrInfo*)addrInfoMem;

	UINT currentIndex = 0;
	for (UINT theIfCount = sNumIPAddrs, addrCount = 0;
		 addrCount < theIfCount; addrCount++)
	{
		struct sockaddr_in* theAddr = (struct sockaddr_in*)&addrListP[addrCount].iiAddress;

		char* theAddrStr = ::inet_ntoa(theAddr->sin_addr);

		//store the IP addr
		sIPAddrInfoArray[currentIndex].fIPAddr = ntohl(theAddr->sin_addr.s_addr);
		
		//store the IP addr as a string
		sIPAddrInfoArray[currentIndex].fIPAddrStr.Len = ::strlen(theAddrStr);
		sIPAddrInfoArray[currentIndex].fIPAddrStr.Ptr = new char[sIPAddrInfoArray[currentIndex].fIPAddrStr.Len + 2];
		::strcpy(sIPAddrInfoArray[currentIndex].fIPAddrStr.Ptr, theAddrStr);

		struct hostent* theDNSName = NULL;
		if (lookupDNSName) //convert this addr to a dns name, and store it
		{
			theDNSName = ::gethostbyaddr((char *)&theAddr->sin_addr, sizeof(theAddr->sin_addr), AF_INET);
		}
		
		if (theDNSName != NULL)
		{
			sIPAddrInfoArray[currentIndex].fDNSNameStr.Len = ::strlen(theDNSName->h_name);
			sIPAddrInfoArray[currentIndex].fDNSNameStr.Ptr = new char[sIPAddrInfoArray[currentIndex].fDNSNameStr.Len + 2];
			::strcpy(sIPAddrInfoArray[currentIndex].fDNSNameStr.Ptr, theDNSName->h_name);
		}
		else
		{
			//if we failed to look up the DNS name, just store the IP addr as a string
			sIPAddrInfoArray[currentIndex].fDNSNameStr.Len = sIPAddrInfoArray[currentIndex].fIPAddrStr.Len;
			sIPAddrInfoArray[currentIndex].fDNSNameStr.Ptr = new char[sIPAddrInfoArray[currentIndex].fDNSNameStr.Len + 2];
			::strcpy(sIPAddrInfoArray[currentIndex].fDNSNameStr.Ptr, sIPAddrInfoArray[currentIndex].fIPAddrStr.Ptr);
		}
		//move onto the next array index
		currentIndex++;
		
	}
 	::closesocket(tempSocket);
	//
	// If LocalHost is the first element in the array, switch it to be the second.
	// The first element is supposed to be the "default" interface on the machine,
	// which should really always be en0.
	if ((sNumIPAddrs > 1) && (::strcmp(sIPAddrInfoArray[0].fIPAddrStr.Ptr, "127.0.0.1") == 0))
	{
		UINT tempIP = sIPAddrInfoArray[1].fIPAddr;
		sIPAddrInfoArray[1].fIPAddr = sIPAddrInfoArray[0].fIPAddr;
		sIPAddrInfoArray[0].fIPAddr = tempIP;
		StrPtrLen tempIPStr(sIPAddrInfoArray[1].fIPAddrStr);
		sIPAddrInfoArray[1].fIPAddrStr = sIPAddrInfoArray[0].fIPAddrStr;
		sIPAddrInfoArray[0].fIPAddrStr = tempIPStr;
		StrPtrLen tempDNSStr(sIPAddrInfoArray[1].fDNSNameStr);
		sIPAddrInfoArray[1].fDNSNameStr = sIPAddrInfoArray[0].fDNSNameStr;
		sIPAddrInfoArray[0].fDNSNameStr = tempDNSStr;
	}
}

//   The routing depends on ( WSAStartup(MAKEWORD(2,2), &wsd) )
//	 Get the numeric string( AF_INET or AF_INET6 )
int  SocketUtils::GetIPV6List()
{
	#define   DEFAULT_IPADDR_NUM   32 
	ReleaseIPV6Ary();
	char					_buf[sizeof(SOCKET_ADDRESS_LIST)*DEFAULT_IPADDR_NUM]; 
	SOCKET					fd			=	socket(AF_INET6,SOCK_DGRAM,0); 
	LPSOCKET_ADDRESS_LIST   paddrlist	=	(LPSOCKET_ADDRESS_LIST)_buf; 
	unsigned   long   nBytesReturned	=	sizeof(SOCKET_ADDRESS_LIST)*DEFAULT_IPADDR_NUM; 
	int					iret			=	WSAIoctl(fd,SIO_ADDRESS_LIST_QUERY,0,   0,   paddrlist,nBytesReturned,   &nBytesReturned,   0,   0); 
	closesocket(fd); 
	if(SOCKET_ERROR==iret) 
		return false;

	sIPV6StringArray = new char*[ DEFAULT_IPADDR_NUM ];
	memset(sIPV6StringArray, 0, sizeof(char*) * DEFAULT_IPADDR_NUM);

	char outRemoteAddr[NI_MAXHOST];
	char lpBuff[NI_MAXHOST];
	int nRemotePort = 0;
	for(int i = 0; i < paddrlist->iAddressCount; i++ ){
		memset(outRemoteAddr,0,NI_MAXHOST);
		sockaddr_in6   *psa6=(sockaddr_in6*)paddrlist->Address[i].lpSockaddr;
		if( psa6 ){
			SocketUtils::FormatAddress((SOCKADDR *)psa6, sizeof(sockaddr_in6), (char*)outRemoteAddr,NI_MAXHOST,(int*)&nRemotePort);

			////去掉IPV4和IPV6兼容地址
			char *lpStr = ::strstr(outRemoteAddr,".");
			if( lpStr ) continue;
			//去掉结尾处的接口索引
			memset(lpBuff,0,NI_MAXHOST);
			memcpy(lpBuff,outRemoteAddr,strlen(outRemoteAddr));
			lpStr = ::strstr(lpBuff,"%");
			if(lpStr) lpStr[0] = '\0';
			//去掉本地回环地址
			if( stricmp(LOCAL_LOOP_ADDR_V6, lpBuff ) == 0 || stricmp(LOCAL_LOOP1_ADDR_V6, lpBuff ) == 0 || stricmp( LOCAL_ERR_ADDR_V6, lpBuff ) == 0 )
				continue;

			int slen = strlen(outRemoteAddr) + 1;
			sIPV6StringArray[ sNumIPV6Addrs ] = new char[slen];
			memset(sIPV6StringArray[ sNumIPV6Addrs ], 0, slen); 
			memcpy(sIPV6StringArray[ sNumIPV6Addrs ],outRemoteAddr,strlen(outRemoteAddr));

			sNumIPV6Addrs++;
		}
	}
	//if( sNumIPV6Addrs > 0 )
	//	return 0;
	//ASSERT( sNumIPV6Addrs <= 0 );

	//struct addrinfo  hints,
	//				*res=NULL,
	//				*ptr=NULL;
	//int rc;

	//// Variables for getnameinfo
	//char    numerichost[NI_MAXHOST] = {0};
	//int     numerichostlen=NI_MAXHOST;

	////
	//// Check for the minumum parameters required
	////
	//char HostName[NI_MAXHOST] = {0};
	//gethostname(HostName, sizeof(HostName));// 获得本机主机名.

	//if (!HostName){
	//	return -1;
	//}
	////
	//// Resolve the name/address - first assume that the name might be a string literal address
	//memset(&hints, 0, sizeof(hints));
	//hints.ai_flags     = AI_CANONNAME; //AI_NUMERICHOST;
	//hints.ai_family    = AF_INET6;     //AF_INET
	//hints.ai_socktype  = SOCK_DGRAM;
	//hints.ai_protocol  = IPPROTO_IP;

	//rc = getaddrinfo(
	//	HostName,
	//	NULL,
	//	&hints,
	//	&res
	//	);
	//if( rc != 0 ) {
	//	return WSAGetLastError();
	//}
	//sNumIPV6Addrs = 0;
	//ptr = res;
	//while( ptr != NULL )
	//{
	//	// 没有名称的地址属性，不能使用...
	//	/*if((ptr->ai_canonname == NULL) || (strlen(ptr->ai_canonname) <= 0)) {
	//		ptr = ptr->ai_next;
	//		continue;
	//	}*/
	//	// 记录有名称的地址属性...
	//	sNumIPV6Addrs++;
	//	ptr = ptr->ai_next;
	//}
	//// IPV6地址必须大于0，因为过滤了地址属性...
	//if( sNumIPV6Addrs <= 0 ) {
	//	sNumIPV6Addrs = 0;
	//	return 0;
	//}
	//// 分配IPV6地址列表空间...
	//ASSERT( sNumIPV6Addrs > 0 );
	//sIPV6StringArray = new char*[ sNumIPV6Addrs ];
	//memset(sIPV6StringArray, 0, sizeof(char*) * sNumIPV6Addrs);
	////
	////Do a reverse lookup on each of the resolved address
	////	
	//int count = 0;
	//ptr   = res;
	//while (ptr)
	//{
	//	// 首先判断地址属性是否有效 => 是否有名称属性...
	//	/*if((ptr->ai_canonname == NULL) || (strlen(ptr->ai_canonname) <= 0)) {
	//		ptr = ptr->ai_next;
	//		continue;
	//	}*/
	//	//
	//	// Convert the binary addresses returned from getaddrinfo into their numeric string .
	//	rc = getnameinfo(
	//		ptr->ai_addr,
	//		ptr->ai_addrlen,
	//		numerichost,
	//		numerichostlen,
	//		NULL,
	//		0,
	//		NI_NUMERICHOST
	//		);
	//	if (rc != 0){
	//		freeaddrinfo(res);
	//		SocketUtils::ReleaseIPV6Ary();
	//		return ::WSAGetLastError();
	//	}
	//	int slen = strlen(numerichost) + 1;
	//	sIPV6StringArray[ count ] = new char[slen];
	//	memset(sIPV6StringArray[ count ], 0, slen); 
	//	memcpy(sIPV6StringArray[ count ],numerichost,strlen(numerichost));
	//	TRACE("%s\n",numerichost);
	//	// Goto the next address
	//	ptr = ptr->ai_next;
	//	count++;
	//}

	//// Free up the results from getaddrinfo
	//freeaddrinfo(res);
	return 0;
}
