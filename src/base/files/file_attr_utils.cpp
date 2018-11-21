// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/files/file_attr_utils.h"
#include "base/safe_load_library/safe_load_library.h"
#include "base/public.h"
#include "base/strings/string_utils.h"

#pragma comment(lib, "Version.lib")

#define PICKUP_STRING(str, name) \
do{ \
  if (str.empty()) \
  { \
    _sntprintf_s(szSubBlock, \
    sizeof(szSubBlock) / sizeof(TCHAR), _TRUNCATE, \
    _T("\\StringFileInfo\\%04x%04x\\%s"), \
    lpTranslate[i].wLanguage, \
    lpTranslate[i].wCodePage, \
    name); \
    \
    pTmpBuf = NULL; \
    \
    if (VerQueryValue( \
    pBuffer, \
    szSubBlock, \
    (LPVOID*)&pTmpBuf, \
    &dwTmpBufLen) \
    != FALSE) \
    { \
      str = pTmpBuf; \
    } \
  } \
} while(0)

#define PICKUP_STRING_T(str, name) \
do{ \
if ('\0' == str[0]) \
  { \
    _sntprintf_s(szSubBlock, \
    sizeof(szSubBlock)/sizeof(TCHAR), _TRUNCATE, \
    _T("\\StringFileInfo\\%04x%04x\\%s"), \
    lpTranslate[i].wLanguage, \
    lpTranslate[i].wCodePage, \
    name); \
    \
    pTmpBuf = NULL; \
    \
    if (VerQueryValue( \
    pBuffer, \
    szSubBlock, \
    (LPVOID*)&pTmpBuf, \
    &dwTmpBufLen) \
    != FALSE) \
    { \
      ::lstrcpyn(str, pTmpBuf, min(dwTmpBufLen, 126)+1); \
    } \
  } \
} while(0)

namespace base {
namespace filemisc {
BOOL GetFileVerInfoA(LPCSTR lpszFile, char* pchVersion, int nSize) {
  if(!lpszFile || '\0' == *lpszFile || !pchVersion || nSize <= 0)
    return FALSE;
  ::memset(pchVersion, 0, nSize * sizeof(char));

  BOOL  bRetResult = FALSE;
  BYTE* pBuffer = NULL;
  DWORD dwBufLen = 0;
  char  strVersion[MAX_PATH + 1] = { 0 };
  char  szSubBlock[MAX_PATH + 1] = { 0 };
  char* pTmpBuf = NULL;
  UINT  dwTmpBufLen = 0;
  UINT  i = 0;
  UINT  nVerCodeLen = 0;
  LANGANDCODEPAGE* lpTranslate = NULL;

  dwBufLen = ::GetFileVersionInfoSizeA((LPSTR)lpszFile, NULL);
  if(0 == dwBufLen)
    goto lExit0;

  pBuffer = new BYTE[dwBufLen];
  if(pBuffer == NULL)
    goto lExit0;

  if(!::GetFileVersionInfoA((LPSTR)lpszFile, NULL, dwBufLen, pBuffer))
    goto lExit0;

  if(!::VerQueryValueA(pBuffer, "\\VarFileInfo\\Translation",
    (LPVOID*)&lpTranslate, &nVerCodeLen
  )
      ) {
    goto lExit0;
  }

  for(i = 0; i < (nVerCodeLen / sizeof(LANGANDCODEPAGE)); i++) {
    ::_snprintf_s(szSubBlock, sizeof(szSubBlock) / sizeof(char), _TRUNCATE,
                  "\\StringFileInfo\\%04x%04x\\FileVersion",
                  lpTranslate[i].wLanguage, lpTranslate[i].wCodePage
    );

    pTmpBuf = NULL;
    if(::VerQueryValueA(pBuffer, szSubBlock, (LPVOID*)&pTmpBuf, &dwTmpBufLen)) {
      ::lstrcpynA(strVersion, pTmpBuf, MAX_PATH);
      break;
    }
  }

  ::lstrcpynA(pchVersion, strVersion, nSize);
  bRetResult = TRUE;
lExit0:
  if(NULL != pBuffer) {
    delete[] pBuffer;
    pBuffer = NULL;
  }

  return bRetResult;
}

BOOL GetFileVerInfoW(LPCWSTR lpszFile, wchar_t* pchVersion, int nSize) {
  BOOL		bRetResult = FALSE;
  BYTE*		pBuffer = NULL;
  DWORD		dwBufLen = 0;
  wchar_t		strVersion[MAX_PATH + 1] = { 0 };
  wchar_t		szSubBlock[MAX_PATH + 1] = { 0 };
  wchar_t*	pTmpBuf = NULL;
  UINT		dwTmpBufLen = 0;
  UINT		i = 0;
  UINT		nVerCodeLen = 0;
  LANGANDCODEPAGE*    lpTranslate = NULL;

  if(NULL == lpszFile || '\0' == *lpszFile || !pchVersion || nSize <= 0) {
    return FALSE;
  }

  ::memset(pchVersion, 0, nSize * sizeof(wchar_t));

  dwBufLen = ::GetFileVersionInfoSizeW((LPWSTR)lpszFile, NULL);
  if(0 == dwBufLen) {
    goto lExit0;
  }

  pBuffer = new BYTE[dwBufLen];
  if(NULL == pBuffer) {
    goto lExit0;
  }

  if(!::GetFileVersionInfoW((LPWSTR)lpszFile, NULL, dwBufLen, pBuffer)) {
    goto lExit0;
  }

  if(!::VerQueryValueW(pBuffer, L"\\VarFileInfo\\Translation",
    (LPVOID*)&lpTranslate, &nVerCodeLen
  )
      ) {
    goto lExit0;
  }

  for(i = 0; i < (nVerCodeLen / sizeof(LANGANDCODEPAGE)); i++) {
    ::_snwprintf_s(szSubBlock, sizeof(szSubBlock) / sizeof(wchar_t), _TRUNCATE,
                    L"\\StringFileInfo\\%04x%04x\\FileVersion",
                    lpTranslate[i].wLanguage, lpTranslate[i].wCodePage
    );

    pTmpBuf = NULL;
    if(::VerQueryValueW(pBuffer, szSubBlock, (LPVOID*)&pTmpBuf, &dwTmpBufLen)) {
      ::lstrcpynW(strVersion, pTmpBuf, MAX_PATH);
      break;
    }
  }

  ::lstrcpynW(pchVersion, strVersion, nSize);
  bRetResult = TRUE;

lExit0:
  if(NULL != pBuffer) {
    delete[] pBuffer;
    pBuffer = NULL;
  }

  return bRetResult;
}

BOOL IsFileExist(LPCTSTR szFile) {
  HANDLE		hFile;

  // 根据文件路径打开文件
  hFile = CreateFile(
    szFile,
    0,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);
  if(hFile == INVALID_HANDLE_VALUE) {
    int err = GetLastError();
    return FALSE;
  } else {
    // 打开成功后关闭
    CloseHandle(hFile);
    return TRUE;
  }
}

BOOL IsDirectoryExist(LPCTSTR szDir) {
  DWORD dwAttr = GetFileAttributes(szDir);
  if(dwAttr != (DWORD)-1 && ((dwAttr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOL GetFileInfo(LPCTSTR szFile,
                  TCHAR* lpszCompany,
                  TCHAR* lpszFileDesc,
                  TCHAR* lpszVersion) {
  BOOL		bRetResult = FALSE;
  BYTE*		pBuffer = NULL;
  DWORD		dwBufLen = 0;
  TCHAR		szSubBlock[MAX_PATH];
  TCHAR*		pTmpBuf = NULL;
  UINT		dwTmpBufLen = 0;
  UINT		i = 0;
  UINT		nVerCodeLen = 0;
  LANGANDCODEPAGE* lpTranslate = NULL;

  dwBufLen = GetFileVersionInfoSize((LPTSTR)szFile, NULL);
  if(dwBufLen == 0)
    goto lExit0;

  pBuffer = new BYTE[dwBufLen];
  if(pBuffer == NULL)
    goto lExit0;

  if(!GetFileVersionInfo((LPTSTR)szFile, NULL, dwBufLen, pBuffer))
    goto lExit0;

  if(!VerQueryValue(pBuffer,
                    TEXT("\\VarFileInfo\\Translation"),
                    (LPVOID*)&lpTranslate,
                    &nVerCodeLen)) {
    goto lExit0;
  }

  ::ZeroMemory(szSubBlock, sizeof(szSubBlock));
  for(i = 0; i<(nVerCodeLen / sizeof(LANGANDCODEPAGE)); i++) {
    if(NULL != lpszCompany && 0 == *lpszCompany) {
      PICKUP_STRING_T(lpszCompany, _T("CompanyName"));
    }
    if(NULL != lpszFileDesc && 0 == *lpszFileDesc) {
      PICKUP_STRING_T(lpszFileDesc, _T("FileDescription"));
    }
    if(NULL != lpszVersion && 0 == *lpszVersion) {
      PICKUP_STRING_T(lpszVersion, _T("FileVersion"));
    }
  }

  bRetResult = TRUE;

lExit0:
  if(pBuffer != NULL)
    delete[] pBuffer;

  return bRetResult;
}

BOOL IsFileChangable(LPCTSTR szFile) {
  HANDLE		hFile;
  DWORD		dwAttr;

  if(!IsFileExist(szFile)) {
    return TRUE;
  }

  dwAttr = ::GetFileAttributes(szFile);
  if(dwAttr & FILE_ATTRIBUTE_READONLY) {
    ::SetFileAttributes(
      szFile,
      (dwAttr&(~FILE_ATTRIBUTE_READONLY))
    );
  }

  hFile = ::CreateFile(szFile, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile != INVALID_HANDLE_VALUE) {
    ::CloseHandle(hFile);
    return TRUE;
  }

  return FALSE;
}

BOOL GetFileTimeFlex(
  LPCTSTR szFile,
  LPFILETIME lpCreationTime,
  LPFILETIME lpLastAccessTime,
  LPFILETIME lpLastWriteTime
) {
  BOOL	bRetResult = FALSE;
  HANDLE	hFile = INVALID_HANDLE_VALUE;

  hFile = CreateFile(
    szFile,
    GENERIC_READ,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );

  if(hFile == INVALID_HANDLE_VALUE) {
    goto lExit0;
  }

  bRetResult = GetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);

lExit0:
  if(hFile != INVALID_HANDLE_VALUE) {
    CloseHandle(hFile);
  }

  return bRetResult;
}

BOOL IsPEFile(LPCTSTR lpszFile) {
  HANDLE hFile = ::CreateFile(lpszFile,
                              GENERIC_READ, FILE_SHARE_READ, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
  );
  if(INVALID_HANDLE_VALUE == hFile) {
    return FALSE;
  }

  IMAGE_DOS_HEADER imgDosHd = { 0 };
  IMAGE_NT_HEADERS imgNtHd = { 0 };
  DWORD dwReaded = 0l;
  BOOL bRet = FALSE;

  if(!::ReadFile(hFile, &imgDosHd, sizeof(imgDosHd), &dwReaded, NULL)
      || dwReaded != sizeof(imgDosHd)
      || imgDosHd.e_magic != IMAGE_DOS_SIGNATURE
      ) {
    goto lExit0;
  }

  if(imgDosHd.e_lfanew !=
    (long)::SetFilePointer(hFile, imgDosHd.e_lfanew, 0l, FILE_BEGIN)
      ) {
    goto lExit0;
  }

  if(!::ReadFile(hFile, &imgNtHd, sizeof(imgNtHd), &dwReaded, NULL)
      || dwReaded != sizeof(imgNtHd)
      || imgNtHd.Signature != IMAGE_NT_SIGNATURE
      ) {
    goto lExit0;
  }

  bRet = TRUE;

lExit0:

  if(INVALID_HANDLE_VALUE != hFile) {
    ::CloseHandle(hFile);
    hFile = NULL;
  }

  return bRet;
}


BOOL GetPEVersion(
  unsigned int* pnMajor,
  unsigned int* pnMinor,
  unsigned int* pnBuild,
  unsigned int* pnLast,
  const TCHAR* ptchPeFile
) {
  BOOL bRet = FALSE;
  TCHAR szPeFile[MAX_PATH + 1] = { 0 };
  TCHAR szVer[MAX_PATH + 1] = { 0 };

  if(ptchPeFile && '\0' != *ptchPeFile) {
    ::lstrcpyn(szPeFile, ptchPeFile, MAX_PATH);
  } else {
    ::GetModuleFileName(NULL, szPeFile, MAX_PATH);
  }

  // 獲取版本信息
  if(GetFileInfo(szPeFile, NULL, NULL, szVer)) {
    LPTSTR p = (LPTSTR)szVer;
    while(p && (*p)) {
      if(TEXT(',') == *p)
        *p = TEXT('.');
      p++;
    }

    int nFieldNum = 0;

    // 轉化成版本號
    nFieldNum = ::_stscanf_s(szVer,
                              _T("%u.%u.%u.%u"),
                              pnMajor,
                              pnMinor,
                              pnBuild,
                              pnLast);
    if(4 == nFieldNum) {
      bRet = TRUE;
    } else {
      bRet = FALSE;
    }
  }

  return bRet;
}

BOOL GetFileVersion(
  LPCWSTR lpszFileName,
  unsigned int& wMajorVersion,
  unsigned int& wMinorVersion,
  unsigned int& wRevisionVersion,
  unsigned int& wBuildVersion) {
  WCHAR		m_lpszFileName[MAX_PATH];
  BYTE*		m_pData;

  ZeroMemory(m_lpszFileName, _countof(m_lpszFileName) * sizeof(WCHAR));
  HRESULT hr = base::string_utils::Strcpy(m_lpszFileName, _countof(m_lpszFileName), lpszFileName);

  DWORD dwVersionInfoSize = ::GetFileVersionInfoSizeW(m_lpszFileName, 0);
  if(dwVersionInfoSize == 0) {
    return FALSE;
  }

  m_pData = new BYTE[dwVersionInfoSize];
  ZeroMemory(m_pData, dwVersionInfoSize);

  BOOL rc1 = ::GetFileVersionInfoW(m_lpszFileName, 0, dwVersionInfoSize, m_pData);
  if(!rc1) {
    return FALSE;
  }

  // Get fixed info
  VS_FIXEDFILEINFO FixedInfo;
  VS_FIXEDFILEINFO* pFixedInfo = NULL;
  UINT nLength = 0;

  BOOL rc2 = ::VerQueryValueW(m_pData, L"\\", (LPVOID*)&pFixedInfo, &nLength);
  if(!rc2) {
    return FALSE;
  }
  memcpy(&FixedInfo, pFixedInfo, sizeof(VS_FIXEDFILEINFO));

  wMajorVersion = HIWORD(FixedInfo.dwFileVersionMS);
  wMinorVersion = LOWORD(FixedInfo.dwFileVersionMS);
  wRevisionVersion = HIWORD(FixedInfo.dwFileVersionLS);
  wBuildVersion = LOWORD(FixedInfo.dwFileVersionLS);

  delete[] m_pData;
  m_pData = NULL;

  return TRUE;

}


BOOL GetFileSingleInfo(LPCTSTR szFile, LPCTSTR szFileInfoName, WCHAR*wchFileInfo, int* pnSize) {
  if(!wchFileInfo) {
    return FALSE;
  }
  BOOL bRetResult = FALSE;
  BYTE* pBuffer = NULL;
  DWORD dwBufLen;
  std::wstring csInfo;
  TCHAR szSubBlock[MAX_PATH];
  TCHAR* pTmpBuf = NULL;
  UINT dwTmpBufLen;
  UINT i;
  UINT nVerCodeLen;
  LANGANDCODEPAGE*    lpTranslate;

  dwBufLen = GetFileVersionInfoSize((LPTSTR)szFile, NULL);
  if(dwBufLen == 0)
    goto Exit0;

  pBuffer = new BYTE[dwBufLen];
  if(pBuffer == NULL)
    goto Exit0;

  if(GetFileVersionInfo((LPTSTR)szFile, NULL, dwBufLen, pBuffer) == FALSE)
    goto Exit0;

  if(VerQueryValue(pBuffer,
                    TEXT("\\VarFileInfo\\Translation"),
                    (LPVOID*)&lpTranslate,
                    &nVerCodeLen)
      == FALSE) {
    goto Exit0;
  }

  memset(szSubBlock, 0, sizeof(szSubBlock));
  for(i = 0; i<(nVerCodeLen / sizeof(LANGANDCODEPAGE)); i++) {
    PICKUP_STRING(csInfo, szFileInfoName);
  }
  if(*pnSize < (int)(csInfo.length() + 1)) {
    *pnSize = csInfo.length() + 1;
    goto Exit0;
  }
  wcsncpy_s(wchFileInfo, *pnSize, csInfo.c_str(), _TRUNCATE);

  bRetResult = TRUE;
Exit0:
  if(pBuffer != NULL)
    delete[] pBuffer;

  return bRetResult;
}

unsigned long GetFileSizeEx(LPCTSTR lpszFile) {
  HANDLE hFile = INVALID_HANDLE_VALUE;
  unsigned long ulSize = 0;

  hFile = ::CreateFile(lpszFile,
                        0, FILE_SHARE_READ,
                        NULL, OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL
  );
  if(INVALID_HANDLE_VALUE == hFile) {
    return ulSize;
  }

  ulSize = ::GetFileSize(hFile, NULL);
  ::CloseHandle(hFile);
  hFile = INVALID_HANDLE_VALUE;

  return ulSize;
}


DWORD64 GetFolderSize(LPCTSTR tszPath, DWORD *pdwFileCnt, DWORD *pdwFolderCnt) {
  DWORD64 dwSize = 0;
  HANDLE hFind = NULL;
  WIN32_FIND_DATA fileinfo = { 0 };
  TCHAR szFilePath[MAX_PATH + 1] = { 0 };
  TCHAR szTmpPath[MAX_PATH + 1] = { 0 };

  ::_sntprintf_s(szFilePath, MAX_PATH, _TRUNCATE, _T("%s\\*.*"), tszPath);

  hFind = ::FindFirstFile(szFilePath, &fileinfo);
  if(INVALID_HANDLE_VALUE != hFind) {
    szFilePath[_tcslen(szFilePath) - 3] = '\0';

    do {
      if(fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if(!_tcsicmp(fileinfo.cFileName, _T(".")) || !_tcsicmp(fileinfo.cFileName, _T(".."))) {
          continue;
        } else {
          ::_sntprintf_s(szTmpPath, MAX_PATH, _TRUNCATE, _T("%s%s"), szFilePath, fileinfo.cFileName);
          dwSize = dwSize + GetFolderSize(szTmpPath, pdwFileCnt, pdwFolderCnt);
          if(pdwFolderCnt != NULL) {
            ++(*pdwFolderCnt);
          }
        }
      } else {
        if(pdwFileCnt != NULL) {
          ++(*pdwFileCnt);
        }
      }

      dwSize += fileinfo.nFileSizeLow;

    } while(FindNextFile(hFind, &fileinfo));

    if(hFind) {
      FindClose(hFind);
      hFind = NULL;
    }
  }

  return dwSize;
}


BOOL GetCurPEVersionA(char* pchBuf, int nSize, const TCHAR* pchPeFile) {
  int nRet = 0;
  unsigned int nMajor = 0;
  unsigned int nMinor = 0;
  unsigned int nBuild = 0;
  unsigned int nLast = 0;

  if(!pchBuf || nSize <= 0) {
    return FALSE;
  }

  ::memset(pchBuf, 0, nSize);

  if(!GetPEVersion(&nMajor, &nMinor, &nBuild, &nLast, pchPeFile)) {
    return FALSE;
  }

  nRet = ::_snprintf_s(pchBuf, nSize - 1, _TRUNCATE, "%d.%d.%d.%d", nMajor, nMinor, nBuild, nLast);
  return (nRet > 0);
}

BOOL GetNumberVerFromStringVer(
  unsigned int* pnMajor,
  unsigned int* pnMinor,
  unsigned int* pnBuild,
  unsigned int* pnLast,
  const TCHAR* pchVersion
) {
  int nFieldNum = 0;

  // 轉化成數字版本號
  nFieldNum = ::_stscanf_s(pchVersion,
                            _T("%u.%u.%u.%u"),
                            pnMajor,
                            pnMinor,
                            pnBuild,
                            pnLast);

  return (4 == nFieldNum) ? TRUE : FALSE;
}

BOOL IsFileUsing(LPCTSTR pszPath) {
  BOOL bRet = TRUE;
  HANDLE hFile = INVALID_HANDLE_VALUE;

  TS_PROCESS_ERROR(pszPath);

  // 判断是否被占用
  hFile = CreateFile(
    pszPath,
    GENERIC_READ,
    0, // 独占打开
    NULL,
    OPEN_EXISTING,
    0,
    0);

  if(INVALID_HANDLE_VALUE != hFile) {
    bRet = FALSE;
  }

  CloseHandle(hFile);

Exit0:
  return bRet;
}

BOOL DllGetVersion(LPCTSTR pchDllName, DLLVERSIONINFO& DllVersion) {
  typedef HRESULT(CALLBACK* DLLGETVERSION)(DLLVERSIONINFO*);
  if(!pchDllName) {
    return FALSE;
  }
  size_t stDllName = ::lstrlen(pchDllName);
  if(0 == stDllName || stDllName > MAX_PATH) {
    return FALSE;
  }

  HMODULE hModule = base::library::SafeLoadLibrary(pchDllName, FALSE);
  if(!hModule) {
    return FALSE;
  }

  BOOL bRet = FALSE;
  ::memset(&DllVersion, 0, sizeof(DllVersion));
  DllVersion.cbSize = sizeof(DllVersion);

  DLLGETVERSION pfnDllGetVersion = (DLLGETVERSION)
    ::GetProcAddress(hModule, "DllGetVersion");
  if(pfnDllGetVersion) {
    bRet = SUCCEEDED((*pfnDllGetVersion)(&DllVersion));
  }

  ::FreeLibrary(hModule);
  hModule = NULL;
  pfnDllGetVersion = NULL;

  return bRet;
}

/*BOOL GetFileMD5(LPCTSTR szFilePath, LPTSTR szMD5, DWORD dwLen)
{
return FileCalcMD5Center::FileGetMD5(szFilePath, szMD5, dwLen);
}*/

/*BOOL FileVerifySign(LPCTSTR szFilePath, BOOL &bValid)
{
return FileSignVerCenter::FileVerifySign(szFilePath, bValid);
}*/

/*HICON GetFileDefaultIcon(LPCTSTR szFilePath)
{
return FileDefaultIconPicker::FileGetDefaultIcon(szFilePath);
}*/

BOOL IsPathFile(LPCTSTR szFilePath, BOOL &bFile) {
  BOOL bRes = FALSE;
  DWORD dwAttrib = 0;

  bRes = ::PathFileExists(szFilePath);
  if(bRes) {
    dwAttrib = ::GetFileAttributes(szFilePath);
    if(FILE_ATTRIBUTE_DIRECTORY & dwAttrib) {
      bFile = FALSE;
    } else {
      bFile = TRUE;
    }
  } else {
    bFile = FALSE;
  }

  return bRes;
}

BOOL IsPathDir(LPCTSTR szFilePath, BOOL &bDir) {
  BOOL bRes = FALSE;
  BOOL bTmp = FALSE;

  bRes = IsPathFile(szFilePath, bTmp);
  if(bRes) {
    bDir = (bTmp ? FALSE : TRUE);
  } else
    bDir = FALSE;

  return bRes;
}



BOOL CreateDirectoryNested(LPCTSTR lpszDir) {
  if(::PathIsDirectory(lpszDir))
    return TRUE;

  TCHAR szPreDir[MAX_PATH + 1];
  _tcscpy_s(szPreDir, lpszDir);

  int nLen = lstrlen(szPreDir);
  if('\\' == szPreDir[nLen - 1])
    szPreDir[nLen - 1] = '0';

  if(!::PathRemoveFileSpec(szPreDir))//将路径末尾的文件名和反斜杠去掉
    return FALSE;

  if(!::PathIsDirectory(szPreDir))
    CreateDirectoryNested(szPreDir);

  return ::CreateDirectory(lpszDir, NULL);
}
}//namespace filemisc
}//namespace base
