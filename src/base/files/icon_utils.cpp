#include "stdafx.h"
#include "File\IconUtils.h"
#include <shlobj.h>

#define RegualPos(pos) \
do \
{ \
if (pos <= 0) \
pos = 0; \
						else if (pos >= 100) \
						pos = 100; \
						else \
						pos = pos; \
						pos *= 12; \
						if (pos % 100 >= 50) \
						pos = pos / 100 + 1; \
						else \
						pos = pos / 100; \
						if (pos <= 0) \
						pos = 0; \
						else if (pos >= 12) \
						pos = 12; \
						else \
						pos = pos; \
} while (0);

#define LOADBMP(hBmp, nBmp) \
do \
{ \
hBmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(nBmp), \
IMAGE_BITMAP, 0, 0, LR_SHARED); \
if (!hBmp) \
goto lExit0; \
} while (0);

#define DrawPos(pos, hBmp, start) \
do \
{ \
if (pos > 0) \
{ \
hOldTempBmp = (HBITMAP)::SelectObject(hTempDC, hBmp); \
::BitBlt(hMemDC, start, 14 - pos, 2, pos, hTempDC, 0, 12 - pos, SRCCOPY); \
::SelectObject(hTempDC, hOldTempBmp); \
} \
} while (0);

namespace Base {
namespace FileMisc {
  HICON LoadIconWithSize(UINT uResId, int cx, int cy, HINSTANCE hResHandle) {
    if(NULL == hResHandle) {
      hResHandle = GetModuleHandle(NULL);
    }

    return (HICON)::LoadImage(
      hResHandle,
      MAKEINTRESOURCE(uResId),
      IMAGE_ICON,
      cx,
      cy,
      LR_SHARED
    );
  }

  HICON GetIcon(BOOL bBigIcon, UINT uResId) {
    int	cx = 0;
    int	cy = 0;

    if(bBigIcon) {
      cx = cy = 32;
    } else {
      cx = cy = 16;
    }

    return LoadIconWithSize(uResId, cx, cy, NULL);
  }

  HICON GetDefIcon(
    UINT nPos1,
    UINT nPos2,
    UINT nPos3,
    UINT nPos4,
    UINT nBmp1,
    UINT nBmp2,
    UINT nBmp3,
    UINT nBmp4,
    UINT nMaskBmp,
    UINT nIconBmp,
    HINSTANCE hInst
  ) {
    if(0 == nBmp1 || 0 == nBmp2 || 0 == nBmp3 || 0 == nBmp4
        || 0 == nMaskBmp || 0 == nIconBmp
        ) {
      return NULL;
    }



    RegualPos(nPos1);
    RegualPos(nPos2);
    RegualPos(nPos3);
    RegualPos(nPos4);

    HDC hDC = NULL, hMemDC = NULL, hTempDC = NULL;
    HBITMAP hBmpMask = NULL, hBitmap = NULL, hOldBmp = NULL, hOldTempBmp = NULL;
    ICONINFO iInfo = { TRUE, 0l, 0l, NULL, NULL };
    HICON hIcon = NULL;
    HBITMAP hBmp1 = NULL, hBmp2 = NULL, hBmp3 = NULL, hBmp4 = NULL;

    hDC = ::GetDC(NULL);
    if(NULL == hDC) {
      goto lExit0;
    }

    hMemDC = ::CreateCompatibleDC(hDC);
    if(NULL == hMemDC) {
      goto lExit0;
    }

    hTempDC = ::CreateCompatibleDC(hMemDC);
    if(NULL == hTempDC) {
      goto lExit0;
    }

    LOADBMP(hBmpMask, nMaskBmp);
    LOADBMP(hBitmap, nIconBmp);
    LOADBMP(hBmp1, nBmp1);
    LOADBMP(hBmp2, nBmp2);
    LOADBMP(hBmp3, nBmp3);
    LOADBMP(hBmp4, nBmp4);

    hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBitmap);
    if(NULL == hOldBmp) {
      goto lExit0;
    }

    DrawPos(nPos1, hBmp1, 2);
    DrawPos(nPos2, hBmp2, 5);
    DrawPos(nPos3, hBmp3, 8);
    DrawPos(nPos4, hBmp4, 11);

    ::SelectObject(hMemDC, hOldBmp);
    hOldBmp = NULL;

    iInfo.hbmMask = hBmpMask;
    iInfo.hbmColor = hBitmap;
    hIcon = ::CreateIconIndirect(&iInfo);

  lExit0:

    if(hBmp1) {
      ::DeleteObject(hBmp1);
    }

    if(hBmp2) {
      ::DeleteObject(hBmp2);
    }

    if(hBmp3) {
      ::DeleteObject(hBmp3);
    }

    if(hBmp4) {
      ::DeleteObject(hBmp4);
    }

    hBmp1 = hBmp2 = hBmp3 = hBmp4 = NULL;

    if(hOldBmp) {
      ::SelectObject(hMemDC, hOldBmp);
    }

    if(hBmpMask) {
      ::DeleteObject(hBmpMask);
    }

    if(hBitmap) {
      ::DeleteObject(hBitmap);
    }

    hBmpMask = hBitmap = NULL;

    hOldTempBmp = hOldBmp = NULL;

    if(hTempDC) {
      ::DeleteDC(hTempDC);
    }

    if(hMemDC) {
      ::DeleteDC(hMemDC);
    }

    if(hDC) {
      ::ReleaseDC(NULL, hDC);
    }

    hDC = hMemDC = hTempDC = NULL;
    return hIcon;
  }

}

}
