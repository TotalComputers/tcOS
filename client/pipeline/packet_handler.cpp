#include "../protocol/protocol.h"
#include "pipeline.h"
#include <iostream>

bool PacketHandler::onConnect(ConnectionContext* ctx) {
    ServerboundHandshakePacket* handshake = new ServerboundHandshakePacket();
    handshake->apiVersion = 0;
    handshake->protocolVersion = 2;
    ctx->write(handshake);

    return true;
}

bool PacketHandler::onDisconnect(ConnectionContext*) {
    return true;
}

void PacketHandler::handle(ConnectionContext* ctx, void* packet) {
    
}