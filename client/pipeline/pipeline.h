#include "../connection_context.h"
#include "../protocol/protocol.h"
#include "../../common/io_factory.h"

class PacketDecoder : public InboundHandler {
public:
    bool onConnect(ConnectionContext*) override;
    bool onDisconnect(ConnectionContext*) override;
    bool decode(ConnectionContext*, void*, std::vector<void*>&) override;

};

class PacketEncoder : public OutboundHandler {
public:
    bool encode(ConnectionContext*, void*, void*&) override;

};

class PacketEncryptor : public OutboundHandler {
public:
    bool encode(ConnectionContext*, void*, void*&) override;

};

class PacketDecryptor : public InboundHandler {
public:
    bool onConnect(ConnectionContext*) override;
    bool onDisconnect(ConnectionContext*) override;
    bool decode(ConnectionContext*, void*, std::vector<void*>&) override;

};

class PacketHandler : public FinalHandler {
public:
    PacketHandler(std::string, IOFactory*);

public:
    bool onConnect(ConnectionContext*) override;
    bool onDisconnect(ConnectionContext*) override;
    void handle(ConnectionContext*, void*) override;

private:
    std::string token;
    IOFactory* io_factory;

};

class PacketLengthPrefixer : public OutboundHandler {
public:
    bool encode(ConnectionContext*, void*, void*&) override;

};

class PacketDefragmentation : public InboundHandler {
public:
    bool onConnect(ConnectionContext*) override;
    bool onDisconnect(ConnectionContext*) override;
    bool decode(ConnectionContext*, void*, std::vector<void*>&) override;

private:
    ByteBuffer* buf = nullptr;
    int remaining = 0;

};