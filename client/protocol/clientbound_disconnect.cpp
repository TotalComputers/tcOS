#include "protocol.h"

ClientboundDisconnectPacket::ClientboundDisconnectPacket() {}

void ClientboundDisconnectPacket::read(ByteBuffer& src) {
    reason = src.readString();
}

ClientboundPacket* ClientboundDisconnectPacket::createInstance() {
    return new ClientboundDisconnectPacket();
}

unsigned char ClientboundDisconnectPacket::getPacketID() const {
    return 0xB2;
}