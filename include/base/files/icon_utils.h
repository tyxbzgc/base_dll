/**
@defgroup Base_Misc_File File_文件杂项模块
@ingroup Base_Misc
*/
/*@{*/
/**
* @file IconUtils.h
* @brief 文件杂项模块导出函数
* @date 2013-10-10
* @version 1.0
*/
#pragma once

#include "CommonDef.h"


namespace Base
{
namespace FileMisc
{
	/**
	* @brief 加载图标
	* @param[in] uResId 图标资源ID
	* @param[in] cx 图标宽度
	* @param[in] cy 图标高度
	* @param[in] hResHandle 图标资源所在的模块句柄
	* @return 图标句柄
	*/
	HICON BASE_EXPORT LoadIconWithSize(UINT uResId, int cx, int cy, HINSTANCE hResHandle);
	
	/**
	* @brief RC资源图标ID的图标句柄
	* @param[in] bBigIcon 获取大图标句柄还是获取小图标句柄
	* @param[in] uResId   图标ID
	* @return 图标句柄
	*/
	HICON BASE_EXPORT GetIcon(BOOL bBigIcon, UINT uResId);

	///**
	//* @brief 获取指定PE文件的图标索引
	//* @param[in]  lpszFile 文件路径
	//* @param[out] pnIcon 图标索引
	//* @param[out] pnSelIcon 图标索引
	//*/
	//BOOL BASE_EXPORT GetFileIconIndex(LPCTSTR lpszFile, int* pnIcon, int* pnSelIcon);

	///**
	//* @brief 获取计算机的图标
	//* @param[in] hWnd 窗口句柄
	//* @param[in] bSmall 小图标还是大图标
	//* @return 计算机的图标
	//*/
	//int BASE_EXPORT GetMyComputerIconIndex(HWND hWnd, BOOL bSmall);

}//namespace FileMisc

}//namespace Base

/*@}*/
