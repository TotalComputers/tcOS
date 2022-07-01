#pragma once

#include "packet.h"

void protocol_registerPackets();

class ClientboundConnectionSuccessPacket : public ClientboundPacket {
public:
    ClientboundConnectionSuccessPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    std::string name;

};