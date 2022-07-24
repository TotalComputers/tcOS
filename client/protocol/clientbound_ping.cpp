#include "protocol.h"

ClientboundPingPacket::ClientboundPingPacket() = default;

void ClientboundPingPacket::read(ByteBuffer& src) {
    payload = src.readLong();
}

ClientboundPacket* ClientboundPingPacket::createInstance() {
    return new ClientboundPingPacket();
}

unsigned char ClientboundPingPacket::getPacketID() const {
    return 0xB5;
}