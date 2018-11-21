// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/system/network_utils.h"
#include <Iphlpapi.h>

namespace base {
namespace sysmisc {
#pragma comment(lib, "iphlpapi.lib")
//#pragma comment(lib, "ws2_32.lib")

static BOOL IsNetWorkOK_Tried() {
  // �����п��ܷǳ�Ƶ��, ���� CACHE_TIME ms ���л������
  // ֻCache s_bIsNetworkOK=TRUE�����, ��s_bIsNetworkOK=FALSEʱ, �������Ǽ��
  static const DWORD CACHE_TIME = 1000;
  static BOOL s_bIsNetworkOK = TRUE;
  static DWORD s_dwLastUpdate = 0;

  DWORD dwTick = ::GetTickCount();

  //����ʹ�ü���, �������֮��, ���Եõ�����.
  //����ʹ����static����, ����Ҫ���Ƕ��̵߳�����, ��s_dwLastUpdate�Ӻ����, �ܼ����õĻ�,һ�㶼�ǵ��̵߳�.
  if(s_bIsNetworkOK && (dwTick - s_dwLastUpdate < CACHE_TIME)) {
    return s_bIsNetworkOK;
  }

  ULONG uSize = 0;
  //����MSDN��ʾ��, ��ʹuSize = 0, pForwardTable����Ϊ0
  PMIB_IPFORWARDTABLE pForwardTable = (PMIB_IPFORWARDTABLE)malloc(sizeof(MIB_IPFORWARDTABLE));
  DWORD dwRtn = ::GetIpForwardTable(pForwardTable, &uSize, FALSE);
  if(dwRtn == ERROR_INSUFFICIENT_BUFFER) {
    free(pForwardTable);

    // ���·�ɱ���������50��, ����п�������IP���������, ���Ǽ���������������綼��ͨ��.
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
