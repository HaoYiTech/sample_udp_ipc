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
 * OSThread => 基础线程类，对线程API进行了封装。
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