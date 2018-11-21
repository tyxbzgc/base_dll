// Copyright (c) 2017 The TAL Authors.
// All rights reserved..

#include "base/crypt/rsa.h"
#include <io.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "base/common.h"
#include<fstream>
using namespace std;
namespace base {
namespace crypt {
class Rsa {
  public:
    static Rsa* GetInstance() {
        if(instance == nullptr) {
            instance = new Rsa();
        }

        return instance;
    }

  private:
    static Rsa* instance;
    Rsa() {
        // localKeypair = NULL;
        // remotePublicKey = NULL;
        init();
    }

    Rsa(unsigned char *remotePublicKey, size_t remotePublicKeyLength) {
        //localKeypair = NULL;
        //this->remotePublicKey = NULL;

        setRemotePublicKey(remotePublicKey, remotePublicKeyLength);
        init();
    }

    ~Rsa() {
        //EVP_PKEY_free(remotePublicKey);
        if (server_public != nullptr) {
            BIO_free_all(server_public);
        }

        if (local_private != nullptr) {
            BIO_free_all(local_private);
        }

        if(local_public != nullptr) {
            BIO_free_all(local_public);
        }

        EVP_CIPHER_CTX_free(rsaEncryptContext);
        EVP_CIPHER_CTX_free(aesEncryptContext);

        EVP_CIPHER_CTX_free(rsaDecryptContext);
        EVP_CIPHER_CTX_free(aesDecryptContext);

        free(aesKey);
        free(aesIv);
    }

  public:
    unsigned char* rsaEncrypt(RSA *pubKey, const unsigned char* msg, int msg_len,
                              int *enc_len) {
        if (pubKey == NULL) {
            return nullptr;
        }

        int rsa_size = RSA_size(pubKey);
        int block_size = rsa_size - 42;
        int blocks = msg_len / block_size;
        int rest = msg_len % block_size;
        unsigned char* enc = 0;
        int curr_len = 0;
        int i = 0;
        if (0 == rest) {
            enc = (unsigned char*)malloc(blocks*rsa_size + 1);
        } else {
            enc = (unsigned char*)malloc((blocks + 1)*rsa_size + 1);
        }

        for (i = 0; i < blocks; i++) {
            std::string temp = (char*)msg + i*block_size;
            if (0 > (curr_len = RSA_public_encrypt(block_size, msg + i*block_size,
                                                   enc + i*rsa_size, pubKey, RSA_PKCS1_OAEP_PADDING))) {
                printf("ERROR: RSA_public_encrypt: %s\n", ERR_error_string(ERR_get_error(),
                        NULL));
            }
            *enc_len += curr_len;
        }
        if (0 != rest) {
            std::string temp = (char*)msg + i*block_size + 32;
            if (0 > (curr_len = RSA_public_encrypt(rest, msg + i*block_size,
                                                   enc + i*rsa_size, pubKey, RSA_PKCS1_OAEP_PADDING))) {
                printf("ERROR: RSA_public_encrypt: %s\n", ERR_error_string(ERR_get_error(),
                        NULL));
            }
            *enc_len += curr_len;
        }
        if (*enc_len == -1)
            printf("ERROR: RSA_public_encrypt: %s\n", ERR_error_string(ERR_get_error(),
                    NULL));
        return enc;
    }
    unsigned char* rsaDecrypt(RSA *privKey, const unsigned char* msg, int msg_len,
                              int *enc_len) {
        int rsa_size = RSA_size(privKey);
        int block_size = rsa_size;
        int blocks = msg_len / block_size;
        int rest = msg_len % block_size;
        unsigned char* enc = 0;
        int curr_len = 0;
        int i = 0;
        *enc_len = 0;
        if (0 == rest) {
            enc = (unsigned char*)malloc(blocks*rsa_size + 1);
        } else {
            enc = (unsigned char*)malloc((blocks + 1)*rsa_size + 1);
        }
        for (i = 0; i < blocks; i++) {
            if (0 > (curr_len = RSA_private_decrypt(block_size, msg + i*block_size,
                                                    enc + (*enc_len), privKey, RSA_PKCS1_OAEP_PADDING))) {
                printf("ERROR: RSA_public_encrypt: %s\n", ERR_error_string(ERR_get_error(),
                        NULL));
                OutputDebugStringA(ERR_error_string(ERR_get_error(), NULL));
                return enc;
            }
            *enc_len += curr_len;
        }
        if (0 != rest) {
            if (0 > (curr_len = RSA_private_decrypt(rest, msg + i*block_size,
                                                    enc + i*rsa_size, privKey, RSA_PKCS1_OAEP_PADDING))) {
                printf("ERROR: RSA_public_encrypt: %s\n", ERR_error_string(ERR_get_error(),
                        NULL));
                return enc;
            }
            enc_len += curr_len;
        }
        if (*enc_len == -1)
            printf("ERROR: RSA_public_encrypt: %s\n", ERR_error_string(ERR_get_error(),
                    NULL));
        return enc;
    }

    int enc(const char *in, int inLen, char *out, int * outLen) {
        try {
            std::string file = file_path + RSA_SERVER_PUBLICKEY_FILE;
            BIO *pBio = BIO_new_file(file.c_str(), "r");
            char buf[1024] = { 0 };
            RSA *pRsa = PEM_read_bio_RSAPublicKey(pBio, NULL, NULL, NULL);
            BIO_free_all(pBio);
            char* enc = (char*)rsaEncrypt(pRsa, (unsigned char*)in, inLen, outLen);
            if (enc == nullptr) {
                return -1;
            }

            RSA_free(pRsa);
            memcpy(out, enc, *outLen);
            if(NULL != enc) {
                free(enc);
                enc = NULL;
            }
            if(*outLen >= 0)
                return 0;
            return -1;
        } catch (...) {
            return -1;
        }
    }

    int enc(const char *key, const char *in, int inLen, char *out, int * outLen) {
        try {
            BIO * pBio = BIO_new_mem_buf(key, strlen(key));
            char buf[1024] = { 0 };
            RSA *pRsa = PEM_read_bio_RSAPublicKey(pBio, NULL, NULL, NULL);
            BIO_free_all(pBio);
            char* enc = (char*)rsaEncrypt(pRsa, (unsigned char*)in, inLen, outLen);
            if(enc == nullptr) {
                return -1;
            }

            RSA_free(pRsa);
            memcpy(out, enc, *outLen);
            if(NULL != enc) {
                free(enc);
                enc = NULL;
            }
            if(*outLen >= 0)
                return 0;
            return -1;
        } catch(...) {
            return -1;
        }
    }

    int dec(const char* key, const char *in, const int inLen, char *out,
            int * outLen) {
        BIO * pBio = BIO_new_mem_buf(key, strlen(key));
        RSA *pRsa = PEM_read_bio_RSAPrivateKey(pBio, NULL, NULL, NULL);
        BIO_free_all(pBio);
        char* dec = (char*)rsaDecrypt(pRsa, (unsigned char *)in, inLen, outLen);
        memcpy(out, dec, *outLen);
        if (NULL != dec) {
            free(dec);
            dec = NULL;
        }
        if (*outLen >= 0)
            return 0;
        return -1;
    }

    int dec(const char *in, const int inLen, char *out, int * outLen) {
        std::string file = file_path + RSA_LOCAL_PRIVATEEY_FILE;
        BIO *pBio = BIO_new_file(file.c_str(), "r");
        RSA *pRsa = PEM_read_bio_RSAPrivateKey(pBio, NULL, NULL, NULL);
        BIO_free_all(pBio);
        char* dec = (char*)rsaDecrypt(pRsa, (unsigned char *)in, inLen, outLen);
        memcpy(out, dec, *outLen);
        if (NULL != dec) {
            free(dec);
            dec = NULL;
        }
        if (*outLen >= 0)
            return 0;
        return -1;
    }

    int aesEncrypt(const unsigned char *message, size_t messageLength,
                   unsigned char **encryptedMessage) {
        // Allocate memory for everything
        size_t blockLength = 0;
        size_t encryptedMessageLength = 0;

        *encryptedMessage = (unsigned char*)malloc(messageLength + AES_BLOCK_SIZE);
        if(encryptedMessage == NULL) {
            return FAILURE;
        }

        // Encrypt it!
        if(!EVP_EncryptInit_ex(aesEncryptContext, EVP_aes_256_cbc(), NULL, aesKey,
                               aesIv)) {
            return FAILURE;
        }

        if(!EVP_EncryptUpdate(aesEncryptContext, *encryptedMessage, (int*)&blockLength,
                              (unsigned char*)message, messageLength)) {
            return FAILURE;
        }
        encryptedMessageLength += blockLength;

        if(!EVP_EncryptFinal_ex(aesEncryptContext,
                                *encryptedMessage + encryptedMessageLength, (int*)&blockLength)) {
            return FAILURE;
        }

        return encryptedMessageLength + blockLength;
    }

    int aesDecrypt(unsigned char *encryptedMessage,
                   size_t encryptedMessageLength, unsigned char **decryptedMessage) {
        // Allocate memory for everything
        size_t decryptedMessageLength = 0;
        size_t blockLength = 0;

        *decryptedMessage = (unsigned char*)malloc(encryptedMessageLength);
        if(*decryptedMessage == NULL) {
            return FAILURE;
        }

        // Decrypt it!
        if(!EVP_DecryptInit_ex(aesDecryptContext, EVP_aes_256_cbc(), NULL, aesKey,
                               aesIv)) {
            return FAILURE;
        }

        if(!EVP_DecryptUpdate(aesDecryptContext, (unsigned char*)*decryptedMessage,
                              (int*)&blockLength, encryptedMessage, (int)encryptedMessageLength)) {
            return FAILURE;
        }
        decryptedMessageLength += blockLength;

        if(!EVP_DecryptFinal_ex(aesDecryptContext,
                                (unsigned char*)*decryptedMessage + decryptedMessageLength,
                                (int*)&blockLength)) {
            return FAILURE;
        }
        decryptedMessageLength += blockLength;

        (*decryptedMessage)[decryptedMessageLength] = '\0';

        return (int)decryptedMessageLength;
    }

    int getRemotePublicKey(unsigned char **publicKey) {
#if 0
        if (server_public == nullptr) {
            std::string file = file_path + RSA_SERVER_PUBLICKEY_FILE;
            server_public = BIO_new_file(file.c_str(), "r");
        }
        return bioToString(server_public, publicKey);
#else
        std::string file_public = file_path + RSA_SERVER_PUBLICKEY_FILE;
        std::ifstream if_temp(file_public.c_str());
        std::ostringstream os_stream;
        os_stream << if_temp.rdbuf();
        std::string szcontent = os_stream.str();
        *publicKey = (unsigned char*)malloc(szcontent.length() + 1);
        memset(*publicKey, 0, szcontent.length() + 1);
        strcpy((char*)*publicKey, szcontent.c_str());
        return szcontent.length();
#endif
    }

    int setRemotePublicKey(unsigned char *publicKey,
                           size_t publicKeyLength) {
        if(BIO_write(server_public, publicKey,
                     publicKeyLength) != (int)publicKeyLength) {
            return FAILURE;
        }

        if(if_write_to_file == true) {
            std::string file = file_path + RSA_SERVER_PUBLICKEY_FILE;
            BIO *pBio = server_public;

            if(pBio == NULL) {
                printf("BIO_new_file = %s  error", file.c_str());
                return -2;
            }
            /*if(PEM_write_bio_RSAPublicKey(pBio, pRsa) == 0) {
                printf("write public key error");
                return -3;
            }*/
        }

        return SUCCESS;
    }

    int getLocalPublicKey(unsigned char **publicKey) {
#if 0
        if (local_public == nullptr) {
            return -1;
        }
        return bioToString(local_public, publicKey);
#else
        std::string file_public = file_path + RSA_LOCAL_PUBLIC_FILE;
        std::ifstream if_temp(file_public.c_str());
        std::ostringstream os_stream;
        os_stream << if_temp.rdbuf();
        std::string szcontent = os_stream.str();
        *publicKey = (unsigned char*)malloc(szcontent.length() + 1);
        memset(*publicKey, 0, szcontent.length() + 1);
        strcpy((char*)*publicKey, szcontent.c_str());
        return szcontent.length();
#endif
    }

    int getLocalPrivateKey(unsigned char **privateKey) {
        if(local_private == nullptr) {
            return -1;
        }
        return bioToString(local_private, privateKey);
    }

    int getAesKey(unsigned char **aesKey) {
        *aesKey = this->aesKey;
        return AES_KEYLEN / 8;
    }

    int setAesKey(unsigned char *aesKey, size_t aesKeyLength) {
        // Ensure the new key is the proper size
        if((int)aesKeyLength != AES_KEYLEN / 8) {
            return FAILURE;
        }

        memcpy(this->aesKey, aesKey, AES_KEYLEN / 8);

        return SUCCESS;
    }

    int getAesIv(unsigned char **aesIv) {
        *aesIv = this->aesIv;
        return AES_KEYLEN / 8;
    }

    int setAesIv(unsigned char *aesIv, size_t aesIvLength) {
        // Ensure the new IV is the proper size
        if((int)aesIvLength != AES_KEYLEN / 8) {
            return FAILURE;
        }

        memcpy(this->aesIv, aesIv, AES_KEYLEN / 8);

        return SUCCESS;
    }

    int writeKeyToFile(std::string _path, int _key) {
        file_path = _path;
        return SUCCESS;
    }

    int generateRsaKeypair() {
        std::string file_public = file_path + RSA_LOCAL_PUBLIC_FILE;
        std::string file_private = file_path + RSA_LOCAL_PRIVATEEY_FILE;
        if (access(file_public.c_str(), 0) == 0
                || access(file_private.c_str(), 0) == 0) {
            return 0;
        }

        //生成key,这里设置了2048，意味着最多可以编解码2048/8-11=245个字节，
        //RSA_F4为公钥指数，一般情况下使用RSA_F4即可，
        //其它两个参数可以设置为NULL
        RSA *pRsa = RSA_generate_key(RSA_BITS, RSA_F4, NULL, NULL);
        if (pRsa == NULL) {
            printf("rsa_generate_key error");
            return -1;
        }

        PEM_write_bio_RSAPrivateKey(local_private, pRsa, NULL, NULL, 0, NULL, NULL);
        PEM_write_bio_RSAPublicKey(local_public, pRsa);

        if (if_write_to_file == true) {
            BIO *pBio = BIO_new_file(file_public.c_str(), "wb");

            if (pBio == NULL) {
                printf("BIO_new_file = %s  error", file_public.c_str());
                return -2;
            }
            if (PEM_write_bio_RSAPublicKey(pBio, pRsa) == 0) {
                printf("write public key error");
                return -3;
            }

            BIO_free_all(pBio);

            pBio = BIO_new_file(file_private.c_str(), "w");
            if (pBio == NULL) {
                printf("BIO_new_file %s error ", file_private.c_str());
                return -4;
            }
            if (PEM_write_bio_RSAPrivateKey(pBio, pRsa, NULL, NULL, 0,
                                            NULL, NULL) == 0) {
                printf("write private key error");
                return -5;
            }
            BIO_free_all(pBio);
        }

        RSA_free(pRsa);
        return 0;
    }

    int aesEncryptFile(char* file_path) {
        if (NULL == file_path ||
                0 != access(file_path, 0))
            return -1;

        std::string file(file_path);
        std::ifstream if_temp(file.c_str());
        std::ostringstream os_stream;
        os_stream << if_temp.rdbuf();
        std::string szcontent = os_stream.str();
        if (szcontent.empty()) return -2;

        int encrypt_len = 0;
        char* pencrypt_buf = NULL;
        if (FAILURE == (encrypt_len = aesEncrypt((unsigned char*)szcontent.c_str(),
                                      szcontent.length(), (unsigned char**)&pencrypt_buf))) {
            return -3;
        }

        std::ofstream of(file_path);
        of << pencrypt_buf;
        of.close();
        if (NULL != pencrypt_buf) {
            free(pencrypt_buf);
            pencrypt_buf = NULL;
        }
        return 0;
    }

  private:
    // static EVP_PKEY *localKeypair;
    // EVP_PKEY *remotePublicKey;
    BIO *server_public;
    BIO *local_private;
    BIO *local_public;

    EVP_CIPHER_CTX *rsaEncryptContext;
    EVP_CIPHER_CTX *aesEncryptContext;

    EVP_CIPHER_CTX *rsaDecryptContext;
    EVP_CIPHER_CTX *aesDecryptContext;

    unsigned char *aesKey;
    unsigned char *aesIv;

    bool if_write_to_file;
    std::string file_path;

    int init() {
        local_private = nullptr;
        server_public = nullptr;
        local_public = nullptr;

        file_path = "";
        if_write_to_file = true;

        // Initalize contexts
        rsaEncryptContext = EVP_CIPHER_CTX_new();
        aesEncryptContext = EVP_CIPHER_CTX_new();

        rsaDecryptContext = EVP_CIPHER_CTX_new();
        aesDecryptContext = EVP_CIPHER_CTX_new();

        // Check if any of the contexts initializations failed
        if(rsaEncryptContext == NULL || aesEncryptContext == NULL
                || rsaDecryptContext == NULL || aesDecryptContext == NULL) {
            return FAILURE;
        }

        // Generate RSA and AES keys
        //generateRsaKeypair();
        generateAesKey(&aesKey, &aesIv);
        strcpy((char*)aesKey, AES_KEY);
        strcpy((char*)aesIv, AES_IV);
        return SUCCESS;
    }

    int generateAesKey(unsigned char **aesKey, unsigned char **aesIv) {
		bool suc = true;
        *aesKey = (unsigned char*)malloc(AES_KEYLEN / 8);
        *aesIv = (unsigned char*)malloc(AES_KEYLEN / 8);

        if(aesKey == NULL || aesIv == NULL) {
            return FAILURE;
        }

        // For the AES key we have the option of using a PBKDF or just using straight random
        // data for the key and IV. Depending on your use case, you will want to pick one or another.
#ifdef USE_PBKDF
        unsigned char *aesPass = (unsigned char*)malloc(AES_KEYLEN / 8);
        unsigned char *aesSalt = (unsigned char*)malloc(8);
        if(aesPass == NULL || aesSalt == NULL) {
            if (aesPass)
                free(aesPass);
            if (aesSalt)
                free(aesSalt);
            return FAILURE;
        }

        
        do {
            // Get some random data to use as the AES pass and salt
            if(RAND_bytes(aesPass, AES_KEYLEN / 8) == 0) {
                suc = false;
                break;
                // return FAILURE;
            }

            if(RAND_bytes(aesSalt, 8) == 0) {
                suc = false;
                break;
                // return FAILURE;
            }

            if(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), aesSalt, aesPass,
                            AES_KEYLEN / 8, AES_ROUNDS, aesKey, aesIv) == 0) {
                suc = false;
                break;
                // return FAILURE;
            }
        } while (0);

        if (aesPass)
            free(aesPass);
        if (aesSalt)
            free(aesSalt);

        return (suc ? SUCCESS : FAILURE);

#else
        if(RAND_bytes(*aesKey, AES_KEYLEN / 8) == 0) {
            return FAILURE;
        }

        if(RAND_bytes(*aesIv, AES_KEYLEN / 8) == 0) {
            return FAILURE;
        }
#endif
        return SUCCESS;
    }

    int bioToString(BIO *bio, unsigned char **string) {
        size_t bioLength = BIO_pending(bio);
        *string = (unsigned char*)malloc(bioLength + 1);

        if(string == NULL) {
            return FAILURE;
        }

        BIO_read(bio, *string, bioLength);

        // Insert the NUL terminator
        (*string)[bioLength] = '\0';

        BIO_free_all(bio);

        return (int)bioLength;
    }
};

Rsa* Rsa::instance = nullptr;

int RsaEncrypt(const char *in, const int inLen, char *out, int * outLen) {
    return Rsa::GetInstance()->enc(in, inLen, out, outLen);
}

int RsaEncrypt(const char* key, const char *in, const int inLen, char *out,
               int * outLen) {
    return Rsa::GetInstance()->enc(key, in, inLen, out, outLen);
}

int RsaDecrypt(const char *in, const int inLen, char *out, int * outLen) {
    return Rsa::GetInstance()->dec(in, inLen, out, outLen);
}

int RsaDecrypt(const char* key, const char *in, const int inLen, char *out,
               int * outLen) {
    return Rsa::GetInstance()->dec(key, in, inLen, out, outLen);
}

int AesEncrypt(const unsigned char *message, size_t messageLength,
               unsigned char **encryptedMessage) {
    return Rsa::GetInstance()->aesEncrypt(message, messageLength, encryptedMessage);
}

int AesDecrypt(unsigned char *encryptedMessage,
               size_t encryptedMessageLength, unsigned char **decryptedMessage) {
    return Rsa::GetInstance()->aesDecrypt(encryptedMessage, encryptedMessageLength,
                                          decryptedMessage);
}

int GetRemotePublicKey(unsigned char **publicKey) {
    return Rsa::GetInstance()->getRemotePublicKey(publicKey);
}

int SetRemotePublicKey(unsigned char *publicKey,
                       size_t publicKeyLength) {
    return Rsa::GetInstance()->setRemotePublicKey(publicKey, publicKeyLength);
}

int GetLocalPublicKey(unsigned char **publicKey) {
    return Rsa::GetInstance()->getLocalPublicKey(publicKey);
}

int GetLocalPrivateKey(unsigned char **privateKey) {
    return Rsa::GetInstance()->getLocalPrivateKey(privateKey);
}

int GetAesKey(unsigned char **aesKey) {
    return Rsa::GetInstance()->getAesKey(aesKey);
}

int SetAesKey(unsigned char *aesKey, size_t aesKeyLength) {
    return Rsa::GetInstance()->setAesKey(aesKey, aesKeyLength);
}

int GetAesIv(unsigned char **aesIv) {
    return Rsa::GetInstance()->getAesIv(aesIv);
}

int SetAesIv(unsigned char *aesIv, size_t aesIvLength) {
    return Rsa::GetInstance()->setAesIv(aesIv, aesIvLength);
}

int WriteKeyToFile(std::string _path, int _key) {
    return Rsa::GetInstance()->writeKeyToFile(_path, _key);
}

int GenerateRsaKeypair() {
    return Rsa::GetInstance()->generateRsaKeypair();
}

int AesEncryptFile(char* file_path) {
    return Rsa::GetInstance()->aesEncryptFile(file_path);
}

}  // namespace crypt
}  // namespace base