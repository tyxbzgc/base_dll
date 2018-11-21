// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef  BASE_STRINGS_STRINGFORMAT_H_
#define  BASE_STRINGS_STRINGFORMAT_H_

#include <stdarg.h>
#include "base/base_export.h"

namespace base {
namespace string_utils {
  /**
  * @name BUFFER
  * @{
  */
  /**
  * @brief 将单字节字符串以格式化方式存储到缓冲区中
  * @param[out] strDstBuffer 字符串格式化后字符串的缓冲区，确保缓冲区足够大
  * @param[in] nLenOfBuffer 参数strDstBuffer缓冲区长度
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  * @retval >=0 格式字符串的字符个数，不包括终止符
  * @retval <0 参数错误、缓冲区不足或格式化过程出错
  * @par 示例
  * @code
  * char buffer[20] = {0};
  * int i = 1;
  * char* ch = "Test";
  * int n = FormatString(buffer, 20, "[%d]:%s",i, ch );
  * buffer结果为"[1]:Test"
  * n等于8
  * @endcode
  */
  int BASE_EXPORT FormatString(char *strDstBuffer, int nLenOfBuffer, const char *format, ...);
  /**
  * @brief 将宽字节字符串以格式化方式存储到缓冲区中
  * @param[out] wstrDstBuffer 字符串格式化后宽字节字符串的缓冲区，确保缓冲区足够大
  * @param[in] nLenOfBuffer 参数strDstBuffer缓冲区长度
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  * @retval >=0 格式字符串的字符个数，不包括终止符
  * @retval <0 参数错误、缓冲区不足或格式化过程出错
  * @see int BASE_EXPORT FormatString( char *strDstBuffer, int nLenOfBuffer, const char *format, ... );
  */
  int BASE_EXPORT FormatString(wchar_t *wstrDstBuffer, int nLenOfBuffer, const wchar_t *format, ...);

  /*@}*/


  /**
  * @name STL_String
  * @{
  */
  /*
  * @brief 将单字节字符串格式化
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  * @return 格式化后的std::string字符串
  * @par 示例
  * @code
  * int i = 1;
  * char* ch = "Test";
  * std::string str= FormatString("[%d]:%s",i, ch );
  * str等于"[1]:Test"
  * @endcode
  * @note 因返回类型为std::string，跨DLL调用存在风险，
  故不设为导出函数，请使用 FormatString( std::string &string, const char *format, ... )

  std::string FormatString( const char *format, ... );
  */

  /*
  * @brief 将宽字节字符串格式化
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  * @return 格式化后的std::wstring字符串
  * @note 因返回类型为std::wstring，跨DLL调用存在风险，
  故不设为导出函数，请使用 FormatString( std::wstring &string, const wchar_t *format, ... )

  std::wstring FormatString( const wchar_t *format, ...);
  */

  /**
  * @brief 将单字节字符串格式化
  * @param[out] string 格式化后的std::string字符串
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  */
  void BASE_EXPORT FormatString(std::string &string, const char *format, ...);
  /**
  * @brief 将宽字节字符串格式化
  * @param[out] string 格式化后的std::wstring字符串
  * @param[in] format 参数输出的格式
  * @param[in] 不定参数
  */
  void BASE_EXPORT FormatString(std::wstring &string, const wchar_t *format, ...);

  /*@}*/


  /**
  * @name date/time formats
  * @{
  */
  /**
  * @brief 将time_t格式时间格式化为SYSTEMTIME格式时间
  * @param[out] stDstTime 格式化后的SYSTEMTIME格式时间
  * @param[in] ttSrcTime time_t格式的时间
  * @retval >=0 格式化成功
  * @retval <0 格式化失败
  */
  int BASE_EXPORT FormatToSysTime(SYSTEMTIME *stDstTime, const time_t *ttSrcTime);
  /**
  * @brief 将FILETIME格式时间格式化为SYSTEMTIME格式时间
  * @param[out] stDstTime 格式化后的SYSTEMTIME格式时间
  * @param[in] ftSrcTime FILETIME格式的时间
  * @retval >=0 格式化成功
  * @retval <0 格式化失败
  */
  int BASE_EXPORT FormatToSysTime(SYSTEMTIME *stDstTime, const FILETIME *ftSrcTime);


}  // namespace string_utils
}  // namespace base
#endif  // BASE_STRINGS_STRINGFORMAT_H_
