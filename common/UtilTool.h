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
 * CUtilTool => ���ϵͳ���ù��ߵ�API��װ��
 ******************************************************/

#pragma once

#include "HYDefine.h"

class CUtilTool
{
public:
	static string		GetGMTDate();				// Get GMT Current Date. 
	static string		GetCHSDate();				// Get CHS Current Date.

	static string		GetSystemVer();
	static HCURSOR		GetSysHandCursor();
	static HMODULE		GetSelfModuleHandle();
	static string		GetExePath(HINSTANCE hInstance);
	static void			GetExePath(LPSTR outPath);
	static void			GetFilePath(LPSTR outPath, LPCTSTR inFile);

	static string		GetGMTTime(__time64_t inTime = 0);
	static LPCTSTR		GetTimeString(__time64_t inTime, LPCTSTR lpszFormat);
	static void			DbgLogInfo(LPCTSTR lpFormat, ...);
	
	static GM_Error		MsgLog( LOG_TXT eType, LPCTSTR lpFormat, ... );		// ��ӡ���ֲ�εĸ�ʽ��Ϣ������̨����̣�
	static GM_Error		MsgLog( GM_Error nErr,				// �������: GM_Error
								LOG_LEVEL nLevel,			// ��������: kLogGM
							    LPCTSTR lpFile,				// �ļ�����: __FILE__
							    int nCodeLine );			// �����к�: __LINE__ 

	static string		GetFileVersionStr(LPCTSTR lpszFileName);
	static ULONGLONG	GetFileFullSize(LPCTSTR lpszFileName);
	static ULONGLONG	GetLocalFileTime();
	static int			VersionCmp( LPCTSTR lpVer1,LPCTSTR lpVer2 );
	
	static string		GetFileModTime(FILETIME * lpLastWrite);
	static string		GetFileSizeStr(DWORD dwHigh, DWORD dwLow);
	static LPCTSTR		GetErrLevel(LOG_LEVEL inLevel);

	static string		GUIDToStr(REFCLSID rCLSID, BOOL bBracket = false);
	static string		UTF8_ANSI(LPCTSTR lpUValue);
	static string		ANSI_UTF8(LPCTSTR lpSValue);
	static string		IntToStr(int inValue);
	static BOOL			IsUTF8(LPCTSTR lpszValue);
	static BOOL			DelDir( LPCTSTR lpszPath );
	static int			CaculDirDepth(LPCTSTR lpszPath);
	static BOOL			CreateDir(LPCTSTR lpszPath);

	static LPCTSTR		GetServerProductName();
	static LPCTSTR		GetServerBasicRealm();
	static LPCTSTR		GetServerBuildDate();
	static LPCTSTR		GetServerUserName();
	static LPCTSTR		GetServerPlatform();
	static LPCTSTR		GetServerDNSName();
	static LPCTSTR		GetServerHeader();
	static LPCTSTR		GetServerOS();
};

class CGMLogWriter
{
public:
	static		CGMLogWriter & GetInstance();
	GM_Error	MsgLog(LOG_TXT eType, LPCTSTR lpszLog);
	void		SetLoggerFile(const string & strFile, BOOL bIsFullPath = false) { m_strLogFile = strFile; m_bIsFullPath = bIsFullPath; }
private:
	GM_Error	InitLogTxt(FILE ** lppFile);
	GM_Error	DoTextLog(LPCTSTR lpszLog);
private:
	CGMLogWriter();
	~CGMLogWriter();
private:
	LOG_TXT		m_eType;			 // ��־����
	string		m_strLogFile;		 // ��־�ļ�
	BOOL		m_bIsFullPath;		 // �Ƿ�������·��
};

#define ANSI_Value(x) CUtilTool::UTF8_ANSI(x).c_str()
#define UTF8_Value(x) CUtilTool::ANSI_UTF8(x).c_str()

#define MsgLogGM(nErr) CUtilTool::MsgLog(nErr, kLogGM, __FILE__, __LINE__)			// ��¼�߼�����(Logger.txt)
#define MsgLogSYS(nErr) CUtilTool::MsgLog(nErr, kLogSYS, __FILE__, __LINE__)		// ��¼ϵͳ����(Logger.txt)
#define MsgLogINFO(sErr) CUtilTool::MsgLog(kTxtLogger, _T("%s\r\n"), TEXT(sErr))	// ��¼ϵͳ��Ϣ(Logger.txt)
#define MsgExpINFO(sErr) CUtilTool::MsgLog(kTxtExcept, _T("%s\r\n"), TEXT(sErr))	// ��¼�쳣��Ϣ(Except.txt)
#define MsgExpFile(sFile) CUtilTool::MsgLog(kTxtExcept, _T("%s(%lu)\r\n�쳣�ļ� = %s"), __FILE__, __LINE__, TEXT(sFile))	// ��¼�쳣��Ϣ(Except.txt)
#define MsgExpCode(nCode) CUtilTool::MsgLog(kTxtExcept, _T("%s(%lu)\r\n�쳣���� = 0x%x(%lu)"), __FILE__, __LINE__, nCode, nCode)	// ��¼�쳣��Ϣ(Except.txt)
