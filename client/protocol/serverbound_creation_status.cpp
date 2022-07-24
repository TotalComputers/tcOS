#include "protocol.h"

ServerboundCreationStatusPacket::ServerboundCreationStatusPacket() = default;

void ServerboundCreationStatusPacket::write(ByteBuffer& dst) {
    dst.writeByte(status);
    dst.writeShort(id);
}

unsigned char ServerboundCreationStatusPacket::getPacketID() const {
    return 0xB8;
}