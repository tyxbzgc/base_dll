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
    //ö��ϵͳ��Ŀ¼
    std::wstring strEnvPath;
    std::wstring strLoadDll;

    //���ܽ����ľ���һ��ȫ·��,�����ξ��ҵ����Ǿ������
    if(strLoadDll.empty()
        && 1 == strDll.find(_T(":"))) {
      strLoadDll = strDll;
      if(!::PathFileExists(strLoadDll.c_str())) {
        strLoadDll.clear();
      }
    }

    //������ϵͳDll,�Ų��ҵ�ǰ����Ŀ¼����ֹhijack©��
    //���Ȼ�ȡ�������е�Ŀ¼
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

    //��ȡϵͳsystem32Ŀ¼
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

    //��ȡϵͳwindowsĿ¼
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

    //��������
    if(!strLoadDll.empty()) {
      if(::PathFileExists(strLoadDll.c_str())) {
        hHandle = ::LoadLibrary(strLoadDll.c_str());
      }
    }

    //ʵ�����Ҳ����ˣ�Ϊ�˻����ܹ�ʹ�ã�����ϵͳ��Ĭ����������
    if(NULL == hHandle) {
      BASEASSERT(0 && _T("Security risks Found!"));//û��ͨ��������·������dll�����ڰ�ȫ����
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
