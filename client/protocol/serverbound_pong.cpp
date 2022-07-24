#include "protocol.h"

ServerboundPongPacket::ServerboundPongPacket() = default;

void ServerboundPongPacket::write(ByteBuffer& dst) {
    dst.writeLong(payload);
}

unsigned char ServerboundPongPacket::getPacketID() const {
    return 0xB6;
}