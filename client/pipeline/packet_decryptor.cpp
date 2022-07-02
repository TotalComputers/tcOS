#include "../protocol/protocol.h"
#include "../encryption/encryption.h"
#include "pipeline.h"

bool PacketDecryptor::onConnect(ConnectionContext*) {
    return true;
}

bool PacketDecryptor::onDisconnect(ConnectionContext*) {
    return true;
}

bool PacketDecryptor::decode(ConnectionContext* ctx, void* in, std::vector<void*>& out) {
    ByteBuffer* inbuf = (ByteBuffer*)in;
    ByteBuffer* outbuf = new ByteBuffer();

    outbuf->writeBytes(aes_decrypt(inbuf->readBytes(inbuf->readableBytes())));

    out.push_back(outbuf);

    return true;
}