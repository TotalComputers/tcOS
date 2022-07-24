#include "protocol.h"

ClientboundTouchPacket::ClientboundTouchPacket() = default;

void ClientboundTouchPacket::read(ByteBuffer& src) {
    id = src.readShort();
    x = src.readShort();
    y = src.readShort();
    type = src.readByte();
    admin = src.readByte();
}

ClientboundPacket* ClientboundTouchPacket::createInstance() {
    return new ClientboundTouchPacket();
}

unsigned char ClientboundTouchPacket::getPacketID() const {
    return 0xC2;
}