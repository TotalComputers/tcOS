#include "protocol.h"

ClientboundHandshakePacket::ClientboundHandshakePacket() = default;

void ClientboundHandshakePacket::read(ByteBuffer& src) {
    serverName = src.readString();
}

ClientboundPacket* ClientboundHandshakePacket::createInstance() {
    return new ClientboundHandshakePacket();
}

unsigned char ClientboundHandshakePacket::getPacketID() const {
    return 0xB1;
}