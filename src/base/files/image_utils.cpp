// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/files/image_utils.h"

namespace base {
namespace filemisc {
BOOL GetBitmapSize(UINT nID, long* px, long* py) {
  HBITMAP hBmp = ::LoadBitmap(::GetModuleHandle(NULL), MAKEINTRESOURCE(nID));
  if (NULL == hBmp) {
    return FALSE;
  }

  BITMAP bmpInfo = { 0 };
  ::GetObject(hBmp, sizeof(bmpInfo), &bmpInfo);
  ::DeleteObject(hBmp);
  hBmp = NULL;
  if (px) {
    *px = bmpInfo.bmWidth;
  }

  if (py) {
    *py = bmpInfo.bmHeight;
  }

  return TRUE;
}

BOOL GetBitmapSize(HBITMAP hBmp, long* px, long* py) {
  BITMAP bmpInfo = { 0 };

  if (NULL == hBmp) {
    return FALSE;
  }

  ::GetObject(hBmp, sizeof(bmpInfo), &bmpInfo);
  if (px) {
    *px = bmpInfo.bmWidth;
  }

  if (py) {
    *py = bmpInfo.bmHeight;
  }

  return TRUE;
}
}  // namespace filemisc
}  // namespace base
