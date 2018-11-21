// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SYSTEM_PROCESS_UTILS_H_
#define BASE_SYSTEM_PROCESS_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace sysmisc {
/**
* @name 进程_判断
* @{
*/

/**
* @brief 有几个进程
* @param[in]  szPro 进程名
* @retval 进程数
*/
int32 BASE_EXPORT GetProcessCount(const TCHAR* szPro);

/**
* @brief 进程是否存在
* @param[in]  szPro 进程名
* @retval TRUE 进程存在
* @retval FALSE 进程不存在或获取失败
*/
BOOL BASE_EXPORT CheckProcess(const TCHAR* szPro);

/**
* @brief 根据进程ID判断该进程是否存在
* @param[in] dwPid 进程ID
* @return 是否存在
* @note 未做提权处理
*/
BOOL BASE_EXPORT IsProcessExist(DWORD dwPid);

/**
* @brief 根据进程名来判断该进程是否正在运行
* @param[in] lpszProcName 进程名
* @return 是否存在
* @note 未做提权处理
*/
BOOL BASE_EXPORT IsProcessExist(LPCTSTR lpszProcName);

/**
* @name 进程_获取
* @{
*/
/**
* @brief 获取父进程的路径
* @param[in] dwPid 子进程ID
* @param[out] ptszProcessPath 父进程的路径
* @param[in] dwPathLen ptszProcessPath长度
* @param[out] pdwParentID 父进程ID
* @return 是否成功获取
*/
BOOL BASE_EXPORT GetParentProcessPath(DWORD dwPid, TCHAR* ptszProcessPath,
                                      DWORD dwPathLen, DWORD* pdwParentID = NULL);

/**
* @brief 根据进程ID获取进程文件名
* @param[in]  dwPid 进程ID
* @param[out] szProcessName 进程文件名
* @param[in]  dwSize szProcessName长度
* @return 是否成功获取
*/
BOOL BASE_EXPORT GetProcessName(DWORD dwPid, LPTSTR szProcessName,
                                DWORD dwSize);

/*@}*/
/**
* @name 进程_修改
* @{
*/
/**
* @brief 修改进程权限
* @param[in] hProcess 进程句柄
* @param[in] szPri 权限名
* @param[in] bEnable 是否使之有效
* @return 是否修改进程权限成功
*/
BOOL BASE_EXPORT EnableProcessPrivilege(HANDLE hProcess, LPTSTR szPri,
                                        BOOL bEnable = TRUE);

/**
* @brief 把当前进程权限改为SE_DEBUG_NAME权限
* @param[in] bEnable 是否使修改权限有效
* @return 是否成功修改权限
*/
BOOL BASE_EXPORT EnableDebugPrivilege(BOOL bEnable = TRUE);

/**
* @brief 取消系统对本进程的DEP检测
* @return 是否成功
* @note move from "CommonFunc.h"
* @remarks 使用这个函数需要满足条件：
# 操作系统是vista sp1、winxp sp3、win2k8
# 操作系统DEP的模式设置为OPTIN OPTOUT，如果是AlwaysON就没有办法了
*/
BOOL BASE_EXPORT SetDEP();

/**
* @brief 调用进程是否属于管理员组（if the caller's process is a member of the Administrators local group）
* @retval TRUE Caller has Administrators local group
* @retval FALSE Caller does not have Administrators local group
* @note move from "CommonFunc.h"
* @remarks Caller is NOT expected to be impersonating anyone and is
expected to be able to open its own process and process token.
*/
BOOL BASE_EXPORT IsUserAdmin(VOID);

/**
* @brief 以管理员身份运行
* @param[in] exe exe文件路径
* @param[in] params 传给exe文件的运行参数
* @param[out] exit_code 返回退出代码
* @return 是否成功
* @note move from "CommonFunc.h"
*/
BOOL BASE_EXPORT ExecuteExeAsAdmin(LPCWSTR exe, LPCWSTR params,
                                   DWORD* exit_code);

}  // namespace sysmisc
}  // namespace base
#endif  // BASE_SYSTEM_PROCESS_UTILS_H_
