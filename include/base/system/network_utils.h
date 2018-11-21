// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_SYSTEM_NETWORK_UTILS_H_
#define BASE_SYSTEM_NETWORK_UTILS_H_

#include "base/base_export.h"

namespace base {
namespace sysmisc {
/**
* @name 网络
* @{
*/
/**
* @brief 是否连接网络
* @return 是否连接
*/
BOOL BASE_EXPORT IsNetWorkOK();

/**
* @brief 是否连接网络
* @return 是否连接
* @see BOOL IsNetWorkOK();
* @note 同IsNetWorkOK()代码完全相同
*/
BOOL BASE_EXPORT IsInternetOK();
}  // namespace sysmisc
}  // namespace base
#endif  // BASE_SYSTEM_NETWORK_UTILS_H_
