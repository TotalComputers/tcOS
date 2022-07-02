#include "pipeline.h"
#include "../encryption/encryption.h"

bool PacketEncryptor::encode(ConnectionContext* ctx, void* in, void*& out) {
    out = new ByteBuffer();
    
    ByteBuffer* inbuf = (ByteBuffer*)in;
    ByteBuffer* dstbuf = (ByteBuffer*)out;

    dstbuf->writeBytes(aes_encrypt(inbuf->readBytes(inbuf->readableBytes())));

    return true;
}