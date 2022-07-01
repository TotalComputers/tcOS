#pragma once

#include "packet.h"
#include <vector>

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

class ClientboundCreationRequestPacket : public ClientboundPacket {
public:
    ClientboundCreationRequestPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    short width;
    short height;
    short id;
    std::string name;

};

class ClientboundDestroyPacket : public ClientboundPacket {
public:
    ClientboundDestroyPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    short id;

};

class ClientboundDisconnectPacket : public ClientboundPacket {
public:
    ClientboundDisconnectPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    std::string reason;

};

class ClientboundEncryption : public ClientboundPacket {
public:
    ClientboundEncryption();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    std::vector<unsigned char> publicKey;

};

class ClientboundHandshakePacket : public ClientboundPacket {
public:
    ClientboundHandshakePacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    std::string serverName;

};

class ClientboundPalettePacket : public ClientboundPacket {
public:
    ClientboundPalettePacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    std::vector<int> palette;

};

class ClientboundPingPacket : public ClientboundPacket {
public:
    ClientboundPingPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    long long payload;

};

class ClientboundTouchPacket : public ClientboundPacket {
public:
    ClientboundTouchPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    unsigned char getPacketID() const override;

public:
    short id;
    short x;
    short y;
    unsigned char type;
    bool admin;

    enum ClickType {
        LEFT_CLICK = 0,
        RIGHT_CLICK = 1
    };

};

class ServerboundConnectPacket : public ServerboundPacket {
public:
    ServerboundConnectPacket();

public:
    void write(ByteBuffer&) override;
    unsigned char getPacketID() const override;

public:
    std::string token;

};

class ServerboundCreationStatusPacket : public ServerboundPacket {
public:
    ServerboundCreationStatusPacket();

public:
    void write(ByteBuffer&) override;
    unsigned char getPacketID() const override;

public:
    unsigned char status;
    short id;

    enum Status {
        OK = 0,
        ERROR = 1
    };

};

class ServerboundEncryptionPacket : public ServerboundPacket {
public:
    ServerboundEncryptionPacket();

public:
    void write(ByteBuffer&) override;
    unsigned char getPacketID() const override;

public:
    std::vector<unsigned char> secret;

};

class ServerboundFramePacket : public ServerboundPacket {
public:
    ServerboundFramePacket();

public:
    void write(ByteBuffer&) override;
    unsigned char getPacketID() const override;

public:
    short id;
    std::vector<unsigned char> compressedData;

};

class ServerboundHandshakePacket : public ServerboundPacket {
public:
    ServerboundHandshakePacket();

public:
    void write(ByteBuffer&) override;
    unsigned char getPacketID() const override;

public:
    short protocolVersion;
    short apiVersion;

}