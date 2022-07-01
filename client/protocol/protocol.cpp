#include "protocol.h"

void protocol_registerPackets() {
    ClientboundConnectionSuccessPacket().registerPacket();
}