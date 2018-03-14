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
 * CUtilTool => 针对系统常用工具的API封装。
 ******************************************************/

#include "stdafx.h"
#include <io.h>
#include <time.h>
#include <Psapi.h>
#include <ShellAPI.h>
#include <ATLComTime.h>
#include "UtilTool.h"

int CUtilTool::VersionCmp( LPCTSTR lpVer1,LPCTSTR lpVer2 )
{
	int iVer[4] = {0};
	int iVerO[4] = {0};
	int s1 = sscanf (lpVer1, "%d.%d.%d%.%d", &iVer[0], &iVer[1], &iVer[2], &iVer[3]) ;
	int s2 = sscanf (lpVer2, "%d.%d.%d%.%d", &iVerO[0], &iVerO[1], &iVerO[2], &iVerO[3]) ;
	int icount = min(s1,s2);

	for( int i = 0 ; i < icount ; i++ ){
		if( iVer[i] == iVerO[i])
			continue;
		else if( iVer[i] < iVerO[i])
			return -1;
		else
			return 1;
	}
	return 0;
}
//
// Get Current Date, Format => "%a, %d %b %Y %H:%M:%S GMT" 
string CUtilTool::GetGMTDate()
{
	string	strGMT;
	struct	tm * local = NULL;
	TCHAR	szDate[MAX_PATH] = {0};
	__time64_t	calendarTime = ::_time64(NULL);
	local = ::_localtime64((__time64_t*)&calendarTime);
	size_t size = ::strftime(szDate, MAX_PATH, "%a, %d %b %Y %H:%M:%S GMT", local);
	ASSERT(size == strlen(szDate));
	strGMT.assign(szDate, size);
	return strGMT;
}
//
// Get CHS Current Date, Format => "%Y-%m-%d %H:%M:%S" 
string CUtilTool::GetCHSDate()
{
	string	strCHS;
	struct	tm * local = NULL;
	TCHAR	szDate[MAX_PATH] = {0};
	__time64_t	calendarTime = ::_time64(NULL);
	local = ::_localtime64((__time64_t*)&calendarTime);
	size_t size = ::strftime(szDate, MAX_PATH, "%Y-%m-%d %H:%M:%S", local);
	ASSERT(size == strlen(szDate));
	strCHS.assign(szDate, size);
	return strCHS;
}

LPCTSTR	CUtilTool::GetServerBuildDate() {
	return (__DATE__ ", "__TIME__);
}

LPCTSTR	CUtilTool::GetServerUserName()
{
	static TCHAR szBuffer[MAX_PATH] = {0};
	if( strlen(szBuffer) > 0 )
		return szBuffer;
	DWORD	dwLen = MAX_PATH;
	memset(szBuffer, 0, MAX_PATH);
	::GetUserName(szBuffer, &dwLen);
	ASSERT(dwLen <= MAX_PATH);
	return szBuffer;
}

LPCTSTR	CUtilTool::GetServerDNSName()
{
	static TCHAR szBuffer[MAX_PATH] = {0};
	if( strlen(szBuffer) > 0 )
		return szBuffer;
	DWORD	dwLen = MAX_PATH;
	::GetComputerName(szBuffer, &dwLen);
	ASSERT(dwLen <= MAX_PATH);
	return szBuffer;
}

LPCTSTR	CUtilTool::GetServerHeader()
{
	static TCHAR szBuffer[MAX_PATH] = {0};
	if( strlen(szBuffer) > 0 )
		return szBuffer;
	/*StringFormatter serverFormatter(szBuffer, MAX_PATH);
	serverFormatter.Put(CUtilTool::GetServerName());
	serverFormatter.PutChar('/');
	serverFormatter.Put(CUtilTool::GetServerVersion());
	ASSERT(strlen(szBuffer) < MAX_PATH);*/
	return szBuffer;
}

LPCTSTR	CUtilTool::GetServerOS()
{
	static TCHAR szBuffer[MAX_PATH] = {0};
	if( strlen(szBuffer) > 0 )
		return szBuffer;
	::sprintf(szBuffer, "%s", CUtilTool::GetSystemVer().c_str());
	ASSERT( ::strlen(szBuffer) <= MAX_PATH );
	return szBuffer;
}
//
// Get the Current System Type Number
// Return: 0(UnKnown), 31(WINDOWS 3.1), 95(WINDOWS95), 98(WINDOWS98), 2000(WINDOWS NT)
string CUtilTool::GetSystemVer()
{
	CString			strVersion = "Windows Unknown";
	OSVERSIONINFO	osv		= {0};
	osv.dwOSVersionInfoSize = sizeof(osv);
	if( !GetVersionEx(&osv) )
		return strVersion;
	strVersion.Format("Windows %ld.%ld.%ld %s", osv.dwMajorVersion, osv.dwMinorVersion, osv.dwBuildNumber, osv.szCSDVersion);
	return strVersion;
}

/* ---------------------------------------- */
/*  寻找执行DLL模块自己的句柄                 */
/* ---------------------------------------- */
HMODULE CUtilTool::GetSelfModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi = {0};

	return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) 
		? (HMODULE) mbi.AllocationBase : NULL);
}

string CUtilTool::GetExePath(HINSTANCE hInstance)
{
	DWORD	dwPathlen	= 0;
	TCHAR   pathbuf[MAX_PATH] = {0};
	dwPathlen = ::GetModuleFileName(hInstance, pathbuf, MAX_PATH);
	ASSERT( dwPathlen < MAX_PATH );
	while( TRUE )
	{
		if( pathbuf[dwPathlen--] =='\\' )
			break;
	}
	pathbuf[++dwPathlen] = '\0';
	return pathbuf;
}
//
// Get the EXE file full path...
void CUtilTool::GetExePath(LPSTR outPath)
{
	ASSERT( outPath != NULL );
	DWORD	dwPathlen	= 0;
	dwPathlen = ::GetModuleFileName(NULL, outPath, MAX_PATH);
	ASSERT( dwPathlen < MAX_PATH );
	while( TRUE )
	{
		if( outPath[dwPathlen--] =='\\' )
			break;
	}
	outPath[++dwPathlen] = '\0';
}

void CUtilTool::GetFilePath(LPSTR outPath, LPCTSTR inFile)
{
	ASSERT( outPath != NULL );
	ASSERT( inFile != NULL );

	TCHAR	thePath[MAX_PATH] = {0};
	CUtilTool::GetExePath(thePath);
	ASSERT( strlen(thePath) > 0 );	
	::sprintf(outPath, "%s\\%s", thePath, inFile);
	ASSERT( strlen(outPath) < MAX_PATH );
}

LPCTSTR CUtilTool::GetTimeString(__time64_t inTime, LPCTSTR lpszFormat)
{
	static TCHAR	szTime[MAX_PATH] = {0};
	if( inTime <= 0 || lpszFormat == NULL )
		return NULL;
	tm	*	lpLocal = ::_localtime64((__time64_t*)&inTime);
	ASSERT( lpLocal != NULL );
	memset(szTime, 0, MAX_PATH);
	::strftime(szTime, MAX_PATH, lpszFormat, lpLocal);
	return szTime;
}

string CUtilTool::GetGMTTime(__time64_t inTime/* = 0*/)
{
	tm		*	theLocal = NULL;
	string		strValue;
	TCHAR		szDateBuf[MAX_PATH] = {0};
	theLocal = ::_localtime64((__time64_t*)&inTime);
	size_t size = ::strftime(szDateBuf, MAX_PATH, "%a, %d %b %Y %H:%M:%S GMT", theLocal);
	ASSERT( strlen(szDateBuf) == size );
	strValue.assign(szDateBuf, strlen(szDateBuf));
	return strValue;
}

void CUtilTool::DbgLogInfo(LPCTSTR lpFormat, ...)
{
#ifdef _DEBUG
	CString	strDebug;
	va_list	argList = NULL;
	va_start( argList, lpFormat );
	strDebug.FormatV( lpFormat, argList );
	va_end( argList );

	if((strDebug.ReverseFind('\r') < 0) && (strDebug.ReverseFind('\n') < 0))
		strDebug.Append("\r\n");
	OutputDebugString(strDebug);
#endif
}
//
// 得到文件的版本信息,返回字符串...
string CUtilTool::GetFileVersionStr(LPCTSTR lpszFileName)
{
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} * lpTranslate;
	lpTranslate = NULL;
	// 进行输入参数的判断...
	string	strReturn = "0.0.0.0";
	if( lpszFileName == NULL || strlen(lpszFileName) <= 0 )
		return strReturn;
	// 构造临时需要的变量内容...
	ASSERT( lpszFileName != NULL );
	DWORD	dwInfoSize = 0;
	DWORD	dwSizeJunk = 0;
	string	strInfo;
	CString	strVerion;
	CString	strContent;
	LPSTR	lpBuffer = NULL;
	UINT	cbVerBytes = 0;
	UINT	cbTranslate = 0;
	dwInfoSize = GetFileVersionInfoSize(lpszFileName, &dwSizeJunk);
	do {
		if( dwInfoSize <= 0 )
			break;
		// 得到版本信息缓冲区...
		strInfo.resize(dwInfoSize);
		if( !GetFileVersionInfo(lpszFileName, 0, dwInfoSize, (LPVOID)strInfo.c_str()) )
			break;
		// 查询语言信息结构...
		if( !VerQueryValue((LPVOID)strInfo.c_str(),	TEXT("\\VarFileInfo\\Translation"),	(LPVOID*)&lpTranslate, &cbTranslate) )
			break;
		if( lpTranslate == NULL || cbTranslate <= 0 )
			break;
		if( (cbTranslate/sizeof(LANGANDCODEPAGE)) <= 0 )
			break;
		// 查询文件版本信息内容...
		strContent.Format("\\StringFileInfo\\%04x%04x\\FileVersion", lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
		if( !VerQueryValue((LPVOID)strInfo.c_str(), (LPSTR)strContent.GetString(), (LPVOID*)&lpBuffer, &cbVerBytes) )
			break;
		if( lpBuffer == NULL || cbVerBytes <= 0 )
			break;
		// 对版本信息进行修正,去掉空格,替换逗号...
		ASSERT( lpBuffer != NULL );
		strVerion = lpBuffer;
		strVerion.Remove(' ');
		strVerion.Replace(',', '.');
	}while(false);
	// 返回得到的版本信息...
	if( lpBuffer == NULL )
		return strReturn;
	strReturn.assign(strVerion);
	ASSERT( strReturn.size() > 0 );
	return strReturn;
}

ULONGLONG CUtilTool::GetLocalFileTime()
{
	FILETIME fileTime  = {0};
	SYSTEMTIME locTime = {0};
	SYSTEMTIME utcTime = {0};
	ULARGE_INTEGER uLarge = {0};
	TIME_ZONE_INFORMATION zoneInfo = {0};
	GetSystemTime(&utcTime);
	GetTimeZoneInformation(&zoneInfo);
	SystemTimeToTzSpecificLocalTime(&zoneInfo, &utcTime, &locTime);
	SystemTimeToFileTime(&locTime, &fileTime);
	memcpy(&uLarge, &fileTime, sizeof(uLarge));
	return uLarge.QuadPart;
}

ULONGLONG CUtilTool::GetFileFullSize(LPCTSTR lpszFileName)
{
	HANDLE	hFile = INVALID_HANDLE_VALUE;
	if( lpszFileName == NULL )
		return 0;
	ASSERT( lpszFileName != NULL );
	hFile = ::CreateFile( lpszFileName, GENERIC_READ,
						  FILE_SHARE_READ, NULL, OPEN_EXISTING, 
						  FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL );
	if( hFile == NULL || hFile == INVALID_HANDLE_VALUE )
		return 0;
	ULARGE_INTEGER ulFileSize = {0};
	ulFileSize.LowPart = ::GetFileSize(hFile, &ulFileSize.HighPart);
	::CancelIo(hFile); ::CloseHandle(hFile);
	return ulFileSize.QuadPart;
}

static LPCTSTR sErrLevel[] = {
	"INFO", "SYS", "GM",
};

LPCTSTR	CUtilTool::GetErrLevel(LOG_LEVEL inLevel)
{
	if( inLevel < 0 || inLevel >= (sizeof(sErrLevel)/sizeof(sErrLevel[0])) )
		return NULL;
	return sErrLevel[inLevel];
}

string CUtilTool::UTF8_ANSI(LPCTSTR lpUValue)
{
	string strSValue;
	USES_CONVERSION;
	if( lpUValue == NULL || strlen(lpUValue) <= 0 )
		return strSValue;				// 验证有效性
	_acp = CP_UTF8;                     // 设置code page
	LPCWSTR lpWValue = A2W(lpUValue);	// UTF-8 to Unicode Wide char
	_acp = CP_ACP;                      // 设置code page
	strSValue = W2A(lpWValue);			// Unicode Wide char to ANSI
	return strSValue;
}

string CUtilTool::ANSI_UTF8(LPCTSTR lpSValue)
{
	string strUValue;
	USES_CONVERSION;
	if( lpSValue == NULL || strlen(lpSValue) <= 0 )
		return strUValue;				// 验证有效性
	_acp = CP_ACP;                      // 设置code page(默认)

	// 2009/09/01 A2W 在某些系统下会发生崩溃...
	// LPCWSTR lpWValue = A2W(lpSValue); // ANSI to Unicode Wide char
	int dwSize = MultiByteToWideChar(_acp, 0, lpSValue, -1, NULL, 0);
	wchar_t * lpWValue = new wchar_t[dwSize];
	MultiByteToWideChar(_acp, 0, lpSValue, -1, lpWValue, dwSize);

	_acp = CP_UTF8;                     // 设置code page, 得到编码长度, Unicode Wide char to UTF-8
	int ret = WideCharToMultiByte(_acp, 0, lpWValue, -1, NULL, NULL, NULL, NULL);
	strUValue.resize(ret); ASSERT( ret == strUValue.size() );

	ret = WideCharToMultiByte(_acp, 0, lpWValue, -1, (LPSTR)strUValue.c_str(), strUValue.size(), NULL, NULL);
	ASSERT( ret == strUValue.size() );

	delete [] lpWValue;

	return strUValue;
}

string CUtilTool::IntToStr(int inValue)
{
	string	strValue;
	TCHAR	szBuf[80] = {0};
	sprintf(szBuf, "%d", inValue);
	strValue = szBuf;
	return strValue;
}

string CUtilTool::GUIDToStr(REFCLSID rCLSID, BOOL bBracket/* = false*/)
{
	string	strValue;
	TCHAR	szBuf[80] = {0};
	LPCTSTR	lpszFormat = bBracket ? "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}" : "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X";
	sprintf(szBuf, lpszFormat, rCLSID.Data1, rCLSID.Data2, rCLSID.Data3,
			rCLSID.Data4[0], rCLSID.Data4[1], rCLSID.Data4[2], rCLSID.Data4[3],
			rCLSID.Data4[4], rCLSID.Data4[5], rCLSID.Data4[6], rCLSID.Data4[7]);
	strValue = szBuf;
	return strValue;
}

string CUtilTool::GetFileModTime(FILETIME * lpLastWrite)
{
	string		strTime;
	TCHAR		theLine[MAX_PATH] = {0};
	SYSTEMTIME	theUTCSys = {0};
	SYSTEMTIME	theLocal  = {0};
	ASSERT( lpLastWrite != NULL );
	FileTimeToSystemTime(lpLastWrite, &theUTCSys);

	TIME_ZONE_INFORMATION theZone = {0};
	GetTimeZoneInformation(&theZone);
	SystemTimeToTzSpecificLocalTime(&theZone, &theUTCSys, &theLocal);

	sprintf(theLine, "%04d-%02d-%02d %02d:%02d", theLocal.wYear, theLocal.wMonth, theLocal.wDay, theLocal.wHour, theLocal.wMinute);
	strTime = theLine;
	return strTime;
}

string CUtilTool::GetFileSizeStr(DWORD dwHigh, DWORD dwLow)
{
	string		strSize, strTemp;
	TCHAR		theLine[MAX_PATH] = {0};
	ULONGLONG	llSize = (ULONGLONG)((dwHigh * ((ULONGLONG)MAXDWORD+1)) + dwLow);
	sprintf(theLine, "%I64u", llSize/1024+1);
	strTemp = theLine;

	int nSize = strTemp.size() - 1;
	int nCount = 1;

	while( nSize >= 0 ) {
		strSize += strTemp.at(nSize);
		if( nSize == 0 )
			break;
		if( nCount != 3 ) {
			++nCount;
		} else {
			strSize += ",";
			nCount = 1;
		}
		nSize--;
	}
	reverse(strSize.begin(), strSize.end());
	return strSize;
}

const BYTE  UTF_LEAD_0	= 0xEF;					// UTF-8的标志
const BYTE  UTF_LEAD_1	= 0xBB;
const BYTE  UTF_LEAD_2	= 0xBF;

BOOL CUtilTool::IsUTF8(LPCTSTR lpszValue)
{
	return (strlen(lpszValue) > 2 && (BYTE)lpszValue[0] == UTF_LEAD_0 && (BYTE)lpszValue[1] == UTF_LEAD_1 && (BYTE)lpszValue[2] == UTF_LEAD_2) ? TRUE : FALSE;
}

//
// 打印各种层次的格式信息到控制台或存盘；
GM_Error CUtilTool::MsgLog(LOG_TXT eType, LPCTSTR lpFormat, ...)
{
	DWORD			dwErr = 0;
	CString			strLog;
	GM_Error		theErr = GM_NoErr;
	CGMLogWriter  &	theLog = CGMLogWriter::GetInstance();

	va_list	argList = NULL;
	va_start( argList, lpFormat );
	strLog.FormatV( lpFormat, argList );
	va_end( argList );
	
	//if( eType == kTxtExcept ) {
	//	COleDateTime dtTime = COleDateTime::GetCurrentTime();
	//	strLog.AppendFormat(", %s\r\n", dtTime.Format());
	//}
	
	if((strLog.ReverseFind('\r') < 0) && (strLog.ReverseFind('\n') < 0))
		strLog.Append("\r\n");
	return theLog.MsgLog(eType, strLog);
}
//
// File(Line) : Level, Error, Time => __FILE__(__LINE__) : Level, Error, Time
GM_Error CUtilTool::MsgLog( GM_Error nErr, LOG_LEVEL nLevel, LPCTSTR lpFile, int nCodeLine )
{
	if( (int)nErr <= 0 || nErr == GM_Continue || nErr == GM_NoReply )
		return GM_NoErr;
	ASSERT( lpFile != NULL );
	TCHAR			szPathBuf[MAX_PATH] = {0};
	CString			strMsg, strFormat;
	COleDateTime	dtTime = COleDateTime::GetCurrentTime();
	CGMLogWriter  &	theLog = CGMLogWriter::GetInstance();

	if( lpFile != NULL ) {
		if( ::GetFullPathName(lpFile, MAX_PATH, szPathBuf, NULL) <= 0 )
			return GM_NoErr;
		ASSERT( strlen(szPathBuf) > 0 );
	}
	strFormat = ((int)nErr < -1) ? _T("%s(%lu) : %s, 0x%x, %s\r\n") : _T("%s(%lu) : %s, %d, %s\r\n");
	strMsg.Format(strFormat, szPathBuf, nCodeLine, CUtilTool::GetErrLevel(nLevel), nErr, dtTime.Format());

	ASSERT( strMsg.GetLength() > 0 );
	return theLog.MsgLog(kTxtLogger, strMsg.GetString());
}
///* ---------------------------------------- */
///*  可以删除多级路径						*/
///* ---------------------------------------- */

BOOL CUtilTool::DelDir( LPCTSTR lpszPath )
{
	WIN32_FIND_DATA theFileData = {0};
	BOOL	bIsOK = TRUE;
	HANDLE	hFind = INVALID_HANDLE_VALUE;
	CAtlString DirName;
	CAtlString sPath = lpszPath;
	if( sPath.GetLength() <= 0 )
		return false;

	if( sPath.Right(1) != "\\" )
		DirName = sPath+"\\*.*";
	else
		DirName = sPath+"*.*";
	hFind = FindFirstFile( DirName, &theFileData );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		while( bIsOK )
		{
			if( (theFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&	// 目录
				(!(theFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))  &&
				(!(strcmp(theFileData.cFileName, ".") == 0)) &&					// 当前
				(!(strcmp(theFileData.cFileName, "..") == 0) )  )				// 上级)	
				//(theFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ||		// 隐藏
				//(theFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) ||		// 系统
			{
				CString sFilePath;
				sFilePath.Format("%s\\%s",sPath,theFileData.cFileName);
				DelDir( sFilePath );
			}
			if( theFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ) 	// 文件
			{
				CString sFilePath;
				sFilePath.Format("%s\\%s",sPath,theFileData.cFileName);
				BOOL Result = ::DeleteFile(sFilePath);
				DWORD dwErr = GetLastError();
			}
			bIsOK = FindNextFile(hFind, &theFileData);
		}
	}
	FindClose(hFind);
	BOOL Result = ::RemoveDirectory(sPath);
	DWORD dwErr = GetLastError();
	return Result;
}
/* ---------------------------------------- */
/*  计算路径深度			                 */
/* ---------------------------------------- */
int CUtilTool::CaculDirDepth(LPCTSTR lpszPath)
{
	CAtlString pathname = lpszPath;
	if( pathname.Right(1) != "\\" )
		pathname += "\\" ;
	int end = pathname.ReverseFind('\\');
	int pt = pathname.Find('\\');
	if (pt == 0 || pathname[pt-1] == ':' || pathname[pt+1] == '\\')
		pt = pathname.Find('\\', pt+1);
	CAtlString path;
	BOOL Result = FALSE;
	int nDepth = 1;
	while(pt != -1 && pt<=end){
		path = pathname.Left(pt+1);
		//		if( !::PathIsDirectory(  path ))//PathFileExists
		//			 Result = ::CreateDirectory( path , NULL );
		pt = pathname.Find('\\', pt+1);
		nDepth += 1;
	}	
	return nDepth;	
}
/* ---------------------------------------- */
/*  可以完成创建多级路径	                 */
/* ---------------------------------------- */
BOOL CUtilTool::CreateDir(LPCTSTR lpszPath)
{
	CAtlString pathname = lpszPath;
	if( pathname.Right(1) != "\\" )
		pathname += "\\" ;
	int end = pathname.ReverseFind('\\');
	int pt = pathname.Find('\\');
	if (pathname[pt-1] == ':')
		pt = pathname.Find('\\', pt+1);
	CAtlString path;
	BOOL Result = FALSE;
	while(pt != -1 && pt<=end)
	{
		path = pathname.Left(pt+1);
		if( !::PathIsDirectory(  path ))//PathFileExists
			Result = ::CreateDirectory( path , NULL );
		pt = pathname.Find('\\', pt+1);
	}
	return Result;	
}

HCURSOR	CUtilTool::GetSysHandCursor()
{
	TCHAR		strWinDir[MAX_PATH] = {0};
	HCURSOR		hHandCursor			= NULL;
	hHandCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));
	
	// Still no cursor handle - load the WinHelp hand cursor
	if( hHandCursor == NULL )
	{
		GetWindowsDirectory(strWinDir, MAX_PATH);
		strcat(strWinDir, _T("\\winhlp32.exe"));
		
		// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
		HMODULE hModule = ::LoadLibrary(strWinDir);
		DWORD	dwErr = GetLastError();
		if( hModule != NULL )
		{
			HCURSOR	 hTempCur = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
			hHandCursor = (hTempCur != NULL) ? CopyCursor(hTempCur) : NULL;
			FreeLibrary(hModule);
		}
	}
	return hHandCursor;
}

//
// 代替日志插件的日志单键实现...
//
CGMLogWriter::CGMLogWriter()
  : m_eType(kTxtLogger)
{
}

CGMLogWriter::~CGMLogWriter()
{
}

CGMLogWriter & CGMLogWriter::GetInstance()
{
	static CGMLogWriter	sLogWriter;
	return sLogWriter;
}

GM_Error CGMLogWriter::InitLogTxt(FILE ** lppFile)
{
	//
	// 得到日志文件路径...
	TCHAR	  szPath[MAX_PATH] = {0};
	GM_Error  theErr = GM_File_Path_Err;
	LPCTSTR	  lpszLogFile = ((m_strLogFile.size() > 0) ? m_strLogFile.c_str() : TXT_LOGGER);
	ASSERT( lpszLogFile != NULL && strlen(lpszLogFile) > 0 );
	if( m_bIsFullPath ) {
		strcpy(szPath, lpszLogFile);
	} else {
		switch( m_eType )
		{
		case kTxtLogger: CUtilTool::GetFilePath(szPath, lpszLogFile); break;
		case kTxtExcept: CUtilTool::GetFilePath(szPath, TXT_EXCEPT);  break;
		default:		 CUtilTool::GetFilePath(szPath, lpszLogFile); break;
		}
	}
	if( strlen(szPath) <= 0 )
		return theErr;
	*lppFile = fopen(szPath, "a+");
	if( *lppFile == NULL )
		return theErr;
	//
	// 当文件大于 5M 时回滚...
	long nSize = _filelength(_fileno(*lppFile));
	if( nSize >= 3 * 1024 * 1024 ) {
		fclose(*lppFile); *lppFile = NULL;
		*lppFile = fopen(szPath, "w");
	}
	return ((*lppFile == NULL) ? theErr : GM_NoErr);
}
//
// Debug   => 简单打印到调试窗口和控制台...
// Release => 存盘到一个日志xml文件当中...
GM_Error CGMLogWriter::MsgLog(LOG_TXT eType, LPCTSTR lpszLog)
{
	m_eType = eType;						// Save the LOG_TXT type
	GM_Error theErr = GM_NoErr;
	ASSERT( lpszLog != NULL && strlen(lpszLog) > 0 );
#ifdef _DEBUG
	TRACE(lpszLog);							// (Debug)TRACE in debugger window
	printf(lpszLog);						// (Debug)Print in console window
	//OutputDebugString(lpszLog);			// (Release)TRACE in debugger window
	//theErr = this->DoTextLog(lpszLog);	// (Release)Save in logger file
#else
	printf(lpszLog);						// (Release)Print in console window
	OutputDebugString(lpszLog);				// (Release)TRACE in debugger window
	theErr = this->DoTextLog(lpszLog);		// (Release)Save in logger file
#endif // _DEBUG
	return theErr;
}

GM_Error CGMLogWriter::DoTextLog(LPCTSTR lpszLog)
{
	GM_Error	theErr = GM_NoErr;
	FILE	*	theFile = NULL;
	theErr = this->InitLogTxt(&theFile);
	if( theErr != GM_NoErr )
		return theErr;
	ASSERT(theFile != NULL);
	fputs(lpszLog, theFile);
	fclose(theFile);
	return GM_NoErr;
}
