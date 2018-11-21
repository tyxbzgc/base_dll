// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SYSTEM_SYS_ATTR_UTILS_H_
#define BASE_SYSTEM_SYS_ATTR_UTILS_H_

#include "base/base_export.h"

/** 
* @enum WinVer
* @brief 定义Windows 版本枚举 
*/
typedef enum WinVer
{
Ver_Sp0 = 0x0000,	///< no Service Pack
Ver_Sp1 = 0x0001,	///< Service Pack 1
Ver_Sp2 = 0x0002,	///< Service Pack 2
Ver_Sp3 = 0x0003,	///< Service Pack 3
Ver_Sp4 = 0x0004,	///< Service Pack 4

Ver_Win32 = 0x0000, ///< windows 32bits
Ver_Win95,			///< windows 95 system
Ver_Win98,			///< windows 98 system
Ver_WinMe,			///< windows Me system
Ver_WinNt,			///< windows NT system
Ver_Win2k,			///< windows 2000 system
Ver_WinXp,			///< windows XP system
Ver_Win2003,		///< windows server 2003 system
Ver_WinVista,		///< windows Vista system
//Ver_Win2008,
Ver_Win7,			///< windows 7 system
Ver_Win8,			/// windows 8 operating system
Ver_Last,
};


//hyc:move from "CommonFunc.h"
#define WINDOWS_OLDEST				long(-1000)
#define WINDOWS_2000				long(1)
#define WINDOWS_XP_NO_SP			long(2)
#define WINDOWS_XP_SP_1				long(3)
#define WINDOWS_XP_SP_2				long(4)
#define WINDOWS_XP_SP_3				long(5)
#define WINDOWS_2003				long(6)
#define WINDOWS_VISTA_NO_SP			long(7)
#define WINDOWS_VISTA_SP_1			long(8)
#define WINDOWS_VISTA_SP_2			long(9)
#define WINDOWS_7					long(10)
#define WINDOWS_8                   long(11)
#define WINDOWS_LATEST				long(1000)


namespace Base
{
namespace SysMisc
{
/**
* @name 系统属性
* @{
*/
/*
* @brief retrieves the NetBIOS name of the local computer 
* @return ComputerName
* @note 因返回类型为std::wstring，跨DLL调用存在风险，故不设为导出函数，请使用 GetComputerName(LPWSTR pName, DWORD dwLen)
*/
std::wstring GetComputerName();
/**
* @brief retrieves the NetBIOS name of the local computer 
* @param[out] pName 存储ComputerName的缓冲区
* @param[in] dwLen 缓冲区长度
* @return succeed or failed
*/
BOOL BASE_EXPORT GetComputerName(LPWSTR pName, DWORD dwLen);

/**
* @brief 获取机器配置高低，要在网络联通后才有效，否则用前一次查询的结果
* @param[out] ucSysl   机器配置，0为高配，1为低配
* @param[out] ucCpul   cpu性能，0为高配，1为低配
* @param[out] ucMeml   内存大小，0为高配，1为低配
* @return 是否成功获取
* @note 这个函数暂没实现
*/
BOOL BASE_EXPORT GetSystemLevel(BYTE* ucSysl,BYTE *ucCpul,BYTE* ucMeml,BYTE* reserved=NULL);

/**
* @brief 得到当前Windows系统版本
* @param[out] dwVersion WinVer枚举值
* @param[out] chWinVer 系统版本描述
* @param[in]  nBuffLen chWinVer长度
* @return 是否成功获取
* @code
* DWORD dwVersion = 0;
* TCHAR szWinVer[MAX_PATH] = {0};
*
* SysMisc::GetWindowsVersion(dwVersion, szWinVer);
* // dwVersion 等于 0x00030006
* // szWinVer  等于 Windows XP Service Pack 3
* // 上述值是当前测试系统获取的
* @endcode
*/ 
BOOL BASE_EXPORT GetWindowsVersion(DWORD& dwVersion, TCHAR* chWinVer, int nBuffLen = MAX_PATH);

/*
* @brief 获取计算机用户名 
* @return 计算机用户名 
* @note 因返回类型为std::wstring，跨DLL调用存在风险，故不设为导出函数，请使用 GetUserName(LPWSTR pUserName, DWORD dwLen)
*/
std::wstring GetUserName();

/**
* @brief 获取计算机用户名 
* @param[out] pUserName 存储计算机用户名的缓冲区
* @param[in] dwLen 缓冲区长度
* @return 是否成功
*/
BOOL BASE_EXPORT GetUserName(LPWSTR pUserName, DWORD dwLen);

/**
* @brief 检测系统是否需要提权
* @return 是否需要提权
*/
BOOL BASE_EXPORT IsOSNeedElevate();


/** 
* @brief 获取所有访问安全属性 
* @return 返回SECURITY_ATTRIBUTES结构
*/
const SECURITY_ATTRIBUTES BASE_EXPORT * GetAllAccessSecurityAttributes();
/** 
* @brief 获取所有访问安全属性 
* @param[out] SecurityAttributes 安全属性
* @param[out] SecurityDescriptor 安全描述符
* @return 是否获取成功
*/
BOOL BASE_EXPORT GetAllAccessSecurityAttributesEx(SECURITY_ATTRIBUTES& SecurityAttributes, SECURITY_DESCRIPTOR& SecurityDescriptor);
	
/**
* @brief 获取当前计算机处理器的多少
* @return 处理器的数目
*/
DWORD BASE_EXPORT GetProcessorsCount();

/**
* @brief 获取当前操作系统版本
* @param[out] uiMajorVersion 操作系统Major版本
* @param[out] uiMinorVersion 操作系统Minor版本
* @return 是否成功
*/
BOOL BASE_EXPORT GetOSVersion( unsigned int& uiMajorVersion, unsigned int& uiMinorVersion);

//hyc:move from "CommonFunc.h"

/**
* @brief 获取当前操作系统版本
* @return 返回当前OS版本的宏，见上面的一系列#define 
*/
long BASE_EXPORT GetOSVersion() ;
/**
* @brief 获取当前操作系统版本
* @param[out] uiMajorVersion 操作系统Major版本
* @param[out] uiMinorVersion 操作系统Minor版本
* @return 返回当前OS版本的宏，见上面的一系列#define 
*/
long BASE_EXPORT GetOSVersionEx(DWORD &dwOSMajorVersion, DWORD &dwOSMinorVersion) ;


/**
* @brief 获取磁盘容量信息
* @param[in] lpDirectoryName 可直接传入文件路径如"E:\\code\\Brwoser"，或磁盘名如"E:"，注意一定要有冒号！
* @param[out] pFreeBytesAvailable 返回用户在该磁盘的可用空间, 单位：字节数
* @param[out] pTotalNumberOfBytes 返回用户在该磁盘拥有的总空间, 单位：字节数
* @param[out] pTotalNumberOfFreeBytes 返回该磁盘所有用户的可用空间, 单位：字节数
* @return 是否成功
* @note 参数nFreeBytesAvailable和nTotalNumberOfBytes在启用了用户配额限制的系统上会因用户配额的大小而改变，
	  在没有启用户配额限制的系统上nFreeBytesAvailable和nTotalNumberOfFreeBytes返回的值是相同的。
* @remarks 三个输出参数中某个参数传入NULL则不返回该参数
*/
BOOL GetDiskSize(LPCTSTR lpDirectoryName, uint64* pFreeBytesAvailable, uint64* pTotalNumberOfBytes, uint64* pTotalNumberOfFreeBytes);
}  // namespace sysmisc
}  // namespace base
#endif  // BASE_SYSTEM_SYS_ATTR_UTILS_H_
