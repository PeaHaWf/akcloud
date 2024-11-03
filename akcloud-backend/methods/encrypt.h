#ifndef ENCODER_H
#define ENCODER_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iostream>
#include <string>
#include <openssl/err.h>
#include <fstream>
class AESEncrypt {
public:
    AESEncrypt();
    ~AESEncrypt();
    bool encrypt(const std::string &in, std::string &out, const unsigned char *key, const unsigned char *ivec);
    bool decrypt(const std::string &in, std::string &out, const unsigned char *key, const unsigned char *ivec);

private:
    EVP_CIPHER_CTX *ctx;
    EVP_CIPHER_CTX *ctx1;
};
#endif