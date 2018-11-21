// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/system/sys_attr_utils.h"
#include "Lmcons.h"     // UNLEN
#include "base/safe_str_func/safe_str_funs.h"
#include "base/safe_load_library/safe_load_library.h"
#include "base/strings/string_utils.h"
/**
* @struct OSVERSIONINFOEX1
* @brief 操作系统版本信息
*/
typedef struct _OSVERSIONINFOEX1 {
DWORD dwOSVersionInfoSize;
DWORD dwMajorVersion;
DWORD dwMinorVersion;
DWORD dwBuildNumber;
DWORD dwPlatformId;
TCHAR szCSDVersion[128];
WORD wServicePackMajor;
WORD wServicePackMinor;
WORD wSuiteMask;
BYTE wProductType;
BYTE wReserved;
} OSVERSIONINFOEX1, *POSVERSIONINFOEX1, *LPOSVERSIONINFOEX1;


namespace base {
namespace sysmisc {
std::wstring GetComputerName() {
  std::wstring s_strName = _T("UNAVAILABLE");

  TCHAR computerName[/*MAX_COMPUTERNAME_LENGTH*/256 + 1];
  DWORD computerNameLen = /*MAX_COMPUTERNAME_LENGTH*/256;
  if(::GetComputerName(computerName, &computerNameLen)) {
    s_strName = computerName;
  }
#ifdef _DEBUG
  else {
    DWORD err = GetLastError();
    printf("GetComputerName failed %d\n", err);
  }
#endif

  return s_strName;
}

BOOL GetComputerName(LPWSTR pName, DWORD dwLen) {
  BOOL bRet = FALSE;

  WCHAR computerName[/*MAX_COMPUTERNAME_LENGTH*/256 + 1];
  DWORD computerNameLen = /*MAX_COMPUTERNAME_LENGTH*/256;
  if(::GetComputerName(computerName, &computerNameLen)) {
    wcscpy_s(pName, dwLen, computerName);
    bRet = TRUE;
  }
#ifdef _DEBUG
  else {
    DWORD err = GetLastError();
    printf("GetComputerName failed %d\n", err);
  }
#endif
  if(!bRet) {
    wcscpy_s(pName, dwLen, _T("UNAVAILABLE"));
  }

  return bRet;
}


BOOL IsVistaSystem() {
  static BOOL s_bVista = FALSE;

  if(!s_bVista) {
    HINSTANCE hInst = NULL;
    TCHAR szSysDir[MAX_PATH + 1] = { 0 };
    typedef int (WINAPI *PGetLocaleInfoEx)(LCID, LCTYPE, LPTSTR, int);

    ::GetSystemDirectory(szSysDir, MAX_PATH);

    TCHAR_BUFFER_STRCAT(szSysDir, TEXT("\\kernel32.dll"));
    //::_tcsncat_s(tchSysDir, lstrlen(tchSysDir), TEXT("\\kccernel32.dll"), MAX_PATH - lstrlen(tchSysDir));
    hInst = base::library::SafeLoadLibrary(szSysDir, TRUE);
    if(NULL != hInst) {
      PGetLocaleInfoEx pGetLocaleInfoEx = (PGetLocaleInfoEx)GetProcAddress(hInst, "GetLocaleInfoEx");
      if(pGetLocaleInfoEx) {
        // 1表示是vista以上系統
        s_bVista = 1;
      } else {
        // 2表示是該函數運行過一次，下次不需要運行
        s_bVista = 2;
      }

      pGetLocaleInfoEx = NULL;
      FreeLibrary(hInst);
      hInst = NULL;
    }
  }

  return (1 == s_bVista) ? TRUE : FALSE;
}

BOOL GetWindowsVersion(DWORD& dwVersion, TCHAR* chWinVer, int nBuffLen) {
  BOOL bOsVerInfoEx = TRUE;
  OSVERSIONINFOEX1 osVer = { 0 };

  dwVersion = 0l;
  // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
  // If that fails, try using the OSVERSIONINFO structure.
  osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX1);
  bOsVerInfoEx = ::GetVersionEx((OSVERSIONINFO*)&osVer);
  if(!bOsVerInfoEx) {
    osVer.dwOSVersionInfoSize = sizeof OSVERSIONINFO;
    if(!::GetVersionEx((OSVERSIONINFO*)&osVer)) {
      return FALSE;
    }
  }

  if(0 == ::_tcsncmp(osVer.szCSDVersion, TEXT("Service Pack 1"), sizeof(osVer.szCSDVersion) / sizeof(TCHAR)))
    dwVersion |= Ver_Sp1;
  else if(0 == ::_tcsncmp(osVer.szCSDVersion, TEXT("Service Pack 2"), sizeof(osVer.szCSDVersion) / sizeof(TCHAR)))
    dwVersion |= Ver_Sp2;
  else if(0 == ::_tcsncmp(osVer.szCSDVersion, TEXT("Service Pack 3"), sizeof(osVer.szCSDVersion) / sizeof(TCHAR)))
    dwVersion |= Ver_Sp3;
  else if(0 == ::_tcsncmp(osVer.szCSDVersion, TEXT("Service Pack 4"), sizeof(osVer.szCSDVersion) / sizeof(TCHAR)))
    dwVersion |= Ver_Sp4;
  else {}
  dwVersion <<= sizeof(WORD) * 8;

  if(IsVistaSystem()) // Vista
  {
    if(VER_PLATFORM_WIN32_NT == osVer.dwPlatformId) {//不考虑server系统
      if(6l == osVer.dwMajorVersion && 2l == osVer.dwMinorVersion) {
        TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows 8"));
        //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows 8 "),  _TRUNCATE);
        dwVersion |= Ver_Win8;
        return TRUE;
      } else if(6l == osVer.dwMajorVersion && 1l == osVer.dwMinorVersion) {
        TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows 7"));
        //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows 7 "),  _TRUNCATE);
        dwVersion |= Ver_Win7;
        return TRUE;
      } else if(6l == osVer.dwMajorVersion && 0l == osVer.dwMinorVersion) {
        TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows Vista"));
        //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows Vista "),  _TRUNCATE);
        dwVersion |= Ver_WinVista;
        return TRUE;
      }
    }

    return FALSE;
  }

  switch(osVer.dwPlatformId) {
  case VER_PLATFORM_WIN32_NT: // Test for the Windows NT product family
  {
    if(6l == osVer.dwMajorVersion && 2l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows 8"));
      //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows 8 "),  _TRUNCATE);
      dwVersion |= Ver_Win8;
    } else if(6l == osVer.dwMajorVersion && 1l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows 7 "));
      //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows 7 "),  _TRUNCATE);
      dwVersion |= Ver_Win7;
    } else if(6l == osVer.dwMajorVersion && 0l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows Vista "));
      //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows Vista "),  _TRUNCATE);
      dwVersion |= Ver_WinVista;
    } else if(5l == osVer.dwMajorVersion && 2l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows Server 2003, "));
      //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows Server 2003, "),  _TRUNCATE);
      dwVersion |= Ver_Win2003;
    } else if(5l == osVer.dwMajorVersion && 1l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows XP "));
      //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows XP "),  _TRUNCATE);
      dwVersion |= Ver_WinXp;
    } else if(5l == osVer.dwMajorVersion && 0l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows 2000 "));
      //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows 2000 "),  _TRUNCATE);
      dwVersion |= Ver_Win2k;
    } else if(osVer.dwMajorVersion <= 4) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows NT "));
      //::_tcsncat_s(chWinVer, nBuffLen, _T("Windows NT "),  _TRUNCATE);
      dwVersion |= Ver_WinNt;
    } else {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T(""));
      //::_tcsncat_s(chWinVer, nBuffLen, _T(""),  _TRUNCATE);
    }

    // Display service pack (if any) and build number

    if(4l == osVer.dwMajorVersion
        && 0 == ::_tcsicmp(osVer.szCSDVersion, _T("Service Pack 6"))) // Test for SP6 versus SP6a
    {
      HKEY hKey = NULL;
      long lRet = 0l;

      // Test for SP6 versus SP6a.
      lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                            _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
                            0l, KEY_QUERY_VALUE, &hKey);
      if(ERROR_SUCCESS == lRet) {
        TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("SP6a"));
        //TCHAR* pWinVer = chWinVer;
        //::_tcsncat_s(pWinVer + ::lstrlen(chWinVer), 0, _T("SP6a"), ::lstrlen(_T("SP6a")));
      } else // Windows NT 4.0 prior to SP6a
      {
        TCHAR_POINT_STRCAT(chWinVer, nBuffLen, osVer.szCSDVersion);
        //TCHAR* pWinVer = chWinVer;
        //::_sntprintf_s(pWinVer + ::lstrlen(chWinVer), ::lstrlen(osVer.szCSDVersion), _T("%s"), osVer.szCSDVersion);
      }

      ::RegCloseKey(hKey);
      hKey = NULL;
    } else // not Windows NT 4.0
    {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, osVer.szCSDVersion);
      //TCHAR* pWinVer = chWinVer;
      //::_sntprintf(pWinVer + ::lstrlen(chWinVer), ::lstrlen(osVer.szCSDVersion), _T("%s"), osVer.szCSDVersion);
    }

    break;
  }

  case VER_PLATFORM_WIN32_WINDOWS: // Test for the Windows Me/98/95
  {
    if(4l == osVer.dwMajorVersion && 0l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows 95 "));
      //::_tcsncat_s(chWinVer, 0, _T("Windows 95 "), ::lstrlen(_T("Windows 95 ")));
      if(_T('C') == osVer.szCSDVersion[1] || _T('B') == osVer.szCSDVersion[1]) {
        TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("OSR2 "));
        //::_tcsncat_s(chWinVer, sizeof(chWinVer)/sizeof(TCHAR), _T("OSR2 "), _TRUNCATE);
      }
      dwVersion |= Ver_Win95;
    } else if(4l == osVer.dwMajorVersion && 10l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows 98 "));
      //::_tcsncat_s(chWinVer, 0, _T("Windows 98 "), ::lstrlen(_T("Windows 98 ")));
      if(_T('A') == osVer.szCSDVersion[1]) {
        TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("SE"));
        //::_tcsncat(chWinVer, sizeof(chWinVer)/sizeof(TCHAR), _T("SE"), _TRUNCATE);
      }
      dwVersion |= Ver_Win98;
    } else if(4l == osVer.dwMajorVersion && 90l == osVer.dwMinorVersion) {
      TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Windows ME"));
      //::_tcsncat_s(chWinVer, 0, _T("Windows ME"), ::lstrlen(_T("Windows ME")));
      dwVersion |= Ver_WinMe;
    } else {}

    break;
  }

  case VER_PLATFORM_WIN32s:
  {
    TCHAR_POINT_STRCAT(chWinVer, nBuffLen, _T("Win32s"));
    //::_tcsncat_s(chWinVer, 0, _T("Win32s"), ::lstrlen(_T("Win32s")));
    dwVersion |= Ver_Win32;

    break;
  }

  default:
  break;
  }

  return TRUE;
}

std::wstring GetUserName() {
  bool gotName = false;
  std::wstring name = _T("UNAVAILABLE");

  if(!gotName) {
    TCHAR userName[UNLEN + 1];
    DWORD userNameLen = UNLEN;

    if(::GetUserName(userName, &userNameLen)) {
      name = userName;
    }

    gotName = true;
  }

  return name;
}
BOOL GetUserName(LPWSTR pUserName, DWORD dwLen) {
  assert(pUserName && dwLen > 0);

  return ::GetUserName(pUserName, &dwLen);
}

BOOL IsOSNeedElevate() {
  OSVERSIONINFO osVerInfo;
  ::memset(&osVerInfo, 0, sizeof(OSVERSIONINFO));
  osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx(&osVerInfo);

  if(osVerInfo.dwMajorVersion >= 6) {
    return TRUE;
  }

  return FALSE;
}

const SECURITY_ATTRIBUTES* GetAllAccessSecurityAttributes() {
  static BOOL bIsInitialize = FALSE;
  static SECURITY_ATTRIBUTES SecurityAttributes;
  static SECURITY_DESCRIPTOR SecurityDescriptor;

  OSVERSIONINFO VerInfo;
  VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&VerInfo);
  if(VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    return NULL;

  if(bIsInitialize) return &SecurityAttributes;

  if(!InitializeSecurityDescriptor(&SecurityDescriptor, SECURITY_DESCRIPTOR_REVISION)
      || !SetSecurityDescriptorDacl(&SecurityDescriptor, TRUE, NULL, FALSE))
    return NULL;

  SecurityAttributes.nLength = sizeof(SecurityAttributes);
  SecurityAttributes.lpSecurityDescriptor = &SecurityDescriptor;
  SecurityAttributes.bInheritHandle = FALSE;

  bIsInitialize = TRUE;

  return &SecurityAttributes;
}

BOOL GetAllAccessSecurityAttributesEx(SECURITY_ATTRIBUTES& SecurityAttributes, SECURITY_DESCRIPTOR& SecurityDescriptor) {

  BOOL bIsInitialize = FALSE;
  //SECURITY_ATTRIBUTES SecurityAttributes;
  //SECURITY_DESCRIPTOR SecurityDescriptor;
  OSVERSIONINFO VerInfo;
  VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&VerInfo);
  if(VerInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    return FALSE;


  if(!InitializeSecurityDescriptor(&SecurityDescriptor, SECURITY_DESCRIPTOR_REVISION)
      || !SetSecurityDescriptorDacl(&SecurityDescriptor, TRUE, NULL, FALSE))
    return FALSE;

  SecurityAttributes.nLength = sizeof(SecurityAttributes);
  SecurityAttributes.lpSecurityDescriptor = &SecurityDescriptor;
  SecurityAttributes.bInheritHandle = FALSE;

  return TRUE;
}
DWORD GetProcessorsCount() {
  //static DWORD dwNum = 0;
  DWORD dwNum = 0;
  // 根據每臺計算機的處理器不可能為零的原理設置來判斷條件
  if(0 == dwNum) {
    SYSTEM_INFO info = { 0 };

    // 獲取系統信息
    ::GetSystemInfo(&info);
    dwNum = info.dwNumberOfProcessors;
  }

  return dwNum;
}

BOOL GetOSVersion(unsigned int& uiMajorVersion, unsigned int& uiMinorVersion) {
  OSVERSIONINFOEX osvi;
  BOOL bOsVersionInfoEx;
  TCHAR OsName[256] = { 0 };
  // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
  // If that fails, try using the OSVERSIONINFO structure.
  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi))) {
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if(!GetVersionEx((OSVERSIONINFO *)&osvi)) {
      return FALSE;
    }
  }
  uiMajorVersion = osvi.dwMajorVersion;
  uiMinorVersion = osvi.dwMinorVersion;
  return TRUE;
}

//hyc: move from "Commonfunc.h"
long GetOSVersionInternal(DWORD &dwOSMajorVersion, DWORD &dwOSMinorVersion) {
  long lCurrentVersion = WINDOWS_OLDEST;
  OSVERSIONINFO os = { sizeof(OSVERSIONINFO) };
  if(::GetVersionEx(&os) && os.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    dwOSMajorVersion = os.dwMajorVersion;
    dwOSMinorVersion = os.dwMinorVersion;
    if(os.dwMajorVersion == 5) {
      if(os.dwMinorVersion == 0)
        lCurrentVersion = WINDOWS_2000;
      else if(os.dwMinorVersion == 1) {
        if(_wcsnicmp(L"Service Pack 1", os.szCSDVersion, 127) == 0)
          lCurrentVersion = WINDOWS_XP_SP_1;
        else if(_wcsnicmp(L"Service Pack 2", os.szCSDVersion, 127) == 0)
          lCurrentVersion = WINDOWS_XP_SP_2;
        else if(_wcsnicmp(L"Service Pack 3", os.szCSDVersion, 127) == 0)
          lCurrentVersion = WINDOWS_XP_SP_3;
        else
          lCurrentVersion = WINDOWS_XP_NO_SP;
      } else if(os.dwMinorVersion == 2)
        lCurrentVersion = WINDOWS_2003;
    } else if(os.dwMajorVersion == 6) {
      if(os.dwMinorVersion == 0) {
        if(_wcsnicmp(L"Service Pack 1", os.szCSDVersion, 127) == 0)
          lCurrentVersion = WINDOWS_VISTA_SP_1;
        else if(_wcsnicmp(L"Service Pack 2", os.szCSDVersion, 127) == 0)
          lCurrentVersion = WINDOWS_VISTA_SP_2;
        else
          lCurrentVersion = WINDOWS_VISTA_NO_SP;
      } else if(os.dwMinorVersion == 1)
        lCurrentVersion = WINDOWS_7;
      else if(os.dwMinorVersion == 2)
        lCurrentVersion = WINDOWS_8;
    } else if(os.dwMajorVersion > 6) {
      lCurrentVersion = WINDOWS_LATEST;
    }
  }

  return lCurrentVersion;
}

long BASE_EXPORT GetOSVersion() {
  DWORD dwOSMajorVersion;
  DWORD dwOSMinorVersion;
  return GetOSVersionInternal(dwOSMajorVersion, dwOSMinorVersion);
}

long BASE_EXPORT GetOSVersionEx(DWORD &dwOSMajorVersion, DWORD &dwOSMinorVersion) {
  return GetOSVersionInternal(dwOSMajorVersion, dwOSMinorVersion);
}


BOOL GetDiskSize(LPCTSTR lpDirectoryName, uint64* pFreeBytesAvailable, uint64* pTotalNumberOfBytes, uint64* pTotalNumberOfFreeBytes) {
  ULARGE_INTEGER nFreeBytes, nTotalBytes, nTotalFreeBytes;
  if(GetDiskFreeSpaceEx(lpDirectoryName, &nFreeBytes, &nTotalBytes, &nTotalFreeBytes)) {
    pFreeBytesAvailable ? *pFreeBytesAvailable = nFreeBytes.QuadPart : ((void)0);
    pTotalNumberOfBytes ? *pTotalNumberOfBytes = nTotalBytes.QuadPart : ((void)0);
    pTotalNumberOfFreeBytes ? *pTotalNumberOfFreeBytes = nTotalFreeBytes.QuadPart : ((void)0);
    return TRUE;
  } else
    return FALSE;
}
}
}
