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
  * @brief �����ֽ��ַ����Ը�ʽ����ʽ�洢����������
  * @param[out] strDstBuffer �ַ�����ʽ�����ַ����Ļ�������ȷ���������㹻��
  * @param[in] nLenOfBuffer ����strDstBuffer����������
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  * @retval >=0 ��ʽ�ַ������ַ���������������ֹ��
  * @retval <0 �������󡢻�����������ʽ�����̳���
  * @par ʾ��
  * @code
  * char buffer[20] = {0};
  * int i = 1;
  * char* ch = "Test";
  * int n = FormatString(buffer, 20, "[%d]:%s",i, ch );
  * buffer���Ϊ"[1]:Test"
  * n����8
  * @endcode
  */
  int BASE_EXPORT FormatString(char *strDstBuffer, int nLenOfBuffer, const char *format, ...);
  /**
  * @brief �����ֽ��ַ����Ը�ʽ����ʽ�洢����������
  * @param[out] wstrDstBuffer �ַ�����ʽ������ֽ��ַ����Ļ�������ȷ���������㹻��
  * @param[in] nLenOfBuffer ����strDstBuffer����������
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  * @retval >=0 ��ʽ�ַ������ַ���������������ֹ��
  * @retval <0 �������󡢻�����������ʽ�����̳���
  * @see int BASE_EXPORT FormatString( char *strDstBuffer, int nLenOfBuffer, const char *format, ... );
  */
  int BASE_EXPORT FormatString(wchar_t *wstrDstBuffer, int nLenOfBuffer, const wchar_t *format, ...);

  /*@}*/


  /**
  * @name STL_String
  * @{
  */
  /*
  * @brief �����ֽ��ַ�����ʽ��
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  * @return ��ʽ�����std::string�ַ���
  * @par ʾ��
  * @code
  * int i = 1;
  * char* ch = "Test";
  * std::string str= FormatString("[%d]:%s",i, ch );
  * str����"[1]:Test"
  * @endcode
  * @note �򷵻�����Ϊstd::string����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� FormatString( std::string &string, const char *format, ... )

  std::string FormatString( const char *format, ... );
  */

  /*
  * @brief �����ֽ��ַ�����ʽ��
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  * @return ��ʽ�����std::wstring�ַ���
  * @note �򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� FormatString( std::wstring &string, const wchar_t *format, ... )

  std::wstring FormatString( const wchar_t *format, ...);
  */

  /**
  * @brief �����ֽ��ַ�����ʽ��
  * @param[out] string ��ʽ�����std::string�ַ���
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  */
  void BASE_EXPORT FormatString(std::string &string, const char *format, ...);
  /**
  * @brief �����ֽ��ַ�����ʽ��
  * @param[out] string ��ʽ�����std::wstring�ַ���
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  */
  void BASE_EXPORT FormatString(std::wstring &string, const wchar_t *format, ...);

  /*@}*/


  /**
  * @name date/time formats
  * @{
  */
  /**
  * @brief ��time_t��ʽʱ���ʽ��ΪSYSTEMTIME��ʽʱ��
  * @param[out] stDstTime ��ʽ�����SYSTEMTIME��ʽʱ��
  * @param[in] ttSrcTime time_t��ʽ��ʱ��
  * @retval >=0 ��ʽ���ɹ�
  * @retval <0 ��ʽ��ʧ��
  */
  int BASE_EXPORT FormatToSysTime(SYSTEMTIME *stDstTime, const time_t *ttSrcTime);
  /**
  * @brief ��FILETIME��ʽʱ���ʽ��ΪSYSTEMTIME��ʽʱ��
  * @param[out] stDstTime ��ʽ�����SYSTEMTIME��ʽʱ��
  * @param[in] ftSrcTime FILETIME��ʽ��ʱ��
  * @retval >=0 ��ʽ���ɹ�
  * @retval <0 ��ʽ��ʧ��
  */
  int BASE_EXPORT FormatToSysTime(SYSTEMTIME *stDstTime, const FILETIME *ftSrcTime);


}  // namespace string_utils
}  // namespace base
#endif  // BASE_STRINGS_STRINGFORMAT_H_
