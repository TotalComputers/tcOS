#include "protocol.h"

ClientboundPalettePacket::ClientboundPalettePacket() {}

void ClientboundPalettePacket::read(ByteBuffer& src) {
    palette.resize(src.readInt());
    for(int i = 0; i < palette.size(); i++)
        palette[i] = src.readInt();
}

ClientboundPacket* ClientboundPalettePacket::createInstance() {
    return new ClientboundPalettePacket();
}

unsigned char ClientboundPalettePacket::getPacketID() const {
    return 0xC1;
}