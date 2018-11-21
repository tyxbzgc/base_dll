// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_STRINGS_STRINGOPERATE_H_
#define BASE_STRINGS_STRINGOPERATE_H_

#include "base\base_export.h"

namespace base {
namespace string_utils {
  /**
  * @name 分割字符串
  * @{
  */
  /**
  * @brief
  * @param[in] s 源字符串
  * @param[out] v 被分割后放入的vector
  * @param[in] c 分割的字符
  * @note
  * @return 无
  */
  void BASE_EXPORT SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

  /**
  * @name 字符串替换
  * @{
  */
  /**
  * @brief 用strTarget 字符串替换strText 字符串中所有strSource 字符串
  * @param[in] strText 源字符串
  * @param[in] strSource 被替换的字符串
  * @param[in] strTarget 替换成的字符串
  * @note 替换成功后strText输出
  * @return 替换次数
  */
  size_t BASE_EXPORT ReplaceString(std::wstring &strText, const std::wstring& strSource, const std::wstring& strTarget);

  /*@}*/

  /**
  * @name 字符串裁剪、分割
  * @{
  */
  /**
  * @brief 裁剪去掉strText头尾部用chSpecial 指定的字符
  * @param[in] strText 被裁剪的字符串
  * @param[out] chOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @param[in] chSpecial 裁剪字符
  * @return 裁剪后strText字符串长度，若返回值<0，则表示参数有误
  * @note 字符大小写敏感
  * @par 示例
  * @code
  * wstring wstr = L"find result afff";
  * wchar_t wch = L'f';
  * wchar output[20];
  * size_t n = TrimString(wstr, output, sizeof(output)/sizeof(output[0]), wch);
  * output等于L"ind result a"
  * n等于12
  * @endcode
  */
  size_t BASE_EXPORT TrimString(const std::wstring& strText, WCHAR* wchOutput, int nOutputSize, WCHAR chSpecial = L' ');
  /**
  * @brief 使用字符chSeparator分割字符串lpszStr，并分别存储在pStrList中，最大分割数为iMaxCount
  * @param[in] lpszStr 被分割的字符串
  * @param[in] chSpecial 分割字符
  * @param[out] pStrList 存放分割后的子字符串静态二维数组的首行指针，见下面的示例；不支持动态二维数组
  * @param[in] dwMaxLength    静态二维数组 pStrList 的列数，即存储一个子键名的数组缓冲区的长度
  * @param[out] dwStrListCnt	 返回存储至 pStrList 的子字符串数
  * @param[in] iMaxCount 最大子字符串数
  * @return 是否正确，主要是参数的值是否违法
  * @note 字符大小写敏感
  * @par 示例
  * @code
  * string str = "findrresultafff";
  * char a[5][20];
  * DWORD count;
  * BOOL ret = Base::StringUtils::SplitStringA(str.c_str(), 'r', a[0], 20, &count, 2);
  * a[0]等于"find",a[1]等于"resultafff"
  * count等于2
  * @endcode
  */
  BOOL BASE_EXPORT SplitStringA(LPCSTR lpszStr, char chSeparator, CHAR* pStrList, DWORD dwMaxLength, DWORD* dwStrListCnt, size_t iMaxCount = 0x7fffffff);

  /**
  * @brief 使用字符chSeparator分割字符串lpszStr，并分别存储在pStrList中，最大分割数为iMaxCount
  * @param[in] lpszStr 被分割的字符串
  * @param[in] chSpecial 分割字符
  * @param[out] pStrList 存放分割后的子字符串静态二维数组的首行指针，见下面的示例；不支持动态二维数组
  * @param[in] dwMaxLength    静态二维数组 pStrList 的列数，即存储一个子键名的数组缓冲区的长度
  * @param[out] dwStrListCnt	 返回存储至 pStrList 的子字符串数
  * @param[in] iMaxCount 最大子字符串数
  * @return 是否正确，主要是参数的值是否违法
  * @note 字符大小写敏感
  * @par 示例
  * @code
  * wstring str = L"findrresultafff";
  * wchar a[5][20];
  * DWORD count;
  * BOOL ret = Base::StringUtils::SplitStringW(str.c_str(), L'r', a[0], 20, &count, 2);
  * a[0]等于L"find",a[1]等于L"resultafff"
  * count等于2
  * @endcode
  */
  BOOL BASE_EXPORT SplitStringW(LPCWSTR lpszStr, WCHAR chSeparator, WCHAR* pStrList, DWORD dwMaxLength, DWORD* dwStrListCnt, size_t iMaxCount = 0x7fffffff);

  /*@}*/

  /**
  * @name 字符串搜索
  * @{
  */
  /**
  * @brief 通过前向搜索 toStrip 字符，截断前面搜索到了字符，获取后段字符串
  * @param[in] source 源字符串
  * @param[in] toStrip 截断字符
  * @param[out] wchBuffer 存储缓冲区：存储截断前面搜索到的toStrip后的后段字符串
  * @param[in] nBufferSize 缓冲区长度
  * @return 若成功，则返回缓冲区 wchBuffer 的地址， 若失败，则返回NULL
  */
  BASE_EXPORT WCHAR* StripLeading(const std::wstring& source, const WCHAR toStrip, WCHAR*wchBuffer, int nBufferSize);

  /**
  * @brief 通过后向搜索 toStrip 字符，截断后面搜索到了字符，获取前段字符串
  * @param[in] source 源字符串
  * @param[in] toStrip 截断字符
  * @param[out] wchBuffer 存储缓冲区：存储截断后面搜索到的toStrip后的前段字符串
  * @param[in] nBufferSize 缓冲区长度
  * @return 若成功，则返回缓冲区 wchBuffer 的地址， 若失败，则返回NULL
  */
  BASE_EXPORT WCHAR* StripTrailing(const std::wstring& source, const WCHAR toStrip, WCHAR*wchBuffer, int nBufferSize);

  /*@}*/


  /**
  * @name 字符串大小写转换
  * @{
  */
  /**
  * @brief 把 str 字符串转换成大写字符串:宽字节版本
  * @param[in] str 需求转换的字符串
  * @param[out] wchOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  * @par 示例
  * @code
  * wstring str = L"abc";
  * wchar output[20];
  * ToUpperW(str, output, sizeof(output)/sizeof(output[0]));
  * output 等于 ABC
  * @endcode
  */
  BASE_EXPORT WCHAR* ToUpperW(const std::wstring& str, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief 把 str 字符串转换成大写字符串：单字节版本
  * @param[in] str 需求转换的字符串
  * @param[out] chOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  * @par 示例
  * @code
  * string str = "abc";
  * char output[20];
  * ToUpperA(str, output, sizeof(output)/sizeof(output[0]));
  * output 等于 ABC
  * @see ToUpperW
  */
  BASE_EXPORT CHAR* ToUpperA(const std::string& str, CHAR* chOutput, int nOutputSize);

  /**
  * @brief 把 str 字符串转换成小写字符串：宽字节版本
  * @param[in] str 需求转换的字符串
  * @param[out] wchOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  * @par 示例
  * @code
  * wstring str = L"ABC";
  * wchar output[20];
  * ToLowerW(str, output, sizeof(output)/sizeof(output[0]));
  * output 等于 abc
  * @endcode
  */
  BASE_EXPORT WCHAR* ToLowerW(const std::wstring& str, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief 把 str 字符串转换成小写字符串:单字节版本
  * @param[in] str 需求转换的字符串
  * @param[out] wchOutput 存储转换后字符串的缓冲区
  * @param[in] nOutputSize 缓冲区长度
  * @return 若成功，返回转换后字符串的首地址，若失败，返回空字符串
  * @par 示例
  * @code
  * string str = "ABC";
  * char output[20];
  * ToLowerA(str, output, sizeof(output)/sizeof(output[0]));
  * output 等于 abc
  * @endcode
  * @see ToLowerW
  */
  BASE_EXPORT CHAR* ToLowerA(const std::string& str, CHAR* chOutput, int nOutputSize);

  /*@}*/


  /**
  * @name 字符串匹配/比较
  * @{
  */
  /**
  * @brief 模糊匹配
  * @param[in] wild   模糊匹配字符串
  * @param[in] string 要匹配的字符串
  * @param[in] bCaseSensitive 是否区别大小写
  * @return 是否匹配成功
  * @note
  修改为不会再修改传入的字符串，原来的程序逻辑没有修改。
  该实现的逻辑是有问题的，因为还在用，所以没有去除，类似模糊匹配的需求可以考虑使用boost::regex。
  */
  BOOL BASE_EXPORT Wildcmp(LPCTSTR wild, LPCTSTR string, BOOL bCaseSensitive);

  /*@}*/

  /**
  * @name 字符串有效性判断
  * @{
  */
  /**
  * @brief 检查字符的有效性，包括26个大小写字母、0~9及_-.@符号
  * @param[in] ch 字符
  * @return 是否有效
  */
  BOOL BASE_EXPORT IsValidChar(TCHAR ch);

  /**
  * @brief EMAIL有效性验证
  * @param[in] pszEmail 邮箱地址
  * @return 是否有效
  */
  BOOL BASE_EXPORT IsValidEmail(LPCTSTR pszEmail);

  /*@}*/


  /**
  * @name 单字节 or 宽字节
  * @{
  */
  /**
  * @brief ANSI代码页一个字符的最大字节长度
  * @return Returns 1 if the ANSI code page is single-byte, or 2 if it is double-byte (DBCS)
  */
  int BASE_EXPORT GetMaxCharSize(void);

  /*@}*/


  /**
  * @name 字符串长度
  * @{
  */
  /**
  * @brief 计算C-style单字节字符串的长度
  * @param[in] str
  * @return 返回字符串长度，不包括结束符
  */
  inline int BASE_EXPORT Strlen(const char *str) { return (int)strlen(str); }
  /**
  * @brief 计算C-style宽字节字符串的长度
  * @param[in] str
  * @return 返回字符串长度，不包括结束符
  */
  inline int BASE_EXPORT Strlen(const wchar_t *str) { return (int)wcslen(str); }

  /*@}*/


  /**
  * @name 字符串拷贝
  * @{
  */
  /**
  * @brief 将字符串src拷贝至dst
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] src 源字符串
  * @return 返回拷贝的字符个数，不包括结束符0
  * @note
  返回值 <=size-1. 如果size等于0, 则返回0并不做任何事.
  dst中的结果以0为结束符.
  */
  int BASE_EXPORT Strcpy(char *dst, int size, const char *src);
  /**
  * @brief 将字符串src拷贝至dst
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] src 源字符串
  * @return 返回拷贝的字符个数，不包括结束符0
  * @note
  返回值 <=size-1. 如果size等于0, 则返回0并不做任何事.
  dst中的结果以0为结束符.
  */
  int BASE_EXPORT Strcpy(wchar_t *dst, int size, const wchar_t *src);


  /**
  * @brief 复制字符串src的len个字符至dst，总是附加结束符0
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] src 源字符串
  * @parma[in] len 最多拷贝的字符个数
  * @return 返回实际拷贝的字符个数
  */
  int BASE_EXPORT Strncpy(char *dst, int size, const char *src, int len);
  /**
  * @brief 复制字符串src的len个字符至dst，总是附加结束符0
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] src 源字符串
  * @parma[in] len 最多拷贝的字符个数
  * @return 返回实际拷贝的字符个数
  */
  int BASE_EXPORT Strncpy(wchar_t *dst, int size, const wchar_t *src, int len);

  /*@}*/

  /**
  * @name 字符串追加
  * @{
  */
  /**
  * @brief 将src追加至dst结尾处
  * @param[out] dst 目标字符串缓冲区
  * @param[in] size 缓冲区dst的字节大小，包括结束符0
  * @param[in] src 源字符串
  * @return 返回实际拷贝的字符个数，不包括结束符0
  * @note
  返回值 <=size-1-strlen(dst). If size>=strlen(dst), returns 0 and does nothing.
  dst中的结果以0为结束符.
  */
  int BASE_EXPORT Strcat(char *dst, int size, const char *src);
  /**
  * @brief 将src追加至dst结尾处
  * @param[out] dst 目标字符串缓冲区
  * @param[in] size 缓冲区dst的字节大小，包括结束符0
  * @param[in] src 源字符串
  * @return 返回实际拷贝的字符个数，不包括结束符0
  * @note
  返回值 <=size-1-Strlen(dst). If size>=Strlen(dst), returns 0 and does nothing.
  dst中的结果以0为结束符.
  */
  int BASE_EXPORT Strcat(wchar_t *dst, int size, const wchar_t *src);

  /*@}*/

  /**
  * @name 字符串格式化
  * @{
  */
  /**
  * @brief 写格式化字符串至dst中
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  * @return 返回实际写的字符的个数，不包括结束符0
  * @note
  The return value is <=size-1. If size is 0, returns 0 and does nothing.
  The result in dst is always 0 terminated.
  */
  int BASE_EXPORT _cdecl Sprintf(char *dst, int size, const char *format, ...);
  /**
  * @brief 写格式化字符串至dst中
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  * @return 返回实际写的字符的个数，不包括结束符0
  * @see int BASE_EXPORT _cdecl Sprintf( char *dst, int size, const char *format, ... );
  */
  int BASE_EXPORT _cdecl Sprintf(wchar_t *dst, int size, const wchar_t *format, ...);
  /**
  * @brief 写格式化字符串至dst中
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] format 参数输出的格式
  * @param[in] args VA_LIST型的变量,这个变量是指向不定参数的指针
  * @return 返回实际写的字符的个数，不包括结束符0
  * @note
  The return value is <=size-1. If size is 0, returns 0 and does nothing.
  The result in dst is always 0 terminated.
  */
  int BASE_EXPORT _cdecl Vsprintf(char *dst, int size, const char *format, va_list args);
  /**
  * @brief 写格式化字符串至dst中
  * @param[out] dst 目标缓冲区
  * @param[in] size 缓冲区dst的字符个数，包括结束符0
  * @param[in] format 参数输出的格式
  * @param[in] args VA_LIST型的变量,这个变量是指向不定参数的指针
  * @return 返回实际写的字符的个数，不包括结束符0
  * @see 	int BASE_EXPORT _cdecl Vsprintf( char *dst, int size, const char *format, va_list args );
  */
  int BASE_EXPORT _cdecl Vsprintf(wchar_t *dst, int size, const wchar_t *format, va_list args);

  /*@}*/


  /**
  * @name 错误信息
  * @{
  */
  /**
  * @brief 根据错误ID得到错误信息
  * @param[in] last_error 错误ID
  * @return 错误信息
  */
  BOOL BASE_EXPORT GetLastErrorMessage(DWORD last_error, WCHAR* wchBuffer, int nBufferSize);

  /*@}*/

}  // namespace string_utils
}  // namespace base
#endif  //#define BASE_STRINGS_STRINGOPERATE_H_
