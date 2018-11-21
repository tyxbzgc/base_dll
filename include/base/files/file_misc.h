// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_MISC_H_
#define BASE_FILES_FILE_MISC_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
/**
* @brief 指定路径是否是文件系统文件夹或网络文件URL
* @param[in] lpszPath 传入的路径
* @return 是否是文件系统文件夹或网络文件URL
*/
BOOL BASE_EXPORT IsPathString(LPCTSTR lpszPath);

BOOL IsHtmlFile(LPCTSTR lpszFile);
LPCWSTR GetPathFileInvalidChar() ;
std::wstring FormatFileName(const std::wstring &szFile) ;

}//namespace FileMisc
}//namespace Base
#endif  // BASE_FILES_FILE_MISC_H_
