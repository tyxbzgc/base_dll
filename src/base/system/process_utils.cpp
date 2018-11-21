// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/system/process_utils.h"
#include <tlhelp32.h>
#include <Psapi.h>
#include <tchar.h>

#include "base/undoc.h"
#include "base/files/file_attr_utils.h"
#include "base/public.h"
#include "base/safe_load_library/safe_load_library.h"
#include "base/public.h"

typedef LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);

typedef BOOLEAN (WINAPI *defWinStationGetProcessSid)(
    HANDLE hServer,
    DWORD ProcessId,
    FILETIME ProcessStartTime,
    PBYTE pProcessUserSid,
    PDWORD dwSidSize
);

namespace base {
namespace sysmisc {
int32 GetProcessCount(const TCHAR* szPro) {
    HANDLE hProcessSnap = NULL;
    PROCESSENTRY32 pe32 = { 0 };
    int32 process_count = 0;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(hProcessSnap == INVALID_HANDLE_VALUE)
        return FALSE;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hProcessSnap, &pe32)) {
        do {
            if(lstrcmpi(pe32.szExeFile, szPro) == 0) {
                process_count++;
                break;
            }
        } while(Process32Next(hProcessSnap, &pe32));
    }
    if(hProcessSnap) {
        CloseHandle(hProcessSnap);
    }
    return process_count;
}

BOOL CheckProcess(const TCHAR* szPro) {
    HANDLE hProcessSnap = NULL;
    PROCESSENTRY32 pe32 = { 0 };
    BOOL bRet = FALSE;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(hProcessSnap == INVALID_HANDLE_VALUE)
        return FALSE;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hProcessSnap, &pe32)) {
        do {
            if(lstrcmpi(pe32.szExeFile, szPro) == 0) {
                bRet = TRUE;
                break;
            }
        } while(Process32Next(hProcessSnap, &pe32));
    }
    if(hProcessSnap) {
        CloseHandle(hProcessSnap);
    }
    return bRet;
}

BOOL IsProcessExist(DWORD dwPid) {
    BOOL bRet = FALSE;
    BOOL bRes = FALSE;
    DWORD dwExitCode = 0;
    HANDLE hProcess = NULL;

    hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION,
                             FALSE, dwPid
                            );
    if(NULL == hProcess) {
        // pid not exist or access deny?
        goto lExit0; // assume the process does no exist
    } else {
        bRes = ::GetExitCodeProcess(hProcess, &dwExitCode);
        BASEASSERT(bRes);
        if(STILL_ACTIVE == dwExitCode) {
            bRet = TRUE;
        }
    }

lExit0:
    if(NULL != hProcess) {
        ::CloseHandle(hProcess);
        hProcess = NULL;
    }
    return bRet;
}


BOOL IsProcessExist(LPCTSTR lpszProcName) {
    DWORD dwArrId[1000] = { 0 };
    DWORD dwNeed = 0l;
    DWORD dwCount = 0;
    TCHAR tchName[MAX_PATH] = { 0 };
    HANDLE hProcess = NULL;
    HMODULE hModule = NULL;

    if(NULL == lpszProcName || '\0' == *lpszProcName) {
        return FALSE;
    }

    if(!::EnumProcesses((DWORD*)&dwArrId, sizeof(dwArrId), &dwNeed)
            || 0 == dwNeed) {
        return FALSE;
    }

    dwCount = dwNeed / sizeof(DWORD);
    for(DWORD i = 0; i < dwCount; i++) {
        hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
                                 dwArrId[i]);
        if(hProcess) {
            hModule = NULL;
            ::memset(tchName, 0, sizeof(tchName));

            if(::EnumProcessModules(hProcess, &hModule, sizeof(hModule), &dwNeed)
                    && ::GetModuleBaseName(hProcess, hModule, tchName, MAX_PATH)
              ) {
                if(0 == ::lstrcmpi(tchName, lpszProcName)) {
                    ::CloseHandle(hProcess);
                    hProcess = NULL;
                    return TRUE;
                }
            }

            ::CloseHandle(hProcess);
            hProcess = NULL;
        }
    }

    return FALSE;
}


BOOL EnableProcessPrivilege(HANDLE hProcess, LPTSTR szPri, BOOL bEnable) {
    BOOL bRet = FALSE, bRes;
    HANDLE hToken = NULL;
    TOKEN_PRIVILEGES tp = { 0 };

    if(0 == hProcess ||
            NULL == szPri ||
            NULL == szPri[0]) {
        goto Exit0;
    }

    bRes = ::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
    if(!bRes) {
        goto Exit0;
    }

    tp.PrivilegeCount = 1;
    bRes = ::LookupPrivilegeValue(NULL, szPri, &tp.Privileges[0].Luid);
    if(!bRes) {
        goto Exit0;
    }

    tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
    bRes = ::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
    if(!bRes) {
        goto Exit0;
    }

    bRet = TRUE;

Exit0:
    if(NULL != hToken) {
        ::CloseHandle(hToken);
    }
    return bRet;
}

BOOL GetParentProcessPath(DWORD dwPid, TCHAR *ptszProcessPath, DWORD dwPathLen,
                          DWORD *pdwParentID) {
    BOOL bRet = FALSE;
    LONG status = 0;
    DWORD dwParentPID = (DWORD)-1;
    HANDLE hProcess = NULL;
    PROCESS_BASIC_INFORMATION pbi = { 0 };
    PROCNTQSIP NtQueryInformationProcess = NULL;
    HANDLE hParentProcess = NULL;
    DWORD dwRet = 0;

    NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(
                                    GetModuleHandle(L"ntdll.dll"),
                                    "ZwQueryInformationProcess"
                                );
    TS_PROCESS_ERROR(NtQueryInformationProcess);

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
    TS_PROCESS_ERROR(hProcess);

    status = NtQueryInformationProcess(
                 hProcess,
                 ProcessBasicInformation,
                 (PVOID)&pbi,
                 sizeof(PROCESS_BASIC_INFORMATION),
                 NULL
             );
    TS_PROCESS_ERROR(status == 0);

    dwParentPID = pbi.InheritedFromUniqueProcessId;
    if(pdwParentID) {
        *pdwParentID = dwParentPID;
    }

    hParentProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
                                 dwParentPID);
    TS_PROCESS_ERROR(hParentProcess);

    dwRet = GetModuleFileNameEx(hParentProcess, NULL, ptszProcessPath, dwPathLen);
    TS_PROCESS_ERROR(dwRet);

    bRet = TRUE;
Exit0:
    if(hProcess) {
        CloseHandle(hProcess);
        hProcess = NULL;
    }

    if(hParentProcess) {
        CloseHandle(hParentProcess);
        hParentProcess = NULL;
    }
    return bRet;
}

BOOL GetProcessName(DWORD dwPid, LPTSTR szProcessName, DWORD dwSize) {
    BOOL bRet = FALSE;
    HANDLE hSnap = NULL;
    PROCESSENTRY32 procEntry;

    hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnap == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if(::Process32First(hSnap, &procEntry)) {
        do {
            if(procEntry.th32ProcessID == dwPid) {
                ::_tcsncpy_s(szProcessName, dwSize, procEntry.szExeFile,
                             _tcslen(procEntry.szExeFile));
                bRet = TRUE;
                break;
            }

        } while(::Process32Next(hSnap, &procEntry));
    }

    CloseHandle(hSnap);

    return bRet;
}

BOOL EnableDebugPrivilege(BOOL bEnable) {
    BOOL bRet = FALSE;
    HANDLE hToken = NULL;

    if(::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES,
                          &hToken)) {
        TOKEN_PRIVILEGES tp = { 0 };

        tp.PrivilegeCount = 1;
        ::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
        tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
        ::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
        bRet = (ERROR_SUCCESS == ::GetLastError());

        ::CloseHandle(hToken);
    }

    return bRet;
}

BOOL SetDEP() {
    typedef BOOL(WINAPI*
                 FPN_SetProcessDEPPolicy)(
                     __in DWORD dwFlags
                 );

    HMODULE hKernel32 = GetModuleHandle(L"Kernel32.dll");

    if(hKernel32 != NULL) {
        FPN_SetProcessDEPPolicy fp = (FPN_SetProcessDEPPolicy)GetProcAddress(hKernel32,
                                     "SetProcessDEPPolicy");
        if(fp != NULL) {
            return fp(PROCESS_DEP_ENABLE);
        }
    }

    return FALSE;
}


/*++
Routine Description: This routine returns TRUE if the caller's
process is a member of the Administrators local group. Caller is NOT
expected to be impersonating anyone and is expected to be able to
open its own process and process token.
Arguments: None.
Return Value:
TRUE - Caller has Administrators local group.
FALSE - Caller does not have Administrators local group.
*/
BOOL IsUserAdmin(VOID) {
    BOOL b;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    b = AllocateAndInitializeSid(
            &NtAuthority,
            2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &AdministratorsGroup);
    if(b) {
        if(!CheckTokenMembership(NULL, AdministratorsGroup, &b)) {
            b = FALSE;
        }
        FreeSid(AdministratorsGroup);
    }

    return(b);
}

BOOL ExecuteExeAsAdmin(LPCWSTR exe, LPCWSTR params,
                       DWORD* exit_code) {
    SHELLEXECUTEINFO info = { 0 };
    info.cbSize = sizeof(SHELLEXECUTEINFO);
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.lpVerb = L"runas";
    //安全调用
    std::wstring strExe(_T("\""));
    strExe.append(exe);
    strExe.append(_T("\""));
    info.lpFile = strExe.c_str();
    std::wstring strParam = params;
    info.lpParameters = strParam.c_str();
    info.nShow = SW_HIDE;
    if(::ShellExecuteEx(&info) == FALSE) {
        return FALSE;
    }

    if(exit_code != NULL) {
        ::WaitForSingleObject(info.hProcess, INFINITE);
        if(!::GetExitCodeProcess(info.hProcess, exit_code))
            return FALSE;
    }

    CloseHandle(info.hProcess);

    return TRUE;
}
}  // namespace sysmisc
}  // namespace base
