#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include "client/pipeline/pipeline.h"
#include <iostream>

int main() {
    protocol_registerPackets();

    Pipeline* pipeline = new Pipeline();

    pipeline->addLast("packet_decoder", new PacketDecoder());
    pipeline->addLast("packet_encoder", new PacketEncoder());
    pipeline->addLast("packet_handler", new PacketHandler());

    tcp_set_pipeline(pipeline);    

    tcp_connect("localhost", 29077);
}