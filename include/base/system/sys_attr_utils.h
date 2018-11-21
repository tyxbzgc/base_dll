// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SYSTEM_SYS_ATTR_UTILS_H_
#define BASE_SYSTEM_SYS_ATTR_UTILS_H_

#include "base/base_export.h"

/** 
* @enum WinVer
* @brief ����Windows �汾ö�� 
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
* @name ϵͳ����
* @{
*/
/*
* @brief retrieves the NetBIOS name of the local computer 
* @return ComputerName
* @note �򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ��ʲ���Ϊ������������ʹ�� GetComputerName(LPWSTR pName, DWORD dwLen)
*/
std::wstring GetComputerName();
/**
* @brief retrieves the NetBIOS name of the local computer 
* @param[out] pName �洢ComputerName�Ļ�����
* @param[in] dwLen ����������
* @return succeed or failed
*/
BOOL BASE_EXPORT GetComputerName(LPWSTR pName, DWORD dwLen);

/**
* @brief ��ȡ�������øߵͣ�Ҫ��������ͨ�����Ч��������ǰһ�β�ѯ�Ľ��
* @param[out] ucSysl   �������ã�0Ϊ���䣬1Ϊ����
* @param[out] ucCpul   cpu���ܣ�0Ϊ���䣬1Ϊ����
* @param[out] ucMeml   �ڴ��С��0Ϊ���䣬1Ϊ����
* @return �Ƿ�ɹ���ȡ
* @note ���������ûʵ��
*/
BOOL BASE_EXPORT GetSystemLevel(BYTE* ucSysl,BYTE *ucCpul,BYTE* ucMeml,BYTE* reserved=NULL);

/**
* @brief �õ���ǰWindowsϵͳ�汾
* @param[out] dwVersion WinVerö��ֵ
* @param[out] chWinVer ϵͳ�汾����
* @param[in]  nBuffLen chWinVer����
* @return �Ƿ�ɹ���ȡ
* @code
* DWORD dwVersion = 0;
* TCHAR szWinVer[MAX_PATH] = {0};
*
* SysMisc::GetWindowsVersion(dwVersion, szWinVer);
* // dwVersion ���� 0x00030006
* // szWinVer  ���� Windows XP Service Pack 3
* // ����ֵ�ǵ�ǰ����ϵͳ��ȡ��
* @endcode
*/ 
BOOL BASE_EXPORT GetWindowsVersion(DWORD& dwVersion, TCHAR* chWinVer, int nBuffLen = MAX_PATH);

/*
* @brief ��ȡ������û��� 
* @return ������û��� 
* @note �򷵻�����Ϊstd::wstring����DLL���ô��ڷ��գ��ʲ���Ϊ������������ʹ�� GetUserName(LPWSTR pUserName, DWORD dwLen)
*/
std::wstring GetUserName();

/**
* @brief ��ȡ������û��� 
* @param[out] pUserName �洢������û����Ļ�����
* @param[in] dwLen ����������
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT GetUserName(LPWSTR pUserName, DWORD dwLen);

/**
* @brief ���ϵͳ�Ƿ���Ҫ��Ȩ
* @return �Ƿ���Ҫ��Ȩ
*/
BOOL BASE_EXPORT IsOSNeedElevate();


/** 
* @brief ��ȡ���з��ʰ�ȫ���� 
* @return ����SECURITY_ATTRIBUTES�ṹ
*/
const SECURITY_ATTRIBUTES BASE_EXPORT * GetAllAccessSecurityAttributes();
/** 
* @brief ��ȡ���з��ʰ�ȫ���� 
* @param[out] SecurityAttributes ��ȫ����
* @param[out] SecurityDescriptor ��ȫ������
* @return �Ƿ��ȡ�ɹ�
*/
BOOL BASE_EXPORT GetAllAccessSecurityAttributesEx(SECURITY_ATTRIBUTES& SecurityAttributes, SECURITY_DESCRIPTOR& SecurityDescriptor);
	
/**
* @brief ��ȡ��ǰ������������Ķ���
* @return ����������Ŀ
*/
DWORD BASE_EXPORT GetProcessorsCount();

/**
* @brief ��ȡ��ǰ����ϵͳ�汾
* @param[out] uiMajorVersion ����ϵͳMajor�汾
* @param[out] uiMinorVersion ����ϵͳMinor�汾
* @return �Ƿ�ɹ�
*/
BOOL BASE_EXPORT GetOSVersion( unsigned int& uiMajorVersion, unsigned int& uiMinorVersion);

//hyc:move from "CommonFunc.h"

/**
* @brief ��ȡ��ǰ����ϵͳ�汾
* @return ���ص�ǰOS�汾�ĺ꣬�������һϵ��#define 
*/
long BASE_EXPORT GetOSVersion() ;
/**
* @brief ��ȡ��ǰ����ϵͳ�汾
* @param[out] uiMajorVersion ����ϵͳMajor�汾
* @param[out] uiMinorVersion ����ϵͳMinor�汾
* @return ���ص�ǰOS�汾�ĺ꣬�������һϵ��#define 
*/
long BASE_EXPORT GetOSVersionEx(DWORD &dwOSMajorVersion, DWORD &dwOSMinorVersion) ;


/**
* @brief ��ȡ����������Ϣ
* @param[in] lpDirectoryName ��ֱ�Ӵ����ļ�·����"E:\\code\\Brwoser"�����������"E:"��ע��һ��Ҫ��ð�ţ�
* @param[out] pFreeBytesAvailable �����û��ڸô��̵Ŀ��ÿռ�, ��λ���ֽ���
* @param[out] pTotalNumberOfBytes �����û��ڸô���ӵ�е��ܿռ�, ��λ���ֽ���
* @param[out] pTotalNumberOfFreeBytes ���ظô��������û��Ŀ��ÿռ�, ��λ���ֽ���
* @return �Ƿ�ɹ�
* @note ����nFreeBytesAvailable��nTotalNumberOfBytes���������û�������Ƶ�ϵͳ�ϻ����û����Ĵ�С���ı䣬
	  ��û�����û�������Ƶ�ϵͳ��nFreeBytesAvailable��nTotalNumberOfFreeBytes���ص�ֵ����ͬ�ġ�
* @remarks �������������ĳ����������NULL�򲻷��ظò���
*/
BOOL GetDiskSize(LPCTSTR lpDirectoryName, uint64* pFreeBytesAvailable, uint64* pTotalNumberOfBytes, uint64* pTotalNumberOfFreeBytes);
}  // namespace sysmisc
}  // namespace base
#endif  // BASE_SYSTEM_SYS_ATTR_UTILS_H_
