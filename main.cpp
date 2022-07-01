#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include <iostream>

int main() {
    protocol_registerPackets();

    ByteBuffer test;
    test.writeString("Clientbound Connection Success packet test");

    auto instance = (ClientboundConnectionSuccessPacket*)ClientboundPacket::createInstanceFromID(0xB4);
    instance->read(test);
    std::cout << instance->name << std::endl;

    tcp_connect("localhost", 29077);
}