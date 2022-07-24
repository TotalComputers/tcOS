#include "protocol.h"

ServerboundConnectPacket::ServerboundConnectPacket() = default;

void ServerboundConnectPacket::write(ByteBuffer& dst) {
    dst.writeString(token);
}

unsigned char ServerboundConnectPacket::getPacketID() const {
    return 0xB3;
}