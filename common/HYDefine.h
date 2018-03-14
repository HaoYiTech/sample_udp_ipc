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
 * 这个头文件是开源代码中经常用到的常规定义汇总
 ******************************************************/

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>
#include <mswsock.h>
#include <process.h>
#include <ws2tcpip.h>
#include <io.h>
#include <direct.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

//
// Basic data type redefine for Apple Code.
//
typedef unsigned char		UInt8;
typedef signed char			SInt8;
typedef unsigned short		UInt16;
typedef signed short		SInt16;
typedef unsigned long		UInt32;
typedef signed long			SInt32;
typedef LONGLONG			SInt64;
typedef ULONGLONG			UInt64;
typedef float				Float32;
typedef double				Float64;
typedef UInt16				Bool16;
typedef UInt8				Bool8;

#pragma warning(disable: 4786)

#include <map>
#include <list>
#include <deque>
#include <string>
#include <vector>
#include <algorithm>
#include <hash_map>

using namespace std;

#include "GMError.h"

#define LOCAL_ADDRESS_V4			"127.0.0.1"				// 本地地址定义-IPV4
#define LOCAL_ADDRESS_V6			"[::1]"					// 本地地址定义-IPV6
#define LINGER_TIME					500						// SOCKET停止时数据链路层BUFF清空的最大延迟时间
#define LOCAL_LOOP_ADDR_V6			"fe80::1"				// 本地地址定义-IPV6
#define LOCAL_LOOP1_ADDR_V6			"::1"					// 本地地址定义-IPV6
#define LOCAL_ERR_ADDR_V6			"fe80::ffff:ffff:fffd"	// 本地地址定义-IPV6

#define SERVER_NAME					"HYServer"				// Server name
#define TXT_EXCEPT					"Except.txt"			// 异常文件
#define TXT_LOGGER					"Logger.txt"			// 日志文件
#define XML_DECLARE_GB2312			"<?xml version=\"1.0\" encoding=\"GB2312\"?>"
#define XML_DECLARE_UTF8			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

typedef enum LOG_TXT {
	kTxtLogger		= 0,		// 日志文件类型
	kTxtExcept		= 1,		// 异常文件类型
};

typedef enum LOG_LEVEL			// 日志层次信息
{
	kLogINFO		= 0,		// 常规信息日志
	kLogSYS			= 1,		// API调用错误日志
	kLogGM			= 2,		// 内部逻辑错误日志
};
