// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_CRYPT_RC4_H_
#define BASE_CRYPT_RC4_H_

#include "base/base_export.h"
#include "base/basic_types.h"

namespace base {
BASE_EXPORT int RC4_Encrypt(const unsigned char *pRC4Key,
                            int nKeyLen,
                            unsigned char *pCipher,
                            int nMaxSize,
                            const unsigned char *pMessage,
                            int nMessageLength);
BASE_EXPORT int RC4_Decrypt(const unsigned char *pRC4Key,
                            int nKeyLen,
                            unsigned char *pMessage,
                            int nMaxSize,
                            const unsigned char *pCipher,
                            int nCipherLength);
}  // namespace base
#endif  // BASE_CRYPT_RC4_H_
