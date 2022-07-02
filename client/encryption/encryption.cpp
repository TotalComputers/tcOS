#include "encryption.h"
#include <openssl/x509.h>
#include <vector>

struct {
    EVP_CIPHER_CTX* ctx_encrypt = NULL;
    EVP_CIPHER_CTX* ctx_decrypt = NULL;
    unsigned int _block_size = 0;
} AES_CFB8_Data;

RSA* rsa_decode_key(std::vector<unsigned char> data) {
    unsigned char* raw = data.data();
    return d2i_RSA_PUBKEY(NULL, (const unsigned char**)&raw, data.size());
}

std::vector<unsigned char> rsa_encrypt(RSA* rsa, std::vector<unsigned char> data) {
    unsigned char* out = (unsigned char*)malloc(RSA_size(rsa));
    size_t size = RSA_public_encrypt(data.size(), data.data(), out, rsa, RSA_PKCS1_PADDING);
    std::vector<unsigned char> dst(out, out + size);
    free(out);
    return dst;
}

void aes_init(std::vector<unsigned char> secret) {
    AES_CFB8_Data.ctx_encrypt = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(AES_CFB8_Data.ctx_encrypt);
    EVP_EncryptInit_ex(AES_CFB8_Data.ctx_encrypt, EVP_aes_128_cfb8(), NULL, secret.data(), secret.data());

    AES_CFB8_Data.ctx_decrypt = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(AES_CFB8_Data.ctx_decrypt);
    EVP_DecryptInit_ex(AES_CFB8_Data.ctx_decrypt, EVP_aes_128_cfb8(), NULL, secret.data(), secret.data());

    AES_CFB8_Data._block_size = EVP_CIPHER_block_size(EVP_aes_128_cfb8());
}

std::vector<unsigned char> aes_encrypt(const std::vector<unsigned char> data) {
    unsigned char* out = (unsigned char*)malloc(data.size() + AES_CFB8_Data._block_size);
    int len;
    EVP_EncryptUpdate(AES_CFB8_Data.ctx_encrypt, out, &len, data.data(), data.size());
    std::vector<unsigned char> dst(out, out + len);
    free(out);
    return dst;
}

std::vector<unsigned char> aes_decrypt(const std::vector<unsigned char> data) {
    unsigned char* out = (unsigned char*)malloc(data.size() + AES_CFB8_Data._block_size);
    int len;
    EVP_DecryptUpdate(AES_CFB8_Data.ctx_decrypt, out, &len, data.data(), data.size());
    std::vector<unsigned char> dst(out, out + len);
    free(out);
    return dst;
}