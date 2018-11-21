// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SYSTEM_NETWORK_UTILS_H_
#define BASE_SYSTEM_NETWORK_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace sysmisc {
/**
* @name ����
* @{
*/
/**
* @brief �Ƿ���������
* @return �Ƿ�����
*/
BOOL BASE_EXPORT IsNetWorkOK();

/**
* @brief �Ƿ���������
* @return �Ƿ�����
* @see BOOL IsNetWorkOK();
* @note ͬIsNetWorkOK()������ȫ��ͬ
*/
BOOL BASE_EXPORT IsInternetOK();
}  // namespace sysmisc
}  // namespace base
#endif  // BASE_SYSTEM_NETWORK_UTILS_H_
