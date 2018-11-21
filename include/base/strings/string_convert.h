// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_STRINGS_STRINGCONVERT_H_
#define BASE_STRINGS_STRINGCONVERT_H_

#include <time.h>
#include "base/base_export.h"

namespace base {

/**
* @brief 字符串模块采用的二级命名空间
*/
namespace string_utils {
  /**
  * @name Byte
  * @{
  */
  /**
  * @brief 把BYTE 类型的参数c 转换成以十六进制形式存储的字符串
  * @param[in] c 一个无符号字节数值
  * @param[out] wchBuffer 存储转换后的十六进制形式字符串的缓冲区
  * @param[in] nBufferSize 缓冲区长度
  * @return 写入缓冲区 wchBuffer 的字符串的字符个数，不包括终止符；若返回值<0，则表示错误
  * @par 示例
  * @code
  * wchar buffer[5];
  * int n = ByteToHexW(255, buffer, 5);
  * buffer 等于FF, n 2等于
  * @endcode
  */
  int BASE_EXPORT ByteToHexW(BYTE c, WCHAR* wchBuffer, int nBufferSize);
  /**
  * @brief 把BYTE 类型的参数c 转换成以十六进制形式存储的字符串
  * @param[in] c 一个无符号字节数值
  * @param[out] chBuffer 存储转换后的十六进制形式字符串的缓冲区
  * @param[in] nBufferSize 缓冲区长度
  * @return 写入缓冲区 chBuffer 的字符串的字符个数，不包括终止符；若返回值<0，则表示错误
  * @par 示例
  * @code
  * char buffer[5];
  * int n = ByteToHexA(255, buffer, 5);
  * buffer 等于FF, n 2等于
  * @endcode
  */
  int BASE_EXPORT ByteToHexA(BYTE c, CHAR* chBuffer, int nBufferSize);

  /**
  * @brief 把BYTE字节流数组转换成十六进制格式字符串
  * @param[in] pBuffer BYTE字节流内存缓存
  * @param[in] iBytes 缓存长度
  * @param[out] wchBuffer 存储转换后字符串的缓冲区
  * @param[in] nBufferSize 缓冲区长度
  * @return 字符串的长度，不包括终止符
  * @par 示例
  * @code
  * BYTE Buffer[3] = {10, 11, 12};
  * wchar dst[10];
  * int n = BytesToStringW(Buffer, sizeof(Buffer), dst, 10);
  * dst 等于L"0A0B0C"，n 等于6
  * @endcode
  */
  int BASE_EXPORT BytesToStringW(const BYTE *pBuffer, size_t iBytes, WCHAR*wchBuffer, int nBufferSize);	//原HexToString

                                                                                                      /**
                                                                                                      * @brief 把以十六进制形式存储的字符串转换成字节流
                                                                                                      * @param[in] str 以十六进制形式存储的字符串
                                                                                                      * @param[out] pBuffer 存储转换后的字节流的数组缓存
                                                                                                      * @param[in] nBytes 数组缓存的长度
                                                                                                      * @par 示例
                                                                                                      * @code
                                                                                                      * BYTE Buffer[3] = {0};
                                                                                                      * std::wstring s = L"0A0B09";
                                                                                                      * StringToBytesW(s, Buffer, 3);
                                                                                                      * Buffer[0] 等于10
                                                                                                      * Buffer[1] 等于11
                                                                                                      * Buffer[2] 等于9
                                                                                                      * @endcode
                                                                                                      */
  void BASE_EXPORT StringToBytesW(const std::wstring &str, BYTE *pBuffer, size_t nBytes);//原StringToHex

                                                                                      /*@}*/


                                                                                      /**
                                                                                      * @name GUID
                                                                                      * @{
                                                                                      */
                                                                                      /**
                                                                                      * @brief GUID 数据结构转换成字符串
                                                                                      * @param[in] uid GUID
                                                                                      * @param[out] wchBuffer 存储字符串GUID的缓冲区
                                                                                      * @param[in] nBufferSize 缓冲区长度
                                                                                      * @return 写入缓冲区的字符串的长度
                                                                                      */
  int BASE_EXPORT GuidToStringW(::GUID uid, WCHAR* wchBuffer, int nBufferSize);
  /**
  * @brief GUID 数据结构转换成字符串
  * @param[in] uid GUID
  * @param[out] chBuffer 存储字符串GUID的缓冲区
  * @param[in] nBufferSize 缓冲区长度
  * @return 写入缓冲区的字符串的长度
  */
  int BASE_EXPORT GuidToStringA(::GUID uid, CHAR* chBuffer, int nBufferSize);

  /**
  * @brief 把以字符串形式存在的GUID 转换成GUID 数据结构
  * @param[in] szGuid 字符串形式存在的GUID
  * @param[out] uid GUID
  * @return 是否成功转换
  */
  BOOL BASE_EXPORT StringToGuid(LPCTSTR szGuid, ::GUID* uid);

  /*@}*/


  /**
  * @name MultiByte_Unicode
  * @{
  */
  /**
  * @brief 多字节编码转换成UNICODE编码
  * @param[in]  strMultiByte 多字节编码字符串
  * @param[out] strUnicode UNICODE编码字符串
  * @param[in]  uCodePage 代码页ID
  * @return 是否转换成功
  * @note 因传出参数类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 MultiByteToUnicode(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp)
  */
  BOOL MultiByteToUnicode(const std::string& strMultiByte, std::wstring& strUnicode, UINT uCodePage);

  /*
  * @brief 多字节编码转换成UNICODE编码
  * @param[in]   strMultiByte 多字节编码字符串
  * @param[in]   uCodePage 代码页ID,默认参数为CP_ACP
  * @return 返回转换成功后UNICODE编码字符串
  * @note 因返回类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 MultiByteToUnicode(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp)
  */
  std::wstring MultiByteToUnicode(const std::string& strMultiByte, UINT uCodePage = CP_ACP);

  /**
  * @brief 多字节编码转换成UNICODE编码
  * @param[out] lpw 存放转换后UNICODE字符串的缓冲区
  * @param[in] lpa 源多字节编码字符串
  * @param[in] nChars 由参数 lpw 指向的缓冲区的宽字符个数
  * @param[in] acp 代码页ID
  * @return 返回转换后的UNICODE字符串
  */
  LPWSTR BASE_EXPORT WINAPI MultiByteToUnicode(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp);

  /**
  * @brief 多字节编码转换成UNICODE编码
  * @param[out] lpt 存放转换后UNICODE字符串的缓冲区
  * @param[in] lpa 源多字节编码字符串
  * @param[in] nChars 由参数lpt指向的缓冲区的宽字符个数
  * @note 代码页ID指定为CP_ACP
  * @return 返回转换后的UNICODE字符串
  */
  LPWSTR BASE_EXPORT WINAPI MultiByteToUnicode(LPWSTR lpt, LPCSTR lpa, int nChars);

  /*
  * @brief UNICODE编码转换成多字节编码
  * @param[in]  strUnicode UNICODE编码字符串
  * @param[out] strUTF8 多字节编码字符串
  * @parma[in]  uCodePage 代码页ID
  * @return 是否转换成功
  * @note 因传出参数类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp)
  */
  BOOL UnicodeToMultiByte(const std::wstring& strUnicode, std::string& strUTF8, UINT uCodePage); // CP_UTF8, CP_ACP

                                                                                                  /*
                                                                                                  * @brief UNICODE编码转换成多字节编码
                                                                                                  * @param[in]   strUnicode UNICODE编码字符串
                                                                                                  * @parma[in]   uCodePage 代码页ID,默认参数为CP_ACP
                                                                                                  * @return 返回转换成功后多字节编码字符串
                                                                                                  * @note 因返回类型为std::string，跨DLL调用存在风险，
                                                                                                  故不设为导出函数，请使用 UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp)
                                                                                                  */
  std::string UnicodeToMultiByte(const std::wstring& strUnicode, UINT uCodePage = CP_ACP);


  /**
  * @brief UNICODE编码转换成多字节编码
  * @param[out] lpa 存放转换后多字节编码字符串的缓冲区
  * @param[in] lpw 源UNICODE编码字符串
  * @param[in] nChars 由参数lpa指向的缓冲区的字符个数
  * @param[in] acp 代码页ID
  * @return 返回转换后的多字节字符串
  */
  LPSTR  BASE_EXPORT WINAPI UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp);

  /**
  * @brief UNICODE编码转换成多字节编码
  * @param[out] lpa 存放转换后多字节编码字符串的缓冲区
  * @param[in] lpt 源UNICODE编码字符串
  * @param[in] nChars 由参数lpa指向的缓冲区的字符个数
  * @note 代码页ID指定为CP_ACP
  * @return 返回转换后的多字节字符串
  */
  LPSTR BASE_EXPORT WINAPI  UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpt, int nChars);

  /*@}*/

  /**
  * @name ANSI_UTF8_convert_vector
  * @{
  */
  /**
  * @brief ANSI编码转换成UTF8编码
  * @param[in] input ANSI编码的待转换string
  * @param[in] buf 存放转换后单字节字符串的vector
  * @return 是否转换成功
  */
  BASE_EXPORT BOOL AToUTF8(const std::string& input, std::vector<char>& buf);

  /**
  * @name ANSI_UTF8_convert_char
  * @{
  */
  /**
  * @brief ANSI编码转换成UTF8编码
  * @param[in] input ANSI编码的待转换string
  * @param[in] buf 存放转换后单字节字符串的buf
  * @param[in] len 长度
  * @return 是否转换成功
  */
  BASE_EXPORT BOOL AToUTF8(const std::string& input, char * buf, int len);

  /**
  * @brief UTF8编码转换成ANSI编码
  * @param[in] input ANSI编码的待转换string
  * @param[in] buf 存放转换后单字节字符串的vector
  * @return 是否转换成功
  */
  BASE_EXPORT CHAR * UTF8ToA(const std::string& utf8Str, char * buf, int len);
  /*@}*/

  /**
  * @name String_Date
  * @{
  */
  /**
  * @brief 把以字符串形式存在的日期转换成time_t 结构体的tmDate
  * @param[in]  lpszDate 以字符串形式日期
  * @param[out] tmDate time_t 结构体的日期
  * @note 传入的日期字符串形如“2013-09-29”
  * @return 是否转换成功
  */
  BOOL BASE_EXPORT StringToDate(LPCTSTR lpszDate, time_t &tmDate);

  /*@}*/


  /**
  * @details 将支持strstream的类型的值转换为std::wstring。
  * Convert a type to a string by streaming it. Requires that there's an ostream
  * inserter available for type T.
  * @param[in] num 字符串
  * @return 转换后的字符串
  */
  template <class T>
  std::wstring ToStringW(T num) {
    std::wstring strNum = _T("");

    {
      std::strstream buf;

      buf << num << std::ends;

      //#ifdef _UNICODE
      std::string temp = buf.str();
      strNum = AToW(temp.c_str());
      //#else 
      //			strNum = buf.str();
      //#endif
      buf.freeze(false);
    }

    return strNum;
  }


  /**
  * @name ATo
  * @{
  */
  /**
  * @brief std::string字符串转换为std::wstring字符串
  * @param[in] input std::string格式字符串
  * @param[out] wchOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT WCHAR* AToW(const std::string &input, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief C风格字符串转换为std::wstring字符串
  * @param[in] pInput C风格字符串，必须以'\0'为字符串结束标识
  * @param[out] wchOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT WCHAR* AToW(const char *pInput, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief 单字节字符串转换为std::wstring字符串
  * @param[in] pInput 单字节字符串
  * @param[in] inputLength 由参数pInput指向的字符串中字节的个数
  * @param[out] wchOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT WCHAR* AToW(const char *pInput, const size_t inputLength, WCHAR* wchOutput, int nOutputSize);

  /**
  * @brief std::string字符串转换为BSTR格式字符串
  * @param[in] input std::string格式字符串
  * @return 转换后的BSTR格式字符串
  */
  BASE_EXPORT BSTR AToBSTR(const std::string &input);
  /**
  * @brief 单字节字符串转换为BSTR格式字符串
  * @param[in] pInput 单字节字符串
  * @param[in] inputLength 传入的字符串中字节的个数
  * @return 转换后的BSTR格式字符串
  */
  BASE_EXPORT BSTR AToBSTR(const char *pInput, const size_t inputLength);

  /*@}*/


  /**
  * @name WTo
  * @{
  */
  /**
  * @brief std::wstring字符串转换为std::string字符串
  * @param[in] input std::wstring格式字符串
  * @param[out] chOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT CHAR* WToA(const std::wstring &input, CHAR* chOutput, int nOutputSize);
  /**
  * @brief C风格宽字符串转换为std::string字符串
  * @param[in] pInput C风格宽字符串，必须以L'\0'为字符串结束标识
  * @param[out] chOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT CHAR* WToA(const wchar_t *pInput, CHAR* chOutput, int nOutputSize);
  /**
  * @brief 宽字节字符串转换为std::string字符串
  * @param[in] pInput 待转换的字符串的缓冲区
  * @param[in] inputLength 由pInput指向的缓冲区的字符个数
  * @param[out] chOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT CHAR* WToA(const wchar_t *pInput, const size_t inputLength, CHAR* chOutput, int nOutputSize);

  /**
  * @brief std::wstring字符串转换为BSTR格式字符串
  * @param[in] input std::wstring格式字符串
  * @return 转换后的BSTR格式字符串
  */
  BASE_EXPORT BSTR WToBSTR(const std::wstring &input);

  /*@}*/


  /**
  * @name BSTRto
  * @{
  */
  /**
  * @brief BSTR格式字符串转换为std::string字符串
  * @param[in] bstr BSTR格式字符串，必须以'\0'为字符串结束标识
  * @param[out] chOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT CHAR* BSTRToA(const BSTR bstr, CHAR* chOutput, int nOutputSize);
  /**
  * @brief BSTR格式字符串转换为std::wstring字符串
  * @param[in] bstr BSTR格式字符串，必须以'\0'为字符串结束标识
  * @return 转换后的std::wstring格式字符串
  */
  BASE_EXPORT WCHAR* BSTRToW(const BSTR bstr, WCHAR* wchOutput, int nOutputSize);

  /*@}*/



  /**
  * @name UTF8
  * @{
  */
  /**
  * @brief UTF8字符集的std::string转换为std::wstring字符串
  * @param[in] input UTF8字符集的std::string
  * @param[out] wchOutput 存储转换后的UTF8字符集字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  */
  BASE_EXPORT WCHAR* UTF8AToW(const std::string &input, WCHAR*wchOutput, int nOutputSize);

  /**
  * @brief UTF8字符集的宽字节字符串转换为单字节字符串，并将每个字符依次存放在vector成员中
  * @param[in] wcs UTF8字符集的宽字节字符串
  * @param[out] buf 存放转换后单字节字符串的vector
  * @return 是否转换成功
  */
  BASE_EXPORT BOOL UTF8WToA(const wchar_t* wcs, std::vector<char>& buf);

  /*@}*/



  //move from ConvertBase.h
  /**
  * @name ATo_with_CodePage
  * @{
  */
  /*
  * @brief 单字节字符串转换为std::wstring字符串
  * @param[in] szSrc 待转换的单字节字符串
  * @param[in] iSrcCount 由参数szSrc指向的字符串中字节的个数
  * @param[in] cpCodePage 代码页ID
  * @note 如果szSrc指定的字符串以空字符终止，iSrcCount可以设置为-1
  * @return 转换后的std::wstring字符串
  * @note 因返回类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 AToW( LPCSTR szSrc, const int iSrcCount,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  std::wstring AToW(LPCSTR szSrc, int iSrcCount, DWORD cpCodePage);

  /*
  * @brief std::string字符串转换为std::wstring字符串
  * @param[in] cstr 待转换的std::string字符串
  * @param[in] cpCodePage  代码页ID
  * @return 转换后的std::wstring字符串
  * @note 因返回类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 AToW( LPCSTR szSrc,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  std::wstring AToW(const std::string& cstr, DWORD cpCodePage);

  /**
  * @brief 单字节字符串转换为宽字节字符串
  * @param[in] szSrc 指向要转换的源字符串，必须以'\0'为结束标识，自动计算长度
  * @param[out] wszDst 字符串转换后存在它指向的内存中，指向的空间必须足够大，用以存储转换后的字符串
  * @param[in] iDstCount 传出参数指针wszDst所指向的内存块的大小
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  */
  BASE_EXPORT LPCWSTR AToW(LPCSTR szSrc, LPWSTR wszDst, const int iDstCount, DWORD cpCodePage);

  /**
  * @brief 单字节字符串转换为宽字节字符串
  * @param[in] szSrc 指向要转换的源字符串
  * @param[in] iSrcCount 源字符串字节个数
  * @param[out] wszDst 字符串转换后存在它指向的内存中，指向的空间必须足够大，用以存储转换后的字符串
  * @param[in] iDstCount 传出参数指针wszDst所指向的内存块的大小
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  */
  BASE_EXPORT LPCWSTR AToW(LPCSTR szSrc, const int iSrcCount, LPWSTR wszDst, const int iDstCount, DWORD cpCodePage);

  /*
  * @brief 单字节字符串转换为宽字节字符串
  * @param[in] szSrc 指向要转换的源字符串，必须以'\0'为结束标识，自动计算长度
  * @param[out] wszDst 字符串转换后的std::wstring字符串
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  * @note 因传出参数类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 AToW( LPCSTR szSrc,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  LPCWSTR AToW(LPCSTR szSrc, std::wstring& wsDst, DWORD cpCodePage);

  /*
  * @brief 单字节字符串转换为宽字节字符串
  * @param[in] szSrc 指向要转换的源字符串
  * @param[in] iSrcCount 源字符串长度
  * @param[out] wszDst 字符串转换后的std::wstring字符串
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  * @note 因传出参数类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 AToW( LPCSTR szSrc, const int iSrcCount,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  LPCWSTR AToW(LPCSTR szSrc, const int iSrcCount, std::wstring& wsDst, DWORD cpCodePage);

  /*@}*/


  /**
  * @name WTo_with_CodePage
  * @{
  */
  /*
  * @brief 宽字节字符串转换为std::string字符串
  * @param[in] wszSrc 待转换的宽字节字符串
  * @param[in] iSrcCount 由参数wszSrc指向的字符串中字符个数
  * @param[in] cpCodePage 代码页ID
  * @note 如果szSrc指定的字符串以空字符终止，iSrcCount可以设置为-1
  * @return 转换后的std::string字符串
  * @note 因返回类型为std::string，跨DLL调用存在风险，
  故不设为导出函数，请使用 WToA( LPCWSTR wszSrc, const int iSrcCount,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  std::string WToA(LPCWSTR wszSrc, const int iSrcCount, DWORD cpCodePage);

  /*
  * @brief std::wstring字符串转换为std::string字符串
  * @param[in] cwstr 待转换的std::wstring字符串
  * @param[in] cpCodePage  代码页ID
  * @return 转换后的std::string字符串
  * @note 因返回类型为std::string，跨DLL调用存在风险，
  故不设为导出函数，请使用 WToA( LPCWSTR wszSrc,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  std::string WToA(const std::wstring& cwstr, DWORD cpCodePage);

  /**
  * @brief 宽字节字符串转换为单字节字符串
  * @param[in] wszSrc 指向要转换的源字符串，必须以L'\0'为结束标识，自动计算长度
  * @param[out] pchDest 字符串转换后存在它指向的内存中，指向的空间必须足够大，用以存储转换后的字符串
  * @param[in] iDstCount 传出参数指针pchDest所指向的内存块的大小
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  */
  BASE_EXPORT LPCSTR WToA(LPCWSTR wszSrc, LPSTR pchDest, const int iDstCount, DWORD cpCodePage);

  /**
  * @brief 宽字节字符串转换为单字节字符串
  * @param[in] wszSrc 指向要转换的源字符串
  * @param[in] iSrcCount 源字符串字符个数
  * @param[out] pchDest 字符串转换后存在它指向的内存中，指向的空间必须足够大，用以存储转换后的字符串
  * @param[in] iDstCount 传出参数指针pchDest所指向的内存块的大小
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  */
  BASE_EXPORT LPCSTR WToA(LPCWSTR wszSrc, const int iSrcCount, LPSTR pchDest, const int iDstCount, DWORD cpCodePage);

  /*
  * @brief 宽字节字符串转换为单字节字符串
  * @param[in] wszSrc 指向要转换的源字符串，必须以L'\0'为结束标识，自动计算长度
  * @param[out] sDest 字符串转换后的std::string字符串
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  * @note 因传出参数类型为std::string，跨DLL调用存在风险，
  故不设为导出函数，请使用 WToA( LPCWSTR wszSrc,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  LPCSTR WToA(LPCWSTR wszSrc, std::string& sDest, DWORD cpCodePage);

  /*
  * @brief 宽字节字符串转换为单字节字符串
  * @param[in] wszSrc 指向要转换的源字符串
  * @param[in] iSrcCount 源字符串长度
  * @param[out] sDest 字符串转换后的std::string字符串
  * @param[in] cpCodePage 要转换的源字符串的代码页ID
  * @return 指向转换后的内存缓冲区
  * @note 因传出参数类型为std::string，跨DLL调用存在风险，
  故不设为导出函数，请使用 WToA( LPCWSTR wszSrc, const int iSrcCount,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  LPCSTR WToA(LPCWSTR wszSrc, const int iSrcCount, std::string& sDest, DWORD cpCodePage);

  /*@}*/

  inline std::wstring AToW(const std::string &input) {
    std::wstring result;

    if(!input.empty()) {
      int nNeedLen = ::MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.length(), NULL, 0);

      // allen 2014.8.26 fix 字符串堆损
      // result.resize(nNeedLen);
      WCHAR* pszOutput = new WCHAR[nNeedLen + 1];

      if(pszOutput) {
        int nReturnLen = ::MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.length(), pszOutput, nNeedLen);
        if(nReturnLen > 0) {
          result.assign(pszOutput, nReturnLen);
        }

        delete[] pszOutput;
        pszOutput = NULL;
      }
    }

    return result;
  }

  inline std::wstring AToW(const std::string &cstr, DWORD cpCodePage) {
    std::wstring result;

    if(!cstr.empty()) {
      int nNeedLen = ::MultiByteToWideChar(cpCodePage, 0, cstr.c_str(), cstr.length(), NULL, 0);

      // allen 2014.08.26 fix 字符串堆损
      // result.resize(nNeedLen);
      WCHAR* pszOutput(new WCHAR[nNeedLen + 1]);
      if(pszOutput) {
        int nReturnLen = ::MultiByteToWideChar(cpCodePage, 0, cstr.c_str(), cstr.length(), pszOutput, nNeedLen);
        if(nReturnLen >= 0) {
          result.assign(pszOutput, nReturnLen);
        }

        delete[] pszOutput;
        pszOutput = NULL;
      }
    }

    return result;
  }

  inline std::string WToA(const std::wstring &input) {
    std::string result;

    if(!input.empty()) {
      int nNeedLen = ::WideCharToMultiByte(CP_ACP, 0, input.c_str(), input.length(), NULL, 0, 0, 0);

      // allen 2014.08.26 fix 字符串堆损
      //result.resize(nNeedLen);
      char* pszOutput = new char[nNeedLen + 1];
      if(pszOutput) {
        int nReturnLen = ::WideCharToMultiByte(CP_ACP, 0, input.c_str(), input.length(), pszOutput, nNeedLen, 0, 0);
        if(nReturnLen > 0) {
          result.assign(pszOutput, nReturnLen);
        }

        delete[] pszOutput;
        pszOutput = NULL;
      }
    }

    return result;
  }

  inline std::string WToA(const std::wstring& cwstr, DWORD cpCodePage) {
    std::string result;

    if(!cwstr.empty()) {
      int nNeedLen = ::WideCharToMultiByte(cpCodePage, 0, cwstr.c_str(), cwstr.length(), NULL, 0, 0, 0);

      // allen 2014.08.26 fix 字符串堆损
      //result.resize(nNeedLen);
      char	* pszOutput = new char[nNeedLen + 1];
      if(pszOutput) {
        int nReturnLen = ::WideCharToMultiByte(cpCodePage, 0, cwstr.c_str(), cwstr.length(), pszOutput, nNeedLen, 0, 0);
        if(nReturnLen > 0) {
          result.assign(pszOutput, nReturnLen);
        }

        delete[] pszOutput;
        pszOutput = NULL;
      }
    }

    return result;
  }

  inline std::wstring UTF8AToW(const std::string &input) {
    const size_t inputLength = input.length();

    std::wstring result;

    if(inputLength != 0) {
      int nNeedLen = ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)inputLength, NULL, 0);

      // allen 2014.08.26 fix 字符串堆损
      // result.resize(nNeedLen);
      WCHAR* pwszOutput = new WCHAR[nNeedLen + 1];
      if(pwszOutput) {
        int nResultLen = ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), (int)inputLength, pwszOutput, nNeedLen);
        if(nResultLen > 0) {
          result.assign(pwszOutput, nResultLen);
        }

        delete[] pwszOutput;
        pwszOutput = NULL;
      }
    }

    return result;
  }

}  // namespace string_utils
}  // namespace base
#endif  // BASE_STRINGS_STRINGCONVERT_H_
