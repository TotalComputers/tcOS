#include "protocol.h"

ClientboundConnectionSuccessPacket::ClientboundConnectionSuccessPacket() {}

void ClientboundConnectionSuccessPacket::read(ByteBuffer& src) {
    name = src.readString();
}

ClientboundPacket* ClientboundConnectionSuccessPacket::createInstance() {
    return new ClientboundConnectionSuccessPacket();
}

unsigned char ClientboundConnectionSuccessPacket::getPacketID() const {
    return 0xB4;
}