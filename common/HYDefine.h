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
 * ���ͷ�ļ��ǿ�Դ�����о����õ��ĳ��涨�����
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

#define LOCAL_ADDRESS_V4			"127.0.0.1"				// ���ص�ַ����-IPV4
#define LOCAL_ADDRESS_V6			"[::1]"					// ���ص�ַ����-IPV6
#define LINGER_TIME					500						// SOCKETֹͣʱ������·��BUFF��յ�����ӳ�ʱ��
#define LOCAL_LOOP_ADDR_V6			"fe80::1"				// ���ص�ַ����-IPV6
#define LOCAL_LOOP1_ADDR_V6			"::1"					// ���ص�ַ����-IPV6
#define LOCAL_ERR_ADDR_V6			"fe80::ffff:ffff:fffd"	// ���ص�ַ����-IPV6

#define SERVER_NAME					"HYServer"				// Server name
#define TXT_EXCEPT					"Except.txt"			// �쳣�ļ�
#define TXT_LOGGER					"Logger.txt"			// ��־�ļ�
#define XML_DECLARE_GB2312			"<?xml version=\"1.0\" encoding=\"GB2312\"?>"
#define XML_DECLARE_UTF8			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

typedef enum LOG_TXT {
	kTxtLogger		= 0,		// ��־�ļ�����
	kTxtExcept		= 1,		// �쳣�ļ�����
};

typedef enum LOG_LEVEL			// ��־�����Ϣ
{
	kLogINFO		= 0,		// ������Ϣ��־
	kLogSYS			= 1,		// API���ô�����־
	kLogGM			= 2,		// �ڲ��߼�������־
};
