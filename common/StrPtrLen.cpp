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
 * StrPtrLen => 字符串管理类，可以对字符串指针和长度管理。
 * 注意：本类并不对字符串进行内存分配，只管理指针和长度。
 ******************************************************/

#include "StdAfx.h"
#include "StrPtrLen.h"
#include "ASSERT.h"

BYTE	StrPtrLen::sCaseInsensitiveMask[] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,				//0-9 
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,		//10-19 
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,		//20-29
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,		//30-39 
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,		//40-49
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59,		//50-59
	60, 61, 62, 63, 64, 97, 98, 99, 100, 101,	//60-69 //stop on every character except a letter
	102, 103, 104, 105, 106, 107, 108, 109, 110, 111,	//70-79
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121,	//80-89
	122, 91, 92, 93, 94, 95, 96, 97, 98, 99,			//90-99
	100, 101, 102, 103, 104, 105, 106, 107, 108, 109,	//100-109
	110, 111, 112, 113, 114, 115, 116, 117, 118, 119,	//110-119
	120, 121, 122, 123, 124, 125, 126, 127, 128, 129	//120-129
};

StrPtrLen::StrPtrLen() 
  : Ptr(NULL), Len(0)
{
}

StrPtrLen::StrPtrLen(char* sp)
  : Ptr(sp), Len(sp != NULL ? strlen(sp) : 0)
{
}

StrPtrLen::StrPtrLen(char *sp, UINT len)
  : Ptr(sp), Len(len) 
{
}

StrPtrLen::~StrPtrLen()
{
}

/*
void StrPtrLen::Delete()
{
	delete [] Ptr; 
	Ptr = NULL; 
	Len = 0; 
}
*/

char * StrPtrLen::ToUpper()
{
	for (UINT x = 0; x < Len ; x++)
	{
		Ptr[x] = ::toupper (Ptr[x]); 
	}
	return Ptr;
}

char * StrPtrLen::ToLower()
{
	for (UINT x = 0; x < Len ; x++)
	{
		Ptr[x] = ::tolower(Ptr[x]); 
	}
	return Ptr;
}

char * StrPtrLen::FindString(StrPtrLen *queryStr, StrPtrLen *outResultStr)
{
	ASSERT(queryStr != NULL);
	ASSERT(queryStr->Ptr != NULL);
	ASSERT(0 == queryStr->Ptr[queryStr->Len]);
	return FindStringCase(queryStr->Ptr, outResultStr,true);
}
		
char * StrPtrLen::FindStringIgnoreCase(StrPtrLen *queryStr, StrPtrLen *outResultStr)
{	
	ASSERT(queryStr != NULL);
	ASSERT(queryStr->Ptr != NULL); 
	ASSERT(0 == queryStr->Ptr[queryStr->Len]); 
	return FindStringCase(queryStr->Ptr, outResultStr,false); 
}

char * StrPtrLen::FindString(StrPtrLen *queryStr)
{	
	ASSERT(queryStr != NULL);
	ASSERT(queryStr->Ptr != NULL);
	ASSERT(0 == queryStr->Ptr[queryStr->Len]);
	return FindStringCase(queryStr->Ptr, NULL,true);
}
		
char * StrPtrLen::FindStringIgnoreCase(StrPtrLen *queryStr)
{
	ASSERT(queryStr != NULL);
	ASSERT(queryStr->Ptr != NULL);
	ASSERT(0 == queryStr->Ptr[queryStr->Len]); 
	return FindStringCase(queryStr->Ptr, NULL,false); 
}
																					
char * StrPtrLen::FindString(char *queryCharStr)
{ 
	return FindStringCase(queryCharStr, NULL,true);
}

char * StrPtrLen::FindStringIgnoreCase(char *queryCharStr)
{ 
	return FindStringCase(queryCharStr, NULL,false);
}

char * StrPtrLen::FindString(char *queryCharStr, StrPtrLen *outResultStr)
{ 
	return FindStringCase(queryCharStr, outResultStr,true);
}

char * StrPtrLen::FindStringIgnoreCase(char *queryCharStr, StrPtrLen *outResultStr)
{ 
	return FindStringCase(queryCharStr, outResultStr,false);
}

char * StrPtrLen::FindString(StrPtrLen &query, StrPtrLen *outResultStr)
{ 
	return FindString( &query, outResultStr);
}

char * StrPtrLen::FindStringIgnoreCase(StrPtrLen &query, StrPtrLen *outResultStr)
{ 
	return FindStringIgnoreCase( &query, outResultStr);
}

char * StrPtrLen::FindString(StrPtrLen &query)
{ 
	return FindString( &query);
}

char * StrPtrLen::FindStringIgnoreCase(StrPtrLen &query)
{ 
	return FindStringIgnoreCase( &query);
}
		
StrPtrLen& StrPtrLen::operator=(const StrPtrLen& newStr)
{ 
	Ptr = newStr.Ptr; 
	Len = newStr.Len;
	return *this; 
}

char StrPtrLen::operator[](int i) 
{ 
	/*ASSERT(i<Len);i*/
	return Ptr[i];
}

void StrPtrLen::Set(char* inPtr, UINT inLen)
{ 
	Ptr = inPtr;
	Len = inLen;
}

void StrPtrLen::Set(char* inPtr) 
{ 
	Ptr = inPtr; 
	Len = (inPtr) ?  ::strlen(inPtr) : 0;
}

BOOL StrPtrLen::Equal(const StrPtrLen &compare) const
{
	if (NULL == compare.Ptr && NULL == Ptr )
		return true;
		
	if (NULL == compare.Ptr)
		return false;

	if ((compare.Len == Len) && (memcmp(compare.Ptr, Ptr, Len) == 0))
		return true;
	else
		return false;
}

BOOL StrPtrLen::Equal(const char* compare, const UINT len) const
{	
	if (NULL == compare && NULL == Ptr )
		return true;
		
	if (NULL == compare)
		return false;
		
	if ((len == Len) && (memcmp(compare, Ptr, Len) == 0))
		return true;
	else
		return false;
}

BOOL StrPtrLen::NumEqualIgnoreCase(const char* compare, const UINT len) const
{
	// compare thru the first "len: bytes
	ASSERT(compare != NULL);
	
	if (len <= Len)
	{
		for (UINT x = 0; x < len; x++)
		{
			if (sCaseInsensitiveMask[Ptr[x]] != sCaseInsensitiveMask[compare[x]])
				return false;
		}
		return true;
	}
	return false;
}

BOOL StrPtrLen::EqualIgnoreCase(const char* compare, const UINT len) const
{
	ASSERT(compare != NULL);
	if (len == Len)
	{
		for (UINT x = 0; x < len; x++)
		{
			if (sCaseInsensitiveMask[Ptr[x]] != sCaseInsensitiveMask[compare[x]])
				return false;
		}
		return true;
	}
	return false;
}

char *StrPtrLen::FindStringCase(char *queryCharStr, StrPtrLen *resultStr, BOOL caseSensitive) const
{
	//
	// Be careful about exiting this method from the middle. This routine deletes allocated memory at the end.
	// 
	if (resultStr)
	{
		resultStr->Set(NULL,0);
	}
	ASSERT (NULL != queryCharStr);
	if (NULL == queryCharStr) return NULL;
	if (NULL == Ptr) return NULL;
	if (0 == Len) return NULL;
	

	StrPtrLen queryStr(queryCharStr);
	char *editSource = NULL;
	char *resultChar = NULL;
	char lastSourceChar = Ptr[Len];
	
	if (lastSourceChar != 0)			// need to modify for termination. 
	{
		editSource = new char[Len + 1]; // Ptr could be a static string so make a copy
		::memcpy( editSource, Ptr, Len );
		editSource[Len] = 0;			// this won't work on static strings so we are modifing a new string here
	}

	char *queryString = queryCharStr;		
	char *dupSourceString = NULL;
	char *dupQueryString = NULL;
	char *sourceString = Ptr;
	UINT foundLen = 0;
	
	if (editSource != NULL)				// a copy of the source ptr and len 0 terminated
		sourceString = editSource;
	
	if (!caseSensitive)
	{
		dupSourceString = ::strdup(sourceString);
		dupQueryString = ::strdup(queryCharStr);				
		if (dupSourceString && dupQueryString) 
		{	
			sourceString = StrPtrLen(dupSourceString).ToUpper();
			queryString = StrPtrLen(dupQueryString).ToUpper();
			resultChar = ::strstr(sourceString,queryString);
		}
	}
	else
	{	
		resultChar = ::strstr(sourceString,queryString);		
	}
	
	if (resultChar != NULL)				// get the start offset
	{
		foundLen = resultChar - sourceString;
		resultChar = Ptr + foundLen;	// return a pointer in the source buffer
		if (resultChar > (Ptr + Len))	// make sure it is in the buffer
			resultChar = NULL;
	}
	
	if (dupSourceString != NULL)		// (2002.09.09)Omega modify
		::free(dupSourceString);
	if (dupQueryString != NULL) 
		::free(dupQueryString);
	
	if (editSource != NULL)  
		delete [] editSource;
	
	if (resultStr != NULL && resultChar != NULL)
		resultStr->Set(resultChar,queryStr.Len);
	
#if STRPTRLENTESTING	
	printf("StrPtrLen::FindStringCase found string=%s\n",resultChar);
#endif

	return resultChar;
}
//
// convert to a "NEW'd" zero terminated char array
//
char * StrPtrLen::GetAsCString() const
{
	// convert to a "NEW'd" zero terminated char array
	// caler is reponsible for the newly allocated memory
	char *theString = new char[Len+1];
	
	if ( Ptr && Len > 0 )
	{
		::memcpy( theString, Ptr, Len );
	}
	theString[Len] = 0;
	
	return theString;
}
#if STRPTRLENTESTING
BOOL	StrPtrLen::Test()
{
	static char* test1 = "2347.;.][';[;]abcdefghijklmnopqrstuvwxyz#%#$$#";
	static char* test2 = "2347.;.][';[;]ABCDEFGHIJKLMNOPQRSTUVWXYZ#%#$$#";
	static char* test3 = "Content-Type:";
	static char* test4 = "cONTent-TYPe:";
	static char* test5 = "cONTnnt-TYPe:";
	static char* test6 = "cONTent-TY";
	
	static char* test7 = "ontent-Type:";
	static char* test8 = "ONTent-TYPe:";
	static char* test9 = "-TYPe:";
	static char* test10 = ":";
	StrPtrLen theVictim1(test1, strlen(test1));

	if (!theVictim1.EqualIgnoreCase(test2, strlen(test2)))
		return false;
		
	if (theVictim1.EqualIgnoreCase(test3, strlen(test3)))
		return false;
	if (!theVictim1.EqualIgnoreCase(test1, strlen(test1)))
		return false;

	StrPtrLen theVictim2(test3, strlen(test3));
	if (!theVictim2.EqualIgnoreCase(test4, strlen(test4)))
		return false;
	if (theVictim2.EqualIgnoreCase(test5, strlen(test5)))
		return false;
	if (theVictim2.EqualIgnoreCase(test6, strlen(test6)))
		return false;
		
	StrPtrLen outResultStr;
	if (!theVictim1.FindStringIgnoreCase(test2, &outResultStr))
		return false;
	if (theVictim1.FindStringIgnoreCase(test3, &outResultStr))
		return false;
	if (!theVictim1.FindStringIgnoreCase(test1, &outResultStr))
		return false;
	if (!theVictim2.FindStringIgnoreCase(test4))
		return false;
	if (theVictim2.FindStringIgnoreCase(test5))
		return false;
	if (!theVictim2.FindStringIgnoreCase(test6))
		return false;
	if (!theVictim2.FindStringIgnoreCase(test7))
		return false;
	if (!theVictim2.FindStringIgnoreCase(test8))
		return false;
	if (!theVictim2.FindStringIgnoreCase(test9))
		return false;
	if (!theVictim2.FindStringIgnoreCase(test10))
		return false;

	if (theVictim1.FindString(test2, &outResultStr))
		return false;
	if (theVictim1.FindString(test3, &outResultStr))
		return false;
	if (!theVictim1.FindString(test1, &outResultStr))
		return false;
	if (theVictim2.FindString(test4))
		return false;
	if (theVictim2.FindString(test5))
		return false;
	if (theVictim2.FindString(test6))
		return false;
	if (!theVictim2.FindString(test7))
		return false;
	if (theVictim2.FindString(test8))
		return false;
	if (theVictim2.FindString(test9))
		return false;
	if (!theVictim2.FindString(test10))
		return false;
	
	StrPtrLen query;
	query.Set(test2);
	if (theVictim1.FindString(query, &outResultStr))
		return false;
	if (outResultStr.Len > 0)
		return false;
	if (outResultStr.Ptr != NULL)
		return false;
		
	query.Set(test3);
	if (theVictim1.FindString(query, &outResultStr))
		return false;
	if (outResultStr.Len > 0)
		return false;
	if (outResultStr.Ptr != NULL)
		return false;
		
	query.Set(test1);
	if (!theVictim1.FindString(query, &outResultStr))
		return false;
	if (!outResultStr.Equal(query))
		return false;
		
	query.Set(test4);
	if (query.Equal(theVictim2.FindString(query)))
		return false;
	
	query.Set(test5);
	if (query.Equal(theVictim2.FindString(query)))
		return false;

	query.Set(test6);
	if (query.Equal(theVictim2.FindString(query)))
		return false;

	query.Set(test7);
	if (!query.Equal(theVictim2.FindString(query)))
		return false;

	query.Set(test8);
	if (query.Equal(theVictim2.FindString(query)))
		return false;

	query.Set(test9);
	if (query.Equal(theVictim2.FindString(query)))
		return false;

	query.Set(test10);
	if (!query.Equal(theVictim2.FindString(query)))
		return false;
	
	query.Set(test10);
	if (!query.Equal(theVictim2.FindString(query)))
		return false;
	
	StrPtrLen partialStaticSource(test1,5);
	query.Set("abcd");
	if (query.Equal(partialStaticSource.FindString(query)))
		return false;
		
	query.Set("47");
	if (query.Equal(partialStaticSource.FindString(query))) // success = !equal because the char str is longer than len
		return false;
	
	if (query.FindString(partialStaticSource.FindString(query))) // success = !found because the 0 term src is not in query
		return false;

	partialStaticSource.FindString(query,&outResultStr);
	if (!outResultStr.Equal(query)) // success =found the result Ptr and Len is the same as the query
		return false;

	return true;
}
#endif
