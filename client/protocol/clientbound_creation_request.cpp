#include "protocol.h"

ClientboundCreationRequestPacket::ClientboundCreationRequestPacket() = default;

void ClientboundCreationRequestPacket::read(ByteBuffer& src) {
    width = src.readShort();
    height = src.readShort();
    id = src.readShort();
    name = src.readString();
}

ClientboundPacket* ClientboundCreationRequestPacket::createInstance() {
    return new ClientboundCreationRequestPacket();
}

unsigned char ClientboundCreationRequestPacket::getPacketID() const {
    return 0xB7;
}