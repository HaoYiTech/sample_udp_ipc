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
 * Socket   => �����׽����࣬���׽���API�����˷�װ��
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
	string			m_RemoteStrV4;			// Զ�̵�ַIPV4
	sockaddr_in 	m_RemoteAddrV4;			// Զ�̵�ַIPV4
	sockaddr_in		m_LocalAddrV4;			// ���ص�ַIPV4
	BOOL			m_bIsIPV6;				// �Ƿ���IPV6�����ӵ�ַ
};