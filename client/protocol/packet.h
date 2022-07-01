#pragma once

#include "../ByteBuffer.h"
#include <memory>

class ClientboundPacket {
public:
    ClientboundPacket();

public:
    virtual void read(ByteBuffer&) = 0;
    virtual ClientboundPacket* createInstance() = 0;
    virtual unsigned char getPacketID() const = 0;
    
public:
    void registerPacket();

public:
    static ClientboundPacket* createInstanceFromID(unsigned char);

};

class ServerboundPacket {
public:
    ServerboundPacket();

public:
    virtual void write(ByteBuffer&) = 0;
    virtual unsigned char getPacketID() const = 0;

};