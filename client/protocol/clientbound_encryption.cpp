#include "protocol.h"

ClientboundEncryption::ClientboundEncryption() {}

void ClientboundEncryption::read(ByteBuffer& src) {
    publicKey = src.readBytes(src.readableBytes());
}

ClientboundPacket* ClientboundEncryption::createInstance() {
    return new ClientboundEncryption();
}

unsigned char ClientboundEncryption::getPacketID() const {
    return 0xC3;
}