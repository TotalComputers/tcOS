#include "protocol.h"

void protocol_registerPackets() {
    ClientboundConnectionSuccessPacket().registerPacket();
    ClientboundCreationRequestPacket().registerPacket();
    ClientboundDestroyPacket().registerPacket();
    ClientboundDisconnectPacket().registerPacket();
    ClientboundEncryption().registerPacket();
    ClientboundHandshakePacket().registerPacket();
    ClientboundPalettePacket().registerPacket();
    ClientboundPingPacket().registerPacket();
}