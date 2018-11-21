// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SAFE_LOAD_LIBRARY_SAFE_LOAD_LIBRARY_H_
#define BASE_SAFE_LOAD_LIBRARY_SAFE_LOAD_LIBRARY_H_

#include "base/base_export.h"

namespace base {
namespace library {
/**
* @brief 为了避免Dll Hijacking，loadlibrary一律使用绝对绝对路径加载
* @param[in] lpDll   要加载的Dll的绝对路径名
* @param[in] bSysDll 是否是系统Dll；如果是系统Dll，则不在运行程序旁边查找dll，减少dll被替换的风险。
* @return 成功载入lpDll，返回Dll句柄，否则返回NULL
  * @remarks
*	调试状态下如果没有通过绝对目录加载，也没有通过相对于当前目录、system目录、windows目录加载会触发断言
*/
HMODULE BASE_EXPORT SafeLoadLibrary( LPCWSTR lpDll, BOOL bSysDll );

/**
* @brief 禁用第三方dll加载路径，避免Dll Hijacking
*/
void BASE_EXPORT ForbidLoadLibrarySearchCurrentDirectory( void );

}
}
#endif  // BASE_SAFE_LOAD_LIBRARY_SAFE_LOAD_LIBRARY_H_
