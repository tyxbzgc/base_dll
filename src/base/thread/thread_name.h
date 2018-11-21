// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.


#ifndef BASE_THREAD_THREAD_NAME_H_
#define BASE_THREAD_THREAD_NAME_H_
/** ---------------------------------------------------------------------------
 *  本文件用于设置函数名字，在调试器下可以显示线程的名字，而不是难懂的启动地址
 *  http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
 */

#include <Windows.h>

namespace base {
struct DebuggerThreadNameMgr {
#pragma pack(push, 8)
  struct ThreadNameInfo {
    DWORD  dwType;
    LPCSTR szName;
    DWORD  dwThreadID;
    DWORD  dwFlags;
  };
#pragma pack(pop)

  /** 设置当前线程名字
   *  通过抛一个特殊的异常，让调试器存储并显示线程名字
   *  \param dwThreadID 线程ID, -1表示当前线程
   *  \param pszThreadName 线程名
   */
  static void SetThreadName(DWORD dwThreadID, LPCSTR pszThreadName) {
    // 如果当前没有挂接debugger，就不必要费力抛异常了
    if (!::IsDebuggerPresent()) return;

    if (dwThreadID == -1) {
      dwThreadID = ::GetCurrentThreadId();
    }

    struct ThreadNameInfo info = { 0x1000, pszThreadName, dwThreadID, 0 };

    __try {
      RaiseException(0x406D1388, 0
        , sizeof(info) / sizeof(ULONG_PTR)
        , reinterpret_cast<ULONG_PTR*>(&info));
    }
    __except(GetExceptionCode() == 0x406D1388 ?
      EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
      (0);  // This line fixes warning:C6322 (Empty _except block)
    }
  }
};
}  // namespace base

#endif  // BASE_THREAD_THREAD_NAME_H_
