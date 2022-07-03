#include "../protocol/protocol.h"
#include "pipeline.h"
#include "../encryption/encryption.h"
#include <openssl/rand.h>
#include <iostream>

PacketHandler::PacketHandler(std::string token)
    : token(token) {}

bool PacketHandler::onConnect(ConnectionContext* ctx) {
    ServerboundHandshakePacket* handshake = new ServerboundHandshakePacket();
    handshake->apiVersion = 0;
    handshake->protocolVersion = 2;
    ctx->write(handshake);

    return true;
}

bool PacketHandler::onDisconnect(ConnectionContext*) {
    return true;
}

void handleEncryption(ConnectionContext* ctx, ClientboundEncryption* packet) {
    RSA* rsa = rsa_decode_key(packet->publicKey);
    if(!rsa) {
        std::cout << "Unable to decode RSA public key" << std::endl;
        return;
    }

    unsigned char raw_secret[16];
    RAND_bytes(raw_secret, 16);
    std::vector<unsigned char> secret(raw_secret, raw_secret + 16);

    ServerboundEncryptionPacket* response = new ServerboundEncryptionPacket();
    response->secret = rsa_encrypt(rsa, secret);
    ctx->write(response);

    aes_init(secret);

    ctx->pipeline->addBefore("packet_decoder", "decryptor", new PacketDecryptor());
    ctx->pipeline->addAfter("packet_encoder", "encryptor", new PacketEncryptor());

}

void handleHandshake(ConnectionContext* ctx, ClientboundHandshakePacket* packet, const std::string& token) {
    ctx->pipeline->addFirst("defrag", new PacketDefragmentation());
    ctx->pipeline->addBefore("packet_handler", "prefixer", new PacketLengthPrefixer());

    std::cout << "Connecting to the " << packet->serverName << " server" << std::endl;

    ServerboundConnectPacket* connect = new ServerboundConnectPacket();
    connect->token = token;
    ctx->write(connect);
}

void handleDisconnect(ConnectionContext* ctx, ClientboundDisconnectPacket* packet) {
    std::cout << "Disconnected: " << packet->reason << std::endl;
}

void handleConnectionSuccess(ConnectionContext* ctx, ClientboundConnectionSuccessPacket* packet) {
    std::cout << "Connected to the player " << packet->name << std::endl;
}

void PacketHandler::handle(ConnectionContext* ctx, void* raw) {
    ClientboundPacket* packet = (ClientboundPacket*)raw;

    std::cout << "Received packet: 0x" << std::hex << (int)packet->getPacketID() << std::endl;

    switch(packet->getPacketID()) {
        case 0xC3: handleEncryption(ctx, (ClientboundEncryption*)raw); break;
        case 0xB1: handleHandshake(ctx, (ClientboundHandshakePacket*)raw, token); break;
        case 0xB2: handleDisconnect(ctx, (ClientboundDisconnectPacket*)raw); break;
        case 0xB4: handleConnectionSuccess(ctx, (ClientboundConnectionSuccessPacket*)raw); break;
    }
}