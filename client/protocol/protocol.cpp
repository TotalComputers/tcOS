#include "protocol.h"

void protocol_registerPackets() {
    ClientboundConnectionSuccessPacket().registerPacket();
    ClientboundCreationRequestPacket().registerPacket();
    ClientboundDestroyPacket().registerPacket();
}