// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_CRYPT_CRYPT_DATA_H_
#define BASE_CRYPT_CRYPT_DATA_H_

#include <windows.h>

#include "base/base_export.h"
#include "base/basic_types.h"

#define MD5_DIGEST_LENGTH 16
#define ENCRYPT_PADLEN    18
#define CRYPT_KEY_SIZE    16

namespace base {
namespace crypt {
/**
* @name TEA加解密算法_ECB模式
* @{
*/
/**
* @brief TEA加密算法
* @param[in] pInBuf 待加密明文，8字节即64位
* @param[in] pKey 密钥，16字节即128位
* @param[out] pOutBuf 加密后的密文，8字节即64位
*/
void BASE_EXPORT TeaEncryptECB(const BYTE *pInBuf,
  const BYTE *pKey,
  BYTE *pOutBuf);
/**
* @brief TEA解密算法
* @param[in] pInBuf 待解密密文，8字节即64位
* @param[in] pKey 密钥，16字节即128位
* @param[out] pOutBuf 解密后的明文，8字节即64位
*/
void BASE_EXPORT TeaDecryptECB(const BYTE *pInBuf,
  const BYTE *pKey,
  BYTE *pOutBuf);

/*@}*/


/**
* @name TEA加解密算法_CBC模式
* @{
*/

/**
* @brief CBC模式TEA加密算法
* @param[in] pInBuf 待加密明文
* @param[in] nInBufLen pInBuf长度
* @param[in] pKey 密钥，16字节即128位
* @param[out] pOutBuf 加密后的密文
* @param[out] pOutBufLen pOutBuf的长度,是8Byte的倍数,至少应预留nInBufLen+17
* @note 密文格式:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
*/
void BASE_EXPORT TeaEncryptCBC(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @brief CBC模式TEA解密算法
* @param[in] pInBuf 待解密密文
* @param[in] nInBufLen pInBuf长度,是8Byte的倍数
* @param[in] pKey 密钥，16字节即128位
* @param[out] pOutBuf 存储解密后的明文的缓冲区
* @param[in][out] pOutBufLen 传入时预置接收缓冲区的长度；
* @返回明文pOutBuf的长度，至少应预留nInBufLen-10
* @retval TRUE 格式正确
* @retval FALSE 格式错误
* @note 密文格式:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
*/
BOOL BASE_EXPORT TeaDecryptCBC(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @brief 计算CBC模式TEA加密后密文长度
* @param[in] nInBufLen 需加密的明文部分(Body)长度
* @return 加密后的长度(是8Byte的倍数)
* @note 密文格式:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)ee
* @note TeaEncryptCBC2Len同BDMTeaEnLen
*/
int BASE_EXPORT TeaEncryptCBC2Len(int nInBufLen);

/**
* @brief 计算CBC模式TEA解密后明文长度
* @param[in] pInBuf 源数据
* @param[in] nInBufLen 要解密的文本的长度
* @param[in] pKey  口令
* @return 解密后的长度
* @note TeaDecryptCBC2Len同BDMTeaDeLen
*/
int BASE_EXPORT TeaDecryptCBC2Len(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey);

/**
* @brief CBC模式TEA加密算法
* @param[in] pInBuf 待加密明文
* @param[in] nInBufLen pInBuf长度
* @param[in] pKey 密钥，16字节即128位
* @param[out] pOutBuf 加密后的密文
* @param[out] pOutBufLen pOutBuf的长度,是8Byte的倍数,至少应预留nInBufLen+17
* @note 密文格式:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
* @note TeaEncryptCBC2同BDMTeaEn
*/
void BASE_EXPORT TeaEncryptCBC2(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @brief CBC模式TEA解密算法
* @param[in] pInBuf 待解密密文
* @param[in] nInBufLen pInBuf长度,是8Byte的倍数
* @param[in] pKey 密钥，16字节即128位
* @param[out] pOutBuf 存储解密后的明文的缓冲区
* @param[in][out] pOutBufLen 传入时预置接收缓冲区的长度；
* @返回明文pOutBuf的长度，至少应预留nInBufLen-10
* @retval TRUE 格式正确
* @retval FALSE 格式错误
* @note 密文格式:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
* @note TeaDecryptCBC2同BDMTeaDe
*/
BOOL BASE_EXPORT TeaDecryptCBC2(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @name TEA加解密算法_4bytes单位
* @{
*/
// Add by Link, 支持4 bytes单位加密的tea
/**
* @brief 4字节单位TEA加密算法
* @param[in][out] sData 4字节数据
* @param[in] sKey 8字节密钥
*/
void BASE_EXPORT TeaEncrypt4bytesAFrame(short *sData, short *sKey);

/**
* @brief 4字节单位TEA解密算法
* @param[in][out] sData 4字节数据
* @param[in] sKey 8字节密钥
*/
void BASE_EXPORT TeaDecrypt4bytesAFrame(short *sData, short *sKey);

/**
* @name RC4加解密算法
* @{
*/
/**
* @brief 加密数据
* @param[in] pRC4Key  口令
* @param[in] nKeyLen  口令字节长度
* @param[out] pCipher  加密后的数据
* @param[in] nMaxSize pCipher最大长度
* @param[in] pMessage 源数据
* @param[in] nMessageLength 源数据长度
* @retval >=0 参数正确
* @retval <0 参数不正确
*/
int BASE_EXPORT __stdcall RC4Encrypt(const BYTE *pRC4Key,
  int nKeyLen,
  BYTE *pCipher,
  int nMaxSize,
  const BYTE *pMessage,
  int nMessageLength);

/**
* @brief 解密数据
* @param[in] pRC4Key  口令
* @param[in] nKeyLen  口令字节长度
* @param[out] pMessage 解密后的数据
* @param[in] nMaxSize pMessage最大长度
* @param[in] pCipher  源数据
* @param[in] nCipherLength 源数据长度
* @retval >=0 参数正确
* @retval <0 参数不正确
*/
int BASE_EXPORT __stdcall RC4Decrypt(const BYTE *pRC4Key,
  int nKeyLen,
  BYTE *pMessage,
  int nMaxSize,
  const BYTE *pCipher,
  int nCipherLength);
}  // namespace crypt
}  // namespace base
#endif
