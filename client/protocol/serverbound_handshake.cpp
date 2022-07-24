#include "protocol.h"

ServerboundHandshakePacket::ServerboundHandshakePacket() = default;

void ServerboundHandshakePacket::write(ByteBuffer& dst) {
    dst.writeShort(protocolVersion);
    dst.writeShort(apiVersion);
}

unsigned char ServerboundHandshakePacket::getPacketID() const {
    return 0xB0;
}