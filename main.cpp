#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include <iostream>

class TestInboundHandler : public InboundHandler {
public:
    bool onConnect(ConnectionContext* ctx) override {
        std::cout << "Connected 1" << std::endl;
        return true;
    }
    
    bool onDisconnect(ConnectionContext* ctx) override {
        std::cout << "Disconnected 1" << std::endl;
        return false;
    }
    
    bool decode(ConnectionContext* ctx, void* in, std::vector<void*>& out) override {
        out.push_back(in);
        out.push_back(nullptr);
        return true;
    }

};

class TestOutboundHandler : public OutboundHandler {
public:
    virtual bool encode(ConnectionContext* ctx, void* data, void*& dst) {
        dst = new ByteBuffer();
        std::cout << "data += 10" << std::endl;
        ((ByteBuffer*)dst)->writeInt(((ByteBuffer*)data)->readInt()+10);
        return true;
    }

};

class TestOutboundHandlerTest : public OutboundHandler {
public:
    virtual bool encode(ConnectionContext* ctx, void* data, void*& dst) {
        std::cout << "Received from previous handler: " << ((ByteBuffer*)data)->readInt() << std::endl;
        return true;
    }

};

class TestFinalHandler : public FinalHandler {
public:
    bool onConnect(ConnectionContext* ctx) override {
        std::cout << "Connected 2" << std::endl;
        return true;
    }
    
    bool onDisconnect(ConnectionContext* ctx) override {
        std::cout << "Disconnected 2" << std::endl;
        return true;
    }

    void handle(ConnectionContext* ctx, void* in) override {
        std::cout << "Read: " << (uint64_t)in;
        if(in)
            std::cout << " (" << ((ByteBuffer*)in)->data.data() << ")";
        std::cout << std::endl;
        int data = 10;
        ByteBuffer buf;
        buf.writeInt(data);
        std::cout << "Write " << data << std::endl;
        ctx->write((void*)&buf);
    }

};

int main() {
    protocol_registerPackets();

    Pipeline* pipeline = new Pipeline();

    pipeline->addLast("test_inbound", new TestInboundHandler());
    pipeline->addLast("test_outbound", new TestOutboundHandler());
    pipeline->addLast("test_outbound2", new TestOutboundHandler());
    pipeline->addLast("test_outbound_test", new TestOutboundHandlerTest());
    pipeline->addLast("test_final", new TestFinalHandler());

    tcp_set_pipeline(pipeline);    

    tcp_connect("localhost", 29077);
}