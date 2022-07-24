#include "../protocol/protocol.h"
#include "../encryption/encryption.h"
#include "pipeline.h"

bool PacketEncryptor::encode(ConnectionContext* ctx, void* in, void*& out) {
    out = new ByteBuffer();
    
    auto* inbuf = (ByteBuffer*) in;
    auto* dstbuf = (ByteBuffer*) out;

    dstbuf->writeBytes(aes_encrypt(inbuf->readBytes(inbuf->readableBytes())));

    delete inbuf;

    return true;
}