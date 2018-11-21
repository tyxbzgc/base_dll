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
* @name TEA�ӽ����㷨_ECBģʽ
* @{
*/
/**
* @brief TEA�����㷨
* @param[in] pInBuf ���������ģ�8�ֽڼ�64λ
* @param[in] pKey ��Կ��16�ֽڼ�128λ
* @param[out] pOutBuf ���ܺ�����ģ�8�ֽڼ�64λ
*/
void BASE_EXPORT TeaEncryptECB(const BYTE *pInBuf,
  const BYTE *pKey,
  BYTE *pOutBuf);
/**
* @brief TEA�����㷨
* @param[in] pInBuf ���������ģ�8�ֽڼ�64λ
* @param[in] pKey ��Կ��16�ֽڼ�128λ
* @param[out] pOutBuf ���ܺ�����ģ�8�ֽڼ�64λ
*/
void BASE_EXPORT TeaDecryptECB(const BYTE *pInBuf,
  const BYTE *pKey,
  BYTE *pOutBuf);

/*@}*/


/**
* @name TEA�ӽ����㷨_CBCģʽ
* @{
*/

/**
* @brief CBCģʽTEA�����㷨
* @param[in] pInBuf ����������
* @param[in] nInBufLen pInBuf����
* @param[in] pKey ��Կ��16�ֽڼ�128λ
* @param[out] pOutBuf ���ܺ������
* @param[out] pOutBufLen pOutBuf�ĳ���,��8Byte�ı���,����ӦԤ��nInBufLen+17
* @note ���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
*/
void BASE_EXPORT TeaEncryptCBC(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @brief CBCģʽTEA�����㷨
* @param[in] pInBuf ����������
* @param[in] nInBufLen pInBuf����,��8Byte�ı���
* @param[in] pKey ��Կ��16�ֽڼ�128λ
* @param[out] pOutBuf �洢���ܺ�����ĵĻ�����
* @param[in][out] pOutBufLen ����ʱԤ�ý��ջ������ĳ��ȣ�
* @��������pOutBuf�ĳ��ȣ�����ӦԤ��nInBufLen-10
* @retval TRUE ��ʽ��ȷ
* @retval FALSE ��ʽ����
* @note ���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
*/
BOOL BASE_EXPORT TeaDecryptCBC(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @brief ����CBCģʽTEA���ܺ����ĳ���
* @param[in] nInBufLen ����ܵ����Ĳ���(Body)����
* @return ���ܺ�ĳ���(��8Byte�ı���)
* @note ���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)ee
* @note TeaEncryptCBC2LenͬBDMTeaEnLen
*/
int BASE_EXPORT TeaEncryptCBC2Len(int nInBufLen);

/**
* @brief ����CBCģʽTEA���ܺ����ĳ���
* @param[in] pInBuf Դ����
* @param[in] nInBufLen Ҫ���ܵ��ı��ĳ���
* @param[in] pKey  ����
* @return ���ܺ�ĳ���
* @note TeaDecryptCBC2LenͬBDMTeaDeLen
*/
int BASE_EXPORT TeaDecryptCBC2Len(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey);

/**
* @brief CBCģʽTEA�����㷨
* @param[in] pInBuf ����������
* @param[in] nInBufLen pInBuf����
* @param[in] pKey ��Կ��16�ֽڼ�128λ
* @param[out] pOutBuf ���ܺ������
* @param[out] pOutBufLen pOutBuf�ĳ���,��8Byte�ı���,����ӦԤ��nInBufLen+17
* @note ���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
* @note TeaEncryptCBC2ͬBDMTeaEn
*/
void BASE_EXPORT TeaEncryptCBC2(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @brief CBCģʽTEA�����㷨
* @param[in] pInBuf ����������
* @param[in] nInBufLen pInBuf����,��8Byte�ı���
* @param[in] pKey ��Կ��16�ֽڼ�128λ
* @param[out] pOutBuf �洢���ܺ�����ĵĻ�����
* @param[in][out] pOutBufLen ����ʱԤ�ý��ջ������ĳ��ȣ�
* @��������pOutBuf�ĳ��ȣ�����ӦԤ��nInBufLen-10
* @retval TRUE ��ʽ��ȷ
* @retval FALSE ��ʽ����
* @note ���ĸ�ʽ:PadLen(1byte)+Padding(var,0-7byte)
*               +Salt(2byte)+Body(var byte)+Zero(7byte)
* @note TeaDecryptCBC2ͬBDMTeaDe
*/
BOOL BASE_EXPORT TeaDecryptCBC2(const BYTE* pInBuf,
  int nInBufLen,
  const BYTE* pKey,
  BYTE* pOutBuf,
  int *pOutBufLen);

/**
* @name TEA�ӽ����㷨_4bytes��λ
* @{
*/
// Add by Link, ֧��4 bytes��λ���ܵ�tea
/**
* @brief 4�ֽڵ�λTEA�����㷨
* @param[in][out] sData 4�ֽ�����
* @param[in] sKey 8�ֽ���Կ
*/
void BASE_EXPORT TeaEncrypt4bytesAFrame(short *sData, short *sKey);

/**
* @brief 4�ֽڵ�λTEA�����㷨
* @param[in][out] sData 4�ֽ�����
* @param[in] sKey 8�ֽ���Կ
*/
void BASE_EXPORT TeaDecrypt4bytesAFrame(short *sData, short *sKey);

/**
* @name RC4�ӽ����㷨
* @{
*/
/**
* @brief ��������
* @param[in] pRC4Key  ����
* @param[in] nKeyLen  �����ֽڳ���
* @param[out] pCipher  ���ܺ������
* @param[in] nMaxSize pCipher��󳤶�
* @param[in] pMessage Դ����
* @param[in] nMessageLength Դ���ݳ���
* @retval >=0 ������ȷ
* @retval <0 ��������ȷ
*/
int BASE_EXPORT __stdcall RC4Encrypt(const BYTE *pRC4Key,
  int nKeyLen,
  BYTE *pCipher,
  int nMaxSize,
  const BYTE *pMessage,
  int nMessageLength);

/**
* @brief ��������
* @param[in] pRC4Key  ����
* @param[in] nKeyLen  �����ֽڳ���
* @param[out] pMessage ���ܺ������
* @param[in] nMaxSize pMessage��󳤶�
* @param[in] pCipher  Դ����
* @param[in] nCipherLength Դ���ݳ���
* @retval >=0 ������ȷ
* @retval <0 ��������ȷ
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
