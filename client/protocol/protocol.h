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
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    std::string name;

};

class ClientboundCreationRequestPacket : public ClientboundPacket {
public:
    ClientboundCreationRequestPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    short width = 0;
    short height = 0;
    short id = 0;
    std::string name;

};

class ClientboundDestroyPacket : public ClientboundPacket {
public:
    ClientboundDestroyPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    short id = 0;

};

class ClientboundDisconnectPacket : public ClientboundPacket {
public:
    ClientboundDisconnectPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    std::string reason;

};

class ClientboundEncryption : public ClientboundPacket {
public:
    ClientboundEncryption();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    std::vector<unsigned char> publicKey;

};

class ClientboundHandshakePacket : public ClientboundPacket {
public:
    ClientboundHandshakePacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    std::string serverName;

};

class ClientboundPalettePacket : public ClientboundPacket {
public:
    ClientboundPalettePacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    std::vector<int> palette;

};

class ClientboundPingPacket : public ClientboundPacket {
public:
    ClientboundPingPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    long long payload = 0;

};

class ClientboundTouchPacket : public ClientboundPacket {
public:
    ClientboundTouchPacket();

public:
    void read(ByteBuffer&) override;
    ClientboundPacket* createInstance() override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    short id = 0;
    short x = 0;
    short y = 0;
    unsigned char type = 0;
    bool admin = false;

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
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    std::string token;

};

class ServerboundCreationStatusPacket : public ServerboundPacket {
public:
    ServerboundCreationStatusPacket();

public:
    void write(ByteBuffer&) override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    unsigned char status = 0;
    short id = 0;

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
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    std::vector<unsigned char> secret;

};

class ServerboundFramePacket : public ServerboundPacket {
public:
    ServerboundFramePacket();

public:
    void write(ByteBuffer&) override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    short id = 0;
    std::vector<unsigned char> compressedData;

};

class ServerboundHandshakePacket : public ServerboundPacket {
public:
    ServerboundHandshakePacket();

public:
    void write(ByteBuffer&) override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    short protocolVersion = 0;
    short apiVersion = 0;

};

class ServerboundPongPacket : public ServerboundPacket {
public:
    ServerboundPongPacket();

public:
    void write(ByteBuffer&) override;
    [[nodiscard]] unsigned char getPacketID() const override;

public:
    long long payload = 0;

};