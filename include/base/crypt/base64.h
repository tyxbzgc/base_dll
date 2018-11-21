// Copyright (c) 2017 The TAL Authors.
// All rights reserved..

#ifndef BASE_CRYPT_BASE_64_H_
#define BASE_CRYPT_BASE_64_H_

#include "base/base_export.h"

namespace base {
namespace crypt {
BASE_EXPORT std::string base64_encode(unsigned char const*, unsigned int len);
BASE_EXPORT std::string base64_decode(std::string const& s);
}  // namespace crypt
}  // namespace base
#endif
