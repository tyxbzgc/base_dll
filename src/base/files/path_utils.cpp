// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/files/path_utils.h"
#include <Shlwapi.h>
#include <tchar.h>
#include <winerror.h>
#include "base/safe_str_func/safe_str_funs.h"
#include "base/public.h"
#include <Shlobj.h>  

namespace base {
namespace filemisc {
std::wstring GetModuleFileName(HINSTANCE hModule /* = 0 */) {
  bool gotName = false;
  std::wstring name = _T("UNAVAILABLE");
  if(!gotName) {
    TCHAR moduleFileName[MAX_PATH + 1];
    DWORD moduleFileNameLen = MAX_PATH;

    if(::GetModuleFileName(hModule, moduleFileName, moduleFileNameLen)) {
      name = moduleFileName;
    }

    gotName = true;
  }

  return name;
}

BOOL GetModuleFileName(LPWSTR pFileName, DWORD dwLen, HINSTANCE hModule) {
  assert(pFileName && dwLen > 0);

  return (::GetModuleFileName(hModule, pFileName, dwLen) != 0);
}

std::wstring GetSystemDirectory() {
  std::wstring s_strSysDir;
  if(s_strSysDir.empty()) {
    TCHAR szPath[MAX_PATH + 1] = { 0 };

    if(::GetSystemDirectory(szPath, MAX_PATH)) {
      s_strSysDir = szPath;
    }
  }

  return s_strSysDir;
}

BOOL GetSystemDirectory(LPWSTR pSysDir, DWORD dwLen) {
  assert(pSysDir && dwLen > 0);

  WCHAR szPath[MAX_PATH + 1] = { 0 };
  if(::GetSystemDirectory(szPath, MAX_PATH) == 0) {
    return FALSE;
  }
  if(dwLen < (wcslen(szPath) + 1)) {
    return FALSE;
  }
  return (0 == wcscpy_s(pSysDir, dwLen, szPath));
}


BOOL GetCurrentProcessPath(TCHAR* pchPath, DWORD dwSizeInTCHAR) {
  if(!pchPath || 0 == dwSizeInTCHAR) {
    return FALSE;
  }
  ::memset(pchPath, 0, dwSizeInTCHAR * sizeof(TCHAR));

  /// 获取当前进程的路径
  if(!::GetModuleFileName(NULL, pchPath, dwSizeInTCHAR)) {
    return FALSE;
  }

  TCHAR* pchLast = ::_tcsrchr(pchPath, _T('\\'));
  if(pchLast) {
    *pchLast = _T('\0');

    return TRUE;
  }

  return FALSE;
}

BOOL GetDrTempFile(LPTSTR lpszFile, int nSize, LPCTSTR lpszPri, LPCTSTR lpszPath) {
  if(!lpszFile || nSize <= 0) {
    return FALSE;
  }

  ::ZeroMemory(lpszFile, nSize * sizeof(TCHAR));

  if(!lpszPath || 0 == *lpszPath) {
    if(0l == ::GetTempPath(MAX_PATH, lpszFile)) {
      return FALSE;
    }
  } else {
    TCHAR_POINT_STRCPY(lpszFile, nSize, lpszPath);
    //::_tcsncpy(lpszFile, lpszPath, MAX_PATH);
  }

  TCHAR tchFileName[MAX_PATH + 1] = { 0 };
  //::srand((unsigned int)::time(NULL));
  ::Sleep(1);
  ::_sntprintf_s(tchFileName, MAX_PATH, _TRUNCATE, TEXT("Dr%x.drt"), ::GetTickCount());

  if(TEXT('\\') != lpszFile[::lstrlen(lpszFile) - 1]) {
    if(::lstrlen(lpszFile) + ::lstrlen(TEXT("\\")) >= nSize) {
      return FALSE;
    }

    TCHAR_POINT_STRCAT(lpszFile, nSize, _T("\\"));
    //::_tcsncat(lpszFile, TEXT("\\"), nSize - _tcslen(lpszFile));
  }
  if(lpszPri && 0 != *lpszPri) {
    if(::lstrlen(lpszFile) + ::lstrlen(lpszPri) >= nSize)
      return FALSE;

    TCHAR_POINT_STRCAT(lpszFile, nSize, lpszPri);
    //::_tcsncat(lpszFile, lpszPri, nSize - _tcslen(lpszFile));
  }
  if(::lstrlen(lpszFile) + ::lstrlen(tchFileName) < nSize) {
    TCHAR_POINT_STRCAT(lpszFile, nSize, tchFileName);
    //::_tcsncat(lpszFile, tchFileName, nSize - _tcslen(lpszFile));
    ::DeleteFile(lpszFile);
    return TRUE;
  }

  return FALSE;
}
BOOL GetMainPath(LPTSTR szFilePath, const DWORD dwLen, INT nLevel) {
  BOOL bRet = FALSE;
  TCHAR szDir[MAX_PATH + 1] = { 0 };
  //INT nLevel = 0;
  int i = 0;
  DWORD dwRes;
  TCHAR *p = NULL;

  dwRes = ::GetModuleFileName(NULL, szDir, MAX_PATH);
  BASEASSERT(0 != dwRes);
  if(0 == dwRes) {
    goto Exit0;
  }

  //nLevel = _GetPathReleativeLevel();
  for(i = 0; i <= nLevel; i++) {
    p = ::StrRChrI(szDir, p, L'\\');
    if(NULL == p) {
      BASEASSERT(FALSE);
      break;
    }

    *p = _T('\0');
  }

  ::_tcsncat_s(szDir, MAX_PATH, _T("\\"), 1);

  if(::_tcslen(szDir) > dwLen) {
    goto Exit0;
  }

  ::_tcsncpy_s(szFilePath, dwLen, szDir, dwLen - 1);

  bRet = TRUE;
Exit0:
  return bRet;
}

BOOL GetCurPathRootPath(LPTSTR szPath, DWORD dwSize, DWORD dwLevel /*=0*/) {
  BOOL bRet = FALSE;
  TCHAR szTmp[MAX_PATH + 1] = { 0 };
  INT i = 0;
  DWORD dwRes = 0;
  LPTSTR p = NULL;

  TS_PROCESS_ERROR(NULL != szPath && 0 != dwSize);

  dwRes = ::GetModuleFileName(NULL, szTmp, MAX_PATH);
  TS_PROCESS_ERROR(0 != dwRes);

  for(i = 0; i <= (INT)dwLevel; i++) {
    p = ::StrRChrI(szTmp, p, _T('\\'));
    TS_PROCESS_ERROR(p);
  }

  *++p = _T('\0');
  TS_PROCESS_ERROR(::_tcslen(szTmp) <= dwSize);
  ::_tcsncpy_s(szPath, dwSize, szTmp, dwSize - 1);

  bRet = TRUE;
Exit0:
  return bRet;
}

BOOL BASE_EXPORT GetAppData(TCHAR* pchPath)
{
    LPITEMIDLIST ppidl;
    wchar_t szappDataPath[_MAX_PATH] = { 0 };
    SHGetSpecialFolderLocation(0, CSIDL_APPDATA, &ppidl);
    SHGetPathFromIDList(ppidl, szappDataPath);
    wcscpy(pchPath, szappDataPath);
    return true;
}

BOOL EnumFileOfDir(LPCTSTR szPath, vstr &vFiles,
                    int nLevel/* = -1*/, LPTSTR szFilter/* = NULL*/) {
  HANDLE hFind = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA FindFileData;
  BOOL bRet = FALSE;
  BOOL bRes = TRUE;
  std::wstring strPath = szPath;

  if(NULL == szFilter) {
    szFilter = _T("*.*");
  }

  if(nLevel-- == 0) {
    return TRUE;
  }

  if(strPath[strPath.size() - 1] != _T('\\')) {
    strPath += _T("\\");
  }

  hFind = ::FindFirstFile((strPath + szFilter).c_str(), &FindFileData);
  TS_PROCESS_ERROR(INVALID_HANDLE_VALUE != hFind);

  while(bRes) {
    if(::_tcscmp(FindFileData.cFileName, _T(".")) != 0 && ::_tcscmp(FindFileData.cFileName, _T("..")) != 0) {
      if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        EnumFileOfDir((strPath + FindFileData.cFileName).c_str(), vFiles, nLevel);
      } else {
        vFiles.push_back((strPath + FindFileData.cFileName).c_str());
      }
    }

    bRes = ::FindNextFile(hFind, &FindFileData);
  }

  ::FindClose(hFind);
  bRet = TRUE;
Exit0:
  return bRet;
}

BOOL EnumFolderOfDir(LPCTSTR szPath, vstr &vFolders) {
  HANDLE hFind = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA FindFileData;
  BOOL bRet = FALSE;
  BOOL bRes = TRUE;
  std::wstring strPath = szPath;

  if(strPath[strPath.size() - 1] != _T('\\')) {
    strPath += _T("\\");
  }

  //strPath.append(_T("*.*"));
  hFind = ::FindFirstFile((strPath + _T("*.*")).c_str(), &FindFileData);
  TS_PROCESS_ERROR(INVALID_HANDLE_VALUE != hFind);

  while(bRes) {
    if(::_tcscmp(FindFileData.cFileName, _T(".")) != 0 && ::_tcscmp(FindFileData.cFileName, _T("..")) != 0) {
      if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        vFolders.push_back((strPath + FindFileData.cFileName).c_str());
      }
    }

    bRes = ::FindNextFile(hFind, &FindFileData);
  }

  ::FindClose(hFind);
  bRet = TRUE;
Exit0:
  return bRet;
}
}  // namespace filemisc
}  // namespace base
