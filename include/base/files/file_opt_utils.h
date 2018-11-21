// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_OPT_UTILS_H_
#define BASE_FILES_FILE_OPT_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
/**
* @brief �����ļ�Ȩ��, ��������ǿɾ by boxer
* @param[in] pszFile �ļ�·��
* @param[in] dwAccessMask Ȩ��
*/
BOOL BASE_EXPORT SetFileSecurity(
  LPCTSTR pszFile,
  const DWORD dwAccessMask = GENERIC_ALL // Ĭ����Ȩ�����Ȩ��
);

/**
* @brief �������Ժ�ɾ���ļ�
* @param[in] ptszPath �ļ�·��
*/
void BASE_EXPORT DeleteFileAfterReboot(TCHAR *ptszPath);
/**
* @brief ɾ���ļ���
* @param[in] ptszPath �ļ���·��
*/
//BOOL BASE_EXPORT DeleteDirectory(const wchar_t* wchFolderName);
/**
* @brief ����Ȩ��ɾ���ļ�
* @param[in] pszPath ɾ���ļ�·��
* @return �Ƿ�ɹ�ɾ��
*/
BOOL BASE_EXPORT DeleteFileEx(LPCTSTR pszPath);

/**
* @brief ����ָ���ļ����У���ճ������
* @param[in] strFilePath �ļ����У�·��
*/
VOID BASE_EXPORT CopyFilePath(LPCTSTR pFilePath);
/**
* @brief ����ָ���ļ����У���ճ������
* @param[in] strFilePath �ļ����У�·��
*/
VOID BASE_EXPORT CopyFilePath(std::wstring strFilePath);
/**
* @brief �����ļ�
* @param[in] wchExistFile Դ�ļ���
* @param[in] wchNewFile Ŀ���ļ���
* @param[in] bOverWirite Ŀ���ļ��Ѿ�����ʱ�Ƿ񸲸�
* @return �����ʾ�ɹ������ʾʧ�ܣ�GetLastError��ô�����Ϣ
* @note ���� bOverWirite ΪTRUEʱĿ���ļ��������򽫱����ǣ�ΪFALSEʱ
* һ��Ŀ���ļ��Ѿ����ڣ��������û�ʧ��
*/
BOOL BASE_EXPORT CopyFile(const wchar_t* wchExistFile,
                          const wchar_t* wchNewFile,
                          BOOL bOverWirite = TRUE
);

/**
* @brief ����Ŀ¼
* @param[in] tszDir Ŀ¼·��
*/
BOOL BASE_EXPORT CreateDir(CONST TCHAR* tszDir);


/**
* @brief ��һ��Ŀ¼����������ļ����ļ��а�Ǩ����һ��Ŀ¼��
* @param[in] pchFrom ԴĿ¼
* @param[in] pchTo   Ŀ��Ŀ¼
* @return �Ƿ��Ǩ�ɹ�
* @note ��ͬһ���̷��в���
*/
BOOL BASE_EXPORT MoveDirectory(LPCTSTR pchFrom, LPCTSTR pchTo);

/**
* @brief �鿴����
* @param[in] szPath ·��
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT ViewFileFolderProperties(LPCTSTR szPath);

/**
* @brief �򿪻���վ
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT OpenRecycle();

/**
* @brief ���� nCSIDL �ļ�·��
* @param[in] nCSIDL ϵͳָ����CSIDLֵ
* @param[in] szPath ·��
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT SetSpecailFolderPath(INT nCSIDL, LPCTSTR szPath);

/**
* @brief ����ͼ��
* @param[in] hIcon ͼ����
* @param[in] szFilePath �ļ�·��
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT SaveIconToFile(HICON hIcon, LPCTSTR szFilePath);

/**
* @brief get the SECURITY_DESCRIPTOR for Everyone group
* @param[in] dwAccessMask Ȩ��
* @param[out] pSD ����SECURITY_DESCRIPTOR�ṹ
* @return �Ƿ�ɹ�
* @see http://www.cnblogs.com/rainbowzc/archive/2007/08/29/2422146.html
*/
PVOID BASE_EXPORT BuildEveryoneSD(const DWORD dwAccessMask,
                                  PSECURITY_DESCRIPTOR pSD);

/**
* @brief ���ַ�����ʽ��ʾ�ļ���С
* @author wangchao����������fildcmdhelper��
* @param[in] dwFileSize �ļ���С
* @param[out] szFileSize �洢�ַ�����ʽ�ļ���С�Ļ�����
* @param[in] dwBufSize ������szFileSize�Ĵ�С
* @note �ַ�����ʽ�ļ���С��λ��Bytes��KB��MB��GB������1λС��
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT FormatFileSize(DWORD dwFileSize, LPTSTR szFileSize, DWORD dwBufSize);

}  // namespace filemisc
}  // namespace base
#endif  // BASE_FILES_FILE_OPT_UTILS_H_
