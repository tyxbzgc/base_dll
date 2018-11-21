// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_STRINGS_STRINGOPERATE_H_
#define BASE_STRINGS_STRINGOPERATE_H_

#include "base\base_export.h"

namespace base {
namespace string_utils {
  /**
  * @name �ָ��ַ���
  * @{
  */
  /**
  * @brief
  * @param[in] s Դ�ַ���
  * @param[out] v ���ָ������vector
  * @param[in] c �ָ���ַ�
  * @note
  * @return ��
  */
  void BASE_EXPORT SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

  /**
  * @name �ַ����滻
  * @{
  */
  /**
  * @brief ��strTarget �ַ����滻strText �ַ���������strSource �ַ���
  * @param[in] strText Դ�ַ���
  * @param[in] strSource ���滻���ַ���
  * @param[in] strTarget �滻�ɵ��ַ���
  * @note �滻�ɹ���strText���
  * @return �滻����
  */
  size_t BASE_EXPORT ReplaceString(std::wstring &strText, const std::wstring& strSource, const std::wstring& strTarget);

  /*@}*/

  /**
  * @name �ַ����ü����ָ�
  * @{
  */
  /**
  * @brief �ü�ȥ��strTextͷβ����chSpecial ָ�����ַ�
  * @param[in] strText ���ü����ַ���
  * @param[out] chOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @param[in] chSpecial �ü��ַ�
  * @return �ü���strText�ַ������ȣ�������ֵ<0�����ʾ��������
  * @note �ַ���Сд����
  * @par ʾ��
  * @code
  * wstring wstr = L"find result afff";
  * wchar_t wch = L'f';
  * wchar output[20];
  * size_t n = TrimString(wstr, output, sizeof(output)/sizeof(output[0]), wch);
  * output����L"ind result a"
  * n����12
  * @endcode
  */
  size_t BASE_EXPORT TrimString(const std::wstring& strText, WCHAR* wchOutput, int nOutputSize, WCHAR chSpecial = L' ');
  /**
  * @brief ʹ���ַ�chSeparator�ָ��ַ���lpszStr�����ֱ�洢��pStrList�У����ָ���ΪiMaxCount
  * @param[in] lpszStr ���ָ���ַ���
  * @param[in] chSpecial �ָ��ַ�
  * @param[out] pStrList ��ŷָ������ַ�����̬��ά���������ָ�룬�������ʾ������֧�ֶ�̬��ά����
  * @param[in] dwMaxLength    ��̬��ά���� pStrList �����������洢һ���Ӽ��������黺�����ĳ���
  * @param[out] dwStrListCnt	 ���ش洢�� pStrList �����ַ�����
  * @param[in] iMaxCount ������ַ�����
  * @return �Ƿ���ȷ����Ҫ�ǲ�����ֵ�Ƿ�Υ��
  * @note �ַ���Сд����
  * @par ʾ��
  * @code
  * string str = "findrresultafff";
  * char a[5][20];
  * DWORD count;
  * BOOL ret = Base::StringUtils::SplitStringA(str.c_str(), 'r', a[0], 20, &count, 2);
  * a[0]����"find",a[1]����"resultafff"
  * count����2
  * @endcode
  */
  BOOL BASE_EXPORT SplitStringA(LPCSTR lpszStr, char chSeparator, CHAR* pStrList, DWORD dwMaxLength, DWORD* dwStrListCnt, size_t iMaxCount = 0x7fffffff);

  /**
  * @brief ʹ���ַ�chSeparator�ָ��ַ���lpszStr�����ֱ�洢��pStrList�У����ָ���ΪiMaxCount
  * @param[in] lpszStr ���ָ���ַ���
  * @param[in] chSpecial �ָ��ַ�
  * @param[out] pStrList ��ŷָ������ַ�����̬��ά���������ָ�룬�������ʾ������֧�ֶ�̬��ά����
  * @param[in] dwMaxLength    ��̬��ά���� pStrList �����������洢һ���Ӽ��������黺�����ĳ���
  * @param[out] dwStrListCnt	 ���ش洢�� pStrList �����ַ�����
  * @param[in] iMaxCount ������ַ�����
  * @return �Ƿ���ȷ����Ҫ�ǲ�����ֵ�Ƿ�Υ��
  * @note �ַ���Сд����
  * @par ʾ��
  * @code
  * wstring str = L"findrresultafff";
  * wchar a[5][20];
  * DWORD count;
  * BOOL ret = Base::StringUtils::SplitStringW(str.c_str(), L'r', a[0], 20, &count, 2);
  * a[0]����L"find",a[1]����L"resultafff"
  * count����2
  * @endcode
  */
  BOOL BASE_EXPORT SplitStringW(LPCWSTR lpszStr, WCHAR chSeparator, WCHAR* pStrList, DWORD dwMaxLength, DWORD* dwStrListCnt, size_t iMaxCount = 0x7fffffff);

  /*@}*/

  /**
  * @name �ַ�������
  * @{
  */
  /**
  * @brief ͨ��ǰ������ toStrip �ַ����ض�ǰ�����������ַ�����ȡ����ַ���
  * @param[in] source Դ�ַ���
  * @param[in] toStrip �ض��ַ�
  * @param[out] wchBuffer �洢���������洢�ض�ǰ����������toStrip��ĺ���ַ���
  * @param[in] nBufferSize ����������
  * @return ���ɹ����򷵻ػ����� wchBuffer �ĵ�ַ�� ��ʧ�ܣ��򷵻�NULL
  */
  BASE_EXPORT WCHAR* StripLeading(const std::wstring& source, const WCHAR toStrip, WCHAR*wchBuffer, int nBufferSize);

  /**
  * @brief ͨ���������� toStrip �ַ����ضϺ������������ַ�����ȡǰ���ַ���
  * @param[in] source Դ�ַ���
  * @param[in] toStrip �ض��ַ�
  * @param[out] wchBuffer �洢���������洢�ضϺ�����������toStrip���ǰ���ַ���
  * @param[in] nBufferSize ����������
  * @return ���ɹ����򷵻ػ����� wchBuffer �ĵ�ַ�� ��ʧ�ܣ��򷵻�NULL
  */
  BASE_EXPORT WCHAR* StripTrailing(const std::wstring& source, const WCHAR toStrip, WCHAR*wchBuffer, int nBufferSize);

  /*@}*/


  /**
  * @name �ַ�����Сдת��
  * @{
  */
  /**
  * @brief �� str �ַ���ת���ɴ�д�ַ���:���ֽڰ汾
  * @param[in] str ����ת�����ַ���
  * @param[out] wchOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  * @par ʾ��
  * @code
  * wstring str = L"abc";
  * wchar output[20];
  * ToUpperW(str, output, sizeof(output)/sizeof(output[0]));
  * output ���� ABC
  * @endcode
  */
  BASE_EXPORT WCHAR* ToUpperW(const std::wstring& str, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief �� str �ַ���ת���ɴ�д�ַ��������ֽڰ汾
  * @param[in] str ����ת�����ַ���
  * @param[out] chOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  * @par ʾ��
  * @code
  * string str = "abc";
  * char output[20];
  * ToUpperA(str, output, sizeof(output)/sizeof(output[0]));
  * output ���� ABC
  * @see ToUpperW
  */
  BASE_EXPORT CHAR* ToUpperA(const std::string& str, CHAR* chOutput, int nOutputSize);

  /**
  * @brief �� str �ַ���ת����Сд�ַ��������ֽڰ汾
  * @param[in] str ����ת�����ַ���
  * @param[out] wchOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  * @par ʾ��
  * @code
  * wstring str = L"ABC";
  * wchar output[20];
  * ToLowerW(str, output, sizeof(output)/sizeof(output[0]));
  * output ���� abc
  * @endcode
  */
  BASE_EXPORT WCHAR* ToLowerW(const std::wstring& str, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief �� str �ַ���ת����Сд�ַ���:���ֽڰ汾
  * @param[in] str ����ת�����ַ���
  * @param[out] wchOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  * @par ʾ��
  * @code
  * string str = "ABC";
  * char output[20];
  * ToLowerA(str, output, sizeof(output)/sizeof(output[0]));
  * output ���� abc
  * @endcode
  * @see ToLowerW
  */
  BASE_EXPORT CHAR* ToLowerA(const std::string& str, CHAR* chOutput, int nOutputSize);

  /*@}*/


  /**
  * @name �ַ���ƥ��/�Ƚ�
  * @{
  */
  /**
  * @brief ģ��ƥ��
  * @param[in] wild   ģ��ƥ���ַ���
  * @param[in] string Ҫƥ����ַ���
  * @param[in] bCaseSensitive �Ƿ������Сд
  * @return �Ƿ�ƥ��ɹ�
  * @note
  �޸�Ϊ�������޸Ĵ�����ַ�����ԭ���ĳ����߼�û���޸ġ�
  ��ʵ�ֵ��߼���������ģ���Ϊ�����ã�����û��ȥ��������ģ��ƥ���������Կ���ʹ��boost::regex��
  */
  BOOL BASE_EXPORT Wildcmp(LPCTSTR wild, LPCTSTR string, BOOL bCaseSensitive);

  /*@}*/

  /**
  * @name �ַ�����Ч���ж�
  * @{
  */
  /**
  * @brief ����ַ�����Ч�ԣ�����26����Сд��ĸ��0~9��_-.@����
  * @param[in] ch �ַ�
  * @return �Ƿ���Ч
  */
  BOOL BASE_EXPORT IsValidChar(TCHAR ch);

  /**
  * @brief EMAIL��Ч����֤
  * @param[in] pszEmail �����ַ
  * @return �Ƿ���Ч
  */
  BOOL BASE_EXPORT IsValidEmail(LPCTSTR pszEmail);

  /*@}*/


  /**
  * @name ���ֽ� or ���ֽ�
  * @{
  */
  /**
  * @brief ANSI����ҳһ���ַ�������ֽڳ���
  * @return Returns 1 if the ANSI code page is single-byte, or 2 if it is double-byte (DBCS)
  */
  int BASE_EXPORT GetMaxCharSize(void);

  /*@}*/


  /**
  * @name �ַ�������
  * @{
  */
  /**
  * @brief ����C-style���ֽ��ַ����ĳ���
  * @param[in] str
  * @return �����ַ������ȣ�������������
  */
  inline int BASE_EXPORT Strlen(const char *str) { return (int)strlen(str); }
  /**
  * @brief ����C-style���ֽ��ַ����ĳ���
  * @param[in] str
  * @return �����ַ������ȣ�������������
  */
  inline int BASE_EXPORT Strlen(const wchar_t *str) { return (int)wcslen(str); }

  /*@}*/


  /**
  * @name �ַ�������
  * @{
  */
  /**
  * @brief ���ַ���src������dst
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] src Դ�ַ���
  * @return ���ؿ������ַ�������������������0
  * @note
  ����ֵ <=size-1. ���size����0, �򷵻�0�������κ���.
  dst�еĽ����0Ϊ������.
  */
  int BASE_EXPORT Strcpy(char *dst, int size, const char *src);
  /**
  * @brief ���ַ���src������dst
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] src Դ�ַ���
  * @return ���ؿ������ַ�������������������0
  * @note
  ����ֵ <=size-1. ���size����0, �򷵻�0�������κ���.
  dst�еĽ����0Ϊ������.
  */
  int BASE_EXPORT Strcpy(wchar_t *dst, int size, const wchar_t *src);


  /**
  * @brief �����ַ���src��len���ַ���dst�����Ǹ��ӽ�����0
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] src Դ�ַ���
  * @parma[in] len ��࿽�����ַ�����
  * @return ����ʵ�ʿ������ַ�����
  */
  int BASE_EXPORT Strncpy(char *dst, int size, const char *src, int len);
  /**
  * @brief �����ַ���src��len���ַ���dst�����Ǹ��ӽ�����0
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] src Դ�ַ���
  * @parma[in] len ��࿽�����ַ�����
  * @return ����ʵ�ʿ������ַ�����
  */
  int BASE_EXPORT Strncpy(wchar_t *dst, int size, const wchar_t *src, int len);

  /*@}*/

  /**
  * @name �ַ���׷��
  * @{
  */
  /**
  * @brief ��src׷����dst��β��
  * @param[out] dst Ŀ���ַ���������
  * @param[in] size ������dst���ֽڴ�С������������0
  * @param[in] src Դ�ַ���
  * @return ����ʵ�ʿ������ַ�������������������0
  * @note
  ����ֵ <=size-1-strlen(dst). If size>=strlen(dst), returns 0 and does nothing.
  dst�еĽ����0Ϊ������.
  */
  int BASE_EXPORT Strcat(char *dst, int size, const char *src);
  /**
  * @brief ��src׷����dst��β��
  * @param[out] dst Ŀ���ַ���������
  * @param[in] size ������dst���ֽڴ�С������������0
  * @param[in] src Դ�ַ���
  * @return ����ʵ�ʿ������ַ�������������������0
  * @note
  ����ֵ <=size-1-Strlen(dst). If size>=Strlen(dst), returns 0 and does nothing.
  dst�еĽ����0Ϊ������.
  */
  int BASE_EXPORT Strcat(wchar_t *dst, int size, const wchar_t *src);

  /*@}*/

  /**
  * @name �ַ�����ʽ��
  * @{
  */
  /**
  * @brief д��ʽ���ַ�����dst��
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  * @return ����ʵ��д���ַ��ĸ�����������������0
  * @note
  The return value is <=size-1. If size is 0, returns 0 and does nothing.
  The result in dst is always 0 terminated.
  */
  int BASE_EXPORT _cdecl Sprintf(char *dst, int size, const char *format, ...);
  /**
  * @brief д��ʽ���ַ�����dst��
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] format ��������ĸ�ʽ
  * @param[in] ��������
  * @return ����ʵ��д���ַ��ĸ�����������������0
  * @see int BASE_EXPORT _cdecl Sprintf( char *dst, int size, const char *format, ... );
  */
  int BASE_EXPORT _cdecl Sprintf(wchar_t *dst, int size, const wchar_t *format, ...);
  /**
  * @brief д��ʽ���ַ�����dst��
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] format ��������ĸ�ʽ
  * @param[in] args VA_LIST�͵ı���,���������ָ�򲻶�������ָ��
  * @return ����ʵ��д���ַ��ĸ�����������������0
  * @note
  The return value is <=size-1. If size is 0, returns 0 and does nothing.
  The result in dst is always 0 terminated.
  */
  int BASE_EXPORT _cdecl Vsprintf(char *dst, int size, const char *format, va_list args);
  /**
  * @brief д��ʽ���ַ�����dst��
  * @param[out] dst Ŀ�껺����
  * @param[in] size ������dst���ַ�����������������0
  * @param[in] format ��������ĸ�ʽ
  * @param[in] args VA_LIST�͵ı���,���������ָ�򲻶�������ָ��
  * @return ����ʵ��д���ַ��ĸ�����������������0
  * @see 	int BASE_EXPORT _cdecl Vsprintf( char *dst, int size, const char *format, va_list args );
  */
  int BASE_EXPORT _cdecl Vsprintf(wchar_t *dst, int size, const wchar_t *format, va_list args);

  /*@}*/


  /**
  * @name ������Ϣ
  * @{
  */
  /**
  * @brief ���ݴ���ID�õ�������Ϣ
  * @param[in] last_error ����ID
  * @return ������Ϣ
  */
  BOOL BASE_EXPORT GetLastErrorMessage(DWORD last_error, WCHAR* wchBuffer, int nBufferSize);

  /*@}*/

}  // namespace string_utils
}  // namespace base
#endif  //#define BASE_STRINGS_STRINGOPERATE_H_
