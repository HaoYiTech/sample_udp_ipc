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

#ifndef	_STRPTRLEN_H
#define	_STRPTRLEN_H

//#include "OSHeaders.h"

#define STRPTRLENTESTING 0

class StrPtrLen
{
	public:
		StrPtrLen();
		StrPtrLen(char * sp);
		StrPtrLen(char * sp, UINT len);
		~StrPtrLen();
		//
		//OPERATORS:
		//
		BOOL Equal(const StrPtrLen &compare) const;
		BOOL Equal(const char* compare, const UINT len) const;
		BOOL EqualIgnoreCase(const char* compare, const UINT len) const;
		BOOL EqualIgnoreCase(const StrPtrLen &compare) const { return EqualIgnoreCase(compare.Ptr, compare.Len); }
		BOOL NumEqualIgnoreCase(const char* compare, const UINT len) const;
		
		//void Delete();
		char *ToUpper();
		char *ToLower();
		
		char *FindStringCase(char *queryCharStr, StrPtrLen *resultStr, BOOL caseSensitive) const;

		char *FindString(StrPtrLen *queryStr, StrPtrLen *outResultStr);
		
		char *FindStringIgnoreCase(StrPtrLen *queryStr, StrPtrLen *outResultStr);

		char *FindString(StrPtrLen *queryStr);
		
		char *FindStringIgnoreCase(StrPtrLen *queryStr);
																					
		char *FindString(char *queryCharStr);
		char *FindStringIgnoreCase(char *queryCharStr);
		char *FindString(char *queryCharStr, StrPtrLen *outResultStr);
		char *FindStringIgnoreCase(char *queryCharStr, StrPtrLen *outResultStr);

		char *FindString(StrPtrLen &query, StrPtrLen *outResultStr);
		char *FindStringIgnoreCase(StrPtrLen &query, StrPtrLen *outResultStr);
		char *FindString(StrPtrLen &query);
		char *FindStringIgnoreCase(StrPtrLen &query);
		
		StrPtrLen& operator=(const StrPtrLen& newStr);
        char operator[](int i);
		void Set(char* inPtr, UINT inLen);
		void Set(char* inPtr);
		char* GetAsCString() const;					// convert to a "NEW'd" zero terminated char array
		//
		//This is a non-encapsulating interface. The class allows you to access its data.
		//
		char* 		Ptr;
		UINT		Len;
#if STRPTRLENTESTING
		static BOOL	Test();
#endif
	private:
		static BYTE 	sCaseInsensitiveMask[];
};
#endif	// _STRPTRLEN_H