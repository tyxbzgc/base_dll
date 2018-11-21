// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/safe_load_library/safe_load_library.h"
#include <Shlwapi.h>
#include "base/public.h"
#include "base/strings/string_utils.h"

namespace base {
namespace library {
HMODULE SafeLoadLibrary(LPCWSTR lpDll, BOOL bSysDll) {
  std::wstring strDll = lpDll;

  HMODULE hHandle = NULL;

  if(!strDll.empty()) {
    //枚举系统的目录
    std::wstring strEnvPath;
    std::wstring strLoadDll;

    //可能进来的就是一个全路径,如果这次就找到了那就最好了
    if(strLoadDll.empty()
        && 1 == strDll.find(_T(":"))) {
      strLoadDll = strDll;
      if(!::PathFileExists(strLoadDll.c_str())) {
        strLoadDll.clear();
      }
    }

    //若不是系统Dll,才查找当前运行目录，防止hijack漏洞
    //最先获取程序运行的目录
    if(strLoadDll.empty()
        && !bSysDll) {
      TCHAR szPath[MAX_PATH + 2] = { 0 };
      GetModuleFileName(NULL, szPath, MAX_PATH);

      strEnvPath = szPath;
      size_t nPos = strEnvPath.rfind(_T('\\'));
      if(-1 != nPos) {
        strEnvPath = strEnvPath.substr(0, nPos + 1);
      }

      if(!strEnvPath.empty()) {
        strLoadDll = strEnvPath + strDll;
        if(!::PathFileExists(strLoadDll.c_str())) {
          strLoadDll.clear();
        }
      }
    }

    //获取系统system32目录
    if(strLoadDll.empty()) {
      strEnvPath.clear();

      TCHAR moduleFileName[MAX_PATH + 1];
      DWORD moduleFileNameLen = MAX_PATH;

      if(::GetSystemDirectory(moduleFileName, moduleFileNameLen)) {
        strEnvPath = moduleFileName;
      }
      if(!strEnvPath.empty()) {

        strLoadDll = strEnvPath + _T("\\") + strDll;
        if(!::PathFileExists(strLoadDll.c_str())) {
          strLoadDll.clear();
        }
      }
    }

    //获取系统windows目录
    if(strLoadDll.empty()) {
      strEnvPath.clear();
      TCHAR moduleFileName[MAX_PATH + 1];
      DWORD moduleFileNameLen = MAX_PATH;

      if(::GetWindowsDirectory(moduleFileName, moduleFileNameLen)) {
        strEnvPath = moduleFileName;
      }
      if(!strEnvPath.empty()) {
        strLoadDll = strEnvPath + _T("\\") + strDll;
        if(!::PathFileExists(strLoadDll.c_str())) {
          strLoadDll.clear();
        }
      }
    }

    //不再找了
    if(!strLoadDll.empty()) {
      if(::PathFileExists(strLoadDll.c_str())) {
        hHandle = ::LoadLibrary(strLoadDll.c_str());
      }
    }

    //实在是找不到了，为了还是能够使用，进行系统的默认搜索加载
    if(NULL == hHandle) {
      BASEASSERT(0 && _T("Security risks Found!"));//没有通过完整的路径加载dll，存在安全隐患
      hHandle = ::LoadLibrary(strDll.c_str());
    }
  }

  return hHandle;
}

void ForbidLoadLibrarySearchCurrentDirectory(void) {
  typedef BOOL(WINAPI *Fun_SetDllDirectory)(__in_opt LPCTSTR);
  Fun_SetDllDirectory lfSetDllDirectory = NULL;

  HMODULE hModule = SafeLoadLibrary(_T("Kernel32.dll"), TRUE);
  if(hModule) {
#ifdef _UNICODE
    lfSetDllDirectory = (Fun_SetDllDirectory)::GetProcAddress(hModule, "SetDllDirectoryW");
#else
    lfSetDllDirectory = (Fun_SetDllDirectory)::GetProcAddress(hModule, "SetDllDirectoryA");
#endif //_UNICODE

    if(lfSetDllDirectory) {
      BOOL b = lfSetDllDirectory(_T(""));
      DWORD dw = GetLastError();
    }
  }

  return;
}
}  // namespace library 
}  // namespace base 
