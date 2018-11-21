// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_STRINGS_STRINGUTLS_H_
#define BASE_STRINGS_STRINGUTLS_H_

#include "base\base_export.h"
#include "base\stldef.h"

//Enable "CString"
#include <minmax.h> //���漸��ͷ�ļ���Ҫ
//#include "atlbase.h"
//#include "base\WTL80\atlapp.h"
//#include "base\WTL80\atlmisc.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <tchar.h>

// If defined, enables the use of WideCharToMultiByte and MultiByteToWideChar
// If not defined, wcstombs and mbstowcs are used instead. they always use the default code page
// and don't support surrogate pairs
//#define STR_USE_WIN32_CONV
//
//// If defined, enables the use of Windows localization functions - GetNumberFormat, GetTimeFormat, etc
//// If not defined, then the numbers, currency, time and date use fixed formats
//#define STR_USE_WIN32_NLS
//
//// If defined, enables support for SYSTEMTIME, FILETIME and DATE
//// If not defined, only time_t is supported as a time format
//#define STR_USE_WIN32_TIME
//
// If defined, enables the use of IsDBCSLeadByte to support DBCS code pages
// If not defined, isleadbyte is used instead
#define STR_USE_WIN32_DBCS
//
//// enables support for STL strings and streams
//#define STR_USE_STL


// _countof: compute the number of elements in a statically-allocated array
// VS2005 supports this, but earlier versions do not
#ifndef _countof
/* ��������Ĵ�С */
#define _countof(x) (sizeof(x)/sizeof((x)[0]))
#endif

/** ��������ID��*/
//#define CP_GB2312 936

//�ַ�������
#include "string_operate.h"
//FormatString
#include "string_format.h"
//�ַ���ת��
#include "string_convert.h"

#endif  // BASE_STRINGS_STRINGUTLS_H_
