#include "protocol.h"

ClientboundPingPacket::ClientboundPingPacket() {}

void ClientboundPingPacket::read(ByteBuffer& src) {
    payload = src.readLong();
}

unsigned char ClientboundPingPacket::getPacketID() const {
    return 0xB5;
}