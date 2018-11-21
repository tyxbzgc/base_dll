// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/files/file_opt_utils.h"
#include <Shellapi.h>
#include <atlstr.h>
#include "base/public.h"
#include "base/strings/string_operate.h"

typedef HRESULT(WINAPI *SHSetFolderPathW_t)(int csidl, HANDLE hToken, DWORD dwFlags, LPCTSTR pszPath);

namespace base {
namespace filemisc {
PVOID BuildEveryoneSD(const DWORD dwAccessMask,
                      PSECURITY_DESCRIPTOR pSD) {
  SID_IDENTIFIER_AUTHORITY siaWorld = SECURITY_WORLD_SID_AUTHORITY;
  SECURITY_INFORMATION si = DACL_SECURITY_INFORMATION;
  PSID psidEveryone = NULL;
  PACL pDacl = NULL;
  DWORD dwAclSize = 0;
  BOOL bRet = FALSE;

  // prepare a Sid representing ther Everyone group
  bRet = AllocateAndInitializeSid(
    &siaWorld,
    1,
    SECURITY_WORLD_RID,
    0,
    0, 0, 0, 0, 0, 0,
    &psidEveryone
  );
  TS_PROCESS_ERROR(bRet);

  // compute size of new acl
  dwAclSize = sizeof(ACL)
    + (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD))
    + GetLengthSid(psidEveryone)
    ;

  // allocate storage for Acl
  pDacl = (PACL)HeapAlloc(GetProcessHeap(), 0, dwAclSize);
  TS_PROCESS_ERROR(bRet);

  bRet = InitializeAcl(pDacl, dwAclSize, ACL_REVISION);
  TS_PROCESS_ERROR(bRet);

  // add the Authenticated Users group ACE to the DACL with access
  bRet = AddAccessAllowedAce(
    pDacl,
    ACL_REVISION,
    dwAccessMask,
    psidEveryone
  );
  TS_PROCESS_ERROR(bRet);

  bRet = InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
  TS_PROCESS_ERROR(bRet);

  bRet = SetSecurityDescriptorDacl(pSD, TRUE, pDacl, FALSE);
Exit0:
  if(NULL != pDacl && !bRet) {
    HeapFree(GetProcessHeap(), 0, pDacl);
    pDacl = NULL;
  }

  if(NULL != psidEveryone) {
    FreeSid(psidEveryone);
    psidEveryone = NULL;
  }

  return pDacl;
}

BOOL SetFileSecurity(
  LPCTSTR pszFile,
  const DWORD dwAccessMask/* = GENERIC_ALL*/) {
  BOOL bRet = FALSE;
  SECURITY_DESCRIPTOR sd = { 0 };
  PVOID pDacl = NULL;

  // get the SECURITY_DESCRIPTOR for Everyone group
  pDacl = BuildEveryoneSD(dwAccessMask, &sd);
  TS_PROCESS_ERROR(pDacl);

  // apply the security descriptor to the registry key
  bRet = ::SetFileSecurity(
    pszFile,
    (SECURITY_INFORMATION)DACL_SECURITY_INFORMATION,
    &sd
  );

  if(NULL != pDacl) {
    HeapFree(GetProcessHeap(), 0, pDacl);
    pDacl = NULL;
  }

Exit0:
  return bRet;
}

void DeleteFileAfterReboot(TCHAR *ptszPath) {
  if(ptszPath == NULL) {
    return;
  }

  MoveFileEx(ptszPath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
}

//BOOL DeleteDirectory( const wchar_t* wchFolderName )
//{
//	WIN32_FIND_DATA FileData; 
//	HANDLE hSearch; 

//	DWORD dwAttrs = GetFileAttributesW(wchFolderName); 
//	if ((dwAttrs & FILE_ATTRIBUTE_READONLY)) 
//	{ 
//		SetFileAttributesW(wchFolderName, 
//			dwAttrs ^ FILE_ATTRIBUTE_READONLY); 
//	} 

//	TCHAR szPathName[MAX_PATH];
//	memset(szPathName, 0, sizeof(szPathName));

//	Base::StringUtils::Strcat(szPathName,sizeof(szPathName)/sizeof(szPathName[0]),wchFolderName);
//	_SAFE_STRING_APPEND_BLASHW(szPathName);
//	_SAFE_STRING_APPENDW(szPathName, _T("*.*"));

//	hSearch = FindFirstFile(szPathName, &FileData); 
//	if (hSearch == INVALID_HANDLE_VALUE) 
//	{ 
//		return FALSE; 
//	} 

//	TCHAR szSubPathName[MAX_PATH];
//	do
//	{
//		if (FileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
//		{
//			memset(szSubPathName, 0, sizeof(szSubPathName));

//			_SAFE_STRING_APPENDW(szSubPathName, wchFolderName);
//			_SAFE_STRING_APPEND_BLASHW(szSubPathName);
//			_SAFE_STRING_APPENDW(szSubPathName, FileData.cFileName);

//			DeleteFile(szSubPathName);
//		}
//		else if (FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
//			&& _tcsicmp(FileData.cFileName, _T(".")) != 0
//			&& _tcsicmp(FileData.cFileName, _T("..")) != 0)
//		{
//			memset(szSubPathName, 0, sizeof(szSubPathName));

//			_SAFE_STRING_APPENDW(szSubPathName, wchFolderName);
//			_SAFE_STRING_APPEND_BLASHW(szSubPathName);
//			_SAFE_STRING_APPENDW(szSubPathName, FileData.cFileName);

//			DeleteDirectory(szSubPathName);
//		}
//	} while (FindNextFile(hSearch, &FileData));

//	// Close the search handle. 
//	FindClose(hSearch); 

//	return (RemoveDirectoryW(wchFolderName) != 0);
//}


BOOL DeleteFileEx(LPCTSTR pszPath) {
  BASEASSERT(pszPath);
  //
  BOOL bRet = FALSE;
  //
  bRet = ::PathFileExists(pszPath);
  if(!bRet) {
    return TRUE;
  }
  //
  bRet = ::DeleteFile(pszPath);
  if(bRet) {
    return TRUE;
  }
  //
  ::SetFileAttributes(pszPath, FILE_ATTRIBUTE_NORMAL);
  bRet = ::DeleteFile(pszPath);
  if(bRet) {
    return TRUE;
  }
  //
  SetFileSecurity(pszPath);
  bRet = ::DeleteFile(pszPath);
  if(bRet) {
    return TRUE;
  }
  //
  return bRet;
}

VOID CopyFilePath(std::wstring csFilePath) {
  HGLOBAL hglbCopy = NULL;
  TCHAR *lptstrFilePath = NULL;

  if(!::OpenClipboard(NULL)) {
    return;
  }

  ::EmptyClipboard();

  // TODO: remember dealloc memory pointed by hglbCopy
  hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE,
    (csFilePath.size() + 1) * sizeof(TCHAR)
  );
  if(hglbCopy == NULL) {
    ::CloseClipboard();
    return;
  }

  lptstrFilePath = (TCHAR *)::GlobalLock(hglbCopy);
  ::lstrcpyn(lptstrFilePath, csFilePath.c_str(), MAX_PATH);
  ::GlobalUnlock(hglbCopy);
  ::SetClipboardData(CF_UNICODETEXT, hglbCopy);
  ::CloseClipboard();
}

VOID CopyFilePath(LPCTSTR pFilePath) {
  std::wstring path = pFilePath;
  CopyFilePath(path);
}

BOOL CopyFile(const wchar_t* wchExistFile,
              const wchar_t* wchNewFile,
              BOOL bOverWirite
) {
  BOOL bResult = ::CopyFile(wchExistFile, wchNewFile, !bOverWirite);

  return bResult;
}

BOOL CreateDir(CONST TCHAR *tszDir) {
  int     nRes = 0;
  CONST TCHAR*  pszTemp = NULL;
  TCHAR   szRealDir[MAX_PATH + 1] = { 0 };

  pszTemp = ::_tcsstr(tszDir, _T(":\\"));
  if(!pszTemp) {
    nRes = ::GetCurrentDirectory(MAX_PATH, szRealDir);
    ::PathAddBackslash(szRealDir);
    ::_tcsncat_s(szRealDir, MAX_PATH + 1, tszDir, MAX_PATH + 1 - nRes - 1);
  } else {
    ::_tcsncpy_s(szRealDir, MAX_PATH + 1, tszDir, MAX_PATH + 1 - 1);
  }

  if(PathFileExists(szRealDir)) {
    nRes = ERROR_SUCCESS;
  } else {
#if 0
    nRes = ::CreateDirectoryEx(NULL, szRealDir, NULL);
#else
    nRes = CreateDirectory(szRealDir, NULL);
#endif
  }

  return (nRes == ERROR_SUCCESS);
}

#define FT_PUB  0x55505446  // "FTPU"
BOOL MoveDirectory(LPCTSTR pchFrom, LPCTSTR pchTo) {
  BOOL bRes = TRUE;
  DWORD dwPathLen = 0;
  HANDLE hFind = NULL;
  WIN32_FIND_DATA wFindData = { 0 };
  TCHAR chFrom[MAX_PATH + 1] = { 0 };

  if(NULL == pchFrom || NULL == pchTo) {
    return FALSE;
  }

  if('\0' == *pchFrom || '\0' == *pchTo) {
    return FALSE;
  }

  if(0 == ::_tcscmp(pchFrom, pchTo)) {
    return FALSE;
  }

  bRes = ::PathIsDirectory(pchFrom);
  if(!bRes) {
    return FALSE;
  }

  bRes = ::PathIsDirectory(pchTo);
  if(!bRes) {
    bRes = CreateDir(pchTo);
    if(!bRes) {
      return FALSE;
    }
  }

  //TCHAR_BUFFER_STRCPY(chFrom, pchFrom);
  ::_tcsncpy_s(chFrom, MAX_PATH, pchFrom, MAX_PATH - 1);
  ::PathAddBackslash(chFrom);
  dwPathLen = static_cast<DWORD>(::_tcslen(chFrom));
  ::_tcsncat_s(chFrom, MAX_PATH, _T("*.*"), 3);

  hFind = ::FindFirstFile(chFrom, &wFindData);
  if(INVALID_HANDLE_VALUE == hFind) {
    return FALSE;
  }

  do {
    if(0 != ::_tcscmp(wFindData.cFileName, _T(".")) &&
        0 != ::_tcscmp(wFindData.cFileName, _T(".."))) {
      DWORD dwDestPathLen = 0;
      TCHAR chCurTo[MAX_PATH + 1] = { 0 };

      chFrom[dwPathLen] = '\0';
      ::_tcsncpy_s(chCurTo, MAX_PATH + 1, pchTo, MAX_PATH);
      ::_tcsncat_s(chFrom, MAX_PATH, wFindData.cFileName, MAX_PATH - dwPathLen - 1);
      ::PathAddBackslash(chCurTo);

      dwDestPathLen = static_cast<DWORD>(::_tcslen(pchTo));
      ::_tcsncat_s(chCurTo, MAX_PATH + 1, wFindData.cFileName, MAX_PATH - dwDestPathLen);

      ::SetFileAttributes(chFrom,
                          wFindData.dwFileAttributes & ~FILE_ATTRIBUTE_HIDDEN &
                          ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_SYSTEM);

      if(wFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if(!MoveDirectory(chFrom, chCurTo)) {
          bRes = FALSE;
        }
      } else {
        if(!::MoveFileEx(chFrom, chCurTo, MOVEFILE_REPLACE_EXISTING)) {
          bRes = FALSE;
        }
      }
    }
  } while(::FindNextFile(hFind, &wFindData));

  ::FindClose(hFind);
  hFind = INVALID_HANDLE_VALUE;

  if(!::RemoveDirectory(pchFrom)) {
    ::MoveFileEx(pchFrom, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
    bRes = FALSE;
  }

  return bRes;
}

BOOL SetSpecailFolderPath(INT nCSIDL, LPCTSTR szPath) {
  BOOL bRet = FALSE;
  BOOL bRes = FALSE;
  SHSetFolderPathW_t SHSetFolderPathW = NULL;
  HMODULE hShell32Dll = NULL;
  HRESULT hrRes = S_OK;
  HANDLE hToken = NULL;

  hShell32Dll = ::GetModuleHandle(_T("shell32.dll"));
  if(!hShell32Dll) {
    goto Exit0;
  }

  SHSetFolderPathW = (SHSetFolderPathW_t)::GetProcAddress(hShell32Dll, (LPCSTR)232L);
  if(!SHSetFolderPathW) {
    goto Exit0;
  }

  bRes = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &hToken);
  if(!bRes) {
    hToken = NULL;
  }

#if (defined(UNICODE) || defined(_UNICODE))
  hrRes = SHSetFolderPathW(nCSIDL, hToken, 0, szPath);
#else
  {
    USES_CONVERSION;
    hrRes = SHSetFolderPathW(nCSIDL, hToken, 0, A2W(szPath));
  }
#endif
  BASEASSERT(SUCCEEDED(hrRes));
  if(FAILED(hrRes)) {
    goto Exit0;
  }

  bRet = TRUE;
Exit0:
  if(hToken) {
    ::CloseHandle(hToken);
    hToken = NULL;
  }

  return bRet;
}

BOOL ViewFileFolderProperties(LPCTSTR szPath) {
  BOOL bRet = FALSE;

  if(NULL == szPath || _T('\0') == szPath[0]) {
    goto exit0;
  }

  if(::PathFileExists(szPath)) {
    SHELLEXECUTEINFO seInfo = { 0 };

    seInfo.cbSize = sizeof(seInfo);
    seInfo.lpFile = szPath;
    seInfo.lpVerb = _T("properties");
    seInfo.fMask = SEE_MASK_INVOKEIDLIST;

    bRet = ::ShellExecuteEx(&seInfo);
  }

exit0:
  return bRet;
}

BOOL OpenRecycle() {
  BOOL  bRet = FALSE;
  DWORD dwRes = 0;
  TCHAR szDllPath[MAX_PATH] = { 0 };

  //todo: call common function instead
  ::GetWindowsDirectory(szDllPath, MAX_PATH);
  ::PathAddBackslash(szDllPath);
  ::_tcsncat_s(szDllPath, MAX_PATH, _T("explorer.exe"), _tcslen(_T("explorer.exe")));

  dwRes = static_cast<DWORD>((DWORD_PTR)::ShellExecute(
    NULL,
    _T("open"),
    szDllPath,
    _T("::{645FF040-5081-101B-9F08-00AA002F954E}"),
    NULL,
    SW_SHOWNORMAL
  ));

  bRet = dwRes > 32 ? TRUE : FALSE;

  return bRet;
}

BOOL FormatFileSize(DWORD dwFileSize, LPTSTR szFileSize, DWORD dwBufSize) {
  BOOL bRet = FALSE;

  T_PROCESS_ERROR(dwFileSize >= 0 && NULL != szFileSize && dwBufSize > 0);

  if(dwFileSize < 1024)
    ::_sntprintf_s(szFileSize, dwBufSize, _TRUNCATE, _T("%u Bytes"), dwFileSize);
  else if(dwFileSize < 1024 * 1024)
    ::_sntprintf_s(szFileSize, dwBufSize, _TRUNCATE, _T("%.1f KB"), (float)dwFileSize / 1024.0);
  else if(dwFileSize < (float)1024 * 1024 * 1024)
    ::_sntprintf_s(szFileSize, dwBufSize, _TRUNCATE, _T("%.1f MB"), (float)dwFileSize / 1024.0 / 1024.0);
  else
    ::_sntprintf_s(szFileSize, dwBufSize, _TRUNCATE, _T("%.1f GB"), (float)dwFileSize / 1024.0 / 1024.0 / 1024.0);
  bRet = TRUE;

Exit0:
  return bRet;
}

BOOL SaveIconToFile(HICON hIcon, LPCTSTR szFilePath) {
  CComPtr<IPicture> pPicture = NULL;
  CComPtr<IStream> pStream = NULL;
  char  szBuf[1024];
  HANDLE hFile = INVALID_HANDLE_VALUE;
  BOOL bRet = FALSE, bRes;
  HRESULT hrRes = E_FAIL;
  LONG   lSize;

  if(NULL == hIcon ||
      NULL == szFilePath ||
      NULL == szFilePath[0])
    goto Exit0;

  { // create picture interface
    PICTDESC picdesc;
    picdesc.cbSizeofstruct = sizeof(PICTDESC);
    picdesc.picType = PICTYPE_ICON;
    picdesc.icon.hicon = hIcon;
    OleCreatePictureIndirect(&picdesc, IID_IPicture, TRUE, (VOID**)&pPicture);
  }

  CreateStreamOnHGlobal(NULL, TRUE, &pStream);
  hrRes = pPicture->SaveAsFile(pStream, TRUE, &lSize);

  hFile = ::CreateFile(szFilePath,
                        GENERIC_WRITE,
                        0,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
  if(INVALID_HANDLE_VALUE == hFile)
    goto Exit0;

  {   // reset file position
    LARGE_INTEGER li;
    ULARGE_INTEGER ulnewpos;

    li.HighPart = 0;
    li.LowPart = 0;

    pStream->Seek(li, STREAM_SEEK_SET, &ulnewpos);
  }
  {   // write to file
    ULONG   uReadCount = 1;
    while(uReadCount > 0) {
      pStream->Read(szBuf, sizeof(szBuf), &uReadCount);
      if(uReadCount > 0) {
        bRes = ::WriteFile(hFile, szBuf, uReadCount, &uReadCount, NULL);
      }
    }
  }

  bRet = TRUE;

Exit0:
  if(INVALID_HANDLE_VALUE != hFile) {
    bRes = ::CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
  }
  return bRet;
}
}  // namespace filemisc
}  // namespace base
