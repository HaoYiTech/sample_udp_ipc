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
 * UDPSocket => UDP套接字管理类，是对UDP相关API的封装。
 * 注意：本类可以处理与UDP相关的大部分操作。
 ******************************************************/

#include "stdafx.h"
#include "UDPSocket.h"

UDPSocket::UDPSocket()
{
	::memset(&m_IPmr, 0, sizeof(m_IPmr));
	::memset(&m_IPV6mr, 0, sizeof(m_IPV6mr));
	::memset(&m_MsgAddr, 0, sizeof(m_MsgAddr));
	//::memset(&m_V6MsgAddr, 0, sizeof(m_V6MsgAddr));
	//m_nV6AddrLen = sizeof(m_V6MsgAddr);
	//m_nAddrLen = sizeof(m_MsgAddr);

	m_lpMultiAddr = NULL;

	::memset(m_lpRemoteAddr,0,256);
}

UDPSocket::~UDPSocket()
{
	if( m_lpMultiAddr ) {
		freeaddrinfo(m_lpMultiAddr);
	}
	m_lpMultiAddr = NULL;
}

GM_Error UDPSocket::JoinMulticastForRecv(UINT MultiAddr, UINT InterAddr)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	memset(&m_IPmr, 0, sizeof(m_IPmr));

	m_IPmr.imr_interface.s_addr = InterAddr;
	m_IPmr.imr_multiaddr.s_addr = MultiAddr;
	int	err = setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&m_IPmr, sizeof(m_IPmr));
	return ((err == SOCKET_ERROR) ? GetLastError() : GM_NoErr);
}


GM_Error UDPSocket::JoinMulticastForSend(UINT MultiAddr, UINT InterAddr)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	memset(&m_IPmr, 0, sizeof(m_IPmr));

	int err = setsockopt(m_hSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&InterAddr, sizeof(DWORD));
	return ((err == SOCKET_ERROR) ? GetLastError() : GM_NoErr);
}

GM_Error UDPSocket::LeaveMulticast(UInt32 inRemoteAddr)
{
	ip_mreq	theMulti = {0};
	theMulti.imr_multiaddr.s_addr = htonl(inRemoteAddr);
	theMulti.imr_interface.s_addr = htonl(m_LocalAddrV4.sin_addr.s_addr);
	int err = setsockopt(m_hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&theMulti, sizeof(theMulti));
	return ((err == -1) ? GetLastError() : GM_NoErr);
}

void UDPSocket::SetTtl(UInt16 timeToLive)
{
	u_char	nOptVal = (u_char)timeToLive;
	int err = setsockopt(m_hSocket, m_bIsIPV6 ? IPPROTO_IPV6 : IPPROTO_IP, IP_MULTICAST_TTL, (char*)&nOptVal, sizeof(nOptVal));
	ASSERT( err == 0 );
}

void UDPSocket::SetRemoteAddr(LPCSTR lpAddr, int nPort)
{
	ASSERT( lpAddr != NULL);
	sockaddr_in	sRmtAdd;
	sRmtAdd.sin_addr.s_addr = inet_addr(lpAddr);
	sRmtAdd.sin_port = htons(nPort);
	sRmtAdd.sin_family = AF_INET;
	memcpy(&m_RemoteAddrV4, &sRmtAdd, sizeof(sockaddr_in));
}

GM_Error UDPSocket::RecvFrom(LPWSABUF lpBuf, LPWSAOVERLAPPED lpOver)
{
	GM_Error theErr = GM_NoErr;
	DWORD dwFlags = 0, dwRecvBytes = 0;
	m_nAddrLen = sizeof(m_MsgAddr);
	if( WSARecvFrom(m_hSocket, lpBuf, 1, &dwRecvBytes, &dwFlags, (sockaddr*)&m_MsgAddr, &m_nAddrLen, lpOver, NULL) == SOCKET_ERROR) {
		theErr = WSAGetLastError();
		if( theErr != ERROR_IO_PENDING ) {
			//MsgLogGM(theErr);
			return theErr;
		}
	}
	return GM_NoErr;
}

GM_Error UDPSocket::SendTo(UInt32 inRemoteAddr, UInt16 inRemotePort, LPWSABUF lpBuf, LPWSAOVERLAPPED lpOver)
{
	memset(&m_SendAddr, 0, sizeof(m_SendAddr));
	m_SendAddr.sin_family = AF_INET;
	m_SendAddr.sin_port	 = htons(inRemotePort);
	m_SendAddr.sin_addr.s_addr = htonl(inRemoteAddr);

	GM_Error theErr = GM_NoErr;
	DWORD dwFlags = 0, dwSendBytes = 0;
	if( WSASendTo(m_hSocket, lpBuf, 1, &dwSendBytes, dwFlags, (sockaddr*)&m_SendAddr, sizeof(m_SendAddr), lpOver, NULL) == SOCKET_ERROR) {
		theErr = WSAGetLastError();
		if( theErr != ERROR_IO_PENDING) {
			//MsgLogGM(theErr);
			return theErr;
		}
	}
	return GM_NoErr;
}

GM_Error UDPSocket::SendTo(UInt32 inRemoteAddr, UInt16 inRemotePort, void* inBuffer, UInt32 inLength)
{
	ASSERT(inBuffer != NULL);
	sockaddr_in theRemoteAddr = {0};
	theRemoteAddr.sin_family = AF_INET;
	theRemoteAddr.sin_port	 = htons(inRemotePort);
	theRemoteAddr.sin_addr.s_addr = htonl(inRemoteAddr);
	//
	// Win32 says that inBuffer is a char*
	GM_Error theErr = GM_NoErr;
	int eErr = ::sendto(m_hSocket, (char*)inBuffer, inLength, 0, (sockaddr*)&theRemoteAddr, sizeof(theRemoteAddr));
	if( eErr == -1 ) {
		theErr = GetLastError();
		return theErr;
	}
	return GM_NoErr;
}

GM_Error UDPSocket::SendTo(void * inBuffer, UInt32 inLength)
{
	ASSERT(inBuffer != NULL);
	//struct sockaddr_in 	theRemoteAddr = {0};
	//theRemoteAddr.sin_family = AF_INET;
	//theRemoteAddr.sin_port	 = htons(inRemotePort);
	//theRemoteAddr.sin_addr.s_addr = htonl(inRemoteAddr);
	//
	// Win32 says that inBuffer is a char*
	GM_Error theErr = GM_NoErr;
	int eErr = ::sendto( m_hSocket, (char*)inBuffer, inLength, 0, (sockaddr*)&m_RemoteAddrV4, sizeof(m_RemoteAddrV4));
	if( eErr == -1 ) {
		theErr = GetLastError();
		return theErr;
	}
	return GM_NoErr;
}

GM_Error UDPSocket::RecvFrom(UInt32* outRemoteAddr, UInt16* outRemotePort, void* ioBuffer, UInt32 inBufLen, UInt32* outRecvLen)
{
	ASSERT(outRecvLen != NULL);
	ASSERT(outRemoteAddr != NULL);
	ASSERT(outRemotePort != NULL);

	GM_Error theErr = GM_NoErr;
	int addrLen = sizeof(m_MsgAddr);
	//
	// Win32 says that ioBuffer is a char*
	SInt32 theRecvLen = ::recvfrom(m_hSocket, (char*)ioBuffer, inBufLen, 0, (sockaddr*)&m_MsgAddr, &addrLen);
	if( theRecvLen == -1 ) {
		theErr = GetLastError();
		return theErr;
	}
	*outRemoteAddr = ntohl(m_MsgAddr.sin_addr.s_addr);
	*outRemotePort = ntohs(m_MsgAddr.sin_port);
	ASSERT(theRecvLen >= 0);
	*outRecvLen = (UInt32)theRecvLen;
	return GM_NoErr;		
}

//------------------------------------------------------------------------------------------------------------------------------
void UDPSocket::SetTtlCM(int timeToLive)
{
	char *optval=NULL;
	int		optlevel,
			option,
			optlen,
			err;

	ASSERT( m_lpMultiAddr );
	if( m_lpMultiAddr == NULL )
		return;
	err = NO_ERROR;
	if ( m_lpMultiAddr->ai_family == AF_INET){
		// Set the options for V4
		optlevel = IPPROTO_IP;
		option   = IP_MULTICAST_TTL;
		optval   = (char *) &timeToLive;
		optlen   = sizeof(timeToLive);
	}
	else if ( m_lpMultiAddr->ai_family == AF_INET6){
		// Set the options for V6
		optlevel = IPPROTO_IPV6;
		option   = IPV6_MULTICAST_HOPS;
		optval   = (char *) &timeToLive;
		optlen   = sizeof(timeToLive);
	}
	else{
		//Attemtping to set TTL for invalid address family!
		err = SOCKET_ERROR;
	}
	if (err != SOCKET_ERROR)	{
		// Set the TTL value
		err = setsockopt( m_hSocket, optlevel, option,optval, optlen );	
		if( err == SOCKET_ERROR ) {
			err = WSAGetLastError();
		}
	}
	ASSERT( err != SOCKET_ERROR );
}
//
// Suitable for the connection of IPV4/IPV6 ...
void UDPSocket::SetRemoteAddrCM(LPCSTR lpAddr, int nPort)
{
	ASSERT( lpAddr != NULL);

	if(m_lpMultiAddr)
		freeaddrinfo(m_lpMultiAddr);
	m_lpMultiAddr = NULL;

	memcpy(m_lpRemoteAddr,lpAddr,strlen(lpAddr));
	m_iPort = nPort;

	char lpPort[10] = {0};
	memset(lpPort,0,10);
	itoa(nPort,lpPort,10);
	m_lpMultiAddr = TransformAddress((char*)lpAddr, lpPort, AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP);
}
//
// Suitable for the connection of IPV4/IPV6 ...
GM_Error UDPSocket::BindCM( LPCTSTR lpszInterAddr, int port ,int AFamily /*= AF_INET*/ )
{
	GM_Error  theErr = GM_NoErr;
	ASSERT(m_hSocket != INVALID_SOCKET);
	struct addrinfo		*resbind;

	char lpPort[10] = {0};
	memset(lpPort,0,10);
	itoa(port,lpPort,10);

	// Resolve the binding address
	resbind = TransformAddress((char*)lpszInterAddr, lpPort, AFamily, SOCK_DGRAM, IPPROTO_UDP);
	if (resbind == NULL){
		theErr = ::WSAGetLastError();
		//MsgLogGM(theErr);
		return theErr;
	}
	int err = bind(m_hSocket, (sockaddr*)resbind->ai_addr, (int)resbind->ai_addrlen);
	if (err == SOCKET_ERROR)
	{
		theErr = ::WSAGetLastError();
		//MsgLogGM(theErr);
		return theErr;
	}
	return GM_NoErr;
}

//
// Suitable for the connection of IPV4/IPV6 ...
GM_Error UDPSocket::JoinMulticastForRecvCM(LPCTSTR lpszMultiAddr, LPCTSTR lpszInterAddr)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	ASSERT( lpszMultiAddr != NULL );

	memset(&m_IPV6mr, 0, sizeof(m_IPV6mr)); //V6
	memset(&m_IPmr, 0,	sizeof(m_IPmr));	//V4

	struct addrinfo		*reslocal,	*resmulti;
	char				*optval=NULL;
	int					optlevel,option,optlen,	err = NO_ERROR;

	//Transform the address of multicast to the addrinfo 
	resmulti = TransformAddress((char*)lpszMultiAddr, NULL, AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP);
	//Transform the host to  the local interface
	reslocal = TransformAddress((char*)lpszInterAddr, "0", resmulti->ai_family, resmulti->ai_socktype, resmulti->ai_protocol);

	// join the multicast group
	if (resmulti->ai_family == AF_INET){
		// 设置V4　OPTION以及　ip_mreq　结构值
		optlevel = IPPROTO_IP;
		option   = IP_ADD_MEMBERSHIP;
		optval   = (char *)& m_IPmr;
		optlen   = sizeof(m_IPmr);

		m_IPmr.imr_multiaddr.s_addr = ((SOCKADDR_IN *)resmulti->ai_addr)->sin_addr.s_addr;
		m_IPmr.imr_interface.s_addr = ((SOCKADDR_IN *)reslocal->ai_addr)->sin_addr.s_addr;

	}
	else if (resmulti->ai_family == AF_INET6){
		// set the value of V6 option and　the  value of ipv6_mreq　structure
		optlevel = IPPROTO_IPV6;
		option   = IPV6_ADD_MEMBERSHIP;
		optval   = (char *) &m_IPV6mr;
		optlen   = sizeof(m_IPV6mr);

		m_IPV6mr.ipv6mr_multiaddr = ((SOCKADDR_IN6 *)resmulti->ai_addr)->sin6_addr;
		m_IPV6mr.ipv6mr_interface = ((SOCKADDR_IN6 *)reslocal->ai_addr)->sin6_scope_id;
	}
	else{
		//用无效的地址家族加入组播组!
		err = SOCKET_ERROR;
	}
	if ( err != SOCKET_ERROR )
		err =setsockopt(m_hSocket, optlevel, option,optval, optlen );

	freeaddrinfo(resmulti);
	freeaddrinfo(reslocal);
	return (( err == SOCKET_ERROR ) ? WSAGetLastError() : GM_NoErr);	
}

// Suitable for the connection of IPV4/IPV6 ...
GM_Error UDPSocket::JoinMulticastForSendCM(LPCTSTR lpszMultiAddr, LPCTSTR lpszInterAddr)
{
	struct addrinfo		*reslocal;
	ASSERT(m_hSocket != INVALID_SOCKET);
	//memset(&m_IPmr, 0, sizeof(m_IPmr));

	char * optval = NULL;
	int    optlevel,	option,	optlen,	err= NO_ERROR;

	reslocal = TransformAddress((char*)lpszInterAddr, "0", AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP);
	//reslocal = TransformAddress(lpszInterAddr, "0", resmulti->ai_family, resmulti->ai_socktype, resmulti->ai_protocol);

	if (reslocal->ai_family == AF_INET){
		// Setup the v4 option values
		optlevel = IPPROTO_IP;
		option   = IP_MULTICAST_IF;
		optval   = (char *) &((SOCKADDR_IN *)reslocal->ai_addr)->sin_addr.s_addr;
		optlen   = sizeof(((SOCKADDR_IN *)reslocal->ai_addr)->sin_addr.s_addr);
	}
	else if (reslocal->ai_family == AF_INET6){
		// Setup the v6 option values
		optlevel = IPPROTO_IPV6;
		option   = IPV6_MULTICAST_IF;
		optval   = (char *) &((SOCKADDR_IN6 *)reslocal->ai_addr)->sin6_scope_id;
		optlen   = sizeof(((SOCKADDR_IN6 *)reslocal->ai_addr)->sin6_scope_id);
	}
	else{
		fprintf(stderr, "Attemtping to set sent interface for invalid address family!\n");
		err  = SOCKET_ERROR;
	}

	// Set send IF
	if ( err != SOCKET_ERROR ){
		// Set the send interface
		err = ::setsockopt( m_hSocket, optlevel, option,optval,optlen	);
		if (err != SOCKET_ERROR)	{			
			//PrintAddress(reslocal->ai_addr, reslocal->ai_addrlen);			
		}
	}
	freeaddrinfo(reslocal);
	return (( err == SOCKET_ERROR ) ? WSAGetLastError() : GM_NoErr);	
}
//
// Suitable for the connection of IPV4/IPV6 ...
GM_Error UDPSocket::SendToCM(LPCTSTR lpszMultiAddr, UInt16 inRemotePort, void* inBuffer, UInt32 inLength)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	struct addrinfo		*resmulti;
	char lpPort[10] = {0};
	memset(lpPort,0,10);
	char * lpTemp = itoa(inRemotePort,lpPort,10);
	//
	// Resolve the multicast address	
	resmulti = TransformAddress((char*)lpszMultiAddr, lpTemp, AF_UNSPEC, SOCK_DGRAM, IPPROTO_UDP);

	// Win32 says that inBuffer is a char*
	int err = ::sendto(m_hSocket, (char*)inBuffer, inLength, 0, (sockaddr*)&resmulti->ai_addr, resmulti->ai_addrlen);
	freeaddrinfo(resmulti);
	if( err == SOCKET_ERROR ) {
		return GetLastError();
	}
	return GM_NoErr;
}
//
// Suitable for the connection of IPV4/IPV6 ...
GM_Error UDPSocket::SendToCM(void * inBuffer, UInt32 inLength)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	ASSERT(inBuffer != NULL);
	GM_Error theErr = GM_NoErr;
	
	//
	// Win32 says that inBuffer is a char*m_lpMultiAddr
	SInt32 theRecvLen =  ::sendto( m_hSocket, (char*)inBuffer, inLength, 0, m_lpMultiAddr->ai_addr, m_lpMultiAddr->ai_addrlen);
	if( theRecvLen == SOCKET_ERROR ) {
		theErr = WSAGetLastError();		
		return theErr;
	}
	return GM_NoErr;
}
//
// Suitable for the connection of IPV4/IPV6 ...
GM_Error UDPSocket::RecvFromCM(LPCTSTR outRemoteAddr, UInt16* outRemotePort, void* ioBuffer, UInt32 inBufLen, UInt32* outRecvLen)
{
	ASSERT(outRecvLen != NULL);
	ASSERT(outRemoteAddr != NULL);
	ASSERT(outRemotePort != NULL);
	ASSERT(ioBuffer != NULL);

	GM_Error theErr = GM_NoErr;

	SOCKADDR_STORAGE	Addrfrom;
	int     ifromlen = 0;
	//
	ifromlen = sizeof(Addrfrom);
	SInt32 theRecvLen = ::recvfrom(	m_hSocket, (char*)ioBuffer, inBufLen, 0,(SOCKADDR *)&Addrfrom, &ifromlen	);
	if (theRecvLen == SOCKET_ERROR){
		theErr =	WSAGetLastError();
		printf("recvfrom failed with: %d\n", theErr);
		return theErr;
	}
	//看看怎么转为字符串
	ASSERT(theRecvLen >= 0);
	int nRemotePort = 0;
	FormatAddress((SOCKADDR *)&Addrfrom, ifromlen, (char*)outRemoteAddr,256,(int*)&nRemotePort);///-----------------------************************?????????????????????????要改呀，一定要改呀
	*outRemotePort = (UInt16)nRemotePort;
	*outRecvLen = (UInt32)theRecvLen;
	return GM_NoErr;		
}


//
// 适合 IPV4/IPV6 的链接方式...
GM_Error UDPSocket::LeaveMulticastCM(LPCTSTR lpszMultiAddr )
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	//struct ipv6_mreq	mreq6;
	//memset(&mreq6, 0, sizeof(mreq6));
	//struct addrinfo		*resmulti;

	//// Resolve the multicast address
	//getaddrinfo(lpszMultiAddr, NULL, NULL, &resmulti);

	//// Join the multicast group
	//mreq6.ipv6mr_multiaddr = ((SOCKADDR_IN6 *)resmulti->ai_addr)->sin6_addr;
	//mreq6.ipv6mr_interface = ((SOCKADDR_IN6 *)resmulti->ai_addr)->sin6_scope_id;

	int	err = setsockopt(m_hSocket, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP,(char *)&m_IPV6mr, sizeof(m_IPV6mr));

	//freeaddrinfo(resmulti);
	return ((err == SOCKET_ERROR) ? GetLastError() : GM_NoErr);

	
}


//
//	这个函数把字符串形式的IP地址转换为一个包函SOCKADDR结构的　addrinfo
//	注意：(1) 如果addr参数为空那么可用在bind时， the socket address structure is set to INADDR_ANY for IPv4 addresses and IN6ADDR_ANY_INIT for IPv6 addresses
//		  (2) The function don't release the memory of addrinfo structure.After used the pointer of addrinfo structure,you would release the memory.The function is [freeaddrinfo(resmulti)];
struct addrinfo * UDPSocket::TransformAddress(char *addr, char *port, int af, int type, int proto)
{
//	ASSERT( addr != NULL && port != NULL );

    struct addrinfo hints,
    *res = NULL;
    int             rs;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags  = ((addr) ? 0 : AI_PASSIVE);
    hints.ai_family = af;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;

    rs = getaddrinfo(
            addr,
            port,
           &hints,
           &res
            );
    if (rs != 0){
		
        return NULL;
    }
    return res;
}
//
//将SOCKADDR转化为字符串的形式，同时输出端口
int UDPSocket::FormatAddress(SOCKADDR *sa, int salen, char * addrbuf,int addrbuflen, int * iport )
{
	ASSERT( sa != NULL && addrbuf != NULL && iport != NULL );
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

//
//  Input the string of address , IPV4 or IPV6
//  Return the family of address, the address family of IPV4 is AF_INET,the address family of IPV6 is AF_INET6;
int UDPSocket::GetAddrFamily(char *addr)
{
	ASSERT( addr != NULL );
	if( addr == NULL ) return -1;
	struct addrinfo *res = NULL;
	int   rs = getaddrinfo( addr,NULL,NULL,&res );
	if (rs != 0){
		return -1;
	}
	int ai_family = res->ai_family;

	freeaddrinfo(res);
//	WSAAddressToString()
	return ai_family;
}
