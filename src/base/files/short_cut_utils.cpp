#include "stdafx.h"
#include "File\ShortCutUtils.h"
/*
快捷方式由以下字段组成：

文件头（必须）
Shell Item ID List 段
文件位置信息段（必须）
描述字符段
相对路径段
工作目录段
命令行段
图标文件段
附加信息段
*/

/*
偏移14h开始的标志dwFlags标示可选属性
Bit  所在位为1时表示
0    有shell item id list
1    指向文件或文件夹,如果此位为0表示指向其他
2    存在描述字符串 
3    存在相对路径
4    存在工作路径
5    存在命令行参数
6    存在自定义图标
*/

/*
偏移18h开始的dwFileAttributes目标文件属性： 
Bit   所在位为1时表示
0     快捷方式所指目标文件有只读属性
1     快捷方式所指目标文件有隐藏属性
2     快捷方式所指目标文件是系统文件
3     快捷方式所指目标是卷标
4     快捷方式所指目标是文件夹
5     快捷方式所指目标文件上次存档后被改变过
6     快捷方式所指目标文件被加密
7     快捷方式所指目标文件属性为一般
8     快捷方式所指目标文件为临时
9     快捷方式所指目标文件为稀疏文件(sparse file)
10    快捷方式所指目标文件有重分析点数据(reparse point)
11    快捷方式所指目标文件被压缩
12    快捷方式所指目标文件脱机                              
*/

const unsigned long culLinkFileSign = 0x0000004c;
const unsigned char cucLinkFileguid[16] =
{
    0x01, 0x14, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xc0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x46
};

// 快捷方式文件头结构
#pragma pack (push, 1)
typedef struct _tagLinkHead
{
    DWORD    dwSign;        // 值常为0x0000004c,为字符"L"    
    BYTE     byGuid[16];    // guid
    DWORD    dwFlags;       // 用来标识快捷方式文件有哪些可选段
    DWORD    dwFileAttributes; // 目标文件属性
    FILETIME dwCreationTime;   // 目标文件创建时间
    FILETIME dwModificationTime; // 目标文件修改时间
    FILETIME dwLastaccessTime;   // 目标文件最后一次访问时间    
    DWORD    dwFileLen;   // 目标文件长度    
    DWORD    dwIconNum;   // 自定义图标个数
    DWORD    dwShowCmd;   // 目标文件窗口显示方式: 1 正常显示 2 最小化 3 最大化
    DWORD    dwHotkey;    // 热键
    DWORD    dwReserved1; // 总为0l
    DWORD    dwReserved2; // 总为0l
} LnkHead, LNKHEAD, *LPLNKHEAD;

// 文件位置信息

typedef struct _tagFileLocationInfo
{
    DWORD dwSize;   //此段的总长度
    DWORD dwFirstOffset;   //头结构长度,固定为1ch
    DWORD dwFlags;   //Flags指示文件在哪些卷有效,这里只用到低两位,第一位置位表示本地卷有效,
                    //反之无效.第二位置位表示网络卷有效,反之无效.
    DWORD dwOffsetOfVolume;   //本地卷信息表的偏移(固定1ch)
    DWORD dwOffsetOfBasePath;   //本地路径信息的偏移
    DWORD dwOffsetOfNetworkVolume;   //网络卷信息表的偏移
    DWORD dwOffsetOfRemainingPath;   //附加信息的偏移
} FileLocationInfo, FILELOCATIONINFO, *LPFILELOCATIONINFO;

typedef struct _tagLocalVolumeTab
{
    DWORD dwSize; //本地卷信息表的长度
    DWORD dwTypeOfVolume; //卷类型
                            /*
                            0 Unknown
                            1 No root directory
                            2 Removable (Floppy, Zip, etc..)
                            3 Fixed (Hard disk)
                            4 Remote (Network drive)
                            5 CD-ROM
                            6 Ram drive (Shortcuts to stuff on a ram drive, now that''s smart...)
                            */
    DWORD dwVolumeSerialNumber; //卷序列号
    DWORD dwOffsetOfVolumeName; //固定长度部分的大小,固定为10h
    char  strVolumeName[1]; //可变长度,卷标签
} LocalVolumeTab, LOCALVOLUMETAB, *LPLOCALVOLUMETAB;

typedef struct _tagNetWorkVolumeTab
{
    DWORD dwSize; //网络卷信息表的长度
    DWORD dwUnknown1; //固定为2h
    DWORD dwOffsetOfNetShareName; //固定长度部分的大小,固定为14h
    DWORD dwUnknown2; //固定为0h
    DWORD dwUnknown3; //固定为20000h
    char strNetShareName[1]; //可变长度,网络共享名
} NetWorkVolumeTab, NETWORKVOLUMETAB, *LPNETWORKVOLUMETAB;
typedef struct _tagLnkAttrib
{
	unsigned long mask;
	TCHAR chDest[MAX_PATH];
	TCHAR chWorkDir[MAX_PATH];
#if WINVER >= 0x0500  // win2k
	TCHAR chDescript[1024];
	TCHAR chCmdLine[1024];
#else
	TCHAR chDescript[MAX_PATH];
	TCHAR chCmdLine[MAX_PATH];
#endif
	TCHAR chRelPath[MAX_PATH];
	TCHAR chIcoFile[MAX_PATH];
	int nIcoIndex;
	WORD wHotKey;
	int nShowCmd;
} LNKATTRIB, LnkAttrib, *LPLNKATTRIB;
enum SHORTCUR_ATTRIB
{
	SCA_NONE          = 0x00000000,
	SCA_ITEMIDLIST    = 0x00000001,
	SCA_DESCRIPT      = 0x00000002,
	SCA_DEST          = 0x00000004,
	SCA_RELPATH       = 0x00000008,
	SCA_WORKDIR       = 0x00000010,
	SCA_CMDLINE       = 0x00000020,
	SCA_ICOFILEINDEX  = 0x00000040,
	SCA_HOTKEY        = 0x00000080,
	SCA_SHOWCMD       = 0x00000100,
	SCA_ADDITION      = 0x00000200,

	SCA_ALL = SCA_ITEMIDLIST | SCA_DESCRIPT | SCA_DEST | SCA_RELPATH | SCA_WORKDIR |
	SCA_CMDLINE | SCA_ICOFILEINDEX | SCA_HOTKEY | SCA_SHOWCMD | SCA_ADDITION,
};

#pragma pack(pop)

namespace Base
{


namespace FileMisc
{
	BOOL CreateShortCut(
		LPCTSTR pszLnkFile, //快捷方式文件
		LPCTSTR lpszDest, //快捷方式对应的目标文件
		LPCTSTR lpszWorkDir,//起始位置
		LPCTSTR lpszDescript,//快捷方式的描述
		LPCTSTR lpszCmdlineArguments,//快捷方式命令行参数
		LPCTSTR lpszIconFile,	//图标文件
		const int nIconIdx,		//图标索引
		const WORD wHotKey,		//快捷键
		const int iShowCmd)//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)之一
	{
		if (!pszLnkFile || 0 == ::_tcslen(pszLnkFile)
			|| !lpszDest || 0 == ::_tcslen(lpszDest))
		{
			return FALSE;
		}

		IShellLink* pShell = NULL;
		IPersistFile* pPerFile = NULL;
		HRESULT hRes = ::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShell);
		if (FAILED(hRes) || !pShell)
		{
			return FALSE;
		}
		hRes = pShell->QueryInterface(IID_IPersistFile, (void**)&pPerFile);
		if (FAILED(hRes) || !pPerFile)
		{
			pShell->Release();
			pShell = NULL;

			return FALSE;
		}

		//设置目标
		if (FAILED(pShell->SetPath(lpszDest)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//设置起始位置
		if (lpszWorkDir && FAILED(pShell->SetWorkingDirectory(lpszWorkDir)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//设置描述
		if (lpszDescript && FAILED(pShell->SetDescription(lpszDescript)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//设置命令行参数
		if (lpszCmdlineArguments && FAILED(pShell->SetArguments(lpszCmdlineArguments)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//设置图标
		if (lpszIconFile && FAILED(pShell->SetIconLocation(lpszIconFile, nIconIdx)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//设置快捷键
		if (0 != wHotKey && FAILED(pShell->SetHotkey(wHotKey)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//设置显示
		if (FAILED(pShell->SetShowCmd(iShowCmd)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//保存快捷方式
	#if defined(_UNICODE) || defined(UNICODE)
		hRes = pPerFile->Save(pszLnkFile, TRUE);
	#else
		WORD wsz[MAX_PATH];
		::memset(wsz, 0, sizeof(WORD) * MAX_PATH);
		::MultiByteToWideChar(CP_ACP, 0, pszLnkFile, -1, wsz, MAX_PATH);
		hRes = pPerFile->Save(wsz, TRUE);
	#endif

		pPerFile->Release();
		pShell->Release();
		pPerFile = NULL;
		pShell = NULL;

		return SUCCEEDED(hRes);
	}

	BOOL ModifyShortCut(
		LPCTSTR pszLnkFile,	//快捷方式名称
		LPCTSTR pszSaveAsLnkFile,	//把快捷方式另存为的文件
		LPCTSTR lpszDest, //快捷方式对应的目标文件
		LPCTSTR lpszWorkDir,//起始位置
		LPCTSTR lpszDescript,//快捷方式的描述
		LPCTSTR lpszCmdlineArguments,//快捷方式命令行参数
		LPCTSTR lpszIconFile,	//图标文件
		const int nIconIdx,			//图标索引
		const WORD wHotKey,			//快捷键
		const int iShowCmd)//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)之一
	{
		if (!pszLnkFile || 0 == ::_tcslen(pszLnkFile))
		{
			return FALSE;
		}

		IShellLink* pShell = NULL;
		IPersistFile* pPerFile = NULL;
		WORD wsz[MAX_PATH] = { 0 };
		HRESULT hRes = ::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShell);
		if (FAILED(hRes) || !pShell)
		{
			return FALSE;
		}
		hRes = pShell->QueryInterface(IID_IPersistFile, (void**)&pPerFile);
		if (FAILED(hRes) || !pPerFile)
		{
			pShell->Release();
			pShell = NULL;

			return FALSE;
		}

		__try {
	#if defined(_UNICODE) || defined(UNICODE)
		if (FAILED(pPerFile->Load(pszLnkFile, STGM_READWRITE)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;
	        
			return FALSE;
		}
	#else
		//加载快捷方式
		::memset(wsz, 0, sizeof(WORD) * MAX_PATH);
		::MultiByteToWideChar(CP_ACP, 0, pszLnkFile,
			-1, wsz, MAX_PATH);
		if (FAILED(pPerFile->Load(wsz, STGM_READWRITE)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}
	#endif
		}
		__except (1)
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//修改目标文件
		if (lpszDest && FAILED(pShell->SetPath(lpszDest)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//修改起始位置
		if (lpszWorkDir && FAILED(pShell->SetWorkingDirectory(lpszWorkDir)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//修改描述
		if (lpszDescript && FAILED(pShell->SetDescription(lpszDescript)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//修改命令行参数
		if (lpszCmdlineArguments && FAILED(pShell->SetArguments(lpszCmdlineArguments)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//修改图标
		if (lpszIconFile && FAILED(pShell->SetIconLocation(lpszIconFile, nIconIdx)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//修改快捷键
		if (0 != wHotKey && FAILED(pShell->SetHotkey(wHotKey)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//修改显示方式
		if (FAILED(pShell->SetShowCmd(iShowCmd)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

	#if defined(_UNICODE) || defined(UNICODE)
		//保存快捷方式
		::memset(wsz, 0, sizeof(WORD) * MAX_PATH);
		if (pszSaveAsLnkFile)
			hRes = pPerFile->Save(pszSaveAsLnkFile, TRUE);
		else
			hRes = pPerFile->Save(pszLnkFile, TRUE);
	#else
		//保存快捷方式
		::memset(wsz, 0, sizeof(WORD) * MAX_PATH);
		if (pszSaveAsLnkFile)
		{
			::MultiByteToWideChar(CP_ACP, 0, pszSaveAsLnkFile,
				-1, wsz, MAX_PATH);
		}
		else
		{
			::MultiByteToWideChar(CP_ACP, 0, pszLnkFile,
				-1, wsz, MAX_PATH);
		}
		hRes = pPerFile->Save(wsz, TRUE);
	#endif

		pPerFile->Release();
		pShell->Release();
		pPerFile = NULL;
		pShell = NULL;

		return SUCCEEDED(hRes);
	}

	BOOL GetShortCutInfo(
		 LPCTSTR pszLnkFile,	//快捷方式文件
		 LPTSTR lpszDest, //快捷方式对应的目标文件
		 LPTSTR lpszWorkDir,//起始位置
		 LPTSTR lpszDescript,//快捷方式的描述
		 LPTSTR lpszCmdlineArguments,//快捷方式命令行参数
		 LPTSTR lpszIconFile,	//图标文件
		 int* nIconIdx,			//图标索引
		 WORD* wHotKey,			//快捷键
		 int* iShowCmd) //SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)之一
	{
		if (!pszLnkFile || 0 == ::_tcslen(pszLnkFile))
			return FALSE;

		IShellLink* pShell = NULL;
		IPersistFile* pPerFile = NULL;
		HRESULT hRes = ::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShell);
		if (FAILED(hRes) || !pShell)
		{
			return FALSE;
		}
		hRes = pShell->QueryInterface(IID_IPersistFile, (void**)&pPerFile);
		if (FAILED(hRes) || !pPerFile)
		{
			pShell->Release();
			pShell = NULL;

			return FALSE;
		}

		__try{   // 避免crash shell32!LookupItemInHashTable
	#if defined(_UNICODE) || defined(UNICODE)
		//加载快捷方式
		if (FAILED(pPerFile->Load(pszLnkFile, STGM_READ)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}
	#else
		//加载快捷方式
		WORD wsz[MAX_PATH];
		::memset(wsz, 0, sizeof(WORD) * MAX_PATH);
		::MultiByteToWideChar(CP_ACP, 0, pszLnkFile, -1, wsz, MAX_PATH);
		if (FAILED(pPerFile->Load(wsz, STGM_READ)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}
	#endif
		}
		__except (1)
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;
			return FALSE;
		}

		//获取显示方式
		if (iShowCmd && FAILED(pShell->GetShowCmd(iShowCmd)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//获取快捷键
		if (wHotKey && FAILED(pShell->GetHotkey(wHotKey)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//获取图标
		if (lpszIconFile && nIconIdx
			&& FAILED(pShell->GetIconLocation(lpszIconFile, MAX_PATH, nIconIdx)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//获取命令行参数
		if (lpszCmdlineArguments && FAILED(pShell->GetArguments(lpszCmdlineArguments, MAX_PATH)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//获取描述
		if (lpszDescript && FAILED(pShell->GetDescription(lpszDescript, MAX_PATH)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//获取工作目录
		if (lpszWorkDir && FAILED(pShell->GetWorkingDirectory(lpszWorkDir, MAX_PATH)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//获取目标
		__try {
		if (lpszDest)
			hRes = pShell->GetPath(lpszDest, MAX_PATH, NULL, SLGP_RAWPATH);
		}
		__except (1)
		{
			hRes = E_FAIL;
		}

		pShell->Release();
		pShell = NULL;
		pPerFile->Release();
		pPerFile = NULL;

		if (!lpszDest)
			return TRUE;
		else
			return SUCCEEDED(hRes);
	}

	BOOL CreateShortCut(LPCTSTR pchLinkFile, LnkAttrib* pAttrib)
	{
		if (!pchLinkFile ||
			0 == ::lstrlen(pchLinkFile) ||
			!pAttrib ||
			pAttrib->mask == SCA_NONE)
		{
			return FALSE;
		}

		IShellLink* pShell = NULL;
		IPersistFile* pFile = NULL;
		HRESULT hr = S_OK;
		BOOL fRet = FALSE;
		WORD wChar[MAX_PATH + 1] = { 0 };

		hr = ::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShell);
		if (hr != S_OK || !pShell)
		{
			goto exit;
		}

		hr = pShell->QueryInterface(IID_IPersistFile, (void**)&pFile);
		if (hr != S_OK || !pFile)
			goto exit;

		if (pAttrib->mask & SCA_DEST)
		{
			hr = pShell->SetPath(pAttrib->chDest);
			if (hr != S_OK)
				goto exit;

		}

		if (pAttrib->mask & SCA_WORKDIR)
		{
			hr = pShell->SetWorkingDirectory(pAttrib->chWorkDir);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_DESCRIPT)
		{
			hr = pShell->SetDescription(pAttrib->chDescript);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_CMDLINE)
		{
			hr = pShell->SetArguments(pAttrib->chCmdLine);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_ICOFILEINDEX)
		{
			hr = pShell->SetIconLocation(pAttrib->chIcoFile, pAttrib->nIcoIndex);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_HOTKEY)
		{
			hr = pShell->SetHotkey(pAttrib->wHotKey);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_SHOWCMD)
		{
			hr = pShell->SetShowCmd(pAttrib->nShowCmd);
			if (hr != S_OK)
				goto exit;
		}

		// Save
	#if defined(_UNICODE) || defined(UNICODE)
		hr = pFile->Save(pchLinkFile, TRUE);
	#else
		::MultiByteToWideChar(CP_ACP, 0, pchLinkFile, -1, wChar, MAX_PATH);
		hr = pFile->Save(wChar, TRUE);
	#endif

		if (hr == S_OK)
			fRet = TRUE;

	exit:

		if (pShell)
		{
			pShell->Release();
			pShell = NULL;
		}

		if (pFile)
		{
			pFile->Release();
			pFile = NULL;
		}

		return fRet;
	}

	BOOL GetShortCut(LPCTSTR pchLinkFile, LnkAttrib* pAttrib)
	{
		if (!pchLinkFile ||
			0 == ::lstrlen(pchLinkFile) ||
			!pAttrib ||
			0l == pAttrib->mask)
		{
			return FALSE;
		}

		IShellLink* pShell = NULL;
		IPersistFile* pFile = NULL;
		WORD wChar[MAX_PATH + 1] = { 0 };
		HRESULT hr = S_OK;
		BOOL fRet = FALSE;

		hr = ::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShell);
		if (hr != S_OK || !pShell)
			goto exit;

		hr = pShell->QueryInterface(IID_IPersistFile, (void**)&pFile);
		if (hr != S_OK || !pFile)
			goto exit;

		__try{   // 避免crash shell32!LookupItemInHashTable
	#if defined(_UNICODE) || defined(UNICODE)
		//加载快捷方式
		hr = pFile->Load(pchLinkFile, STGM_READ);
	#else
		//加载快捷方式
		::MultiByteToWideChar(CP_ACP, 0, pchLinkFile, -1, wChar, MAX_PATH);
		hr = pFile->Load(wChar, STGM_READ);
	#endif
		}
		__except (1)
		{
			hr = E_FAIL;
		}

		if (hr != S_OK)
			goto exit;

		if (pAttrib->mask & SCA_SHOWCMD)
		{
			hr = pShell->GetShowCmd(&pAttrib->nShowCmd);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_HOTKEY)
		{
			hr = pShell->GetHotkey(&pAttrib->wHotKey);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_DEST)
		{
			__try {
				hr = pShell->GetPath(pAttrib->chDest, MAX_PATH, NULL, 0l);
			}
			__except (1)
			{
				hr = E_FAIL;
			}
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_WORKDIR)
		{
			hr = pShell->GetWorkingDirectory(pAttrib->chWorkDir, MAX_PATH);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_DESCRIPT)
		{
		#if WINVER >= 0x0500
			hr = pShell->GetDescription(pAttrib->chDescript, INFOTIPSIZE);
		#else
			hr = pShell->GetDescription(pAttrib->chDescript, MAX_PATH);
		#endif
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_CMDLINE)
		{
		#if WINVER >= 0x0500
			hr = pShell->GetArguments(pAttrib->chCmdLine, INFOTIPSIZE);
		#else
			hr = pShell->GetArguments(pAttrib->chCmdLine, MAX_PATH);
		#endif
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_ICOFILEINDEX)
		{
			hr = pShell->GetIconLocation(pAttrib->chIcoFile,
				MAX_PATH, &pAttrib->nIcoIndex);
			if (hr != S_OK)
				goto exit;
		}

		fRet = TRUE;

	exit:

		if (pShell)
		{
			pShell->Release();
			pShell = NULL;
		}

		if (pFile)
		{
			pFile->Release();
			pFile = NULL;
		}

		return fRet;
	}

	BOOL SetShortCut(LPCTSTR pchLinkFile, LnkAttrib* pAttrib)
	{
		if (!pchLinkFile ||
			0 == ::lstrlen(pchLinkFile) ||
			!pAttrib ||
			0l == pAttrib->mask)
		{
			return FALSE;
		}

		IShellLink* pShell = NULL;
		IPersistFile* pFile = NULL;
		WORD wChar[MAX_PATH + 1] = { 0 };
		HRESULT hr = S_OK;
		BOOL fRet = FALSE;

		hr = ::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShell);
		if (hr != S_OK || !pShell)
		{
			goto exit;
		}

		hr = pShell->QueryInterface(IID_IPersistFile, (void**)&pFile);
		if (hr != S_OK || !pFile)
		{
			goto exit;
		}

		__try {
	#if defined(_UNICODE) || defined(UNICODE)
		//加载快捷方式
		hr = pFile->Load(pchLinkFile, STGM_READWRITE);
	#else
		//加载快捷方式
		::MultiByteToWideChar(CP_ACP, 0, pchLinkFile, -1, wChar, MAX_PATH);
		hr = pFile->Load(wChar, STGM_READWRITE);
	#endif
		}
		__except (1)
		{
			hr = E_FAIL;
		}

		if (hr != S_OK)
			goto exit;

		if (pAttrib->mask & SCA_DEST)
		{
			hr = pShell->SetPath(pAttrib->chDest);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_WORKDIR)
		{
			hr = pShell->SetWorkingDirectory(pAttrib->chWorkDir);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_DESCRIPT)
		{
			hr = pShell->SetDescription(pAttrib->chDescript);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_CMDLINE)
		{
			hr = pShell->SetArguments(pAttrib->chCmdLine);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_ICOFILEINDEX)
		{
			hr = pShell->SetIconLocation(pAttrib->chIcoFile, pAttrib->nIcoIndex);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_HOTKEY)
		{
			hr = pShell->SetHotkey(pAttrib->wHotKey);
			if (hr != S_OK)
				goto exit;
		}

		if (pAttrib->mask & SCA_SHOWCMD)
		{
			hr = pShell->SetShowCmd(pAttrib->nShowCmd);
			if (hr != S_OK)
				goto exit;
		}

		hr = pFile->Save((LPCOLESTR)wChar, TRUE);

		if (hr == S_OK)
			fRet = TRUE;

	exit:

		if (pShell)
		{
			pShell->Release();
			pShell = NULL;
		}

		if (pFile)
		{
			pFile->Release();
			pFile = NULL;
		}

		return fRet;
	}
}

namespace FileMisc
{
	CShortcut::CShortcut(BOOL bAutoSave) :
		m_pShellLink(NULL),
		m_pPersistFile(NULL)
	{
		m_bAutoSave = bAutoSave;
		CShortcut::Init();
	}

	CShortcut::CShortcut(LPCTSTR pShortutFile, BOOL bAutoSave):
		m_pShellLink(NULL),
		m_pPersistFile(NULL)
	{
		m_bAutoSave = bAutoSave;

		CShortcut::Init();

		CShortcut::Load(pShortutFile);
	}

	CShortcut::~CShortcut()
	{
		CShortcut::Uninit();	
	}

	BOOL CShortcut::Init()
	{
		CShortcut::Uninit();

		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&m_pShellLink);
		if (S_OK != hr || !m_pShellLink)
		{
			return FALSE;
		}

		hr = m_pShellLink->QueryInterface(IID_IPersistFile, (void**)&m_pPersistFile);
		if (S_OK != hr || !m_pPersistFile)
		{
			m_pShellLink->Release();
			m_pShellLink = NULL;

			return FALSE;
		}

		return TRUE;
	}

	BOOL CShortcut::Uninit()
	{
		if (m_bAutoSave)
			CShortcut::Save();

		if (m_pShellLink)
		{
			m_pShellLink->Release();
			m_pShellLink = NULL;
		}

		if (m_pPersistFile)
		{
			m_pPersistFile->Release();
			m_pPersistFile = NULL;
		}

		return TRUE;
	}

	BOOL CShortcut::SetAutoSave(BOOL bAutoSave)
	{
		BOOL bRet = m_bAutoSave;

		m_bAutoSave = bAutoSave;

		return bRet;
	}

	BOOL CShortcut::Load(LPCTSTR pShortcutFile)
	{
		if (m_pPersistFile && pShortcutFile && 0 != ::lstrlen(pShortcutFile))
		{
			DWORD dwMode = 0l;
			HANDLE hFile = ::CreateFile(pShortcutFile, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				if (2 == ::GetLastError())
					dwMode = STGM_CREATE | STGM_READWRITE;
				else
					return FALSE;
			}
			else
			{
				::CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
				dwMode = STGM_READWRITE;
			}

			HRESULT hr = E_FAIL;
			__try {
	#if (defined(_UNICODE) || defined(UNICODE))
				hr = m_pPersistFile->Load(pShortcutFile, dwMode);
	#else
				wchar_t wChar[MAX_PATH + 1] = { 0 };
				::MultiByteToWideChar(CP_ACP, 0l, pShortcutFile, -1, wChar, MAX_PATH);
				hr = m_pPersistFile->Load(wChar, dwMode);
	#endif
			}
			__except (1)
			{
				hr = E_FAIL;
			}
			return SUCCEEDED(hr);
		}

		return FALSE;
	}

	BOOL CShortcut::Save(LPCTSTR pchFileName, BOOL fRemember)
	{
		if (m_pPersistFile)
		{
			HRESULT hr = S_OK;
			if (pchFileName)
			{
			#if defined(_UNICODE) || defined(UNICODE)
				hr = m_pPersistFile->Save(pchFileName, fRemember);
			#else
				WORD wChar[MAX_PATH] = { 0 };
				::MultiByteToWideChar(CP_ACP, 0l, pchFileName, -1, wChar, MAX_PATH);
				hr = m_pPersistFile->Save(wChar, fRemember);
			#endif
			}
			else
				hr = m_pPersistFile->Save(NULL, fRemember);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SaveCompleted(LPCTSTR pchFileName)
	{
		if (m_pPersistFile)
		{
			#if defined(_UNICODE) || defined(UNICODE)
				HRESULT hr = m_pPersistFile->SaveCompleted(pchFileName);
			#else
				WORD wChar[MAX_PATH] = { 0 };
				::MultiByteToWideChar(CP_ACP, 0l, pchFileName, -1, wChar, MAX_PATH);
				HRESULT hr = m_pPersistFile->SaveCompleted(wChar);
			#endif

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::GetCurFile(LPTSTR pchFileName, int cchMax)
	{
		if (m_pPersistFile && pchFileName && cchMax > 0)
		{
			WORD* pwFileName = NULL;
			HRESULT hr = m_pPersistFile->GetCurFile((LPOLESTR*)&pwFileName);
			if (S_OK == hr && pwFileName)
			{
			#if defined(_UNICODE) || defined(UNICODE)
				::_tcsncpy_s(pchFileName, cchMax, (wchar_t*)pwFileName, _tcslen((wchar_t*)pwFileName));
			#else
				::WideCharToMultiByte(CP_ACP, 0l, pwFileName, -1,
					pchFileName, cchMax, NULL, NULL);
			#endif
				pwFileName = NULL;
			}

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::IsChange()
	{
		if (m_pPersistFile)
		{
			HRESULT hr = m_pPersistFile->IsDirty();

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::GetPath(LPTSTR pchFile, int cchMaxPath,
		WIN32_FIND_DATA* pfd, DWORD fFlags)
	{
		if (m_pShellLink && pchFile && cchMaxPath > 0)
		{
			HRESULT hr = E_FAIL;
			__try {
				hr = m_pShellLink->GetPath(pchFile, cchMaxPath, pfd, fFlags);
			}
			__except (1)
			{
				hr = E_FAIL;
			}
			return S_OK == hr;
		}

		return FALSE;
	}

	// ID List
	BOOL CShortcut::GetList(LPITEMIDLIST* ppidl)
	{
		if (m_pShellLink)
		{
			HRESULT hr = m_pShellLink->GetIDList(ppidl);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SetList(LPCITEMIDLIST pidl)
	{
		if (m_pShellLink && pidl)
		{
			HRESULT hr = m_pShellLink->SetIDList(pidl);

			return S_OK == hr;
		}

		return FALSE;
	}

	// Description
	BOOL CShortcut::GetDesc(LPTSTR pchDesc, int cchMaxDesc)
	{
		if (m_pShellLink && pchDesc && cchMaxDesc > 0)
		{
			HRESULT hr = m_pShellLink->GetDescription(pchDesc, cchMaxDesc);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SetDesc(LPCTSTR pchDesc)
	{
		if (m_pShellLink && pchDesc)
		{
			HRESULT hr = m_pShellLink->SetDescription(pchDesc);

			return S_OK == hr;
		}

		return FALSE;
	}

	// WorkingDirectory
	BOOL CShortcut::GetDir(LPTSTR pchDir, int cchMaxPath)
	{
		if (m_pShellLink)
		{
			HRESULT hr = m_pShellLink->GetWorkingDirectory(pchDir, cchMaxPath);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SetDir(LPCTSTR pchDir)
	{
		if (m_pShellLink)
		{
			HRESULT hr = m_pShellLink->SetWorkingDirectory(pchDir);

			return S_OK == hr;
		}

		return FALSE;
	}

	// Arguments
	BOOL CShortcut::GetArgs(LPTSTR pchArgs, int cchMaxArgs)
	{
		if (m_pShellLink && pchArgs && cchMaxArgs > 0)
		{
			HRESULT hr = m_pShellLink->GetArguments(pchArgs, cchMaxArgs);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SetArgs(LPCTSTR pchArgs)
	{
		if (m_pShellLink && pchArgs)
		{
			HRESULT hr = m_pShellLink->SetArguments(pchArgs);

			return S_OK == hr;
		}

		return FALSE;
	}

	// HotKey
	BOOL CShortcut::GetHotKey(WORD* pwHotKey)
	{
		if (m_pShellLink && pwHotKey)
		{
			HRESULT hr = m_pShellLink->GetHotkey(pwHotKey);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SetHotKey(WORD wHotKey)
	{
		if (m_pShellLink)
		{
			HRESULT hr = m_pShellLink->SetHotkey(wHotKey);

			return S_OK == hr;
		}

		return FALSE;
	}

	// ShowCommand
	BOOL CShortcut::GetShow(int* piShowCmd)
	{
		if (m_pShellLink)
		{
			HRESULT hr = m_pShellLink->GetShowCmd(piShowCmd);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SetShow(int iShowCmd)
	{
		if (iShowCmd != SW_SHOWNORMAL &&
			iShowCmd != SW_SHOWMAXIMIZED &&
			iShowCmd != SW_SHOWMINNOACTIVE)
		{
			return FALSE;
		}

		if (m_pShellLink)
		{
			HRESULT hr = m_pShellLink->SetShowCmd(iShowCmd);

			return S_OK == hr;
		}

		return FALSE;
	}

	// Ico Location
	BOOL CShortcut::GetIco(LPTSTR pchIcoPath, int cchIcoPath, int* piIco)
	{
		if (m_pShellLink && pchIcoPath && cchIcoPath > 0 && piIco)
		{
			HRESULT hr = m_pShellLink->GetIconLocation(pchIcoPath, cchIcoPath, piIco);

			return S_OK == hr;
		}

		return FALSE;
	}

	BOOL CShortcut::SetIco(LPCTSTR pchIcoPath, int iIco)
	{
		if (m_pShellLink && pchIcoPath && 0 != ::lstrlen(pchIcoPath))
		{
			HRESULT hr = m_pShellLink->SetIconLocation(pchIcoPath, iIco);

			return S_OK == hr;
		}

		return FALSE;
	}

	// Releative path
	BOOL CShortcut::SetRelPath(LPCTSTR pchPathRel)
	{
		if (m_pShellLink && pchPathRel && 0 != ::lstrlen(pchPathRel))
		{
			HRESULT hr = m_pShellLink->SetRelativePath(pchPathRel, 0l);

			return S_OK == hr;
		}

		return FALSE;
	}

	// Resolve
	BOOL CShortcut::Resolve(HWND hwnd, DWORD fFlags)
	{
		if (m_pShellLink)
		{
			HRESULT hr = m_pShellLink->Resolve(hwnd, fFlags);

			return S_OK == hr;
		}

		return FALSE;
	}

	// SetPath
	BOOL CShortcut::SetPath(LPCTSTR pchPath)
	{
		if (m_pShellLink && pchPath && 0 != ::lstrlen(pchPath))
		{
			HRESULT hr = m_pShellLink->SetPath(pchPath);

			return S_OK == hr;
		}

		return FALSE;
	}

}


}//namespace Base