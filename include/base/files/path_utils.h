// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_PATH_UTILS_H_
#define BASE_FILES_PATH_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
typedef std::vector<std::basic_string<TCHAR>> vstr;

/**
* @brief ����ģ������ȡģ��·��
* @param[out] wchBuffer �洢ģ��·���Ļ�����
* @param[in] nBufferSize ����������
* @param[in] hModule ģ����������ò���ΪNULL���ú������ظ�Ӧ�ó���ȫ·��
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT GetModuleFileName(LPWSTR pFileName, DWORD dwLen,
                                   HINSTANCE hModule = NULL);

/*
*�򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ��ʲ���Ϊ������������ʹ�� GetModuleFileName(LPWSTR pFileName, DWORD dwLen, HINSTANCE hModule = NULL )
*/
std::wstring GetModuleFileName(HINSTANCE hModule = NULL);

/*
* @brief ��ȡϵͳĿ¼
* @param void
* @return ��ȡϵͳĿ¼
* @note �򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ��ʲ���Ϊ������������ʹ�� GetSystemDirectory(LPWSTR pSysDir, DWORD dwLen)
*/
std::wstring  GetSystemDirectory();

/**
* @brief ��ȡϵͳĿ¼
* @param[out] pSysDir �洢ϵͳĿ¼�Ļ�����
* @param[in] dwLen ����������
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT GetSystemDirectory(LPWSTR pSysDir, DWORD dwLen);

/**
* @brief �õ���ǰ���̵�·��Ŀ¼
* @param[out] pchPath ·��Ŀ¼
* @param[in]  dwSize  pchPath����
* @return �Ƿ�ɹ���ȡ
*/
BOOL BASE_EXPORT GetCurrentProcessPath(TCHAR* pchPath, DWORD dwSize);

/**
* @brief ��ȡ��ʱ�ļ�·��
* @param[out] lpszFile ���ɵ���ʱ�ļ�·��
* @param[in] nSize  lpszFile����
* @param[in] lpszPath ָ�������ļ�·��
* @param[in] lpszPri ��ʾǰ׺
* @return �Ƿ�ɹ�
* @note lpszPathΪNULLʱ���ɵ�ϵͳ��ʱĿ¼
*/
BOOL BASE_EXPORT GetDrTempFile(LPTSTR lpszFile,
                               int nSize,
                               LPCTSTR lpszPri = NULL,
                               LPCTSTR lpszPath = NULL
                              );

/**
* @brief ö���ļ�
* @param[in] szPath Ҫö�ٵ�Ŀ¼
* @param[out] vFiles ö�ٵ����ļ�����
* @param[in] nLevel ö�ٶ��ٲ���Ŀ¼��Ĭ�ϵݹ����в�
* @param[in] szFilter �����ļ�����
* @return �Ƿ�ɹ�ö��
*/
BOOL BASE_EXPORT EnumFileOfDir(LPCTSTR szPath, vstr &vFiles, int nLevel = -1,
                               LPTSTR szFilter = NULL);

/**
* @brief ö��Ŀ¼
* @param[in]  szPath Ҫö�ٵ�Ŀ¼
* @param[out] vFolders ö�ٵ���Ŀ¼����
* @note ���ڷ�����У��ú���������ӵ�д���������Ŀ¼��Ŀ¼ö��
* @return �Ƿ�ɹ�ö��
*/
BOOL BASE_EXPORT EnumFolderOfDir(LPCTSTR szPath, vstr& vFolders);

/**
* @brief ��ȡ��ǰ����·��
* @param[in] szFilePath �ļ�·��
* @param[in] dwLen szFilePath����
* @note ����ͨ��_SetPathRelativeLevel���������û�ȡnLevel��·��
*/
BOOL BASE_EXPORT GetMainPath(LPTSTR szFilePath, const DWORD dwLen, INT nLevel);

/**
* @brief ��ȡ��ǰexe����·������dwLevel��·����0Ϊ��ǰ·��
* @param[out] szPath ·��
* @param[in] dwSize szPath����
* @param[in] dwLevel ����·��
* @return �Ƿ��ȡ·��
*/
BOOL BASE_EXPORT GetCurPathRootPath(LPTSTR szPath, DWORD dwSize,
                                    DWORD dwLevel = 0);
/**
* @brief ��ȡϵͳ�û�AppData·��
* @param[out] pchPath ·��
* @return �Ƿ��ȡ·��
*/
BOOL BASE_EXPORT GetAppData(TCHAR* pchPath);

}  // namespace filemisc
}  // namespace base
#endif  // BASE_FILES_PATH_UTILS_H_
