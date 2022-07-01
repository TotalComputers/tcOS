#include "protocol.h"

ServerboundConnectPacket::ServerboundConnectPacket() {}

void ServerboundConnectPacket::write(ByteBuffer& dst) {
    dst.writeString(token);
}

unsigned char ServerboundConnectPacket::getPacketID() const {
    return 0xB3;
}