#include <openssl/rsa.h>
#include <vector>

RSA* rsa_decode_key(std::vector<unsigned char>);
std::vector<unsigned char> rsa_encrypt(RSA*, std::vector<unsigned char>);

void aes_init(std::vector<unsigned char>);
std::vector<unsigned char> aes_encrypt(const std::vector<unsigned char>);
std::vector<unsigned char> aes_decrypt(const std::vector<unsigned char>);