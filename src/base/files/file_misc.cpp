// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/files/file_misc.h"
#include <WinInet.h>
#include <atlstr.h>
#include <atlfile.h>
#include <set>
#include "base/files/file_attr_utils.h"
#pragma comment(lib, "Version.lib")

// htmlÎÄ¼þºó×º
static LPCWSTR szHtmlType[] = { L".html", L".htm", L".mht" };
__declspec(selectany) long g_lIEVersion = 0;

namespace base {
namespace filemisc {
BOOL IsPathString(LPCTSTR lpszPath) {
  BOOL bDir;
  filemisc::IsPathDir(lpszPath, bDir);
  if(bDir)
    return true;
  return false;
}

BOOL IsHtmlFile(LPCTSTR lpszFile) {
  if(!lpszFile)
    return FALSE;

  BOOL bHtml = FALSE;

  LPCTSTR lpexe = PathFindExtension(lpszFile);

  if(lstrlen(lpexe) > 0) {
    LPTSTR lpexe2 = _wcsdup(lpexe);
    _wcslwr_s(lpexe2, lstrlen(lpexe2) + 1);

    static std::set<CString> setHtml;
    if(setHtml.empty()) {
      for(int i = 0; i < _countof(szHtmlType); i++) {
        setHtml.insert(szHtmlType[i]);
      }
    }

    bHtml = (setHtml.find(lpexe2) != setHtml.end());

    free(lpexe2);
    lpexe2 = NULL;
  }

  return bHtml;
}

LPCWSTR GetPathFileInvalidChar() {
  return _T("/\\*?\"<>|:");
}

std::wstring FormatFileName(const std::wstring &szFile) {
  const wchar_t* pSysStr[] = { L"con" , L"aux" , L"com1",L"com2",L"com3",L"com4",L"com5",L"com6",L"com7",L"com8",L"com9",L"lpt1",L"lpt2",L"lpt3",L"lpt4",L"lpt5",L"lpt6",L"lpt7",L"lpt8",L"lpt9",L"prn",L"nul" };
  size_t nPos = szFile.find_last_of('.');
  std::wstring szName = szFile.substr(0, nPos);
  std::wstring szExt = nPos == std::wstring::npos ? L"" : szFile.substr(nPos, szFile.length() - nPos);

  DWORD nInvaildPos = szName.find_first_of(base::filemisc::GetPathFileInvalidChar(), 0);
  bool bReplaceAtLeastOnce = false;
  while(nInvaildPos != std::wstring::npos) {
    bReplaceAtLeastOnce = true;
    szName[nInvaildPos] = '_';
    nInvaildPos = szName.find_first_of(base::filemisc::GetPathFileInvalidChar(), nInvaildPos);
  }

  if(bReplaceAtLeastOnce)
    return szName + szExt;

  for(int i = 0; i < _countof(pSysStr); i++) {
    if(_wcsnicmp(szName.c_str(), pSysStr[i], wcslen(pSysStr[i])) == 0) {
      szName += L"_";
      return szName + szExt;
    }
  }

  return szFile;
}
}  // namespace filemisc
}  // namespace base
