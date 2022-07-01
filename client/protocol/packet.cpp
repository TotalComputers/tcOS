#include "packet.h"
#include <map>

static std::map<unsigned char, ClientboundPacket*> registered;

ClientboundPacket::ClientboundPacket() {}

void ClientboundPacket::registerPacket() {
    registered[getPacketID()] = createInstance();
}

ClientboundPacket* ClientboundPacket::createInstanceFromID(unsigned char id) {
    return registered[id]->createInstance();
}

ServerboundPacket::ServerboundPacket() {}