// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_IMAGE_UTILS_H_
#define BASE_FILES_IMAGE_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
/**
* @brief RC 资源图片ID 获取图片的宽度和高度
* @param[in]  nID 资源图片ID
* @param[out] px  图片的宽度
* @param[out] py  图片的高度
* @return 是否成功获取
*/
BOOL BASE_EXPORT GetBitmapSize(UINT nID, long* px = NULL, long* py = NULL);

/**
* @brief 图片句柄获取图片的长度和宽度
* @param[in]  hBmp 图片句柄
* @param[out] px   图片的宽度
* @param[out] py   图片的高度
* @return 是否成功获取
*/
BOOL BASE_EXPORT GetBitmapSize(HBITMAP hBmp, long* px = NULL, long* py = NULL);

}//namespace filemisc
}//namespace base
#endif  // BASE_FILES_IMAGE_UTILS_H_
