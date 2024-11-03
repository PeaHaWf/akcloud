#include "encrypt.h"
AESEncrypt::AESEncrypt() {
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    ctx1 = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx1);
}
AESEncrypt::~AESEncrypt() {
    // 释放CTX
    EVP_CIPHER_CTX_cleanup(ctx);
    EVP_CIPHER_CTX_free(ctx);
    EVP_CIPHER_CTX_cleanup(ctx1);
    EVP_CIPHER_CTX_free(ctx1);
}
bool AESEncrypt::encrypt(const std::string &in, std::string &out, const unsigned char *key, const unsigned char *ivec) {
    bool ret = 0;
    std::ifstream fIn(in, std::ios::in | std::ios::binary);
    std::ofstream fOut(out, std::ios::out | std::ios::binary);
    
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, ivec);
    
    fIn.seekg(0, std::ios::end);
    uint64_t inputFileLen = fIn.tellg();
    fIn.seekg(0, std::ios::beg);


    char readBuf[8192] = {0x00};

    uint8_t writeBuf[8192 + 32] = {0x00};

    uint64_t Total = 0;
    while (Total < inputFileLen) {
        int curRead = sizeof(readBuf);
        if (inputFileLen - Total < sizeof(readBuf)) {
            curRead = inputFileLen - Total;
        }
        fIn.read(readBuf, curRead);
        Total = Total + curRead;
        int toEnBufLen = 0;
        int curEnLen = 0;
        if (!EVP_EncryptUpdate(ctx, writeBuf, &toEnBufLen, reinterpret_cast<const unsigned char *>(readBuf), curRead)) {
            return ret;
        }
        //分块加密，如果是最后一块,调用结束,获取填充数据
        if (curRead < sizeof(readBuf)) {
            if (!EVP_EncryptFinal_ex(ctx, writeBuf + toEnBufLen, &curEnLen)) {
                return ret;
            };
            toEnBufLen += curEnLen;
        }
        fOut.write(reinterpret_cast<const char *>(writeBuf), toEnBufLen);
    }
    fIn.close();
    fOut.close();
    ret = 1;
    return ret;
}
bool AESEncrypt::decrypt(const std::string &in, std::string &out, const unsigned char *key, const unsigned char *ivec) {
    std::ifstream fIn(in, std::ios::in | std::ios::binary);

    std::ofstream fOut(out, std::ios::out | std::ios::binary);
    int rr = EVP_DecryptInit_ex(ctx1, EVP_aes_256_cbc(), NULL, key, ivec);

    fIn.seekg(0, std::ios::end);
    uint64_t inputFileLen = fIn.tellg();
    fIn.seekg(0, std::ios::beg);

    char readBuf[8192] = {0x00};

    uint8_t writeBuf[8192 + 32] = {0x00};
    uint64_t Total = 0;
    while (Total < inputFileLen) {
        int curRead = sizeof(readBuf);
        if (inputFileLen - Total < sizeof(readBuf)) {
            curRead = inputFileLen - Total;
        }
        fIn.read(readBuf, curRead);
        Total += curRead;
        int toEnBufLen = 0;
        int curEnLen = 0;
        if (!EVP_DecryptUpdate(ctx1, writeBuf, &toEnBufLen, reinterpret_cast<const unsigned char *>(readBuf), curRead)) {
            printf("EVP_DecryptUpdate failed!  err:%s  \n", ERR_error_string(ERR_get_error(), NULL));
            return false;
        }

        if (curRead < sizeof(readBuf)) {
            if (!EVP_DecryptFinal(ctx1, writeBuf + toEnBufLen, &curEnLen)) {
                printf("EVP_DecryptFinal failed!  err:%s  \n", ERR_error_string(ERR_get_error(), NULL));
                return false;
            }
            toEnBufLen += curEnLen;
        }
        fOut.write(reinterpret_cast<const char *>(writeBuf), toEnBufLen);
    }
    fIn.close();
    fOut.close();
    return true;
}
