// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_FILES_IMAGE_UTILS_H_
#define BASE_FILES_IMAGE_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace filemisc {
/**
* @brief RC ��ԴͼƬID ��ȡͼƬ�Ŀ�Ⱥ͸߶�
* @param[in]  nID ��ԴͼƬID
* @param[out] px  ͼƬ�Ŀ��
* @param[out] py  ͼƬ�ĸ߶�
* @return �Ƿ�ɹ���ȡ
*/
BOOL BASE_EXPORT GetBitmapSize(UINT nID, long* px = NULL, long* py = NULL);

/**
* @brief ͼƬ�����ȡͼƬ�ĳ��ȺͿ��
* @param[in]  hBmp ͼƬ���
* @param[out] px   ͼƬ�Ŀ��
* @param[out] py   ͼƬ�ĸ߶�
* @return �Ƿ�ɹ���ȡ
*/
BOOL BASE_EXPORT GetBitmapSize(HBITMAP hBmp, long* px = NULL, long* py = NULL);

}//namespace filemisc
}//namespace base
#endif  // BASE_FILES_IMAGE_UTILS_H_
