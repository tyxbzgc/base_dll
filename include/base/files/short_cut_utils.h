/**
@defgroup Base_Misc_File File_�ļ�����ģ��
@ingroup Base_Misc
*/
/*@{*/
/**
* @file ShortCutUtils.h
* @brief �ļ�����ģ�鵼������
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
	* @brief ������ݷ�ʽ
	* @param[in] pszLnkFile ��ݷ�ʽ�ļ�
	* @param[in] lpszDest ��ݷ�ʽ��Ӧ��Ŀ���ļ�
	* @param[in] lpszWorkDir ��ʼλ��
	* @param[in] lpszDescript ��ݷ�ʽ������
	* @param[in] lpszCmdlineArguments ��ݷ�ʽ�����в���
	* @param[in] lpszIconFile ͼ���ļ�
	* @param[in] nIconIdx ͼ������
	* @param[in] wHotKey ��ݼ�
	* @param[in] iShowCmd ���
	* @return �Ƿ�ɹ�
	* @note ��Ҫ����CoInitialize��CoUninitialize
	*/
	BOOL BASE_EXPORT CreateShortCut(
		LPCTSTR pszLnkFile, //��ݷ�ʽ�ļ�
		LPCTSTR lpszDest, //��ݷ�ʽ��Ӧ��Ŀ���ļ�
		LPCTSTR lpszWorkDir = NULL,//��ʼλ��
		LPCTSTR lpszDescript = NULL,//��ݷ�ʽ������
		LPCTSTR lpszCmdlineArguments = NULL,//��ݷ�ʽ�����в���
		LPCTSTR lpszIconFile = NULL,	//ͼ���ļ�
		const int nIconIdx = 0,			//ͼ������
		const WORD wHotKey = 0,			//��ݼ�
		const int iShowCmd = SW_SHOWNORMAL);//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)֮һ

	/**
	* @brief �޸Ŀ�ݷ�ʽ������
	* @param[in] pszLnkFile ��ݷ�ʽ����
	* @param[in] pszSaveAsLnkFile �ѿ�ݷ�ʽ���Ϊ���ļ�,���ΪNULL,����ԭ���ļ�
	* @param[in] lpszDest ��ݷ�ʽ��Ӧ��Ŀ���ļ�
	* @param[in] lpszWorkDir ��ʼλ��
	* @param[in] lpszDescript ��ݷ�ʽ������
	* @param[in] lpszCmdlineArguments ��ݷ�ʽ�����в���
	* @param[in] lpszIconFile ͼ���ļ�
	* @param[in] nIconIdx ͼ������
	* @param[in] wHotKey ��ݼ�
	* @param[in] iShowCmd ���
	* @return �Ƿ�ɹ�
	* @note ��Ҫ����CoInitialize��CoUninitialize
	*/
	BOOL BASE_EXPORT ModifyShortCut(
		LPCTSTR pszLnkFile,	//��ݷ�ʽ����
		LPCTSTR pszSaveAsLnkFile = NULL,	//�ѿ�ݷ�ʽ���Ϊ���ļ�,���ΪNULL,����ԭ���ļ�
		LPCTSTR lpszDest = NULL, //��ݷ�ʽ��Ӧ��Ŀ���ļ�
		LPCTSTR lpszWorkDir = NULL,//��ʼλ��
		LPCTSTR lpszDescript = NULL,//��ݷ�ʽ������
		LPCTSTR lpszCmdlineArguments = NULL,//��ݷ�ʽ�����в���
		LPCTSTR lpszIconFile = NULL,	//ͼ���ļ�
		const int nIconIdx = 0,			//ͼ������
		const WORD wHotKey = 0,			//��ݼ�
		const int iShowCmd = SW_SHOWNORMAL);//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)֮һ

	/**
	* @brief ��ȡ��ݷ�ʽ��Ϣ
	* @param[in] pszLnkFile ��ݷ�ʽ����
	* @param[in] lpszDest ��ݷ�ʽ��Ӧ��Ŀ���ļ�
	* @param[in] lpszWorkDir ��ʼλ��
	* @param[in] lpszDescript ��ݷ�ʽ������
	* @param[in] lpszCmdlineArguments ��ݷ�ʽ�����в���
	* @param[in] lpszIconFile ͼ���ļ�
	* @param[in] nIconIdx ͼ������
	* @param[in] wHotKey ��ݼ�
	* @param[in] iShowCmd ���
	* @return �Ƿ�ɹ�
	* @note ��Ҫ����CoInitialize��CoUninitialize
	*/
	BOOL BASE_EXPORT GetShortCutInfo(
		LPCTSTR pszLnkFile = NULL,	//��ݷ�ʽ�ļ�
		LPTSTR lpszDest = NULL, //��ݷ�ʽ��Ӧ��Ŀ���ļ�
		LPTSTR lpszWorkDir = NULL,//��ʼλ��
		LPTSTR lpszDescript = NULL,//��ݷ�ʽ������
		LPTSTR lpszCmdlineArguments = NULL,//��ݷ�ʽ�����в���
		LPTSTR lpszIconFile = NULL,	//ͼ���ļ�
		int* nIconIdx = NULL,			//ͼ������
		WORD* wHotKey = NULL,			//��ݼ�
		int* iShowCmd = NULL);//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)֮һ
}//namespace FileMisc

namespace FileMisc
{
	/** 
	* @class IShortcut
	* @detail ����Ǵ�����ݷ�ʽ������Ҫ����Load
	*  ���ø��������Ȼ�����Save��Ҫ�ƶ�·����
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
		* @brief Resolve : ��ݷ�ʽ��Ŀ�겻����ʱ������
		*/
		virtual BOOL Resolve(HWND hwnd, DWORD fFlags = SLR_UPDATE) = 0;

		/**
		* @brief SetPath : Path + FileName
		*/
		virtual BOOL SetPath(LPCTSTR pchPath) = 0;
	};

	/** 
	* @class CShortcut
	* @detail IShortcut�ӿڵ�ʵ��
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

		/// Resolve : ��ݷ�ʽ��Ŀ�겻����ʱ������
		BOOL Resolve(HWND hwnd, DWORD fFlags = SLR_UPDATE);

		/// SetPath : Path + FileName
		BOOL SetPath(LPCTSTR pchPath);
	};
}//namespace FileMisc

}//namespace Base

/*@}*/