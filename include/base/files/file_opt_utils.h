// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_OPT_UTILS_H_
#define BASE_FILES_FILE_OPT_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
/**
* @brief 设置文件权限, 可以做到强删 by boxer
* @param[in] pszFile 文件路径
* @param[in] dwAccessMask 权限
*/
BOOL BASE_EXPORT SetFileSecurity(
  LPCTSTR pszFile,
  const DWORD dwAccessMask = GENERIC_ALL // 默认提权到最高权限
);

/**
* @brief 重启电脑后删除文件
* @param[in] ptszPath 文件路径
*/
void BASE_EXPORT DeleteFileAfterReboot(TCHAR *ptszPath);
/**
* @brief 删除文件夹
* @param[in] ptszPath 文件夹路径
*/
//BOOL BASE_EXPORT DeleteDirectory(const wchar_t* wchFolderName);
/**
* @brief 提升权限删除文件
* @param[in] pszPath 删除文件路径
* @return 是否成功删除
*/
BOOL BASE_EXPORT DeleteFileEx(LPCTSTR pszPath);

/**
* @brief 拷贝指定文件（夹）到粘贴板上
* @param[in] strFilePath 文件（夹）路径
*/
VOID BASE_EXPORT CopyFilePath(LPCTSTR pFilePath);
/**
* @brief 拷贝指定文件（夹）到粘贴板上
* @param[in] strFilePath 文件（夹）路径
*/
VOID BASE_EXPORT CopyFilePath(std::wstring strFilePath);
/**
* @brief 复制文件
* @param[in] wchExistFile 源文件名
* @param[in] wchNewFile 目标文件名
* @param[in] bOverWirite 目标文件已经存在时是否覆盖
* @return 非零表示成功；零表示失败，GetLastError获得错误信息
* @note 参数 bOverWirite 为TRUE时目标文件若存在则将被覆盖，为FALSE时
* 一旦目标文件已经存在，则函数调用会失败
*/
BOOL BASE_EXPORT CopyFile(const wchar_t* wchExistFile,
                          const wchar_t* wchNewFile,
                          BOOL bOverWirite = TRUE
);

/**
* @brief 创建目录
* @param[in] tszDir 目录路径
*/
BOOL BASE_EXPORT CreateDir(CONST TCHAR* tszDir);


/**
* @brief 把一个目录下面的所有文件和文件夹搬迁到另一个目录下
* @param[in] pchFrom 源目录
* @param[in] pchTo   目标目录
* @return 是否搬迁成功
* @note 在同一个盘符中操作
*/
BOOL BASE_EXPORT MoveDirectory(LPCTSTR pchFrom, LPCTSTR pchTo);

/**
* @brief 查看属性
* @param[in] szPath 路径
* @return 是否成功
*/
BOOL BASE_EXPORT ViewFileFolderProperties(LPCTSTR szPath);

/**
* @brief 打开回收站
* @return 是否成功
*/
BOOL BASE_EXPORT OpenRecycle();

/**
* @brief 设置 nCSIDL 文件路径
* @param[in] nCSIDL 系统指定了CSIDL值
* @param[in] szPath 路径
* @return 是否成功
*/
BOOL BASE_EXPORT SetSpecailFolderPath(INT nCSIDL, LPCTSTR szPath);

/**
* @brief 保存图标
* @param[in] hIcon 图标句柄
* @param[in] szFilePath 文件路径
* @return 是否成功
*/
BOOL BASE_EXPORT SaveIconToFile(HICON hIcon, LPCTSTR szFilePath);

/**
* @brief get the SECURITY_DESCRIPTOR for Everyone group
* @param[in] dwAccessMask 权限
* @param[out] pSD 返回SECURITY_DESCRIPTOR结构
* @return 是否成功
* @see http://www.cnblogs.com/rainbowzc/archive/2007/08/29/2422146.html
*/
PVOID BASE_EXPORT BuildEveryoneSD(const DWORD dwAccessMask,
                                  PSECURITY_DESCRIPTOR pSD);

/**
* @brief 用字符串形式显示文件大小
* @author wangchao：导出，给fildcmdhelper用
* @param[in] dwFileSize 文件大小
* @param[out] szFileSize 存储字符串形式文件大小的缓冲区
* @param[in] dwBufSize 缓冲区szFileSize的大小
* @note 字符串形式文件大小单位：Bytes、KB、MB、GB，保留1位小数
* @return 是否成功
*/
BOOL BASE_EXPORT FormatFileSize(DWORD dwFileSize, LPTSTR szFileSize, DWORD dwBufSize);

}  // namespace filemisc
}  // namespace base
#endif  // BASE_FILES_FILE_OPT_UTILS_H_
