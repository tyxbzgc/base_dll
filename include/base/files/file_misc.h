// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_MISC_H_
#define BASE_FILES_FILE_MISC_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
/**
* @brief ָ��·���Ƿ����ļ�ϵͳ�ļ��л������ļ�URL
* @param[in] lpszPath �����·��
* @return �Ƿ����ļ�ϵͳ�ļ��л������ļ�URL
*/
BOOL BASE_EXPORT IsPathString(LPCTSTR lpszPath);

BOOL IsHtmlFile(LPCTSTR lpszFile);
LPCWSTR GetPathFileInvalidChar() ;
std::wstring FormatFileName(const std::wstring &szFile) ;

}//namespace FileMisc
}//namespace Base
#endif  // BASE_FILES_FILE_MISC_H_
