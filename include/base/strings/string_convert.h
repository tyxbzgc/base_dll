// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_STRINGS_STRINGCONVERT_H_
#define BASE_STRINGS_STRINGCONVERT_H_

#include <time.h>
#include "base/base_export.h"

namespace base {

/**
* @brief �ַ���ģ����õĶ��������ռ�
*/
namespace string_utils {
  /**
  * @name Byte
  * @{
  */
  /**
  * @brief ��BYTE ���͵Ĳ���c ת������ʮ��������ʽ�洢���ַ���
  * @param[in] c һ���޷����ֽ���ֵ
  * @param[out] wchBuffer �洢ת�����ʮ��������ʽ�ַ����Ļ�����
  * @param[in] nBufferSize ����������
  * @return д�뻺���� wchBuffer ���ַ������ַ���������������ֹ����������ֵ<0�����ʾ����
  * @par ʾ��
  * @code
  * wchar buffer[5];
  * int n = ByteToHexW(255, buffer, 5);
  * buffer ����FF, n 2����
  * @endcode
  */
  int BASE_EXPORT ByteToHexW(BYTE c, WCHAR* wchBuffer, int nBufferSize);
  /**
  * @brief ��BYTE ���͵Ĳ���c ת������ʮ��������ʽ�洢���ַ���
  * @param[in] c һ���޷����ֽ���ֵ
  * @param[out] chBuffer �洢ת�����ʮ��������ʽ�ַ����Ļ�����
  * @param[in] nBufferSize ����������
  * @return д�뻺���� chBuffer ���ַ������ַ���������������ֹ����������ֵ<0�����ʾ����
  * @par ʾ��
  * @code
  * char buffer[5];
  * int n = ByteToHexA(255, buffer, 5);
  * buffer ����FF, n 2����
  * @endcode
  */
  int BASE_EXPORT ByteToHexA(BYTE c, CHAR* chBuffer, int nBufferSize);

  /**
  * @brief ��BYTE�ֽ�������ת����ʮ�����Ƹ�ʽ�ַ���
  * @param[in] pBuffer BYTE�ֽ����ڴ滺��
  * @param[in] iBytes ���泤��
  * @param[out] wchBuffer �洢ת�����ַ����Ļ�����
  * @param[in] nBufferSize ����������
  * @return �ַ����ĳ��ȣ���������ֹ��
  * @par ʾ��
  * @code
  * BYTE Buffer[3] = {10, 11, 12};
  * wchar dst[10];
  * int n = BytesToStringW(Buffer, sizeof(Buffer), dst, 10);
  * dst ����L"0A0B0C"��n ����6
  * @endcode
  */
  int BASE_EXPORT BytesToStringW(const BYTE *pBuffer, size_t iBytes, WCHAR*wchBuffer, int nBufferSize);	//ԭHexToString

                                                                                                      /**
                                                                                                      * @brief ����ʮ��������ʽ�洢���ַ���ת�����ֽ���
                                                                                                      * @param[in] str ��ʮ��������ʽ�洢���ַ���
                                                                                                      * @param[out] pBuffer �洢ת������ֽ��������黺��
                                                                                                      * @param[in] nBytes ���黺��ĳ���
                                                                                                      * @par ʾ��
                                                                                                      * @code
                                                                                                      * BYTE Buffer[3] = {0};
                                                                                                      * std::wstring s = L"0A0B09";
                                                                                                      * StringToBytesW(s, Buffer, 3);
                                                                                                      * Buffer[0] ����10
                                                                                                      * Buffer[1] ����11
                                                                                                      * Buffer[2] ����9
                                                                                                      * @endcode
                                                                                                      */
  void BASE_EXPORT StringToBytesW(const std::wstring &str, BYTE *pBuffer, size_t nBytes);//ԭStringToHex

                                                                                      /*@}*/


                                                                                      /**
                                                                                      * @name GUID
                                                                                      * @{
                                                                                      */
                                                                                      /**
                                                                                      * @brief GUID ���ݽṹת�����ַ���
                                                                                      * @param[in] uid GUID
                                                                                      * @param[out] wchBuffer �洢�ַ���GUID�Ļ�����
                                                                                      * @param[in] nBufferSize ����������
                                                                                      * @return д�뻺�������ַ����ĳ���
                                                                                      */
  int BASE_EXPORT GuidToStringW(::GUID uid, WCHAR* wchBuffer, int nBufferSize);
  /**
  * @brief GUID ���ݽṹת�����ַ���
  * @param[in] uid GUID
  * @param[out] chBuffer �洢�ַ���GUID�Ļ�����
  * @param[in] nBufferSize ����������
  * @return д�뻺�������ַ����ĳ���
  */
  int BASE_EXPORT GuidToStringA(::GUID uid, CHAR* chBuffer, int nBufferSize);

  /**
  * @brief �����ַ�����ʽ���ڵ�GUID ת����GUID ���ݽṹ
  * @param[in] szGuid �ַ�����ʽ���ڵ�GUID
  * @param[out] uid GUID
  * @return �Ƿ�ɹ�ת��
  */
  BOOL BASE_EXPORT StringToGuid(LPCTSTR szGuid, ::GUID* uid);

  /*@}*/


  /**
  * @name MultiByte_Unicode
  * @{
  */
  /**
  * @brief ���ֽڱ���ת����UNICODE����
  * @param[in]  strMultiByte ���ֽڱ����ַ���
  * @param[out] strUnicode UNICODE�����ַ���
  * @param[in]  uCodePage ����ҳID
  * @return �Ƿ�ת���ɹ�
  * @note �򴫳���������Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� MultiByteToUnicode(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp)
  */
  BOOL MultiByteToUnicode(const std::string& strMultiByte, std::wstring& strUnicode, UINT uCodePage);

  /*
  * @brief ���ֽڱ���ת����UNICODE����
  * @param[in]   strMultiByte ���ֽڱ����ַ���
  * @param[in]   uCodePage ����ҳID,Ĭ�ϲ���ΪCP_ACP
  * @return ����ת���ɹ���UNICODE�����ַ���
  * @note �򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� MultiByteToUnicode(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp)
  */
  std::wstring MultiByteToUnicode(const std::string& strMultiByte, UINT uCodePage = CP_ACP);

  /**
  * @brief ���ֽڱ���ת����UNICODE����
  * @param[out] lpw ���ת����UNICODE�ַ����Ļ�����
  * @param[in] lpa Դ���ֽڱ����ַ���
  * @param[in] nChars �ɲ��� lpw ָ��Ļ������Ŀ��ַ�����
  * @param[in] acp ����ҳID
  * @return ����ת�����UNICODE�ַ���
  */
  LPWSTR BASE_EXPORT WINAPI MultiByteToUnicode(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp);

  /**
  * @brief ���ֽڱ���ת����UNICODE����
  * @param[out] lpt ���ת����UNICODE�ַ����Ļ�����
  * @param[in] lpa Դ���ֽڱ����ַ���
  * @param[in] nChars �ɲ���lptָ��Ļ������Ŀ��ַ�����
  * @note ����ҳIDָ��ΪCP_ACP
  * @return ����ת�����UNICODE�ַ���
  */
  LPWSTR BASE_EXPORT WINAPI MultiByteToUnicode(LPWSTR lpt, LPCSTR lpa, int nChars);

  /*
  * @brief UNICODE����ת���ɶ��ֽڱ���
  * @param[in]  strUnicode UNICODE�����ַ���
  * @param[out] strUTF8 ���ֽڱ����ַ���
  * @parma[in]  uCodePage ����ҳID
  * @return �Ƿ�ת���ɹ�
  * @note �򴫳���������Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp)
  */
  BOOL UnicodeToMultiByte(const std::wstring& strUnicode, std::string& strUTF8, UINT uCodePage); // CP_UTF8, CP_ACP

                                                                                                  /*
                                                                                                  * @brief UNICODE����ת���ɶ��ֽڱ���
                                                                                                  * @param[in]   strUnicode UNICODE�����ַ���
                                                                                                  * @parma[in]   uCodePage ����ҳID,Ĭ�ϲ���ΪCP_ACP
                                                                                                  * @return ����ת���ɹ�����ֽڱ����ַ���
                                                                                                  * @note �򷵻�����Ϊstd::string����DLL���ô��ڷ��գ�
                                                                                                  �ʲ���Ϊ������������ʹ�� UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp)
                                                                                                  */
  std::string UnicodeToMultiByte(const std::wstring& strUnicode, UINT uCodePage = CP_ACP);


  /**
  * @brief UNICODE����ת���ɶ��ֽڱ���
  * @param[out] lpa ���ת������ֽڱ����ַ����Ļ�����
  * @param[in] lpw ԴUNICODE�����ַ���
  * @param[in] nChars �ɲ���lpaָ��Ļ��������ַ�����
  * @param[in] acp ����ҳID
  * @return ����ת����Ķ��ֽ��ַ���
  */
  LPSTR  BASE_EXPORT WINAPI UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp);

  /**
  * @brief UNICODE����ת���ɶ��ֽڱ���
  * @param[out] lpa ���ת������ֽڱ����ַ����Ļ�����
  * @param[in] lpt ԴUNICODE�����ַ���
  * @param[in] nChars �ɲ���lpaָ��Ļ��������ַ�����
  * @note ����ҳIDָ��ΪCP_ACP
  * @return ����ת����Ķ��ֽ��ַ���
  */
  LPSTR BASE_EXPORT WINAPI  UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpt, int nChars);

  /*@}*/

  /**
  * @name ANSI_UTF8_convert_vector
  * @{
  */
  /**
  * @brief ANSI����ת����UTF8����
  * @param[in] input ANSI����Ĵ�ת��string
  * @param[in] buf ���ת�����ֽ��ַ�����vector
  * @return �Ƿ�ת���ɹ�
  */
  BASE_EXPORT BOOL AToUTF8(const std::string& input, std::vector<char>& buf);

  /**
  * @name ANSI_UTF8_convert_char
  * @{
  */
  /**
  * @brief ANSI����ת����UTF8����
  * @param[in] input ANSI����Ĵ�ת��string
  * @param[in] buf ���ת�����ֽ��ַ�����buf
  * @param[in] len ����
  * @return �Ƿ�ת���ɹ�
  */
  BASE_EXPORT BOOL AToUTF8(const std::string& input, char * buf, int len);

  /**
  * @brief UTF8����ת����ANSI����
  * @param[in] input ANSI����Ĵ�ת��string
  * @param[in] buf ���ת�����ֽ��ַ�����vector
  * @return �Ƿ�ת���ɹ�
  */
  BASE_EXPORT CHAR * UTF8ToA(const std::string& utf8Str, char * buf, int len);
  /*@}*/

  /**
  * @name String_Date
  * @{
  */
  /**
  * @brief �����ַ�����ʽ���ڵ�����ת����time_t �ṹ���tmDate
  * @param[in]  lpszDate ���ַ�����ʽ����
  * @param[out] tmDate time_t �ṹ�������
  * @note ����������ַ������硰2013-09-29��
  * @return �Ƿ�ת���ɹ�
  */
  BOOL BASE_EXPORT StringToDate(LPCTSTR lpszDate, time_t &tmDate);

  /*@}*/


  /**
  * @details ��֧��strstream�����͵�ֵת��Ϊstd::wstring��
  * Convert a type to a string by streaming it. Requires that there's an ostream
  * inserter available for type T.
  * @param[in] num �ַ���
  * @return ת������ַ���
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
  * @brief std::string�ַ���ת��Ϊstd::wstring�ַ���
  * @param[in] input std::string��ʽ�ַ���
  * @param[out] wchOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT WCHAR* AToW(const std::string &input, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief C����ַ���ת��Ϊstd::wstring�ַ���
  * @param[in] pInput C����ַ�����������'\0'Ϊ�ַ���������ʶ
  * @param[out] wchOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT WCHAR* AToW(const char *pInput, WCHAR* wchOutput, int nOutputSize);
  /**
  * @brief ���ֽ��ַ���ת��Ϊstd::wstring�ַ���
  * @param[in] pInput ���ֽ��ַ���
  * @param[in] inputLength �ɲ���pInputָ����ַ������ֽڵĸ���
  * @param[out] wchOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT WCHAR* AToW(const char *pInput, const size_t inputLength, WCHAR* wchOutput, int nOutputSize);

  /**
  * @brief std::string�ַ���ת��ΪBSTR��ʽ�ַ���
  * @param[in] input std::string��ʽ�ַ���
  * @return ת�����BSTR��ʽ�ַ���
  */
  BASE_EXPORT BSTR AToBSTR(const std::string &input);
  /**
  * @brief ���ֽ��ַ���ת��ΪBSTR��ʽ�ַ���
  * @param[in] pInput ���ֽ��ַ���
  * @param[in] inputLength ������ַ������ֽڵĸ���
  * @return ת�����BSTR��ʽ�ַ���
  */
  BASE_EXPORT BSTR AToBSTR(const char *pInput, const size_t inputLength);

  /*@}*/


  /**
  * @name WTo
  * @{
  */
  /**
  * @brief std::wstring�ַ���ת��Ϊstd::string�ַ���
  * @param[in] input std::wstring��ʽ�ַ���
  * @param[out] chOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT CHAR* WToA(const std::wstring &input, CHAR* chOutput, int nOutputSize);
  /**
  * @brief C�����ַ���ת��Ϊstd::string�ַ���
  * @param[in] pInput C�����ַ�����������L'\0'Ϊ�ַ���������ʶ
  * @param[out] chOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT CHAR* WToA(const wchar_t *pInput, CHAR* chOutput, int nOutputSize);
  /**
  * @brief ���ֽ��ַ���ת��Ϊstd::string�ַ���
  * @param[in] pInput ��ת�����ַ����Ļ�����
  * @param[in] inputLength ��pInputָ��Ļ��������ַ�����
  * @param[out] chOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT CHAR* WToA(const wchar_t *pInput, const size_t inputLength, CHAR* chOutput, int nOutputSize);

  /**
  * @brief std::wstring�ַ���ת��ΪBSTR��ʽ�ַ���
  * @param[in] input std::wstring��ʽ�ַ���
  * @return ת�����BSTR��ʽ�ַ���
  */
  BASE_EXPORT BSTR WToBSTR(const std::wstring &input);

  /*@}*/


  /**
  * @name BSTRto
  * @{
  */
  /**
  * @brief BSTR��ʽ�ַ���ת��Ϊstd::string�ַ���
  * @param[in] bstr BSTR��ʽ�ַ�����������'\0'Ϊ�ַ���������ʶ
  * @param[out] chOutput �洢ת�����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT CHAR* BSTRToA(const BSTR bstr, CHAR* chOutput, int nOutputSize);
  /**
  * @brief BSTR��ʽ�ַ���ת��Ϊstd::wstring�ַ���
  * @param[in] bstr BSTR��ʽ�ַ�����������'\0'Ϊ�ַ���������ʶ
  * @return ת�����std::wstring��ʽ�ַ���
  */
  BASE_EXPORT WCHAR* BSTRToW(const BSTR bstr, WCHAR* wchOutput, int nOutputSize);

  /*@}*/



  /**
  * @name UTF8
  * @{
  */
  /**
  * @brief UTF8�ַ�����std::stringת��Ϊstd::wstring�ַ���
  * @param[in] input UTF8�ַ�����std::string
  * @param[out] wchOutput �洢ת�����UTF8�ַ����ַ����Ļ�����
  * @param[in] nOutputSize ����������
  * @return ���ɹ�������ת�����ַ������׵�ַ����ʧ�ܣ����ؿ��ַ���
  */
  BASE_EXPORT WCHAR* UTF8AToW(const std::string &input, WCHAR*wchOutput, int nOutputSize);

  /**
  * @brief UTF8�ַ����Ŀ��ֽ��ַ���ת��Ϊ���ֽ��ַ���������ÿ���ַ����δ����vector��Ա��
  * @param[in] wcs UTF8�ַ����Ŀ��ֽ��ַ���
  * @param[out] buf ���ת�����ֽ��ַ�����vector
  * @return �Ƿ�ת���ɹ�
  */
  BASE_EXPORT BOOL UTF8WToA(const wchar_t* wcs, std::vector<char>& buf);

  /*@}*/



  //move from ConvertBase.h
  /**
  * @name ATo_with_CodePage
  * @{
  */
  /*
  * @brief ���ֽ��ַ���ת��Ϊstd::wstring�ַ���
  * @param[in] szSrc ��ת���ĵ��ֽ��ַ���
  * @param[in] iSrcCount �ɲ���szSrcָ����ַ������ֽڵĸ���
  * @param[in] cpCodePage ����ҳID
  * @note ���szSrcָ�����ַ����Կ��ַ���ֹ��iSrcCount��������Ϊ-1
  * @return ת�����std::wstring�ַ���
  * @note �򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� AToW( LPCSTR szSrc, const int iSrcCount,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  std::wstring AToW(LPCSTR szSrc, int iSrcCount, DWORD cpCodePage);

  /*
  * @brief std::string�ַ���ת��Ϊstd::wstring�ַ���
  * @param[in] cstr ��ת����std::string�ַ���
  * @param[in] cpCodePage  ����ҳID
  * @return ת�����std::wstring�ַ���
  * @note �򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� AToW( LPCSTR szSrc,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  std::wstring AToW(const std::string& cstr, DWORD cpCodePage);

  /**
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] szSrc ָ��Ҫת����Դ�ַ�����������'\0'Ϊ������ʶ���Զ����㳤��
  * @param[out] wszDst �ַ���ת���������ָ����ڴ��У�ָ��Ŀռ�����㹻�����Դ洢ת������ַ���
  * @param[in] iDstCount ��������ָ��wszDst��ָ����ڴ��Ĵ�С
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  */
  BASE_EXPORT LPCWSTR AToW(LPCSTR szSrc, LPWSTR wszDst, const int iDstCount, DWORD cpCodePage);

  /**
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] szSrc ָ��Ҫת����Դ�ַ���
  * @param[in] iSrcCount Դ�ַ����ֽڸ���
  * @param[out] wszDst �ַ���ת���������ָ����ڴ��У�ָ��Ŀռ�����㹻�����Դ洢ת������ַ���
  * @param[in] iDstCount ��������ָ��wszDst��ָ����ڴ��Ĵ�С
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  */
  BASE_EXPORT LPCWSTR AToW(LPCSTR szSrc, const int iSrcCount, LPWSTR wszDst, const int iDstCount, DWORD cpCodePage);

  /*
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] szSrc ָ��Ҫת����Դ�ַ�����������'\0'Ϊ������ʶ���Զ����㳤��
  * @param[out] wszDst �ַ���ת�����std::wstring�ַ���
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  * @note �򴫳���������Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� AToW( LPCSTR szSrc,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  LPCWSTR AToW(LPCSTR szSrc, std::wstring& wsDst, DWORD cpCodePage);

  /*
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] szSrc ָ��Ҫת����Դ�ַ���
  * @param[in] iSrcCount Դ�ַ�������
  * @param[out] wszDst �ַ���ת�����std::wstring�ַ���
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  * @note �򴫳���������Ϊstd::wstring����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� AToW( LPCSTR szSrc, const int iSrcCount,  LPWSTR wszDst, const int iDstCount, DWORD cpCodePage )
  */
  LPCWSTR AToW(LPCSTR szSrc, const int iSrcCount, std::wstring& wsDst, DWORD cpCodePage);

  /*@}*/


  /**
  * @name WTo_with_CodePage
  * @{
  */
  /*
  * @brief ���ֽ��ַ���ת��Ϊstd::string�ַ���
  * @param[in] wszSrc ��ת���Ŀ��ֽ��ַ���
  * @param[in] iSrcCount �ɲ���wszSrcָ����ַ������ַ�����
  * @param[in] cpCodePage ����ҳID
  * @note ���szSrcָ�����ַ����Կ��ַ���ֹ��iSrcCount��������Ϊ-1
  * @return ת�����std::string�ַ���
  * @note �򷵻�����Ϊstd::string����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� WToA( LPCWSTR wszSrc, const int iSrcCount,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  std::string WToA(LPCWSTR wszSrc, const int iSrcCount, DWORD cpCodePage);

  /*
  * @brief std::wstring�ַ���ת��Ϊstd::string�ַ���
  * @param[in] cwstr ��ת����std::wstring�ַ���
  * @param[in] cpCodePage  ����ҳID
  * @return ת�����std::string�ַ���
  * @note �򷵻�����Ϊstd::string����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� WToA( LPCWSTR wszSrc,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  std::string WToA(const std::wstring& cwstr, DWORD cpCodePage);

  /**
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] wszSrc ָ��Ҫת����Դ�ַ�����������L'\0'Ϊ������ʶ���Զ����㳤��
  * @param[out] pchDest �ַ���ת���������ָ����ڴ��У�ָ��Ŀռ�����㹻�����Դ洢ת������ַ���
  * @param[in] iDstCount ��������ָ��pchDest��ָ����ڴ��Ĵ�С
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  */
  BASE_EXPORT LPCSTR WToA(LPCWSTR wszSrc, LPSTR pchDest, const int iDstCount, DWORD cpCodePage);

  /**
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] wszSrc ָ��Ҫת����Դ�ַ���
  * @param[in] iSrcCount Դ�ַ����ַ�����
  * @param[out] pchDest �ַ���ת���������ָ����ڴ��У�ָ��Ŀռ�����㹻�����Դ洢ת������ַ���
  * @param[in] iDstCount ��������ָ��pchDest��ָ����ڴ��Ĵ�С
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  */
  BASE_EXPORT LPCSTR WToA(LPCWSTR wszSrc, const int iSrcCount, LPSTR pchDest, const int iDstCount, DWORD cpCodePage);

  /*
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] wszSrc ָ��Ҫת����Դ�ַ�����������L'\0'Ϊ������ʶ���Զ����㳤��
  * @param[out] sDest �ַ���ת�����std::string�ַ���
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  * @note �򴫳���������Ϊstd::string����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� WToA( LPCWSTR wszSrc,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  LPCSTR WToA(LPCWSTR wszSrc, std::string& sDest, DWORD cpCodePage);

  /*
  * @brief ���ֽ��ַ���ת��Ϊ���ֽ��ַ���
  * @param[in] wszSrc ָ��Ҫת����Դ�ַ���
  * @param[in] iSrcCount Դ�ַ�������
  * @param[out] sDest �ַ���ת�����std::string�ַ���
  * @param[in] cpCodePage Ҫת����Դ�ַ����Ĵ���ҳID
  * @return ָ��ת������ڴ滺����
  * @note �򴫳���������Ϊstd::string����DLL���ô��ڷ��գ�
  �ʲ���Ϊ������������ʹ�� WToA( LPCWSTR wszSrc, const int iSrcCount,  LPSTR pchDest,  const int iDstCount, DWORD cpCodePage )
  */
  LPCSTR WToA(LPCWSTR wszSrc, const int iSrcCount, std::string& sDest, DWORD cpCodePage);

  /*@}*/

  inline std::wstring AToW(const std::string &input) {
    std::wstring result;

    if(!input.empty()) {
      int nNeedLen = ::MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.length(), NULL, 0);

      // allen 2014.8.26 fix �ַ�������
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

      // allen 2014.08.26 fix �ַ�������
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

      // allen 2014.08.26 fix �ַ�������
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

      // allen 2014.08.26 fix �ַ�������
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

      // allen 2014.08.26 fix �ַ�������
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
