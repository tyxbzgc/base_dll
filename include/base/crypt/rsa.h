// Copyright (c) 2017 The TAL Authors.
// All rights reserved..

#ifndef BASE_CRYPT_RSA_H_
#define BASE_CRYPT_RSA_H_

#include "base/base_export.h"
#include <string.h>

#define RSA_KEYLEN 2048
#define AES_KEYLEN 256
#define AES_ROUNDS 6

#define PSEUDO_CLIENT

//#define USE_PBKDF

#define SUCCESS 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
#define KEY_CLIENT_PUB 2
#define KEY_AES        3
#define KEY_AES_IV     4

namespace base {
namespace crypt {
int BASE_EXPORT RsaEncrypt(const char *in, int inLen, char *out, int * outLen);
int BASE_EXPORT RsaEncrypt(const char* key, const char *in, const int inLen,
                           char *out,
                           int * outLen);
int BASE_EXPORT RsaDecrypt(const char *in, const int inLen, char *out,
                           int * outLen);
int BASE_EXPORT RsaDecrypt(const char* key, const char *in, const int inLen,
                           char *out,
                           int * outLen);
int BASE_EXPORT AesEncrypt(const unsigned char *message, size_t messageLength,
                           unsigned char **encryptedMessage);
int BASE_EXPORT AesDecrypt(unsigned char *encryptedMessage,
                           size_t encryptedMessageLength,
                           unsigned char **decryptedMessage);

int BASE_EXPORT GetRemotePublicKey(unsigned char **publicKey);
int BASE_EXPORT SetRemotePublicKey(unsigned char *publicKey,
                                   size_t publicKeyLength);

int BASE_EXPORT GetLocalPublicKey(unsigned char **publicKey);
int BASE_EXPORT GetLocalPrivateKey(unsigned char **privateKey);

int BASE_EXPORT GetAesKey(unsigned char **aesKey);
int BASE_EXPORT SetAesKey(unsigned char *aesKey, size_t aesKeyLen);

int BASE_EXPORT GetAesIv(unsigned char **aesIv);
int BASE_EXPORT SetAesIv(unsigned char *aesIv, size_t aesIvLen);

int BASE_EXPORT WriteKeyToFile(std::string _path, int _key);
//zgc 20180410 add
int BASE_EXPORT GenerateRsaKeypair();
int BASE_EXPORT AesEncryptFile(char* file_path);
}  // namespace crypt
}  // namespace base
#endif  // BASE_CRYPT_RSA_H_
