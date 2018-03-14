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

#include "StdAfx.h"
#include "OSThread.h"
#include "process.h"
#include <time.h>

DWORD	OSThread::sThreadStorageIndex = 0;

OSThread::OSThread()
  : fStopRequested(FALSE),
	fJoined(FALSE),
	fThreadID(NULL),
	fThreadMark(0)
{
}

OSThread::~OSThread()
{
	this->StopAndWaitForThread();						// Wait for exit.
}
//
// OSThread Static Initialize 
//
void OSThread::Initialize()
{
	if(sThreadStorageIndex == 0)
	{
		sThreadStorageIndex = ::TlsAlloc();
	}
	ASSERT(sThreadStorageIndex >= 0);
}

void OSThread::UnInitialize()
{
	if(sThreadStorageIndex > 0)
	{
		if(::TlsFree(sThreadStorageIndex))
		{
			sThreadStorageIndex = 0;
		}
	}
}
//
// Get Current Thread Pointer.
//
OSThread * OSThread::GetCurrent()
{
	return (OSThread *)::TlsGetValue(sThreadStorageIndex);
}
//
// Thread Start Point.
//
void OSThread::Start()
{
	fThreadID = (HANDLE)_beginthreadex(	NULL, 			// Inherit security
										0,				// Inherit stack size
										_Entry,			// Entry function
										(void*)this,	// Entry arg
										0,				// Begin executing immediately
										&fThreadMark );	// We need to care about thread identify
	ASSERT(fThreadID != NULL);
	fStopRequested = FALSE;
}
//
// Stop And Wait For Thread Function Exit.
//
void OSThread::StopAndWaitForThread()
{
	if( fThreadID == NULL )					// Is not start?
		return;
	fStopRequested = TRUE;
	if( !fJoined )							// Have Exit?
	{
		fJoined = TRUE;						// Set Flag.
		DWORD theErr = ::WaitForSingleObject(fThreadID, INFINITE);
		//(theErr != WAIT_OBJECT_0) ? MsgLogGM(theErr) : NULL;
		CloseHandle(fThreadID);
	}
	fThreadID = NULL;
}
//
// Static Thread Entry.
//
UINT WINAPI OSThread::_Entry(LPVOID inThread)
{
	OSThread * theThread = (OSThread*)inThread;		// OSThread Pointer.
	BOOL theErr = ::TlsSetValue(sThreadStorageIndex, theThread);
	ASSERT(theErr == TRUE);
	theThread->Entry();								// Run the thread.

	_endthread();
	theThread->fStopRequested = FALSE;				// Reset member
	theThread->fThreadMark = 0;
	theThread->fThreadID = NULL;
	theThread->fJoined = FALSE;
	return NULL;
}
