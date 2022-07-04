#include "compression.h"

#include <zlib.h>
#include <cstdlib>
#include <iostream>

std::vector<unsigned char> compress_bytes(std::vector<unsigned char> data) {
    const char* in = (const char*)data.data();
    long long srcLen = data.size();
    uLong destLen = compressBound(srcLen);
    unsigned char* out = (unsigned char*)malloc(destLen);
    int res = compress((Bytef*)out, &destLen, (Bytef*)in, (uLong)srcLen); 
    if(res == Z_BUF_ERROR) {
        std::cout << "too small buffer" << std::endl;
        return {};
    }
    if(res ==  Z_MEM_ERROR) {
        std::cout << "not enough memory" << std::endl;
        return {};
    }
    std::vector<unsigned char> dst(out, out + destLen);
    free(out);
    return dst;
}