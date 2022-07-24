#include "protocol.h"

ClientboundDestroyPacket::ClientboundDestroyPacket() = default;

void ClientboundDestroyPacket::read(ByteBuffer& src) {
    id = src.readShort();
}

ClientboundPacket* ClientboundDestroyPacket::createInstance() {
    return new ClientboundDestroyPacket();
}

unsigned char ClientboundDestroyPacket::getPacketID() const {
    return 0xB9;
}