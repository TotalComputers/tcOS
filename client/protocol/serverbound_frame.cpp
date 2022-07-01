#include "protocol.h"

ServerboundFramePacket::ServerboundFramePacket() {}

void ServerboundFramePacket::write(ByteBuffer& dst) {
    dst.writeShort(id);
    dst.writeBytes(compressedData);
}

unsigned char ServerboundFramePacket::getPacketID() const {
    return 0xC0;
}