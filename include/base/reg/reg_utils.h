// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_REG_REG_UTILS_H_
#define BASE_REG_REG_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace reg {
class RegUtils {
  public:
    static BOOL RegOpenKey(HKEY hKey, const string16& lpSubKey, DWORD ulOptions,
                           REGSAM samDesired, PHKEY phkResult) {
        LONG lRet = ::RegOpenKeyEx(hKey, lpSubKey.c_str(), ulOptions, samDesired,
                                   phkResult);

        if(ERROR_ACCESS_DENIED == lRet) {

            lRet = ::RegOpenKeyEx(hKey, lpSubKey.c_str(), ulOptions, samDesired, phkResult);
        }

        if(lRet == ERROR_SUCCESS) {
            return TRUE;
        }
        return FALSE;
    }

    static BOOL RegCloseKey(HKEY key) {
        ::RegCloseKey(key);
        return TRUE;
    }

    static BOOL IsKeyValueExist(HKEY hRoot, const string16& strSubKey,
                                const string16& strValueName, BOOL b64) {
        BOOL bRet = FALSE;
        HKEY hSubKey = NULL;
        REGSAM samDesired = KEY_READ;
        if(b64) {
            samDesired |= KEY_WOW64_64KEY;
        }

        if(ERROR_SUCCESS == RegOpenKeyEx(hRoot, strSubKey.c_str(), 0, samDesired,
                                         &hSubKey)) {
            if(ERROR_SUCCESS == RegQueryValueEx(hSubKey, strValueName.c_str(), 0, NULL,
                                                NULL, NULL)) {
                bRet = TRUE;
            }

            ::RegCloseKey(hSubKey);
            hSubKey = NULL;
        }

        return bRet;
    }

    static BOOL IsKeyExist(HKEY hRoot, const string16& strSubKey, BOOL b64) {
        BOOL bRet = FALSE;
        HKEY hSubKey = NULL;
        LONG lRet = 0;
        REGSAM samDesired = KEY_READ;
        if(b64) {
            samDesired |= KEY_WOW64_64KEY;
        }

        if(ERROR_SUCCESS == RegOpenKeyEx(hRoot, strSubKey.c_str(), 0, samDesired,
                                         &hSubKey)) {
            bRet = TRUE;
            ::RegCloseKey(hSubKey);
            hSubKey = NULL;
        }

        return bRet;
    }

    static BOOL CreateKey(HKEY parentKey, const string16& keyName, BOOL b64) {
        HKEY newKey;
        REGSAM samDesired = KEY_CREATE_SUB_KEY | KEY_WRITE;
        if(b64) {
            samDesired |= KEY_WOW64_64KEY;
        }
        LONG ret = RegCreateKeyEx(parentKey, keyName.c_str()
                                  , 0, NULL, REG_OPTION_NON_VOLATILE,
                                  samDesired, NULL,
                                  &newKey, NULL);
        if(ret != ERROR_SUCCESS) {
            return FALSE;
        }
        ::RegCloseKey(newKey);
        return TRUE;
    }


    static BOOL CreateValue(HKEY key, const string16& valName, DWORD type,
                            PBYTE val, DWORD len) {
        LONG ret = RegSetValueEx(key, valName.c_str(), 0, type, val, len);
        if(ERROR_SUCCESS != ret) {
            return FALSE;
        }
        return TRUE;
    }

    static BOOL QueryValue(HKEY hKey, const string16& strValueName, BYTE* value,
                           DWORD len) {
        BOOL ret = FALSE;
        DWORD dwType = 0;
        if(ERROR_SUCCESS == RegQueryValueEx(hKey, strValueName.c_str(), NULL, &dwType,
                                            value, &len)) {
            ret = TRUE;
        }

        return ret;
    }

    static BOOL ReadRegStrValue(const string16& strSubKey,
                                const string16& strKeyName,
                                string16& strValue,
                                HKEY hDefaultKey,
                                BOOL b64) {
        HKEY hkey;
        DWORD dwType = REG_SZ;
        DWORD ValueSize = MAX_PATH;
        //strValue.resize(MAX_PATH);
        REGSAM samDesired = KEY_READ;
        if(b64) {
            samDesired |= KEY_WOW64_64KEY;
        }
        if(RegOpenKeyEx(hDefaultKey, strSubKey.c_str(), 0, samDesired,
                        &hkey) == ERROR_SUCCESS) {
            TCHAR buf[MAX_PATH] = { 0 };
            if(RegQueryValueEx(hkey, strKeyName.c_str(), NULL, &dwType, (BYTE*)&buf,
                               &ValueSize) == ERROR_SUCCESS) {
                strValue = buf;
                RegCloseKey(hkey);
            } else {
                RegCloseKey(hkey);
                return FALSE;
            }
        } else {
            return FALSE;
        }
        return TRUE;
    }

    static BOOL WriteRegStrValue(const string16& strSubKey,
                                 const string16& strName,
                                 const string16& strValue,
                                 DWORD& dwFailReason,
                                 DWORD& dwLastError,
                                 HKEY hDefaultKey,
                                 BOOL b64) {
        HKEY hKey;
        DWORD dwType = REG_SZ;
        DWORD dwDisposition;

        WCHAR szValues[MAX_PATH] = { 0 };
        wcscpy_s(szValues, MAX_PATH, strValue.c_str());
        REGSAM samDesired = KEY_SET_VALUE;
        if(b64) {
            samDesired |= KEY_WOW64_64KEY;
        }
        if(RegCreateKeyEx(hDefaultKey, strSubKey.c_str(), 0, NULL,
                          REG_OPTION_NON_VOLATILE, samDesired, NULL, &hKey,
                          &dwDisposition) == ERROR_SUCCESS) {
            if(RegSetValueEx(hKey, strName.c_str(), NULL, dwType, (BYTE*)szValues,
                             (wcslen(szValues) + 1) * sizeof(WCHAR)) == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                return TRUE;
            } else {
                dwFailReason = 1;
                dwLastError = GetLastError();
                RegCloseKey(hKey);
                return FALSE;
            }

        } else {
            dwFailReason = 2;
            dwLastError = GetLastError();
            return FALSE;
        }
    }
};
}  // namespace reg
}  // namespace base
#endif  // BASE_REG_REG_UTILS_H_