// Copyright (c) 2015 The Baidu Authors.
// All rights reserved.


#ifndef BASE_THREAD_THREAD_NAME_H_
#define BASE_THREAD_THREAD_NAME_H_
/** ---------------------------------------------------------------------------
 *  ���ļ��������ú������֣��ڵ������¿�����ʾ�̵߳����֣��������Ѷ���������ַ
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

  /** ���õ�ǰ�߳�����
   *  ͨ����һ��������쳣���õ������洢����ʾ�߳�����
   *  \param dwThreadID �߳�ID, -1��ʾ��ǰ�߳�
   *  \param pszThreadName �߳���
   */
  static void SetThreadName(DWORD dwThreadID, LPCSTR pszThreadName) {
    // �����ǰû�йҽ�debugger���Ͳ���Ҫ�������쳣��
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
