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
 * OSThread => �����߳��࣬���߳�API�����˷�װ��
 ******************************************************/

#ifndef _OSTHREAD_H
#define	_OSTHREAD_H

#include "StrPtrLen.h"

class OSThread
{
public:
	static	void		Initialize();								// OSThread Static Initialize 
	static	void		UnInitialize();
	static	OSThread *	GetCurrent();								// Get Current Thread Pointer.
	static	GM_Error	GetOSErr() { return ::GetLastError(); }
	static	DWORD		GetCurrentThreadID() { return ::GetCurrentThreadId(); }

	virtual void		Entry() = 0;								// Thread Virtual Entry. 
			void 		Start();									// Thread Start Point.
			void		StopAndWaitForThread();						// Stop And Wait.
			void		SendStopRequest()	{ fStopRequested = ((fThreadID != NULL) ? TRUE : FALSE); }
			BOOL		IsStopRequested()	{ return fStopRequested; }
			HANDLE		GetThreadHandle()	{ return fThreadID; }	// Get Thread Handle.
			DWORD		GetThreadMark()		{ return fThreadMark; }
			int			GetThreadPriority() { return ::GetThreadPriority(fThreadID); }
			BOOL		SetThreadPriority(int nPriority) { return ::SetThreadPriority(fThreadID, nPriority); }

			virtual 	~OSThread();
						OSThread();
protected: 
			BOOL		fStopRequested;								// Stop Flag
			HANDLE		fThreadID;									// Thread Handle.
private:
			
			UINT		fThreadMark;								// Thread Mark For Thread Message
			BOOL		fJoined;									// We have Exit Flag		
	static	DWORD		sThreadStorageIndex;						// Thread Storage Index
	static	UINT WINAPI _Entry(LPVOID inThread);					// Static Thread Entry.
};

#endif	// _OSTHREAD_H