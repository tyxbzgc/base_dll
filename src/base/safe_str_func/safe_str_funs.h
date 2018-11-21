/* -------------------------------------------------------------------------
//	FileName	：	SafeStrFuns.h
//	Author		：	yanjun
//	CrateTime	：	2010-9-25
//  Desc        ：	
//                  简单封装字符串操作函数，方便日后维护，以及操作失败后，debug下可以提示
//
// -----------------------------------------------------------------------*/
/**
* @cond Doxygen忽略该段代码
*/
#pragma once
#include <atldef.h>
#include <string.h>
#include <tchar.h>

class CSafeStrFuns
{
public:
	static bool SFStrCat(TCHAR *strDest, size_t bufferSizeInTCHAR, const TCHAR *strSource, size_t count = _TRUNCATE)
	{
		bool bR = false;
#if _MSC_VER <= 1200  // VC6及以下版本
		bR = ((_tcsncat(strDest, strSource, min(bufferSizeInTCHAR, count))) == 0);
#else
		bR = ((_tcsncat_s(strDest, bufferSizeInTCHAR, strSource, count)) == 0);
#endif
		if(!bR)
		{
			ATLASSERT(false);
		}

		return bR;
	}

	static bool SFStrCpy(TCHAR *strDest, size_t sizeInTCHAR, const TCHAR *strSource, size_t count = _TRUNCATE)
	{
		bool bR = false;
#if _MSC_VER <= 1200  // VC6及以下版本
		bR  = (_tcsncpy(strDest, strSource, min(sizeInTCHAR, count)) == 0);
#else
		bR  = ((_tcsncpy_s(strDest, sizeInTCHAR, strSource, count)) == 0);
#endif
		if(!bR)
		{
			ATLASSERT(false);
		}

		return bR;
	}
};

#ifndef TCHAR_BUFFER_STRCAT
#define TCHAR_BUFFER_STRCAT(DEST, SRC) CSafeStrFuns::SFStrCat(DEST, sizeof(DEST)/sizeof(TCHAR), SRC, _TRUNCATE)
#endif

#ifndef TCHAR_POINT_STRCAT
#define TCHAR_POINT_STRCAT(DEST, DEST_COUNT, SRC) CSafeStrFuns::SFStrCat(DEST, DEST_COUNT, SRC, _TRUNCATE)
#endif

#ifndef TCHAR_BUFFER_STRCPY
#define TCHAR_BUFFER_STRCPY(DEST, SRC) CSafeStrFuns::SFStrCpy(DEST, sizeof(DEST)/sizeof(TCHAR), SRC, _TRUNCATE)
#endif

#ifndef TCHAR_POINT_STRCPY
#define TCHAR_POINT_STRCPY(DEST, DEST_COUNT, SRC) CSafeStrFuns::SFStrCpy(DEST, DEST_COUNT, SRC, _TRUNCATE)
#endif

/**
* @endcond
*/
