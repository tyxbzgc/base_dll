/**
@defgroup Base_Misc_File File_文件杂项模块
@ingroup Base_Misc
*/
/*@{*/
/**
* @file ShortCutUtils.h
* @brief 文件杂项模块导出函数
* @date 2013-10-10
* @version 1.0
*/
#pragma once
#include "ShObjIdl.h"
#include "CommonDef.h"

namespace Base
{

namespace FileMisc
{
	/**
	* @brief 创建快捷方式
	* @param[in] pszLnkFile 快捷方式文件
	* @param[in] lpszDest 快捷方式对应的目标文件
	* @param[in] lpszWorkDir 起始位置
	* @param[in] lpszDescript 快捷方式的描述
	* @param[in] lpszCmdlineArguments 快捷方式命令行参数
	* @param[in] lpszIconFile 图标文件
	* @param[in] nIconIdx 图标索引
	* @param[in] wHotKey 快捷键
	* @param[in] iShowCmd 风格
	* @return 是否成功
	* @note 需要调用CoInitialize和CoUninitialize
	*/
	BOOL BASE_EXPORT CreateShortCut(
		LPCTSTR pszLnkFile, //快捷方式文件
		LPCTSTR lpszDest, //快捷方式对应的目标文件
		LPCTSTR lpszWorkDir = NULL,//起始位置
		LPCTSTR lpszDescript = NULL,//快捷方式的描述
		LPCTSTR lpszCmdlineArguments = NULL,//快捷方式命令行参数
		LPCTSTR lpszIconFile = NULL,	//图标文件
		const int nIconIdx = 0,			//图标索引
		const WORD wHotKey = 0,			//快捷键
		const int iShowCmd = SW_SHOWNORMAL);//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)之一

	/**
	* @brief 修改快捷方式的属性
	* @param[in] pszLnkFile 快捷方式名称
	* @param[in] pszSaveAsLnkFile 把快捷方式另存为的文件,如果为NULL,覆盖原来文件
	* @param[in] lpszDest 快捷方式对应的目标文件
	* @param[in] lpszWorkDir 起始位置
	* @param[in] lpszDescript 快捷方式的描述
	* @param[in] lpszCmdlineArguments 快捷方式命令行参数
	* @param[in] lpszIconFile 图标文件
	* @param[in] nIconIdx 图标索引
	* @param[in] wHotKey 快捷键
	* @param[in] iShowCmd 风格
	* @return 是否成功
	* @note 需要调用CoInitialize和CoUninitialize
	*/
	BOOL BASE_EXPORT ModifyShortCut(
		LPCTSTR pszLnkFile,	//快捷方式名称
		LPCTSTR pszSaveAsLnkFile = NULL,	//把快捷方式另存为的文件,如果为NULL,覆盖原来文件
		LPCTSTR lpszDest = NULL, //快捷方式对应的目标文件
		LPCTSTR lpszWorkDir = NULL,//起始位置
		LPCTSTR lpszDescript = NULL,//快捷方式的描述
		LPCTSTR lpszCmdlineArguments = NULL,//快捷方式命令行参数
		LPCTSTR lpszIconFile = NULL,	//图标文件
		const int nIconIdx = 0,			//图标索引
		const WORD wHotKey = 0,			//快捷键
		const int iShowCmd = SW_SHOWNORMAL);//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)之一

	/**
	* @brief 获取快捷方式信息
	* @param[in] pszLnkFile 快捷方式名称
	* @param[in] lpszDest 快捷方式对应的目标文件
	* @param[in] lpszWorkDir 起始位置
	* @param[in] lpszDescript 快捷方式的描述
	* @param[in] lpszCmdlineArguments 快捷方式命令行参数
	* @param[in] lpszIconFile 图标文件
	* @param[in] nIconIdx 图标索引
	* @param[in] wHotKey 快捷键
	* @param[in] iShowCmd 风格
	* @return 是否成功
	* @note 需要调用CoInitialize和CoUninitialize
	*/
	BOOL BASE_EXPORT GetShortCutInfo(
		LPCTSTR pszLnkFile = NULL,	//快捷方式文件
		LPTSTR lpszDest = NULL, //快捷方式对应的目标文件
		LPTSTR lpszWorkDir = NULL,//起始位置
		LPTSTR lpszDescript = NULL,//快捷方式的描述
		LPTSTR lpszCmdlineArguments = NULL,//快捷方式命令行参数
		LPTSTR lpszIconFile = NULL,	//图标文件
		int* nIconIdx = NULL,			//图标索引
		WORD* wHotKey = NULL,			//快捷键
		int* iShowCmd = NULL);//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)之一
}//namespace FileMisc

namespace FileMisc
{
	/** 
	* @class IShortcut
	* @detail 如果是创建快捷方式，则不需要调用Load
	*  设置各项参数，然后调用Save（要制定路径）
	*/
	class BASE_EXPORT IShortcut
	{
	public:
		virtual BOOL Load(LPCTSTR pShortcutFile) = 0;
		virtual BOOL Save(LPCTSTR pchFileName = NULL, BOOL fRemember = TRUE) = 0;

		virtual BOOL SaveCompleted(LPCTSTR pchFileName) = 0;
		virtual BOOL GetCurFile(LPTSTR pchFileName, int cchMax) = 0;

		virtual BOOL IsChange() = 0;

		virtual BOOL SetAutoSave(BOOL bAutoSave = TRUE) = 0;

	public:
		virtual BOOL GetPath(LPTSTR pchFile, int cchMaxPath,
			WIN32_FIND_DATA* pfd = NULL, DWORD fFlags = 0l) = 0;

		/**
		* @name ID List
		* @{
		*/
		virtual BOOL GetList(LPITEMIDLIST* ppidl) = 0;
		virtual BOOL SetList(LPCITEMIDLIST pidl) = 0;
		/*@}*/

		/**
		* @name Description
		* @{
		*/
		virtual BOOL GetDesc(LPTSTR pchDesc, int cchMaxDesc) = 0;
		virtual BOOL SetDesc(LPCTSTR pchDesc) = 0;
		/*@}*/

		/**
		* @name WorkingDirectory
		* @{
		*/
		virtual BOOL GetDir(LPTSTR pchDir, int cchMaxPath) = 0;
		virtual BOOL SetDir(LPCTSTR pchDir) = 0;
		/*@}*/

		/**
		* @name Arguments
		* @{
		*/
		virtual BOOL GetArgs(LPTSTR pchArgs, int cchMaxArgs) = 0;
		virtual BOOL SetArgs(LPCTSTR pchArgs) = 0;
		/*@}*/

		/**
		* @name HotKey
		* @{
		*/
		virtual BOOL GetHotKey(WORD* pwHotKey) = 0;
		virtual BOOL SetHotKey(WORD wHotKey) = 0;
		/*@}*/

		/**
		* @name ShowCommand 
		* @note SW_SHOWNORMAL SW_SHOWMAXIMIZED SW_SHOWMINNOACTIVE
		* @{
		*/
		virtual BOOL GetShow(int* piShowCmd) = 0;
		virtual BOOL SetShow(int iShowCmd) = 0;
		/*@}*/

		/**
		* @name Ico Location
		* @{
		*/
		virtual BOOL GetIco(LPTSTR pchIcoPath, int cchIcoPath, int* piIco) = 0;
		virtual BOOL SetIco(LPCTSTR pchIcoPath, int iIco) = 0;
		/*@}*/

		/**
		* @brief Releative path
		*/
		virtual BOOL SetRelPath(LPCTSTR pchPathRel) = 0;

		/**
		* @brief Resolve : 快捷方式的目标不存在时，查找
		*/
		virtual BOOL Resolve(HWND hwnd, DWORD fFlags = SLR_UPDATE) = 0;

		/**
		* @brief SetPath : Path + FileName
		*/
		virtual BOOL SetPath(LPCTSTR pchPath) = 0;
	};

	/** 
	* @class CShortcut
	* @detail IShortcut接口的实现
	* @see IShortcut
	*/
	class BASE_EXPORT CShortcut : public IShortcut
	{
	public:
		CShortcut(BOOL bAutoSave = FALSE);
		CShortcut(LPCTSTR pShortutFile, BOOL bAutoSave = FALSE);
		virtual ~CShortcut();

	protected:
		IShellLink* m_pShellLink;
		IPersistFile* m_pPersistFile;

		BOOL m_bAutoSave;

	protected:
		BOOL Init();
		BOOL Uninit();

	public:
		BOOL Load(LPCTSTR pShortcutFile);
		BOOL Save(LPCTSTR pchFileName = NULL, BOOL fRemember = TRUE);

		BOOL SaveCompleted(LPCTSTR pchFileName);
		BOOL GetCurFile(LPTSTR pchFileName, int cchMax);

		BOOL IsChange();

		BOOL SetAutoSave(BOOL bAutoSave = TRUE);

	public:
		BOOL GetPath(LPTSTR pchFile, int cchMaxPath,
			WIN32_FIND_DATA* pfd = NULL, DWORD fFlags = 0l);

		/// ID List
		BOOL GetList(LPITEMIDLIST* ppidl);
		BOOL SetList(LPCITEMIDLIST pidl);

		/// Description
		BOOL GetDesc(LPTSTR pchDesc, int cchMaxDesc);
		BOOL SetDesc(LPCTSTR pchDesc);

		/// WorkingDirectory
		BOOL GetDir(LPTSTR pchDir, int cchMaxPath);
		BOOL SetDir(LPCTSTR pchDir);

		/// Arguments
		BOOL GetArgs(LPTSTR pchArgs, int cchMaxArgs);
		BOOL SetArgs(LPCTSTR pchArgs);

		/// HotKey
		BOOL GetHotKey(WORD* pwHotKey);
		BOOL SetHotKey(WORD wHotKey);

		/// ShowCommand /*SW_SHOWNORMAL SW_SHOWMAXIMIZED SW_SHOWMINNOACTIVE*/
		BOOL GetShow(int* piShowCmd);
		BOOL SetShow(int iShowCmd);

		/// Ico Location
		BOOL GetIco(LPTSTR pchIcoPath, int cchIcoPath, int* piIco);
		BOOL SetIco(LPCTSTR pchIcoPath, int iIco);

		/// Releative path
		BOOL SetRelPath(LPCTSTR pchPathRel);

		/// Resolve : 快捷方式的目标不存在时，查找
		BOOL Resolve(HWND hwnd, DWORD fFlags = SLR_UPDATE);

		/// SetPath : Path + FileName
		BOOL SetPath(LPCTSTR pchPath);
	};
}//namespace FileMisc

}//namespace Base

/*@}*/