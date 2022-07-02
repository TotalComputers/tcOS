#include "../connection_context.h"

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

class PacketHandler : public FinalHandler {
public:
    bool onConnect(ConnectionContext*) override;
    bool onDisconnect(ConnectionContext*) override;
    void handle(ConnectionContext*, void*) override;

};