// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SYSTEM_PROCESS_UTILS_H_
#define BASE_SYSTEM_PROCESS_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace sysmisc {
/**
* @name ����_�ж�
* @{
*/

/**
* @brief �м�������
* @param[in]  szPro ������
* @retval ������
*/
int32 BASE_EXPORT GetProcessCount(const TCHAR* szPro);

/**
* @brief �����Ƿ����
* @param[in]  szPro ������
* @retval TRUE ���̴���
* @retval FALSE ���̲����ڻ��ȡʧ��
*/
BOOL BASE_EXPORT CheckProcess(const TCHAR* szPro);

/**
* @brief ���ݽ���ID�жϸý����Ƿ����
* @param[in] dwPid ����ID
* @return �Ƿ����
* @note δ����Ȩ����
*/
BOOL BASE_EXPORT IsProcessExist(DWORD dwPid);

/**
* @brief ���ݽ��������жϸý����Ƿ���������
* @param[in] lpszProcName ������
* @return �Ƿ����
* @note δ����Ȩ����
*/
BOOL BASE_EXPORT IsProcessExist(LPCTSTR lpszProcName);

/**
* @name ����_��ȡ
* @{
*/
/**
* @brief ��ȡ�����̵�·��
* @param[in] dwPid �ӽ���ID
* @param[out] ptszProcessPath �����̵�·��
* @param[in] dwPathLen ptszProcessPath����
* @param[out] pdwParentID ������ID
* @return �Ƿ�ɹ���ȡ
*/
BOOL BASE_EXPORT GetParentProcessPath(DWORD dwPid, TCHAR* ptszProcessPath,
                                      DWORD dwPathLen, DWORD* pdwParentID = NULL);

/**
* @brief ���ݽ���ID��ȡ�����ļ���
* @param[in]  dwPid ����ID
* @param[out] szProcessName �����ļ���
* @param[in]  dwSize szProcessName����
* @return �Ƿ�ɹ���ȡ
*/
BOOL BASE_EXPORT GetProcessName(DWORD dwPid, LPTSTR szProcessName,
                                DWORD dwSize);

/*@}*/
/**
* @name ����_�޸�
* @{
*/
/**
* @brief �޸Ľ���Ȩ��
* @param[in] hProcess ���̾��
* @param[in] szPri Ȩ����
* @param[in] bEnable �Ƿ�ʹ֮��Ч
* @return �Ƿ��޸Ľ���Ȩ�޳ɹ�
*/
BOOL BASE_EXPORT EnableProcessPrivilege(HANDLE hProcess, LPTSTR szPri,
                                        BOOL bEnable = TRUE);

/**
* @brief �ѵ�ǰ����Ȩ�޸�ΪSE_DEBUG_NAMEȨ��
* @param[in] bEnable �Ƿ�ʹ�޸�Ȩ����Ч
* @return �Ƿ�ɹ��޸�Ȩ��
*/
BOOL BASE_EXPORT EnableDebugPrivilege(BOOL bEnable = TRUE);

/**
* @brief ȡ��ϵͳ�Ա����̵�DEP���
* @return �Ƿ�ɹ�
* @note move from "CommonFunc.h"
* @remarks ʹ�����������Ҫ����������
# ����ϵͳ��vista sp1��winxp sp3��win2k8
# ����ϵͳDEP��ģʽ����ΪOPTIN OPTOUT�������AlwaysON��û�а취��
*/
BOOL BASE_EXPORT SetDEP();

/**
* @brief ���ý����Ƿ����ڹ���Ա�飨if the caller's process is a member of the Administrators local group��
* @retval TRUE Caller has Administrators local group
* @retval FALSE Caller does not have Administrators local group
* @note move from "CommonFunc.h"
* @remarks Caller is NOT expected to be impersonating anyone and is
expected to be able to open its own process and process token.
*/
BOOL BASE_EXPORT IsUserAdmin(VOID);

/**
* @brief �Թ���Ա�������
* @param[in] exe exe�ļ�·��
* @param[in] params ����exe�ļ������в���
* @param[out] exit_code �����˳�����
* @return �Ƿ�ɹ�
* @note move from "CommonFunc.h"
*/
BOOL BASE_EXPORT ExecuteExeAsAdmin(LPCWSTR exe, LPCWSTR params,
                                   DWORD* exit_code);

}  // namespace sysmisc
}  // namespace base
#endif  // BASE_SYSTEM_PROCESS_UTILS_H_
