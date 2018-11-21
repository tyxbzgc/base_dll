// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_PATH_UTILS_H_
#define BASE_FILES_PATH_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
typedef std::vector<std::basic_string<TCHAR>> vstr;

/**
* @brief 根据模块句柄获取模块路径
* @param[out] wchBuffer 存储模块路径的缓冲区
* @param[in] nBufferSize 缓冲区长度
* @param[in] hModule 模块句柄，如果该参数为NULL，该函数返回该应用程序全路径
* @return 是否成功
*/
BOOL BASE_EXPORT GetModuleFileName(LPWSTR pFileName, DWORD dwLen,
                                   HINSTANCE hModule = NULL);

/*
*因返回类型为std::wstring，跨DLL调用存在风险，故不设为导出函数，请使用 GetModuleFileName(LPWSTR pFileName, DWORD dwLen, HINSTANCE hModule = NULL )
*/
std::wstring GetModuleFileName(HINSTANCE hModule = NULL);

/*
* @brief 获取系统目录
* @param void
* @return 获取系统目录
* @note 因返回类型为std::wstring，跨DLL调用存在风险，故不设为导出函数，请使用 GetSystemDirectory(LPWSTR pSysDir, DWORD dwLen)
*/
std::wstring  GetSystemDirectory();

/**
* @brief 获取系统目录
* @param[out] pSysDir 存储系统目录的缓冲区
* @param[in] dwLen 缓冲区长度
* @return 是否成功
*/
BOOL BASE_EXPORT GetSystemDirectory(LPWSTR pSysDir, DWORD dwLen);

/**
* @brief 得到当前进程的路径目录
* @param[out] pchPath 路径目录
* @param[in]  dwSize  pchPath长度
* @return 是否成功获取
*/
BOOL BASE_EXPORT GetCurrentProcessPath(TCHAR* pchPath, DWORD dwSize);

/**
* @brief 获取临时文件路径
* @param[out] lpszFile 生成的临时文件路径
* @param[in] nSize  lpszFile长度
* @param[in] lpszPath 指定生成文件路径
* @param[in] lpszPri 表示前缀
* @return 是否成功
* @note lpszPath为NULL时生成到系统临时目录
*/
BOOL BASE_EXPORT GetDrTempFile(LPTSTR lpszFile,
                               int nSize,
                               LPCTSTR lpszPri = NULL,
                               LPCTSTR lpszPath = NULL
                              );

/**
* @brief 枚举文件
* @param[in] szPath 要枚举的目录
* @param[out] vFiles 枚举到的文件队列
* @param[in] nLevel 枚举多少层子目录，默认递归所有层
* @param[in] szFilter 过滤文件类型
* @return 是否成功枚举
*/
BOOL BASE_EXPORT EnumFileOfDir(LPCTSTR szPath, vstr &vFiles, int nLevel = -1,
                               LPTSTR szFilter = NULL);

/**
* @brief 枚举目录
* @param[in]  szPath 要枚举的目录
* @param[out] vFolders 枚举到的目录队列
* @note 由于放入队列，该函数不适用拥有大数量的子目录的目录枚举
* @return 是否成功枚举
*/
BOOL BASE_EXPORT EnumFolderOfDir(LPCTSTR szPath, vstr& vFolders);

/**
* @brief 获取当前进程路径
* @param[in] szFilePath 文件路径
* @param[in] dwLen szFilePath长度
* @note 可以通过_SetPathRelativeLevel函数来设置获取nLevel层路径
*/
BOOL BASE_EXPORT GetMainPath(LPTSTR szFilePath, const DWORD dwLen, INT nLevel);

/**
* @brief 获取当前exe所在路径的上dwLevel层路径，0为当前路径
* @param[out] szPath 路径
* @param[in] dwSize szPath长度
* @param[in] dwLevel 几层路径
* @return 是否获取路径
*/
BOOL BASE_EXPORT GetCurPathRootPath(LPTSTR szPath, DWORD dwSize,
                                    DWORD dwLevel = 0);
/**
* @brief 获取系统用户AppData路径
* @param[out] pchPath 路径
* @return 是否获取路径
*/
BOOL BASE_EXPORT GetAppData(TCHAR* pchPath);

}  // namespace filemisc
}  // namespace base
#endif  // BASE_FILES_PATH_UTILS_H_
