// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#include "base/crypt/rc4.h"

namespace base {
int RC4_Encrypt(const unsigned char *pRC4Key,
  int nKeyLen,
  unsigned char *pCipher,
  int nMaxSize,
  const unsigned char *pMessage,
  int nMessageLength) {
  int i, j, k, t;
  unsigned char S[256], K[256];
  unsigned char uc;

  if (!pRC4Key) return -1;
  if (nKeyLen <= 0) return -2;
  if (!pMessage) return -3;
  if (nMaxSize <=0 ) return -4;
  if (!pCipher) return -5;
  if (nMessageLength <= 0) return -6;
  if (nMaxSize < nMessageLength) return -7;

  // Initialize S[] and K[]
  k = 0;
  for (i = 0; i < 256; i++) {
    S[i] = (unsigned char)i;
    K[i] = pRC4Key[k];
    k = (k+1) % nKeyLen;
  }

  j = 0;
  for (i = 0; i < 256; i++) {
    j = (j + static_cast<int>(S[i]) + static_cast<int>(K[i])) % 256;
    uc = S[j];
    S[j] = S[i];
    S[i] = uc;
  }

  // Encrypt
  i = j = 0;
  for (k = 0; k < nMessageLength; k++) {
    i = (i + 1) % 256;
    j = (j + static_cast<int>(S[i])) % 256;
    uc = S[j];
    S[j] = S[i];
    S[i] = uc;
    t = (static_cast<int>(S[i]) + static_cast<int>(S[j])) % 256;
    pCipher[k] = S[t] ^ pMessage[k];
  }
  return 0;
}

int RC4_Decrypt(const unsigned char *pRC4Key,
  int nKeyLen,
  unsigned char *pMessage,
  int nMaxSize,
  const unsigned char *pCipher,
  int nCipherLength) {
  int i, j, k, t;
  unsigned char S[256], K[256];
  unsigned char uc;

  if (!pRC4Key) return -1;
  if (nKeyLen <= 0) return -2;
  if (!pMessage) return -3;
  if (nMaxSize <= 0) return -4;
  if (!pCipher) return -5;
  if (nCipherLength <= 0) return -6;
  if (nMaxSize < nCipherLength) return -7;

  // Initialize S[] and K[]
  k = 0;
  for (i = 0; i < 256; i++) {
    S[i] = (unsigned char)i;
    K[i] = pRC4Key[k];
    k = (k + 1) % nKeyLen;
  }

  j = 0;
  for (i = 0; i < 256; i++) {
    j = (j + static_cast<int>(S[i]) + static_cast<int>(K[i])) % 256;
    uc = S[j];
    S[j] = S[i];
    S[i] = uc;
  }

  // Decrypt. (same as Encrypt)
  i = j = 0;
  for (k = 0; k < nCipherLength; k++) {
    i = (i + 1) % 256;
    j = (j + static_cast<int>(S[i])) % 256;
    uc = S[j];
    S[j] = S[i];
    S[i] = uc;
    t = (static_cast<int>(S[i]) + static_cast<int>(S[j])) % 256;
    pMessage[k] = S[t] ^ pCipher[k];
  }
  return 0;
}
}  // namespace base
