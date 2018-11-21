/**
@defgroup Base_Misc_File File_�ļ�����ģ��
@ingroup Base_Misc
*/
/*@{*/
/**
* @file IconUtils.h
* @brief �ļ�����ģ�鵼������
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
	* @brief ����ͼ��
	* @param[in] uResId ͼ����ԴID
	* @param[in] cx ͼ����
	* @param[in] cy ͼ��߶�
	* @param[in] hResHandle ͼ����Դ���ڵ�ģ����
	* @return ͼ����
	*/
	HICON BASE_EXPORT LoadIconWithSize(UINT uResId, int cx, int cy, HINSTANCE hResHandle);
	
	/**
	* @brief RC��Դͼ��ID��ͼ����
	* @param[in] bBigIcon ��ȡ��ͼ�������ǻ�ȡСͼ����
	* @param[in] uResId   ͼ��ID
	* @return ͼ����
	*/
	HICON BASE_EXPORT GetIcon(BOOL bBigIcon, UINT uResId);

	///**
	//* @brief ��ȡָ��PE�ļ���ͼ������
	//* @param[in]  lpszFile �ļ�·��
	//* @param[out] pnIcon ͼ������
	//* @param[out] pnSelIcon ͼ������
	//*/
	//BOOL BASE_EXPORT GetFileIconIndex(LPCTSTR lpszFile, int* pnIcon, int* pnSelIcon);

	///**
	//* @brief ��ȡ�������ͼ��
	//* @param[in] hWnd ���ھ��
	//* @param[in] bSmall Сͼ�껹�Ǵ�ͼ��
	//* @return �������ͼ��
	//*/
	//int BASE_EXPORT GetMyComputerIconIndex(HWND hWnd, BOOL bSmall);

}//namespace FileMisc

}//namespace Base

/*@}*/
