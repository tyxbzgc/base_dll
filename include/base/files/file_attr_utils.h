// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_ATTR_UTILS_H_
#define BASE_FILES_FILE_ATTR_UTILS_H_

#include <Shlwapi.h>
#include "base/base_export.h"

/**
* @cond Doxygen���Ըöδ���
*/
/**
* @struct LANGANDCODEPAGE
* @brief ϵͳ������Ϣ
*/
typedef struct BASE_EXPORT tagLANGANDCODEPAGE {
WORD wLanguage;
WORD wCodePage;
} LANGANDCODEPAGE;
/**
* @endcond
*/

namespace base {
/**
* @brief �ļ�����ģ����õĶ��������ռ�
*/
namespace filemisc {
  /**
  * @name �ж�
  * @{
  */
  /**
  * @brief �ж��ļ��Ƿ����
  * @param[in] szFile �ļ�·��
  * @return �Ƿ����
  */
  BOOL BASE_EXPORT IsFileExist(LPCTSTR szFile);

  /**
  * @brief �ж�Ŀ¼�Ƿ����
  * @param[in] szDir Ŀ¼·��
  * @return �Ƿ����ָ��Ŀ¼
  */
  BOOL BASE_EXPORT IsDirectoryExist(LPCTSTR szDir);

  /**
  * @brief �ж�ָ���ļ��Ƿ��д�������ֻ���ˣ��᳢���޸����Ը�Ϊ�ɶ���д��
  * @param[in] szFile �ļ�·��
  * @retval FALSE ��ʾ��д
  * @retval TRUE ��ʾ����д
  */
  BOOL BASE_EXPORT IsFileChangable(LPCTSTR szFile);

  /**
  * @brief �ж�lpszFile �Ƿ���pe�ļ�
  * @param[in] lpszFile �ļ�·��
  * @return �Ƿ���pe�ļ�
  */
  BOOL BASE_EXPORT IsPEFile(LPCTSTR lpszFile);

  /**
  * @brief �ж��ļ��Ƿ�ռ��
  * @param[in] pszPath �ļ�·��
  * @return �Ƿ�ռ��
  */
  BOOL BASE_EXPORT IsFileUsing(LPCTSTR pszPath);

  /**
  * @brief ����·���Ƿ����ļ�
  * @param[in] szFilePath ·��
  * @param[out] bFile �����Ƿ����ļ�����·��������bFileҲ����FALSE
  * @return �����·���Ƿ����
  */
  BOOL BASE_EXPORT IsPathFile(LPCTSTR szFilePath, BOOL &bFile);

  /**
  * @brief ����·���Ƿ����ļ���
  * @param[in] szFilePath ·��
  * @param[out] bDir �����Ƿ����ļ��У���·��������bDirҲ����FALSE
  * @return �����·���Ƿ����
  */
  BOOL BASE_EXPORT IsPathDir(LPCTSTR szFilePath, BOOL &bDir);

  /*@}*/



  /**
  * @name �ļ���Ϣ
  * @{
  */

  ///**
  //* @brief �õ��ļ���Ϣ
  //* @param[in]  szFile �ļ�·��
  //* @param[out] lpszCompany  ���й�˾��
  //* @param[out] lpszFileDesc �ļ�����
  //* @param[out] lpszVersion  �汾��Ϣ
  //* @�Ƿ�ɹ����
  //* @������������Ϊwstring����dll���ô��ڷ��գ�
  //   ����ʹ��BOOL GetFileInfo(LPCTSTR szFile, TCHAR* lpszCompany, TCHAR* lpszFileDesc, TCHAR* lpszVersion);
  //*/
  //BOOL BASE_EXPORT GetFileInfo(
  //	LPCTSTR szFile,
  //	std::wstring* lpstrCompany,
  //	std::wstring* lpstrFileDesc,
  //	std::wstring* lpstrVersion );

  /**
  * @brief �õ��ļ���Ϣ
  * @param[in]  szFile �ļ�·��
  * @param[out] lpszCompany  ���й�˾��
  * @param[out] lpszFileDesc �ļ�����
  * @param[out] lpszVersion  �汾��Ϣ
  * @�Ƿ�ɹ����
  */
  BOOL BASE_EXPORT GetFileInfo(
    LPCTSTR szFile,
    TCHAR* lpstrCompany,
    TCHAR* lpstrFileDesc,
    TCHAR* lpstrVersion);

  /**
  * @brief �õ��ļ��Ĵ���ʱ�䡢������ʱ�������޸�ʱ��
  * @param[in] szFile �ļ�·��
  * @param[out] lpCreationTime   ����ʱ��
  * @param[out] lpLastAccessTime ������ʱ��
  * @param[out] lpLastWriteTime  ����޸�ʱ��
  * @return �Ƿ�ɹ���ȡ�ļ���ʱ��
  */
  BOOL BASE_EXPORT GetFileTimeFlex(
    LPCTSTR szFile,
    LPFILETIME lpCreationTime,
    LPFILETIME lpLastAccessTime,
    LPFILETIME lpLastWriteTime
  );

  /**
  * @brief �õ�pe�ļ��汾����Ϣ
  * @param[out] pnMajor ��汾��ָ��
  * @param[out] pnMinor С�汾��ָ��
  * @param[out] pnBuild ����汾��
  * @param[out] pnLast  �汾β��
  * @param[in] pchPeFile �ļ�·��
  * @return �Ƿ��óɹ������pe�ļ��ް汾�ţ�Ҳ����FALSE
  * @note ��pchPeFile = NULL ʱ��ȡ���ǵ�ǰ���̵İ汾��Ϣ
  */
  BOOL BASE_EXPORT GetPEVersion(
    unsigned int* pnMajor,
    unsigned int* pnMinor,
    unsigned int* pnBuild,
    unsigned int* pnLast,
    const TCHAR* pchPeFile = NULL
  );
  /**
  * @brief �õ��ļ��汾����Ϣ
  * @param[in] lpszFileName �ļ�·��
  * @param[out] wMajorVersion ��汾��ָ��
  * @param[out] wMinorVersion С�汾��ָ��
  * @param[out] wRevisionVersion �޶��汾��
  * @param[out] wBuildVersion  ����汾��
  * @return �Ƿ��óɹ�������ļ��ް汾�ţ�Ҳ����FALSE
  */
  BOOL BASE_EXPORT GetFileVersion(
    LPCWSTR lpszFileName,
    unsigned int& wMajorVersion,
    unsigned int& wMinorVersion,
    unsigned int& wRevisionVersion,
    unsigned int& wBuildVersion);
  /**
  * @brief ��ȡ�ļ���Ϣ
  * @param[in] szFile �ļ�·��
  * @param[in] szFileInfoName �ļ���Ϣ��
  * @param[out] wchFileInfo �洢��ȡ���ļ���Ϣ�Ļ�����
  * @param[in][out]  nSize  ������ wchFileInfo �ĳ��ȣ������Ȳ��㣬�᷵�����賤��
  * @return �Ƿ��ȡ�ɹ�
  */
  BOOL BASE_EXPORT GetFileSingleInfo(LPCTSTR szFile, LPCTSTR szFileInfoName, WCHAR*wchFileInfo, int* pnSize);


  /**
  * @brief �õ�ָ��PE�ļ��İ汾
  * @param[out] pchBuf �洢�汾��Ϣ�ַ����Ļ�����
  * @param[out] nSize ������pchBuf�ĳ���
  * @param[in] pchPeFile pe�ļ���·��
  * @return �Ƿ��óɹ������pe�ļ��ް汾�ţ�Ҳ����FALSE
  * @note ��pchPeFile = NULL ʱ��ȡ���ǵ�ǰ���̵İ汾��Ϣ���汾��Ϣ�ַ�������"1.0.4.0"
  */
  BOOL BASE_EXPORT GetCurPEVersionA(char* pchBuf, int nSize, const TCHAR* pchPeFile = NULL);

  /**
  * @brief �õ��ļ��汾����Ϣ
  * @param[out] pnMajor ��汾��
  * @param[out] pnMinor С�汾��
  * @param[out] pnBuild ����汾��
  * @param[out] pnLast  �汾β��
  * @param[in] pchVersion �ļ��汾��Ϣ�ַ���������"1.0.4.0"
  */
  BOOL BASE_EXPORT GetNumberVerFromStringVer(
    unsigned int* pnMajor,
    unsigned int* pnMinor,
    unsigned int* pnBuild,
    unsigned int* pnLast,
    const TCHAR* pchVersion
  );


  /**
  * @brief ����ļ��汾��Ϣ
  * @param[in] lpszFile �ļ�·��
  * @param[out] pchVerion �汾���ַ���
  * @param[in] nSize �洢�汾���ַ�����buffer����
  * @return �Ƿ�ɹ�
  */
  BOOL BASE_EXPORT GetFileVerInfoA(LPCSTR lpszFile, char* pchVersion, int nSize);
  /**
  * @brief ����ļ��汾��Ϣ
  * @param[in] lpszFile �ļ�·��
  * @param[out] pchVerion �汾���ַ���
  * @param[in] nSize �洢�汾���ַ�����buffer����
  * @return �Ƿ�ɹ�
  * @see GetFileVerInfoA
  */
  BOOL BASE_EXPORT GetFileVerInfoW(LPCWSTR lpszFile, wchar_t* pchVersion, int nSize);

#if (defined(_UNICODE) || defined(UNICODE))
#define GetFileVerInfo GetFileVerInfoW
#else
#define GetFileVerInfo GetFileVerInfoA
#endif


  /**
  * @brief ��ȡdll�İ汾��
  * @param[in] pchDllName dll��·��
  * @param[out] DllVersion ����dll�İ汾��Ϣ
  * @see http://blog.csdn.net/kvs112219/article/details/6070676
  * @return �Ƿ�ɹ���ȡ
  */
  BOOL BASE_EXPORT DllGetVersion(LPCTSTR pchDllName, DLLVERSIONINFO& DllVersion);

  /*@}*/



  /**
  * @name ��С
  * @{
  */

  /**
  * @brief �õ��ļ���С
  * @param[in] lpszFile �ļ�·��
  * @return �����ļ���С
  */
  unsigned long BASE_EXPORT GetFileSizeEx(LPCTSTR lpszFile);

  /**
  * @brief �õ��ļ��д�С
  * @param[in]  tszPath     �ļ���·��
  * @param[out] pdwFileCnt  �������ļ�����
  * @param[out] pdwFolderCnt���ļ�������
  * @return ���ظ�ָ��·�����ļ��ܴ�С
  * @note �ú������������Ƕ�׵����ļ��к��ļ���
  * @code
  * DWORD64 dwSize    = 0;
  * DWORD dwFileCnt   = 0;
  * DWORD dwFolderCnt = 0;
  *
  * // �����Ҫ�������ļ����������ļ�������
  * dwSize = FileMisc::GetFolderSize(_T("d:\\TMP\\metadata"), &dwFileCnt, &dwFolderCnt);
  *
  * // �������Ҫ�������ļ��к��ļ�����
  * dwSize = FileMisc::GetFolderSize(_T("d:\\TMP\\metadata"), NULL, NULL);
  *
  * @endcode
  */
  DWORD64 BASE_EXPORT GetFolderSize(LPCTSTR tszPath, DWORD* pdwFileCnt, DWORD* pdwFolderCnt);

  /*@}*/


  /**
  * @name ����
  * @{
  */
  /**
  * @brief ����Ƕ���ļ���
  * @param[in] lpszDir Ƕ���ļ���·��
  * @return �Ƿ񴴽��ɹ�
  * @code
  * BOOL ret = CreateDirectoryNested(_T("C:\\Users\\Administrator\\Desktop\\1\\2\\3\\4.txt"));
  * �����洴��Ƕ���ļ���\1\2\3\4.txt\��ע��4.txt���ļ������ƶ�����һ���ļ�
  * @endcode
  */
  BOOL BASE_EXPORT CreateDirectoryNested(LPCTSTR lpszDir);

}  // namespace filemisc
}  // namespace base

#endif  // BASE_FILES_FILE_ATTR_UTILS_H_