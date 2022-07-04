#include "../protocol/protocol.h"
#include "pipeline.h"
#include "../encryption/encryption.h"
#include "../graphics/color.h"
#include "../../common/io_storage.h"
#include "../../common/timer.h"
#include <openssl/rand.h>
#include <iostream>

std::unordered_map<short, RepeatingTask*> global_tasks;

PacketHandler::PacketHandler(std::string token, IOFactory* factory)
    : token(token), io_factory(factory) {}

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

void handlePalette(ConnectionContext* ctx, ClientboundPalettePacket* packet) {
    std::cout << "Setting up palette... ";
    set_palette((const uint32_t*)packet->palette.data(), packet->palette.size());
    std::cout << "Caching colors... ";
    cache_palette();
    std::cout << "Done. " << std::endl;
}

void handlePing(ConnectionContext* ctx, ClientboundPingPacket* packet) {
    ServerboundPongPacket* pong = new ServerboundPongPacket();
    pong->payload = packet->payload;
    ctx->write(pong);
}

void handleCreationRequest(ConnectionContext* ctx, ClientboundCreationRequestPacket* packet, IOFactory* io_factory) {
    std::shared_ptr<IOInterface> i = io_factory->create();
    i->id = packet->id;
    i->name = packet->name;
    i->set_frame(new_empty_image(packet->width, packet->height));
    IOStorage::put(i);
    i->init();

    ServerboundCreationStatusPacket* status = new ServerboundCreationStatusPacket();
    status->id = packet->id;
    status->status = ServerboundCreationStatusPacket::OK;
    ctx->write(status);

    global_tasks[packet->id] = new RepeatingTask();
    global_tasks[packet->id]->start([i]() {
        std::cout << "Hello world from " << i->id << std::endl;
    }, 1000, 4000);
}

void handleDestroy(ConnectionContext* ctx, ClientboundDestroyPacket* packet) {
    global_tasks[packet->id]->stop();
    delete global_tasks[packet->id];
    global_tasks.erase(packet->id);
    std::shared_ptr<IOInterface>& io = IOStorage::request(packet->id);
    io->destroy();
    IOStorage::remove(packet->id);
    io.reset();
}

void handleTouch(ConnectionContext* ctx, ClientboundTouchPacket* packet) {
    IOStorage::request(packet->id)->handle_touch(packet->x, packet->y, (bool)packet->type, packet->admin);
};

void PacketHandler::handle(ConnectionContext* ctx, void* raw) {
    ClientboundPacket* packet = (ClientboundPacket*)raw;

    std::cout << "Received packet: 0x" << std::hex << (int)packet->getPacketID() << std::dec << std::endl;

    switch(packet->getPacketID()) {
        case 0xC3: handleEncryption(ctx, (ClientboundEncryption*)raw); break;
        case 0xB1: handleHandshake(ctx, (ClientboundHandshakePacket*)raw, token); break;
        case 0xB2: handleDisconnect(ctx, (ClientboundDisconnectPacket*)raw); break;
        case 0xB4: handleConnectionSuccess(ctx, (ClientboundConnectionSuccessPacket*)raw); break;
        case 0xC1: handlePalette(ctx, (ClientboundPalettePacket*)raw); break;
        case 0xB5: handlePing(ctx, (ClientboundPingPacket*)raw); break;
        case 0xB7: handleCreationRequest(ctx, (ClientboundCreationRequestPacket*)raw, io_factory); break;
        case 0xB9: handleDestroy(ctx, (ClientboundDestroyPacket*)raw); break;
        case 0xC2: handleTouch(ctx, (ClientboundTouchPacket*)raw); break;
    }
}