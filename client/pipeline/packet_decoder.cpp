#include "../protocol/protocol.h"
#include "pipeline.h"
#include <iostream>

bool PacketDecoder::onConnect(ConnectionContext*) {
    return true;
}

bool PacketDecoder::onDisconnect(ConnectionContext*) {
    return true;
}

bool PacketDecoder::decode(ConnectionContext* ctx, void* src, std::vector<void*>& dst) {
    ByteBuffer* buf = (ByteBuffer*)src;

    if(buf->readableBytes() < 7) {
        std::cout << "Received too small packet" << std::endl;
        return false;
    }

    if(buf->readByte() != 0x0A || buf->readByte() != 0x1F) {
        std::cout << "Invalid magic value" << std::endl;
        return false;
    }

    unsigned char packetId = buf->readByte();
    ClientboundPacket* packet = ClientboundPacket::createInstanceFromID(packetId);

    if(!packet) {
        std::cout << "Unable to find packet with id " << packetId << std::endl;
        return false;
    }
    
    int length = buf->readInt();
    if(length != buf->readableBytes()) {
        std::cout << "Invalid packet length" << std::endl;
        return false;
    }

    packet->read(*buf);

    dst.push_back(packet);

    delete buf;
    
    return true;
}