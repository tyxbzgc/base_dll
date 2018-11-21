#include "stdafx.h"
#include "File\ShortCutUtils.h"
/*
��ݷ�ʽ�������ֶ���ɣ�

�ļ�ͷ�����룩
Shell Item ID List ��
�ļ�λ����Ϣ�Σ����룩
�����ַ���
���·����
����Ŀ¼��
�����ж�
ͼ���ļ���
������Ϣ��
*/

/*
ƫ��14h��ʼ�ı�־dwFlags��ʾ��ѡ����
Bit  ����λΪ1ʱ��ʾ
0    ��shell item id list
1    ָ���ļ����ļ���,�����λΪ0��ʾָ������
2    ���������ַ��� 
3    �������·��
4    ���ڹ���·��
5    ���������в���
6    �����Զ���ͼ��
*/

/*
ƫ��18h��ʼ��dwFileAttributesĿ���ļ����ԣ� 
Bit   ����λΪ1ʱ��ʾ
0     ��ݷ�ʽ��ָĿ���ļ���ֻ������
1     ��ݷ�ʽ��ָĿ���ļ�����������
2     ��ݷ�ʽ��ָĿ���ļ���ϵͳ�ļ�
3     ��ݷ�ʽ��ָĿ���Ǿ��
4     ��ݷ�ʽ��ָĿ�����ļ���
5     ��ݷ�ʽ��ָĿ���ļ��ϴδ浵�󱻸ı��
6     ��ݷ�ʽ��ָĿ���ļ�������
7     ��ݷ�ʽ��ָĿ���ļ�����Ϊһ��
8     ��ݷ�ʽ��ָĿ���ļ�Ϊ��ʱ
9     ��ݷ�ʽ��ָĿ���ļ�Ϊϡ���ļ�(sparse file)
10    ��ݷ�ʽ��ָĿ���ļ����ط���������(reparse point)
11    ��ݷ�ʽ��ָĿ���ļ���ѹ��
12    ��ݷ�ʽ��ָĿ���ļ��ѻ�                              
*/

const unsigned long culLinkFileSign = 0x0000004c;
const unsigned char cucLinkFileguid[16] =
{
    0x01, 0x14, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xc0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x46
};

// ��ݷ�ʽ�ļ�ͷ�ṹ
#pragma pack (push, 1)
typedef struct _tagLinkHead
{
    DWORD    dwSign;        // ֵ��Ϊ0x0000004c,Ϊ�ַ�"L"    
    BYTE     byGuid[16];    // guid
    DWORD    dwFlags;       // ������ʶ��ݷ�ʽ�ļ�����Щ��ѡ��
    DWORD    dwFileAttributes; // Ŀ���ļ�����
    FILETIME dwCreationTime;   // Ŀ���ļ�����ʱ��
    FILETIME dwModificationTime; // Ŀ���ļ��޸�ʱ��
    FILETIME dwLastaccessTime;   // Ŀ���ļ����һ�η���ʱ��    
    DWORD    dwFileLen;   // Ŀ���ļ�����    
    DWORD    dwIconNum;   // �Զ���ͼ�����
    DWORD    dwShowCmd;   // Ŀ���ļ�������ʾ��ʽ: 1 ������ʾ 2 ��С�� 3 ���
    DWORD    dwHotkey;    // �ȼ�
    DWORD    dwReserved1; // ��Ϊ0l
    DWORD    dwReserved2; // ��Ϊ0l
} LnkHead, LNKHEAD, *LPLNKHEAD;

// �ļ�λ����Ϣ

typedef struct _tagFileLocationInfo
{
    DWORD dwSize;   //�˶ε��ܳ���
    DWORD dwFirstOffset;   //ͷ�ṹ����,�̶�Ϊ1ch
    DWORD dwFlags;   //Flagsָʾ�ļ�����Щ����Ч,����ֻ�õ�����λ,��һλ��λ��ʾ���ؾ���Ч,
                    //��֮��Ч.�ڶ�λ��λ��ʾ�������Ч,��֮��Ч.
    DWORD dwOffsetOfVolume;   //���ؾ���Ϣ���ƫ��(�̶�1ch)
    DWORD dwOffsetOfBasePath;   //����·����Ϣ��ƫ��
    DWORD dwOffsetOfNetworkVolume;   //�������Ϣ���ƫ��
    DWORD dwOffsetOfRemainingPath;   //������Ϣ��ƫ��
} FileLocationInfo, FILELOCATIONINFO, *LPFILELOCATIONINFO;

typedef struct _tagLocalVolumeTab
{
    DWORD dwSize; //���ؾ���Ϣ��ĳ���
    DWORD dwTypeOfVolume; //������
                            /*
                            0 Unknown
                            1 No root directory
                            2 Removable (Floppy, Zip, etc..)
                            3 Fixed (Hard disk)
                            4 Remote (Network drive)
                            5 CD-ROM
                            6 Ram drive (Shortcuts to stuff on a ram drive, now that''s smart...)
                            */
    DWORD dwVolumeSerialNumber; //�����к�
    DWORD dwOffsetOfVolumeName; //�̶����Ȳ��ֵĴ�С,�̶�Ϊ10h
    char  strVolumeName[1]; //�ɱ䳤��,���ǩ
} LocalVolumeTab, LOCALVOLUMETAB, *LPLOCALVOLUMETAB;

typedef struct _tagNetWorkVolumeTab
{
    DWORD dwSize; //�������Ϣ��ĳ���
    DWORD dwUnknown1; //�̶�Ϊ2h
    DWORD dwOffsetOfNetShareName; //�̶����Ȳ��ֵĴ�С,�̶�Ϊ14h
    DWORD dwUnknown2; //�̶�Ϊ0h
    DWORD dwUnknown3; //�̶�Ϊ20000h
    char strNetShareName[1]; //�ɱ䳤��,���繲����
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
		LPCTSTR pszLnkFile, //��ݷ�ʽ�ļ�
		LPCTSTR lpszDest, //��ݷ�ʽ��Ӧ��Ŀ���ļ�
		LPCTSTR lpszWorkDir,//��ʼλ��
		LPCTSTR lpszDescript,//��ݷ�ʽ������
		LPCTSTR lpszCmdlineArguments,//��ݷ�ʽ�����в���
		LPCTSTR lpszIconFile,	//ͼ���ļ�
		const int nIconIdx,		//ͼ������
		const WORD wHotKey,		//��ݼ�
		const int iShowCmd)//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)֮һ
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

		//����Ŀ��
		if (FAILED(pShell->SetPath(lpszDest)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//������ʼλ��
		if (lpszWorkDir && FAILED(pShell->SetWorkingDirectory(lpszWorkDir)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//��������
		if (lpszDescript && FAILED(pShell->SetDescription(lpszDescript)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//���������в���
		if (lpszCmdlineArguments && FAILED(pShell->SetArguments(lpszCmdlineArguments)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//����ͼ��
		if (lpszIconFile && FAILED(pShell->SetIconLocation(lpszIconFile, nIconIdx)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//���ÿ�ݼ�
		if (0 != wHotKey && FAILED(pShell->SetHotkey(wHotKey)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//������ʾ
		if (FAILED(pShell->SetShowCmd(iShowCmd)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//�����ݷ�ʽ
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
		LPCTSTR pszLnkFile,	//��ݷ�ʽ����
		LPCTSTR pszSaveAsLnkFile,	//�ѿ�ݷ�ʽ���Ϊ���ļ�
		LPCTSTR lpszDest, //��ݷ�ʽ��Ӧ��Ŀ���ļ�
		LPCTSTR lpszWorkDir,//��ʼλ��
		LPCTSTR lpszDescript,//��ݷ�ʽ������
		LPCTSTR lpszCmdlineArguments,//��ݷ�ʽ�����в���
		LPCTSTR lpszIconFile,	//ͼ���ļ�
		const int nIconIdx,			//ͼ������
		const WORD wHotKey,			//��ݼ�
		const int iShowCmd)//SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)֮һ
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
		//���ؿ�ݷ�ʽ
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

		//�޸�Ŀ���ļ�
		if (lpszDest && FAILED(pShell->SetPath(lpszDest)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//�޸���ʼλ��
		if (lpszWorkDir && FAILED(pShell->SetWorkingDirectory(lpszWorkDir)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//�޸�����
		if (lpszDescript && FAILED(pShell->SetDescription(lpszDescript)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//�޸������в���
		if (lpszCmdlineArguments && FAILED(pShell->SetArguments(lpszCmdlineArguments)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//�޸�ͼ��
		if (lpszIconFile && FAILED(pShell->SetIconLocation(lpszIconFile, nIconIdx)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//�޸Ŀ�ݼ�
		if (0 != wHotKey && FAILED(pShell->SetHotkey(wHotKey)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//�޸���ʾ��ʽ
		if (FAILED(pShell->SetShowCmd(iShowCmd)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

	#if defined(_UNICODE) || defined(UNICODE)
		//�����ݷ�ʽ
		::memset(wsz, 0, sizeof(WORD) * MAX_PATH);
		if (pszSaveAsLnkFile)
			hRes = pPerFile->Save(pszSaveAsLnkFile, TRUE);
		else
			hRes = pPerFile->Save(pszLnkFile, TRUE);
	#else
		//�����ݷ�ʽ
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
		 LPCTSTR pszLnkFile,	//��ݷ�ʽ�ļ�
		 LPTSTR lpszDest, //��ݷ�ʽ��Ӧ��Ŀ���ļ�
		 LPTSTR lpszWorkDir,//��ʼλ��
		 LPTSTR lpszDescript,//��ݷ�ʽ������
		 LPTSTR lpszCmdlineArguments,//��ݷ�ʽ�����в���
		 LPTSTR lpszIconFile,	//ͼ���ļ�
		 int* nIconIdx,			//ͼ������
		 WORD* wHotKey,			//��ݼ�
		 int* iShowCmd) //SW_SHOW(NORMAL,MAXIMIZED,MINNOACTIVE)֮һ
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

		__try{   // ����crash shell32!LookupItemInHashTable
	#if defined(_UNICODE) || defined(UNICODE)
		//���ؿ�ݷ�ʽ
		if (FAILED(pPerFile->Load(pszLnkFile, STGM_READ)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}
	#else
		//���ؿ�ݷ�ʽ
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

		//��ȡ��ʾ��ʽ
		if (iShowCmd && FAILED(pShell->GetShowCmd(iShowCmd)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//��ȡ��ݼ�
		if (wHotKey && FAILED(pShell->GetHotkey(wHotKey)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//��ȡͼ��
		if (lpszIconFile && nIconIdx
			&& FAILED(pShell->GetIconLocation(lpszIconFile, MAX_PATH, nIconIdx)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//��ȡ�����в���
		if (lpszCmdlineArguments && FAILED(pShell->GetArguments(lpszCmdlineArguments, MAX_PATH)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//��ȡ����
		if (lpszDescript && FAILED(pShell->GetDescription(lpszDescript, MAX_PATH)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//��ȡ����Ŀ¼
		if (lpszWorkDir && FAILED(pShell->GetWorkingDirectory(lpszWorkDir, MAX_PATH)))
		{
			pShell->Release();
			pShell = NULL;
			pPerFile->Release();
			pPerFile = NULL;

			return FALSE;
		}

		//��ȡĿ��
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

		__try{   // ����crash shell32!LookupItemInHashTable
	#if defined(_UNICODE) || defined(UNICODE)
		//���ؿ�ݷ�ʽ
		hr = pFile->Load(pchLinkFile, STGM_READ);
	#else
		//���ؿ�ݷ�ʽ
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
		//���ؿ�ݷ�ʽ
		hr = pFile->Load(pchLinkFile, STGM_READWRITE);
	#else
		//���ؿ�ݷ�ʽ
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