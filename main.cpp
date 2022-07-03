#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include "client/pipeline/pipeline.h"
#include <iostream>

int main() {
    std::string ip;
    std::string token;
    int port;

    std::cout << "Enter IP (Without port): ";
    std::cin >> ip;
    std::cout << "Enter port: ";
    std::cin >> port;
    std::cout << "Enter token: ";
    std::cin >> token;

    protocol_registerPackets();

    Pipeline* pipeline = new Pipeline();

    pipeline->addLast("packet_decoder", new PacketDecoder());
    pipeline->addLast("packet_encoder", new PacketEncoder());
    pipeline->addLast("packet_handler", new PacketHandler(token));

    tcp_set_pipeline(pipeline);    

    tcp_connect(ip.c_str(), port);
}