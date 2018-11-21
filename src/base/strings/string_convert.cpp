// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base\strings\string_utils.h"
#include <strstream>

#pragma hdrstop

namespace base {
namespace string_utils {
  using std::string;
  using std::wstring;

  //非导出函数，DLL内部使用
  wstring AToW(const char *pInput,
                const size_t inputLength)		//CStringConverter::
  {
    wstring result;

    if(inputLength != 0) {
      int nNeedLen = ::MultiByteToWideChar(CP_ACP, 0, pInput, (int)inputLength, NULL, 0);
      result.resize(nNeedLen);

      if(0 == ::MultiByteToWideChar(CP_ACP, 0, pInput, (int)inputLength, const_cast<wchar_t *>(result.c_str()), (int)result.length())) {
        //throw CWin32Exception(_T("CStringConverter::AtoW()"), ::GetLastError());
      }
    }

    return result;
  }


  //导出函数
  WCHAR* AToW(const std::string &input, WCHAR* wchOutput, int nOutputSize) {
    if(!wchOutput) {
      return L"";
    }
    wstring wstr = AToW(input.c_str(), input.length());
    if(nOutputSize < (int)wstr.length() + 1) {
      return L"";
    }
    wcsncpy_s(wchOutput, nOutputSize, wstr.c_str(), _TRUNCATE);

    return wchOutput;
  }


  //导出函数
  WCHAR* AToW(const char *pInput, WCHAR* wchOutput, int nOutputSize) {
    if(!wchOutput) {
      return L"";
    }
    wstring wstr = AToW(pInput, strlen(pInput));
    if(nOutputSize < (int)wstr.length() + 1) {
      return L"";
    }
    wcsncpy_s(wchOutput, nOutputSize, wstr.c_str(), _TRUNCATE);

    return wchOutput;
  }

  WCHAR* AToW(const char *pInput, const size_t inputLength, WCHAR* wchOutput, int nOutputSize) {
    if(!wchOutput) {
      return L"";
    }
    wstring wstr = AToW(pInput, inputLength);
    if(nOutputSize < (int)wstr.length() + 1) {
      return L"";
    }
    wcsncpy_s(wchOutput, nOutputSize, wstr.c_str(), _TRUNCATE);

    return wchOutput;

  }


  BSTR AToBSTR(const string &input)		//CStringConverter::
  {
    const wstring output = AToW(input.c_str(), input.length());

    return ::SysAllocStringLen(output.c_str(), (UINT)output.length());
  }

  BSTR AToBSTR(const char *pInput,
                const size_t inputLength)		//CStringConverter::
  {
    const wstring output = AToW(pInput, inputLength);

    return ::SysAllocStringLen(output.c_str(), (UINT)output.length());
  }

  string WToA(const wchar_t *pInput,
              const size_t inputLength)		//CStringConverter::
  {
    string result;

    if(inputLength != 0) {
      int nNeedLen = ::WideCharToMultiByte(CP_ACP, 0, pInput, (int)inputLength, NULL, 0, 0, 0);
      result.resize(nNeedLen);

      if(0 == ::WideCharToMultiByte(CP_ACP, 0, pInput, (int)inputLength, const_cast<char *>(result.c_str()), (int)result.length(), 0, 0)) {
        //throw CWin32Exception(_T("CStringConverter::WtoA()"), ::GetLastError());
      }
    }

    return result;
  }


  CHAR* WToA(const std::wstring &input, CHAR* chOutput, int nOutputSize) {
    if(!chOutput) {
      return "";
    }
    string str = WToA(input.c_str(), input.length());
    if(nOutputSize < (int)str.length() + 1) {
      return "";
    }

    strncpy_s(chOutput, nOutputSize, str.c_str(), _TRUNCATE);

    return chOutput;
  }


  CHAR* WToA(const wchar_t *pInput, CHAR* chOutput, int nOutputSize) {
    if(!pInput || !chOutput) {
      return "";
    }

    string str = WToA(pInput, wcslen(pInput));

    if(nOutputSize < (int)str.length() + 1) {
      return "";
    }

    strncpy_s(chOutput, nOutputSize, str.c_str(), _TRUNCATE);

    return chOutput;
  }



  CHAR* WToA(const wchar_t *pInput, const size_t inputLength, CHAR* chOutput, int nOutputSize) {
    if(!pInput || !chOutput) {
      return "";
    }

    string str = WToA(pInput, inputLength);

    if(nOutputSize < (int)str.length() + 1) {
      return "";
    }

    strncpy_s(chOutput, nOutputSize, str.c_str(), _TRUNCATE);

    return chOutput;

  }

  BSTR WToBSTR(const wstring &input)		//CStringConverter::
  {
    return ::SysAllocStringLen(input.c_str(), (UINT)input.length());
  }

  // BSTRto

  CHAR* BSTRToA(const BSTR bstr, CHAR* chOutput, int nOutputSize) {

    if(!chOutput || ::SysStringLen(bstr) == 0) {
      return "";
    }

    string str = WToA(bstr, ::SysStringLen(bstr));

    if(nOutputSize < (int)str.length() + 1) {
      return "";
    }

    strncpy_s(chOutput, nOutputSize, str.c_str(), _TRUNCATE);
    return chOutput;
  }

  WCHAR* BSTRToW(const BSTR bstr, WCHAR* wchOutput, int nOutputSize) {
    if(!wchOutput || ::SysStringLen(bstr) == 0) {
      return L"";
    }

    wcsncpy_s(wchOutput, nOutputSize, bstr, SysStringLen(bstr));

    return wchOutput;
  }

  WCHAR* UTF8AToW(const std::string &input, WCHAR*wchOutput, int nOutputSize) {
    if(!wchOutput) {
      return L"";
    }
    wstring wstr = UTF8AToW(input);
    if(nOutputSize < (int)wstr.length() + 1) {
      return L"";
    }

    wcsncpy_s(wchOutput, nOutputSize, wstr.c_str(), _TRUNCATE);
    return wchOutput;
  }

  int BASE_EXPORT GuidToStringW(::GUID uid, WCHAR* wchBuffer, int nBufferSize) {
    if(!wchBuffer) {
      return -1;
    }

    return ::_snwprintf_s(wchBuffer, nBufferSize, _TRUNCATE,
                          L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                          uid.Data1, uid.Data2, uid.Data3,
                          uid.Data4[0], uid.Data4[1], uid.Data4[2],
                          uid.Data4[3], uid.Data4[4], uid.Data4[5],
                          uid.Data4[6], uid.Data4[7]
    );
  }

  int BASE_EXPORT GuidToStringA(::GUID uid, CHAR* chBuffer, int nBufferSize) {
    if(!chBuffer) {
      return -1;
    }

    return ::_snprintf_s(chBuffer, nBufferSize, _TRUNCATE,
                          "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                          uid.Data1, uid.Data2, uid.Data3,
                          uid.Data4[0], uid.Data4[1], uid.Data4[2],
                          uid.Data4[3], uid.Data4[4], uid.Data4[5],
                          uid.Data4[6], uid.Data4[7]
    );

  }

  BOOL StringToGuid(LPCTSTR szGuid, ::GUID* uid) {
    int el[11] = { 0 };
    int nRet = 0;

    nRet = ::_stscanf_s(szGuid,
                        _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
                        &el[0], &el[1], &el[2],
                        &el[3], &el[4], &el[5],
                        &el[6], &el[7], &el[8],
                        &el[9], &el[10]
    );
    if(nRet != 11) {
      return FALSE;
    }

    uid->Data1 = el[0];
    uid->Data2 = el[1];
    uid->Data3 = el[2];
    uid->Data4[0] = el[3];
    uid->Data4[1] = el[4];
    uid->Data4[2] = el[5];
    uid->Data4[3] = el[6];
    uid->Data4[4] = el[7];
    uid->Data4[5] = el[8];
    uid->Data4[6] = el[9];
    uid->Data4[7] = el[10];


    return TRUE;
  }


  // 下面的代码从StringUtil.cpp中移植过来
  BOOL MultiByteToUnicode(const std::string& strMultiByte, std::wstring& strUnicode, UINT uCodePage) {
    int nUnicodeCount = MultiByteToWideChar(uCodePage, 0, strMultiByte.c_str(), -1, NULL, 0);
    int nWrittenChar = -1;

    if(nUnicodeCount > 0) {
      wchar_t* szTemp = new wchar_t[nUnicodeCount];
      nWrittenChar = MultiByteToWideChar(uCodePage, 0, strMultiByte.c_str(), -1, szTemp, nUnicodeCount);
      strUnicode = szTemp;
      delete[] szTemp;
    }

    return (nUnicodeCount == nWrittenChar);
  }

  BOOL UnicodeToMultiByte(const std::wstring& strUnicode, std::string& strMultiByte, UINT uCodePage) {
    int nUTF8Count = WideCharToMultiByte(uCodePage, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
    int nWrittenChar = -1;

    if(nUTF8Count > 0) {
      char* szTemp = new char[nUTF8Count];
      nWrittenChar = WideCharToMultiByte(uCodePage, 0, strUnicode.c_str(), -1, szTemp, nUTF8Count, NULL, NULL);
      strMultiByte = szTemp;
      delete[] szTemp;
    }

    return (nUTF8Count == nWrittenChar);
  }

  std::wstring MultiByteToUnicode(const std::string& strMultiByte, UINT uCodePage) {
    std::wstring Temp;

    MultiByteToUnicode(strMultiByte, Temp, uCodePage);
    return Temp;
  }

  std::string UnicodeToMultiByte(const std::wstring& strUnicode, UINT uCodePage) {
    std::string Temp;

    UnicodeToMultiByte(strUnicode, Temp, uCodePage);
    return Temp;
  }

  BOOL StringToDate(LPCTSTR lpszDate, time_t &tmDate) {
    if(lpszDate && *lpszDate) {
      tm tmTemp;
      int FieldCount = 0;

      ::ZeroMemory(&tmTemp, sizeof(tmTemp));
      FieldCount = ::_stscanf_s(lpszDate, _T("%04d-%02d-%02d"), &tmTemp.tm_year, &tmTemp.tm_mon, &tmTemp.tm_mday);
      if(3 == FieldCount) {
        tmTemp.tm_year -= 1900;
        tmTemp.tm_mon -= 1;

        tmDate = mktime(&tmTemp);
        if(tmDate != -1) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }


  LPSTR WINAPI UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp) {
    lpa[0] = '\0';
    WideCharToMultiByte(acp, 0, lpw, -1, lpa, nChars, NULL, NULL);
    return lpa;
  }

  LPSTR WINAPI UnicodeToMultiByte(LPSTR lpa, LPCWSTR lpt, int nChars) {
    lpa[0] = '\0';

    //#if defined(UNICODE) || defined(_UNICODE)
    WideCharToMultiByte(CP_ACP, 0, lpt, -1, lpa, nChars, NULL, NULL);
    //#else
    //		strncpy(lpa, lpt, nChars);
    //#endif

    return lpa;
  }

  LPWSTR WINAPI MultiByteToUnicode(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp) {
    lpw[0] = '\0';
    MultiByteToWideChar(acp, 0, lpa, -1, lpw, nChars);
    return lpw;
  }

  LPWSTR WINAPI MultiByteToUnicode(LPWSTR lpt, LPCSTR lpa, int nChars) {
    lpt[0] = '\0';

    //#if defined(UNICODE) || defined(_UNICODE)
    MultiByteToWideChar(CP_ACP, 0, lpa, -1, lpt, nChars);
    //#else
    //		strncpy(lpt, lpa, nChars);
    //#endif

    return lpt;
  }

  int BASE_EXPORT BytesToStringW(const BYTE *pBuffer, size_t iBytes, WCHAR*wchBuffer, int nBufferSize) {
    if(!wchBuffer) {
      return -1;
    }
    BYTE c = 0;
    BYTE b = 0;
    std::wstring result;

    for(size_t i = 0; i < iBytes; i++) {
      b = pBuffer[i] >> 4;
      if(9 >= b) {
        c = b + '0';
      } else {
        c = (b - 10) + 'A';
      }

      result += (TCHAR)c;

      b = pBuffer[i] & 0x0f;

      if(9 >= b) {
        c = b + '0';
      } else {
        c = (b - 10) + 'A';
      }

      result += (TCHAR)c;
    }
    if(nBufferSize < (int)result.length() + 1) {
      return -1;
    }

    wcsncpy_s(wchBuffer, nBufferSize, result.c_str(), _TRUNCATE);

    return result.length();
  }


  static std::string WideToMultiByte(const wchar_t* pwcsText) {
    std::string strText;
    int chLen = 0;
    char* pszBuf = NULL;

    chLen = (int)(2 * (1 + wcslen(pwcsText)));
    pszBuf = new char[chLen];
    ::ZeroMemory(pszBuf, chLen);
    ::WideCharToMultiByte(CP_ACP, 0l, pwcsText, -1, pszBuf, chLen, NULL, NULL);
    strText = pszBuf;
    delete[] pszBuf;
    pszBuf = NULL;

    return strText;
  }

  static std::wstring MultiByteToWide(const char* pszText) {
    std::wstring strText;
    int chLen = 0;
    wchar_t* pszBuf = NULL;

    chLen = static_cast<int>(1 + strlen(pszText));
    pszBuf = new wchar_t[chLen];
    ::ZeroMemory(pszBuf, chLen);
    ::MultiByteToWideChar(CP_ACP, 0l, pszText, -1, pszBuf, chLen);
    strText = pszBuf;
    delete[] pszBuf;
    pszBuf = NULL;

    return strText;
  }


  BOOL AToUTF8(const std::string& input, std::vector<char>& buf) {
    std::wstring wstr = AToW(input, CP_ACP);
    return UTF8WToA(wstr.c_str(), buf);
  }

  BOOL AToUTF8(const std::string& input, char * buf, int len) {
    std::wstring wstr = AToW(input, CP_ACP);
    std::vector<char> vecChar;
    BOOL bRet = UTF8WToA(wstr.c_str(), vecChar);
    if(len < vecChar.size()) {
      return FALSE;
    }
    for(int i = 0; i<vecChar.size(); i++) {
      buf[i] = vecChar[i];
    }
    buf[vecChar.size()] = 0;
    return bRet;
  }

  CHAR * UTF8ToA(const std::string& utf8Str, char * buf, int len) {
    std::wstring wstr = MultiByteToUnicode(utf8Str, CP_UTF8);
    std::string str = WToA(wstr, CP_ACP);
    if(len < (int)str.size()) {
      return "";
    } else {
      strncpy_s(buf, len, str.c_str(), str.size());
      return buf;
    }
  }

  void StringToBytesW(
    const std::wstring &ts,
    BYTE *pBuffer,
    size_t nBytes) {
    BYTE b = 0;
    BYTE b1 = 0;
    BYTE val = 0;
    size_t stringOffset = 0;
    std::string s;

    //#ifdef _UNICODE

    s = WideToMultiByte(ts.c_str());
    for(size_t i = 0; i < nBytes; i++) {
      stringOffset = i * 2;
      b = s[stringOffset];

      if(isdigit(b)) {
        val = (BYTE)((b - '0') * 16);
      } else {
        val = (BYTE)(((toupper(b) - 'A') + 10) * 16);
      }

      b1 = s[stringOffset + 1];
      if(isdigit(b1)) {
        val += b1 - '0';
      } else {
        val += (BYTE)((toupper(b1) - 'A') + 10);
      }

      pBuffer[i] = val;
    }
  }

  int BASE_EXPORT ByteToHexW(BYTE c, WCHAR* wchBuffer, int nBufferSize) {
    if(!wchBuffer || nBufferSize < 3) {
      return -1;
    }

    const int val = c;

    return ::_snwprintf_s(wchBuffer, nBufferSize, _TRUNCATE, _T("%02X"), val);
  }
  int BASE_EXPORT ByteToHexA(BYTE c, CHAR* chBuffer, int nBufferSize) {
    if(!chBuffer || nBufferSize < 3) {
      return -1;
    }

    const int val = c;

    return ::_snprintf_s(chBuffer, nBufferSize, _TRUNCATE, "%02X", val);

  }


  /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  //move from ConvertBase


  std::wstring AToW(__in LPCSTR szSrc, __in int iSrcCount, __in DWORD cpCodePage) {
    if(NULL == szSrc)
      return std::wstring();

    std::wstring wsDst;
    LPCWSTR l_pwstr = AToW(szSrc, iSrcCount, wsDst, cpCodePage);
    if(NULL != l_pwstr)
      return wsDst;
    else
      return std::wstring();
  }

  //std::wstring AToW(const std::string& cstr,__in DWORD cpCodePage)
  //{
  //	return AToW(cstr.c_str(),cstr.size(), cpCodePage) ;
  //}

  //************************************************************************
  // 功能说明: 多字符转换为UNICODE
  //
  // 函数名称: AstrToWstr
  //
  // 访问权限: public
  //
  // 返回值类型: LPCWSTR	
  //
  // 参数1: LPCSTR	szSrc   传入参数,指向要转换的源字符串
  //
  // 参数2: LPWSTR	 wszDst   传出参数，字符串转换后存在它指向的内存中，指向的空间
  //								必须足够大，用以存储转换后的字符串。
  //
  // 参数3: const DWORD iDstCount  传出参数指针l_pchDest所指向的内存块的大小
  // 
  // 参数4: CodePage cpCodePage 要转换的源字符串的类型,默认是CP_UTF8,其它值参见CodePage
  // 
  //************************************************************************
  LPCWSTR AToW(__in LPCSTR szSrc, __out LPWSTR wszDst, __in const int iDstCount, __in DWORD cpCodePage) {
    return AToW(szSrc, -1, wszDst, iDstCount, cpCodePage);
  }

  inline LPCWSTR AToW(__in LPCSTR szSrc, __in const int iSrcCount, __out LPWSTR wszDst, __in const int iDstCount, __in DWORD cpCodePage) {
    if(NULL == szSrc)
      return NULL;

    int iCount = MultiByteToWideChar(cpCodePage, 0, szSrc, iSrcCount, wszDst, iDstCount);
    if(0 == iCount)
      return NULL;
    else
      return wszDst;
  }

  LPCWSTR AToW(__in LPCSTR szSrc, __out std::wstring& wsDst, __in DWORD cpCodePage) {
    return AToW(szSrc, -1, wsDst, cpCodePage);
  }

  LPCWSTR AToW(__in LPCSTR szSrc, __in const int iSrcCount, __out std::wstring& wsDst, __in DWORD cpCodePage) {
    if(NULL == szSrc)
      return NULL;
    wchar_t szBuf[512] = { 0 };
    int iConvert = ::MultiByteToWideChar(cpCodePage, 0, szSrc, iSrcCount, szBuf, _countof(szBuf) - 1);
    if(iConvert > 0) {
      szBuf[iConvert] = 0;
      wsDst = szBuf;
      return wsDst.c_str();
    } else {
      // 为了运行速度，牺牲一点点空间了
      int dstCount = (iSrcCount == -1 ? strlen(szSrc) : iSrcCount) + 1;
      wchar_t *wBuf = new wchar_t[dstCount + 1];
      iConvert = ::MultiByteToWideChar(cpCodePage, 0, szSrc, iSrcCount, wBuf, dstCount);
      wBuf[iConvert] = 0;
      wsDst = iConvert > 0 ? wBuf : L"";
      delete[] wBuf;
      return wsDst.c_str();
    }

    return NULL;
  }

  //************************************************************************
  // 功能说明: UNICODE字符转为多字符
  //
  // 函数名称: WstrToAstr
  //
  // 访问权限: public
  //
  // 返回值类型: LPCSTR	转换后的字符串，如果失败返回 NULL
  //
  // 参数1: LPCWSTR cpwchSource   传入参数,指向要转换的UNICODE源字符串
  //
  // 参数2: LPSTR	 pchDest   传出参数，字符串转换后存在它指向的内存中，指向的空间
  //								必须足够大，用以存储转换后的字符串。
  //
  // 参数3: const DWORD cdwDestSize  传出参数指针l_pchDest所指向的内存块的大小
  // 
  // 参数4: CodePage cpCodePage 要转换的源字符串的类型,默认是CP_Ansi,其它值参见CodePage
  // 
  //************************************************************************
  LPCSTR WToA(__in LPCWSTR wszSrc,
              __out LPSTR szDst,
              __in const int iDstCount,
              __in DWORD cpCodePage) {
    return WToA(wszSrc, -1, szDst, iDstCount, cpCodePage);
  }

  LPCSTR WToA(__in LPCWSTR wszSrc, __in const int iSrcCount, __out LPSTR szDst, __in const int iDstCount, __in DWORD cpCodePage) {
    if(NULL == wszSrc)
      return NULL;

    int iCount = WideCharToMultiByte(cpCodePage, 0, wszSrc, iSrcCount, szDst, iDstCount, NULL, FALSE);
    if(0 == iCount)
      return NULL;
    else
      return szDst;
  }


  //************************************************************************
  // 功能说明: UNICODE字符转为多字符
  //
  // 函数名称: WstrToAstr
  //
  // 访问权限: public
  //
  // 返回值类型: LPCSTR	
  //
  // 参数1: LPCWSTR cpwchSource   传入参数,指向要转换的UNICODE源字符串
  //
  // 参数2: std::wstring& sDest 储存转换后的结果
  // 
  // 参数4: CodePage cpCodePage 要转换的源字符串的类型,默认是CP_Ansi,其它值参见CodePage
  // 
  //************************************************************************
  LPCSTR WToA(__in LPCWSTR wszSrc,
              __out std::string& sDest,
              __in DWORD cpCodePage) {
    return WToA(wszSrc, -1, sDest, cpCodePage);
  }

  LPCSTR WToA(__in LPCWSTR wszSrc, __in const int iSrcCount, __out std::string& sDest, __in DWORD cpCodePage) {
    if(NULL == wszSrc || iSrcCount == 0)
      return NULL;

    char buf[512] = { 0 };
    int iConvert = ::WideCharToMultiByte(cpCodePage, 0, wszSrc, iSrcCount, buf, _countof(buf) - 1, NULL, FALSE);
    if(iConvert>0) {
      buf[iConvert] = 0;
      sDest = iConvert > 0 ? buf : "";
      return sDest.c_str();
    } else if(GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      int dstCount = ::WideCharToMultiByte(cpCodePage, 0, wszSrc, iSrcCount, 0, 0, 0, 0);
      dstCount += 1;
      char * newBuf = new char[dstCount];
      if(newBuf != NULL) {
        iConvert = ::WideCharToMultiByte(cpCodePage, 0, wszSrc, iSrcCount, newBuf, dstCount, NULL, FALSE);
        newBuf[iConvert] = 0;
        sDest = iConvert > 0 ? newBuf : "";
        delete[] newBuf;
        return sDest.c_str();
      }
    }

    return NULL;
  }


  //************************************************************************
  // 功能说明：把UNICODE字符串转化为ANSI字符串
  //
  // 函数名称: WStrToAStr
  //
  // 访问权限: public
  //
  // 返回值类型: std::string	返回转换后的ANSI字符串
  //
  // 参数1: LPCTSTR wszSrc	要转换的字符串
  //
  //************************************************************************
  // 把UNICODE字符串转化为ANSI字符串

  std::string WToA(__in LPCWSTR wszSrc, __in const int iSrcCount, __in DWORD cpCodePage) {
    if(NULL == wszSrc)
      return std::string();

    std::string l_str;
    LPCSTR l_pstr = WToA(wszSrc, iSrcCount, l_str, cpCodePage);
    if(NULL != l_pstr)
      return l_str;
    else
      return std::string();
  }

  //std::string WToA(__in const std::wstring& cwstr,__in DWORD cpCodePage)
  //{
  //	return WToA(cwstr.c_str(),cwstr.size()+1,cpCodePage);
  //}

  BOOL UTF8WToA(const wchar_t* wcs, __out std::vector<char>& utf8) {
    int len = ::WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
    if(len == 0) return FALSE;

    utf8.resize(len);
    ::WideCharToMultiByte(CP_UTF8, 0, wcs, -1, &utf8[0], len, NULL, NULL);

    return TRUE;
  }

}  // namespace string_utils
}  // namespace base
