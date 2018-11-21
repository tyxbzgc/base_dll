// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_FILE_ATTR_UTILS_H_
#define BASE_FILES_FILE_ATTR_UTILS_H_

#include <Shlwapi.h>
#include "base/base_export.h"

/**
* @cond Doxygen忽略该段代码
*/
/**
* @struct LANGANDCODEPAGE
* @brief 系统语言信息
*/
typedef struct BASE_EXPORT tagLANGANDCODEPAGE {
WORD wLanguage;
WORD wCodePage;
} LANGANDCODEPAGE;
/**
* @endcond
*/

namespace base {
/**
* @brief 文件杂项模块采用的二级命名空间
*/
namespace filemisc {
  /**
  * @name 判断
  * @{
  */
  /**
  * @brief 判断文件是否存在
  * @param[in] szFile 文件路径
  * @return 是否存在
  */
  BOOL BASE_EXPORT IsFileExist(LPCTSTR szFile);

  /**
  * @brief 判断目录是否存在
  * @param[in] szDir 目录路径
  * @return 是否存在指定目录
  */
  BOOL BASE_EXPORT IsDirectoryExist(LPCTSTR szDir);

  /**
  * @brief 判断指定文件是否可写，如果是只读了，会尝试修改属性改为可读可写。
  * @param[in] szFile 文件路径
  * @retval FALSE 表示可写
  * @retval TRUE 表示不可写
  */
  BOOL BASE_EXPORT IsFileChangable(LPCTSTR szFile);

  /**
  * @brief 判断lpszFile 是否是pe文件
  * @param[in] lpszFile 文件路径
  * @return 是否是pe文件
  */
  BOOL BASE_EXPORT IsPEFile(LPCTSTR lpszFile);

  /**
  * @brief 判断文件是否被占用
  * @param[in] pszPath 文件路径
  * @return 是否被占用
  */
  BOOL BASE_EXPORT IsFileUsing(LPCTSTR pszPath);

  /**
  * @brief 传入路径是否是文件
  * @param[in] szFilePath 路径
  * @param[out] bFile 返回是否是文件，若路径不存在bFile也返回FALSE
  * @return 传入的路径是否存在
  */
  BOOL BASE_EXPORT IsPathFile(LPCTSTR szFilePath, BOOL &bFile);

  /**
  * @brief 传入路径是否是文件夹
  * @param[in] szFilePath 路径
  * @param[out] bDir 返回是否是文件夹，若路径不存在bDir也返回FALSE
  * @return 传入的路径是否存在
  */
  BOOL BASE_EXPORT IsPathDir(LPCTSTR szFilePath, BOOL &bDir);

  /*@}*/



  /**
  * @name 文件信息
  * @{
  */

  ///**
  //* @brief 得到文件信息
  //* @param[in]  szFile 文件路径
  //* @param[out] lpszCompany  发行公司名
  //* @param[out] lpszFileDesc 文件描述
  //* @param[out] lpszVersion  版本信息
  //* @是否成功获得
  //* @传出参数类型为wstring，跨dll调用存在风险，
  //   建议使用BOOL GetFileInfo(LPCTSTR szFile, TCHAR* lpszCompany, TCHAR* lpszFileDesc, TCHAR* lpszVersion);
  //*/
  //BOOL BASE_EXPORT GetFileInfo(
  //	LPCTSTR szFile,
  //	std::wstring* lpstrCompany,
  //	std::wstring* lpstrFileDesc,
  //	std::wstring* lpstrVersion );

  /**
  * @brief 得到文件信息
  * @param[in]  szFile 文件路径
  * @param[out] lpszCompany  发行公司名
  * @param[out] lpszFileDesc 文件描述
  * @param[out] lpszVersion  版本信息
  * @是否成功获得
  */
  BOOL BASE_EXPORT GetFileInfo(
    LPCTSTR szFile,
    TCHAR* lpstrCompany,
    TCHAR* lpstrFileDesc,
    TCHAR* lpstrVersion);

  /**
  * @brief 得到文件的创建时间、最后访问时间和最后修改时间
  * @param[in] szFile 文件路径
  * @param[out] lpCreationTime   创建时间
  * @param[out] lpLastAccessTime 最后访问时间
  * @param[out] lpLastWriteTime  最后修改时间
  * @return 是否成功获取文件的时间
  */
  BOOL BASE_EXPORT GetFileTimeFlex(
    LPCTSTR szFile,
    LPFILETIME lpCreationTime,
    LPFILETIME lpLastAccessTime,
    LPFILETIME lpLastWriteTime
  );

  /**
  * @brief 得到pe文件版本号信息
  * @param[out] pnMajor 大版本号指针
  * @param[out] pnMinor 小版本号指针
  * @param[out] pnBuild 编译版本号
  * @param[out] pnLast  版本尾数
  * @param[in] pchPeFile 文件路径
  * @return 是否获得成功；如果pe文件无版本号，也返回FALSE
  * @note 当pchPeFile = NULL 时获取的是当前进程的版本信息
  */
  BOOL BASE_EXPORT GetPEVersion(
    unsigned int* pnMajor,
    unsigned int* pnMinor,
    unsigned int* pnBuild,
    unsigned int* pnLast,
    const TCHAR* pchPeFile = NULL
  );
  /**
  * @brief 得到文件版本号信息
  * @param[in] lpszFileName 文件路径
  * @param[out] wMajorVersion 大版本号指针
  * @param[out] wMinorVersion 小版本号指针
  * @param[out] wRevisionVersion 修订版本号
  * @param[out] wBuildVersion  编译版本号
  * @return 是否获得成功；如果文件无版本号，也返回FALSE
  */
  BOOL BASE_EXPORT GetFileVersion(
    LPCWSTR lpszFileName,
    unsigned int& wMajorVersion,
    unsigned int& wMinorVersion,
    unsigned int& wRevisionVersion,
    unsigned int& wBuildVersion);
  /**
  * @brief 获取文件信息
  * @param[in] szFile 文件路径
  * @param[in] szFileInfoName 文件信息名
  * @param[out] wchFileInfo 存储获取的文件信息的缓冲区
  * @param[in][out]  nSize  缓冲区 wchFileInfo 的长度，若长度不足，会返回所需长度
  * @return 是否获取成功
  */
  BOOL BASE_EXPORT GetFileSingleInfo(LPCTSTR szFile, LPCTSTR szFileInfoName, WCHAR*wchFileInfo, int* pnSize);


  /**
  * @brief 得到指定PE文件的版本
  * @param[out] pchBuf 存储版本信息字符串的缓冲区
  * @param[out] nSize 缓冲区pchBuf的长度
  * @param[in] pchPeFile pe文件的路径
  * @return 是否获得成功；如果pe文件无版本号，也返回FALSE
  * @note 当pchPeFile = NULL 时获取的是当前进程的版本信息；版本信息字符串形如"1.0.4.0"
  */
  BOOL BASE_EXPORT GetCurPEVersionA(char* pchBuf, int nSize, const TCHAR* pchPeFile = NULL);

  /**
  * @brief 得到文件版本号信息
  * @param[out] pnMajor 大版本号
  * @param[out] pnMinor 小版本号
  * @param[out] pnBuild 编译版本号
  * @param[out] pnLast  版本尾数
  * @param[in] pchVersion 文件版本信息字符串，形如"1.0.4.0"
  */
  BOOL BASE_EXPORT GetNumberVerFromStringVer(
    unsigned int* pnMajor,
    unsigned int* pnMinor,
    unsigned int* pnBuild,
    unsigned int* pnLast,
    const TCHAR* pchVersion
  );


  /**
  * @brief 获得文件版本信息
  * @param[in] lpszFile 文件路径
  * @param[out] pchVerion 版本号字符串
  * @param[in] nSize 存储版本号字符串的buffer长度
  * @return 是否成功
  */
  BOOL BASE_EXPORT GetFileVerInfoA(LPCSTR lpszFile, char* pchVersion, int nSize);
  /**
  * @brief 获得文件版本信息
  * @param[in] lpszFile 文件路径
  * @param[out] pchVerion 版本号字符串
  * @param[in] nSize 存储版本号字符串的buffer长度
  * @return 是否成功
  * @see GetFileVerInfoA
  */
  BOOL BASE_EXPORT GetFileVerInfoW(LPCWSTR lpszFile, wchar_t* pchVersion, int nSize);

#if (defined(_UNICODE) || defined(UNICODE))
#define GetFileVerInfo GetFileVerInfoW
#else
#define GetFileVerInfo GetFileVerInfoA
#endif


  /**
  * @brief 获取dll的版本号
  * @param[in] pchDllName dll的路径
  * @param[out] DllVersion 返回dll的版本信息
  * @see http://blog.csdn.net/kvs112219/article/details/6070676
  * @return 是否成功获取
  */
  BOOL BASE_EXPORT DllGetVersion(LPCTSTR pchDllName, DLLVERSIONINFO& DllVersion);

  /*@}*/



  /**
  * @name 大小
  * @{
  */

  /**
  * @brief 得到文件大小
  * @param[in] lpszFile 文件路径
  * @return 返回文件大小
  */
  unsigned long BASE_EXPORT GetFileSizeEx(LPCTSTR lpszFile);

  /**
  * @brief 得到文件夹大小
  * @param[in]  tszPath     文件夹路径
  * @param[out] pdwFileCnt  包含的文件数量
  * @param[out] pdwFolderCnt子文件夹数量
  * @return 返回该指定路径下文件总大小
  * @note 该函数会计算所有嵌套的子文件夹和文件数
  * @code
  * DWORD64 dwSize    = 0;
  * DWORD dwFileCnt   = 0;
  * DWORD dwFolderCnt = 0;
  *
  * // 如果需要计算子文件夹数量和文件数量数
  * dwSize = FileMisc::GetFolderSize(_T("d:\\TMP\\metadata"), &dwFileCnt, &dwFolderCnt);
  *
  * // 如果不需要计算子文件夹和文件数量
  * dwSize = FileMisc::GetFolderSize(_T("d:\\TMP\\metadata"), NULL, NULL);
  *
  * @endcode
  */
  DWORD64 BASE_EXPORT GetFolderSize(LPCTSTR tszPath, DWORD* pdwFileCnt, DWORD* pdwFolderCnt);

  /*@}*/


  /**
  * @name 创建
  * @{
  */
  /**
  * @brief 创建嵌套文件夹
  * @param[in] lpszDir 嵌套文件夹路径
  * @return 是否创建成功
  * @code
  * BOOL ret = CreateDirectoryNested(_T("C:\\Users\\Administrator\\Desktop\\1\\2\\3\\4.txt"));
  * 在桌面创建嵌套文件夹\1\2\3\4.txt\，注意4.txt是文件夹名称而不是一个文件
  * @endcode
  */
  BOOL BASE_EXPORT CreateDirectoryNested(LPCTSTR lpszDir);

}  // namespace filemisc
}  // namespace base

#endif  // BASE_FILES_FILE_ATTR_UTILS_H_