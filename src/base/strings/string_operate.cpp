// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base\strings\string_utils.h"
#include <time.h>
#include <assert.h>

namespace base {
namespace string_utils {
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c) {
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2) {
    v.push_back(s.substr(pos1, pos2 - pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

#ifdef STR_USE_WIN32_DBCS
  static int g_MaxCharSize = 0;
#endif

  // Returns 1 if the ANSI code page is single-byte, or 2 if it is double-byte (DBCS)
  int GetMaxCharSize(void) {
#ifdef STR_USE_WIN32_DBCS
    if(!g_MaxCharSize) {
      CPINFO info;
      if(GetCPInfo(CP_ACP, &info))
        g_MaxCharSize = info.MaxCharSize;
    }
    return g_MaxCharSize;
#else
    return MB_CUR_MAX;
#endif
  }

  // Copies src to dst. size is the size of dst in bytes, including the terminating 0.
  // Returns the number of characters copied, excluding the terminating 0.
  // The return value is <=size-1. If size is 0, returns 0 and does nothing.
  // The result in dst is always 0 terminated.
  int Strcpy(char *dst, int size, const char *src) {
    assert(dst);
    assert(src);
    assert(size > 0);
    if(size <= 0) return 0;
    char *dst0 = dst;
    if(GetMaxCharSize() == 1) {
      // SBCS version
      while(size > 1) {
        if(*src == 0) break;
        *dst++ = *src++;
        size--;
      }
    } else {
      // DBCS version
      while(size > 1) {
        if(*src == 0) break;
#ifdef STR_USE_WIN32_DBCS
        if(IsDBCSLeadByte((BYTE)*src)) {
#else
        if(isleadbyte((unsigned char)*src)) {
#endif
          if(size == 2) break;
          if(src[1] == 0) break;
          *dst++ = *src++;
          *dst++ = *src++;
          size -= 2;
        } else {
          *dst++ = *src++;
          size--;
        }
        }
      }
    *dst = 0;
    return (int)(dst - dst0);
    }

  int Strcpy(wchar_t *dst, int size, const wchar_t *src) {
    assert(dst);
    assert(src);
    assert(size > 0);
    if(size <= 0) return 0;
    int len = Strlen(src);
    if(len > size - 1)
      len = size - 1;
    if(len > 0 && src[len] >= 0xDC00 && src[len] <= 0xDFFF) // check for trailing surrogate
      len--;
    memcpy(dst, src, len * sizeof(wchar_t));
    dst[len] = 0;
    return len;
  }

  // Copies src to dst. size is the size of dst in characters, including the terminating 0.
  // Copies up to len characters and always appends terminating 0.
  int Strncpy(char *dst, int size, const char *src, int len) {
    assert(dst);
    assert(src);
    assert(size > 0);
    if(size <= 0) return 0;
    char *dst0 = dst;
    const char *end = src + len;
    if(GetMaxCharSize() == 1) {
      // SBCS version
      while(size > 1) {
        if(src == end) break;
        *dst++ = *src++;
        size--;
      }
    } else {
      // DBCS version
      while(size > 1) {
        if(src == end) break;
#ifdef STR_USE_WIN32_DBCS
        if(IsDBCSLeadByte((BYTE)*src)) {
#else
        if(isleadbyte((unsigned char)*src)) {
#endif
          if(size == 2) break;
          if(src + 1 == end) break;
          *dst++ = *src++;
          *dst++ = *src++;
          size -= 2;
        } else {
          *dst++ = *src++;
          size--;
        }
        }
      }
    *dst = 0;
    return (int)(dst - dst0);
    }

  int Strncpy(wchar_t *dst, int size, const wchar_t *src, int len) {
    assert(dst);
    assert(src);
    assert(size > 0);
    if(size <= 0) return 0;
    if(len > size - 1)
      len = size - 1;
    if(len > 0 && src[len] >= 0xDC00 && src[len] <= 0xDFFF) // check for trailing surrogate
      len--;
    memcpy(dst, src, len * sizeof(wchar_t));
    dst[len] = 0;
    return len;
  }

  // Appends src to dst. size is the size of dst in bytes, including the terminating 0.
  // Returns the number of characters copied, excluding the terminating 0.
  // The return value is <=size-1-strlen(dst). If size>=strlen(dst), returns 0 and does nothing.
  // The result in dst is always 0 terminated.
  int Strcat(char *dst, int size, const char *src) {
    assert(dst);
    int len = Strlen(dst);
    assert(len < size);
    return Strcpy(dst + len, size - len, src);
  }

  int Strcat(wchar_t *dst, int size, const wchar_t *src) {
    assert(dst);
    int len = Strlen(dst);
    assert(len < size);
    return Strcpy(dst + len, size - len, src);
  }

  // Writes formatted string to dst. size is the size of dst in characters, including the terminating 0.
  // Returns the number of characters written, excluding the terminating 0.
  // The return value is <=size-1. If size is 0, returns 0 and does nothing.
  // The result in dst is always 0 terminated.
  int Sprintf(char *dst, int size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int len = Vsprintf(dst, size, format, args);
    va_end(args);
    return len;
  }

  int Sprintf(wchar_t *dst, int size, const wchar_t *format, ...) {
    va_list args;
    va_start(args, format);
    int len = Vsprintf(dst, size, format, args);
    va_end(args);
    return len;
  }

  int Vsprintf(char *dst, int size, const char *format, va_list args) {
    assert(dst);
    assert(format);
    assert(size > 0);
    if(size <= 0) return 0;
#if _MSC_VER>=1400 // VC8.0
    int len = _vsnprintf_s(dst, size, size - 1, format, args);
#else
    int len = _vsnprintf(dst, size - 1, format, args);
#endif
    if(len < 0)
      len = size - 1;
    dst[len] = 0;
    return len;
  }

  int Vsprintf(wchar_t *dst, int size, const wchar_t *format, va_list args) {
    assert(dst);
    assert(format);
    assert(size > 0);
    if(size <= 0) return 0;
#if _MSC_VER>=1400 // VC8.0
    int len = _vsnwprintf_s(dst, size, size - 1, format, args);
#else
    int len = _vsnwprintf(dst, size - 1, format, args);
#endif
    if(len < 0)
      len = size - 1;
    dst[len] = 0;
    return len;
  }



  //下面代码从StringHelper.h移植过来
  BOOL SplitStringA(LPCSTR lpszStr, char chSeparator, CHAR** pStrList, DWORD dwMaxLength, DWORD* dwStrListCnt, size_t iMaxCount) {
    if(!lpszStr || !pStrList || chSeparator == '\0') {
      return FALSE;
    }
    DWORD nIndex = 0;
    int iLast = 0;
    for(int i = 0; lpszStr[i]; i++) {
      if(lpszStr[i] == chSeparator) {
        if(iMaxCount == nIndex + 1)
          break;

        std::string str = std::string(lpszStr + iLast, i - iLast);
        CHAR*p = (CHAR*)(pStrList + (nIndex++)*(dwMaxLength));
        strncpy_s(p, dwMaxLength, str.c_str(), _TRUNCATE);

        iLast = i + 1;
      }
    }
    if(iMaxCount > nIndex) {
      CHAR*p = (CHAR*)(pStrList + (nIndex++)*(dwMaxLength));
      strncpy_s(p, dwMaxLength, lpszStr + iLast, _TRUNCATE);
    }

    if(dwStrListCnt)
      *dwStrListCnt = nIndex;
    return TRUE;
  }

  BOOL SplitStringW(LPCWSTR lpszStr, WCHAR chSeparator, WCHAR* pStrList, DWORD dwMaxLength, DWORD* dwStrListCnt, size_t iMaxCount) {
    if(!lpszStr || !pStrList || chSeparator == L'\0') {
      return FALSE;
    }
    DWORD nIndex = 0;
    int iLast = 0;
    for(int i = 0; lpszStr[i]; i++) {
      if(lpszStr[i] == chSeparator) {
        if(iMaxCount == nIndex + 1)
          break;

        std::wstring str = std::wstring(lpszStr + iLast, i - iLast);
        WCHAR*p = (WCHAR*)(pStrList + (nIndex++)*(dwMaxLength));
        wcsncpy_s(p, dwMaxLength, str.c_str(), _TRUNCATE);

        iLast = i + 1;
      }
    }
    if(iMaxCount > nIndex) {
      WCHAR*p = (WCHAR*)(pStrList + (nIndex++)*(dwMaxLength));
      wcsncpy_s(p, dwMaxLength, lpszStr + iLast, _TRUNCATE);
    }

    if(dwStrListCnt)
      *dwStrListCnt = nIndex;
    return TRUE;
  }




  size_t ReplaceString(std::wstring &strText, const std::wstring& strSource, const std::wstring& strTarget) {
    size_t uCount = 0;
    std::basic_string<TCHAR>::size_type uFindPos = strText.find(strSource);
    while(uFindPos != std::wstring::npos) {
      strText.replace(uFindPos, strSource.size(), strTarget);
      uFindPos = strText.find(strSource, uFindPos);
      uCount++;
    }

    return uCount;
  }

  size_t BASE_EXPORT TrimString(const std::wstring& strText, WCHAR* wchOutput, int nOutputSize, WCHAR chSpecial /*= L' '*/) {
    if(!wchOutput) {
      return -1;
    }
    wstring wstr = strText;
    std::wstring::size_type pos = wstr.find_last_not_of(chSpecial);
    if(pos != std::wstring::npos) {
      wstr.erase(pos + 1);
      pos = wstr.find_first_not_of(chSpecial);
      if(pos != std::wstring::npos) {
        wstr.erase(0, pos);
      }
    } else {
      wstr.erase(wstr.begin(), wstr.end());
    }
    if(nOutputSize < (int)wstr.length() + 1) {
      return -1;
    }
    wcsncpy_s(wchOutput, nOutputSize, wstr.c_str(), wstr.length());

    return wstr.length();
  }


  WCHAR* StripLeading(const std::wstring& source, const WCHAR toStrip, WCHAR*wchBuffer, int nBufferSize) {
    const WCHAR *pSrc = source.c_str();

    while(pSrc && *pSrc == toStrip) {
      ++pSrc;
    }

    if(!wchBuffer || nBufferSize < (int)wcslen(pSrc) + 1) {
      return L"";
    }

    wcsncpy_s(wchBuffer, nBufferSize, pSrc, _TRUNCATE);

    return wchBuffer;
  }

  WCHAR* StripTrailing(const std::wstring& source, const WCHAR toStrip, WCHAR*wchBuffer, int nBufferSize) {
    if(!wchBuffer) {
      return L"";
    }
    size_t i = source.length();
    const WCHAR *pSrc = source.c_str() + i;

    --pSrc;

    while(i && *pSrc == toStrip) {
      --pSrc;
      --i;
    }

    std::wstring ret = source.substr(0, i);

    if(nBufferSize < (int)ret.length() + 1) {
      return L"";
    }

    wcsncpy_s(wchBuffer, nBufferSize, ret.c_str(), ret.length());

    return wchBuffer;
  }

  WCHAR* ToUpperW(const std::wstring& str, WCHAR* wchOutput, int nOutputSize) {
    if(!wchOutput || nOutputSize < (int)str.length() + 1) {
      return L"";
    }
    wstring strTemp = str;
    ::_wcsupr_s((WCHAR *)strTemp.c_str(), strTemp.length() + 1);
    wcsncpy_s(wchOutput, nOutputSize, strTemp.c_str(), _TRUNCATE);

    return wchOutput;
  }

  CHAR* ToUpperA(const std::string& str, CHAR* chOutput, int nOutputSize) {
    if(!chOutput || nOutputSize < (int)str.length() + 1) {
      return "";
    }
    string strTemp = str;
    ::_strupr_s((CHAR *)strTemp.c_str(), strTemp.length() + 1);
    strncpy_s(chOutput, nOutputSize, strTemp.c_str(), _TRUNCATE);

    return chOutput;
  }

  WCHAR* ToLowerW(const std::wstring& str, WCHAR* wchOutput, int nOutputSize) {
    if(!wchOutput || nOutputSize < (int)str.length() + 1) {
      return L"";
    }

    wstring strTemp = str;
    ::_wcslwr_s((WCHAR *)strTemp.c_str(), strTemp.length() + 1);
    wcsncpy_s(wchOutput, nOutputSize, strTemp.c_str(), _TRUNCATE);

    return wchOutput;
  }

  CHAR* ToLowerA(const std::string& str, CHAR* chOutput, int nOutputSize) {
    if(!chOutput || nOutputSize < (int)str.length() + 1) {
      return "";
    }
    string strTemp = str;
    ::_strlwr_s((CHAR *)strTemp.c_str(), strTemp.length() + 1);
    strncpy_s(chOutput, nOutputSize, strTemp.c_str(), _TRUNCATE);

    return chOutput;
  }



  BOOL CmpEq(TCHAR a, TCHAR b, BOOL bCaseSensitive) {
    return bCaseSensitive ? a == b : toupper(a) == toupper(b);
  }
  BOOL Wildcmp(LPCTSTR wild, LPCTSTR string, BOOL bCaseSensitive) {
    TCHAR *cp = NULL;
    TCHAR *mp = NULL;

    if(!_tcscmp(wild, _T("*"))) {
      return TRUE;
    }

    TCHAR *pW = const_cast<TCHAR*>(wild);
    TCHAR *pS = const_cast<TCHAR*>(string);

    while((*pS) && (*pW != '*')) {
      if(!CmpEq(*pW, *pS, bCaseSensitive) && (*pW != '?') && (*pW != '|')) {
        return 0;
      }

      else if(*pW == '|' && !CmpEq(*++pW, *pS, bCaseSensitive)) {
        return 0;
      }

      pW++;
      pS++;
    }

    while(*pS) {
      if(*pW == '*') {
        if(!*++pW) {
          return 1;
        }

        mp = pW;
        cp = pS + 1;
      } else if(CmpEq(*pW, *pS, bCaseSensitive) || (*pW == '?')) {
        pW++;
        pS++;
      } else if(*pW == '|' && CmpEq(*++pW, *pS, bCaseSensitive)) {
        pW++;
        pS++;
      } else {
        pW = mp;
        pS = cp++;
      }
    }

    while(*pW == '*') {
      pW++;
    }

    return !*pW;
  }



  // 检查字符的有效性，包括26个大小写字母、0~9及_-.@符号
  BOOL IsValidChar(TCHAR ch) {
    // 26个小写字母
    if((ch >= _T('a')) && (ch <= _T('z'))) {
      return TRUE;
    }

    // 26个大写字母
    if((ch >= _T('A')) && (ch <= _T('Z'))) {
      return TRUE;
    }

    // 0~9
    if((ch >= _T('0')) && (ch <= _T('9'))) {
      return TRUE;
    }

    // _-.@
    if((ch == _T('_')) || (ch == _T('-')) || (ch == _T('.')) || (ch == _T('@'))) {
      return TRUE;
    }

    return FALSE;
  }

  // 总结了一下合法的email地址格式如下：
  // 1. 首字符必须用字母，而且其它的字符只能用26个大小写字母、0~9及_-.@符号
  // 2. 必须包含一个并且只有一个符号“@”
  // 3. @后必须包含至少一个至多三个符号“.”
  // 4. 第一个字符不得是“@”或者“.”(第一步已检查过了)
  // 5. 不允许出现“@.”或者.@
  // 6. 结尾不得是字符“@”或者“.”

  //EMAIL有效性验证
  BOOL IsValidEmail(LPCTSTR pszEmail) {
    int iAtPos = 0;
    int iLastDotPos = 0;
    int i = 0;
    int iAtTimes = 0;

    if(pszEmail == NULL || 64 < ::_tcsclen(pszEmail)) {
      return FALSE;
    }

    while(*(pszEmail + i) != _T('\0')) {
      TCHAR ch = *(pszEmail + i);

      if(i == 0) {
        //首字符必须是字母
        if(!(((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))) {
          return FALSE;
        }
      }

      if(!(IsValidChar(ch))) {
        return FALSE;
      }


      if(!_istprint(ch) || _istspace(ch))  //空格和控制字符是非法的，限制得还比较宽松
      {
        iAtTimes = 0;
        break;
      }

      if(ch == _T('@')) {
        iAtPos = i;
        iAtTimes++;
      } else if(ch == _T('.')) {
        iLastDotPos = i;
      }

      i++;
    }

    if(i > 64 || iAtPos < 1 || (iLastDotPos - 2) < iAtPos ||
      (i - iLastDotPos) < 3 || (i - iLastDotPos) > 5 || iAtTimes > 1 || iAtTimes == 0) //对@以及域名依靠位置来判断，限制长度为64
    {
      return FALSE;
    }

    return TRUE;
  }

  BOOL BASE_EXPORT GetLastErrorMessage(DWORD last_error, WCHAR* wchBuffer, int nBufferSize) {
    if(!wchBuffer) {
      return FALSE;
    }
    static WCHAR errmsg[512];

    if(!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                      0,
                      last_error,
                      0,
                      errmsg,
                      511,
                      NULL)) {
      /* if we fail, call ourself to find out why and return that error */
      WCHAR msg[MAX_PATH] = { 0 };
      return (GetLastErrorMessage(GetLastError(), msg, sizeof(msg) / sizeof(msg[0])));
    }
    if(nBufferSize < (int)wcslen(errmsg) + 1) {
      return FALSE;
    }
    return (0 == wcsncpy_s(wchBuffer, nBufferSize, errmsg, _TRUNCATE));
  }

}  // namespace string_utils
}  // namespace base
