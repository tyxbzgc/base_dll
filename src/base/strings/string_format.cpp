// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base\strings\string_utils.h"

namespace base {
namespace string_utils {
  // Formats a string in a fixed size buffer. The result is always zero-terminated.
  // The return value is the number of characters in the output (excluding the zero terminator)
  int Format(char *strDstBuffer, int nLenOfBuffer, const char *format, va_list args) {
    int      len;
    int res = 0;
    int ret = 0;

    if(0 == res) {
      if(NULL == strDstBuffer) {
        res = -1;
      }
    }

    if(0 == res) {
      // _vscprintf doesn't count the null terminating string so we add 1.
      len = _vscprintf_p(format, args) + 1;

      if(len > nLenOfBuffer) {
        res = -2;
      }
    }

    if(0 == res) {
      // return the number of characters written, not including the terminating null character, 
      // or a negative value if an output error occurs.
      if((ret = _vsprintf_p(strDstBuffer, nLenOfBuffer, format, args)) < 0) {
        res = -3;
      }
    }
    return ret;
  }

  int FormatString(char *strDstBuffer, int nLenOfBuffer, const char *format, ...) {
    va_list  args;
    va_start(args, format);

    int ret = Format(strDstBuffer, nLenOfBuffer, format, args);

    va_end(args);

    return ret;
  }

  // Formats a string in a fixed size buffer. The result is always zero-terminated.
  // The return value is the number of characters in the output (excluding the zero terminator)
  int Format(wchar_t *wstrDstBuffer, int nLenOfBuffer, const wchar_t *format, va_list args) {
    int      len;
    int res = 0;
    int ret = 0;

    if(0 == res) {
      if(NULL == wstrDstBuffer) {
        res = -1;
      }
    }

    if(0 == res) {
      // _vscprintf doesn't count the null terminating string so we add 1.
      len = _vscwprintf_p(format, args) + 1;

      if(len > nLenOfBuffer) {
        res = -2;
      }
    }

    if(0 == res) {
      // return the number of characters written, not including the terminating null character, 
      // or a negative value if an output error occurs.
      if((ret = _vswprintf_p(wstrDstBuffer, nLenOfBuffer, format, args)) < 0) {
        res = -3;
      }
    }
    return ret;
  }

  int FormatString(wchar_t *wstrDstBuffer, int nLenOfBuffer, const wchar_t *format, ...) {
    va_list  args;
    va_start(args, format);

    int ret = Format(wstrDstBuffer, nLenOfBuffer, format, args);

    va_end(args);

    return ret;
  }

  std::string Format(const char *format, va_list args) {
    int res = 0;
    std::string ret;
    // _vscprintf doesn't count the null terminating string so we add 1.
    int len = _vscprintf_p(format, args) + 1;
    char * buffer = NULL;
    if(0 == res) {
      buffer = new(std::nothrow) char[len];
      if(NULL == buffer) {
        res = -1;
      }
    }

    if(0 == res) {
      if(Format(buffer, len, format, args) < 0) {
        ret = std::string("");
        res = -2;
      }
    }

    if(0 == res) {
      ret = std::string(buffer);
    }

    if(buffer) {
      delete[]buffer;
      buffer = NULL;
    }

    return ret;
  }

  std::string FormatString(const char *format, ...) {
    int res = 0;
    std::string ret;
    va_list  args;
    va_start(args, format);

    ret = Format(format, args);

    va_end(args);

    return ret;
  }

  void FormatString(std::string &string, const char *format, ...) {
    int res = 0;
    va_list  args;
    va_start(args, format);

    string = Format(format, args);

    va_end(args);
  }

  // UNICODE

  std::wstring Format(const wchar_t *format, va_list args) {
    int res = 0;
    std::wstring ret;
    // _vscprintf doesn't count the null terminating string so we add 1.
    int len = _vscwprintf_p(format, args) + 1;
    wchar_t * buffer = NULL;
    if(0 == res) {
      buffer = new(std::nothrow) wchar_t[len];
      if(NULL == buffer) {
        res = -1;
      }
    }

    if(0 == res) {
      if(Format(buffer, len, format, args) < 0) {
        ret = std::wstring(L"");
        res = -2;
      }
    }

    if(0 == res) {
      ret = std::wstring(buffer);
    }

    if(buffer) {
      delete[]buffer;
      buffer = NULL;
    }

    return ret;
  }

  std::wstring FormatString(const wchar_t *format, ...) {
    int res = 0;
    std::wstring ret;
    va_list  args;
    va_start(args, format);

    ret = Format(format, args);

    va_end(args);

    return ret;
  }

  void FormatString(std::wstring &string, const wchar_t *format, ...) {
    int res = 0;
    va_list  args;
    va_start(args, format);

    string = Format(format, args);

    va_end(args);
  }



  ///////////////////////////////////////////////////////////////////////////////
  // Time conversions

  // Converts time_t to FILETIME. Does not perform any timezone conversions
  void UnixTimeToFileTime(time_t t, FILETIME &ft) {
    __int64 ll = ((__int64)t) * 10000000 + 116444736000000000;
    ft.dwLowDateTime = (DWORD)ll;
    ft.dwHighDateTime = (DWORD)(ll >> 32);
  }


  int FormatToSysTime(SYSTEMTIME *stDstTime, const time_t *ttSrcTime) {
    int res = 0;
    if(0 == res) {
      if(!stDstTime || !ttSrcTime) {
        res = -1;
      }
    }

    if(0 == res) {
      FILETIME ft;
      UnixTimeToFileTime(*ttSrcTime, ft);
      if(0 == FileTimeToSystemTime(&ft, stDstTime))//FileTimeToSystemTime返回0表示失败，非0为成功
      {
        res = -2;
      }
    }

    return res;
  }

  int FormatToSysTime(SYSTEMTIME *stDstTime, const FILETIME *ftSrcTime) {
    int res = 0;
    if(0 == res) {
      if(!stDstTime || !ftSrcTime) {
        res = -1;
      }
    }

    if(0 == res) {
      if(0 == FileTimeToSystemTime(ftSrcTime, stDstTime))//FileTimeToSystemTime返回0表示失败，非0为成功
      {
        res = -2;
      }
    }

    return res;
  }
}  // namespace string_utils
}  // namespace base