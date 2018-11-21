// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/system/network_utils.h"
#include <Iphlpapi.h>

namespace base {
namespace sysmisc {
#pragma comment(lib, "iphlpapi.lib")
//#pragma comment(lib, "ws2_32.lib")

static BOOL IsNetWorkOK_Tried() {
  // 调用有可能非常频繁, 采用 CACHE_TIME ms 进行缓存控制
  // 只Cache s_bIsNetworkOK=TRUE的情况, 当s_bIsNetworkOK=FALSE时, 我们总是检查
  static const DWORD CACHE_TIME = 1000;
  static BOOL s_bIsNetworkOK = TRUE;
  static DWORD s_dwLastUpdate = 0;

  DWORD dwTick = ::GetTickCount();

  //特意使用减法, 这样溢出之后, 可以得到更新.
  //这里使用了static变量, 还需要考虑多线程的问题, 将s_dwLastUpdate延后更新, 密集调用的话,一般都是单线程的.
  if(s_bIsNetworkOK && (dwTick - s_dwLastUpdate < CACHE_TIME)) {
    return s_bIsNetworkOK;
  }

  ULONG uSize = 0;
  //根据MSDN的示例, 即使uSize = 0, pForwardTable不能为0
  PMIB_IPFORWARDTABLE pForwardTable = (PMIB_IPFORWARDTABLE)malloc(sizeof(MIB_IPFORWARDTABLE));
  DWORD dwRtn = ::GetIpForwardTable(pForwardTable, &uSize, FALSE);
  if(dwRtn == ERROR_INSUFFICIENT_BUFFER) {
    free(pForwardTable);

    // 如果路由表行数超过50条, 则很有可能是有IP过滤软件等, 我们假设这种情况的网络都是通的.
    static const DWORD MAX_TABLE_SIZE = 50 * sizeof(MIB_IPFORWARDROW) + sizeof(MIB_IPFORWARDTABLE);
    if(uSize > MAX_TABLE_SIZE) {
      s_bIsNetworkOK = TRUE;
      s_dwLastUpdate = dwTick;
      return TRUE;
    }

    pForwardTable = (PMIB_IPFORWARDTABLE)malloc(uSize);
    dwRtn = ::GetIpForwardTable(pForwardTable, &uSize, FALSE);
  }

  if(dwRtn != NO_ERROR) {
    free(pForwardTable);
    s_bIsNetworkOK = TRUE;
    s_dwLastUpdate = dwTick;
    return TRUE;
  }

  for(DWORD i = 0; i < pForwardTable->dwNumEntries; ++i) {
    MIB_IPFORWARDROW & forwardRaw = pForwardTable->table[i];

    if(0 == forwardRaw.dwForwardDest && 0 == forwardRaw.dwForwardMask) {
      free(pForwardTable);
      s_bIsNetworkOK = TRUE;
      s_dwLastUpdate = dwTick;
      return TRUE;
    }
  }

  free(pForwardTable);
  s_bIsNetworkOK = FALSE;
  s_dwLastUpdate = dwTick;
  return FALSE;
}

BOOL IsNetWorkOK() {
  BOOL ret = TRUE;
  __try {
    ret = IsNetWorkOK_Tried(); // GetIpForwardTable crash sometime, since the 3rd party module inside.
  } __except(EXCEPTION_EXECUTE_HANDLER) {

  }
  return ret;
}

BOOL IsInternetOK() {
  return IsNetWorkOK();
}
}  // namespace sysmisc
}  // namespace base
