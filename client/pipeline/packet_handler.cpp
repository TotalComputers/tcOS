#include "../protocol/protocol.h"
#include "pipeline.h"
#include "../encryption/encryption.h"
#include "../graphics/color.h"
#include "../../common/io_storage.h"
#include "../../common/timer.h"
#include "../compression.h"
#include "../graphics/slicing.h"
#include <openssl/rand.h>
#include <iostream>
#include <utility>

std::unordered_map<short, RepeatingTask*> global_tasks;

PacketHandler::PacketHandler(std::string token, IOFactory* factory)
    : token(std::move(token)), io_factory(factory) {}

bool PacketHandler::onConnect(ConnectionContext* ctx) {
    auto* handshake = new ServerboundHandshakePacket();
    handshake->apiVersion = 0;
    handshake->protocolVersion = 2;
    ctx->write(handshake);
    delete handshake;

    return true;
}

bool PacketHandler::onDisconnect(ConnectionContext*) {
    return true;
}

void handleEncryption(ConnectionContext* ctx, ClientboundEncryption* packet) {
    RSA* rsa = rsa_decode_key(packet->publicKey);
    if (!rsa) {
        std::cout << "Unable to decode RSA public key" << std::endl;
        return;
    }

    unsigned char raw_secret[16];
    RAND_bytes(raw_secret, 16);
    std::vector<unsigned char> secret(raw_secret, raw_secret + 16);

    auto* response = new ServerboundEncryptionPacket();
    response->secret = rsa_encrypt(rsa, secret);
    ctx->write(response);
    delete response;

    aes_init(secret);

    ctx->pipeline->addBefore("packet_decoder", "decryptor", new PacketDecryptor());
    ctx->pipeline->addAfter("packet_encoder", "encryptor", new PacketEncryptor());

    delete packet;
}

void handleHandshake(ConnectionContext* ctx, ClientboundHandshakePacket* packet, const std::string& token) {
    ctx->pipeline->addFirst("defrag", new PacketDefragmentation());
    ctx->pipeline->addBefore("packet_handler", "prefixer", new PacketLengthPrefixer());

    std::cout << "Connecting to the " << packet->serverName << " server" << std::endl;

    auto* connect = new ServerboundConnectPacket();
    connect->token = token;
    ctx->write(connect);
    delete connect;

    delete packet;
}

void handleDisconnect(ConnectionContext*, ClientboundDisconnectPacket* packet) {
    std::cout << "Disconnected: " << packet->reason << std::endl;
    delete packet;
}

void handleConnectionSuccess(ConnectionContext*, ClientboundConnectionSuccessPacket* packet) {
    std::cout << "Connected to the player " << packet->name << std::endl;
    delete packet;
}

void handlePalette(ConnectionContext*, ClientboundPalettePacket* packet) {
    std::cout << "Setting up palette... ";
    set_palette((const uint32_t*) packet->palette.data(), (int) packet->palette.size());
    std::cout << "Caching colors... ";
    cache_palette();
    std::cout << "Done. " << std::endl;
    
    delete packet;
}

void handlePing(ConnectionContext* ctx, ClientboundPingPacket* packet) {
    auto* pong = new ServerboundPongPacket();
    pong->payload = packet->payload;
    ctx->write(pong);
    delete pong;

    delete packet;
}

void handleCreationRequest(ConnectionContext* ctx, ClientboundCreationRequestPacket* packet, IOFactory* io_factory) {
    std::shared_ptr<IOInterface> i = io_factory->create();
    i->id = packet->id;
    i->name = packet->name;
    i->set_frame(new_empty_image(packet->width, packet->height));
    IOStorage::put(i);
    i->init();

    auto* status = new ServerboundCreationStatusPacket();
    status->id = packet->id;
    status->status = ServerboundCreationStatusPacket::OK;
    ctx->write(status);
    delete status;

    global_tasks[packet->id] = new RepeatingTask();
    global_tasks[packet->id]->start([=]() {
        image_t& frame = i->provide_frame();
        if (!frame.data) {
            return;
        }
        std::vector<unsigned char> indices = match_image(frame.raw8, frame.width, frame.height, 4);
        std::vector<unsigned char> sliced = slice_indices(indices, frame.width, frame.height);
        std::vector<unsigned char> deflated = compress_bytes(sliced);
        auto* frame_packet = new ServerboundFramePacket();
        frame_packet->id = i->id;
        frame_packet->compressedData = deflated;
        ctx->write(frame_packet, true);
        delete frame_packet;
    }, 1000, 1000 / 20);

    delete packet;
}

void handleDestroy(ConnectionContext*, ClientboundDestroyPacket* packet) {
    global_tasks[packet->id]->stop();
    short id = packet->id;

    DelayedTask::start([=]() {
        std::shared_ptr<IOInterface>& io = IOStorage::request(id);
        delete global_tasks[id];
        global_tasks.erase(id);
        io->destroy();
        IOStorage::remove(id);
    }, 2000);

    delete packet;
}

void handleTouch(ConnectionContext*, ClientboundTouchPacket* packet) {
    IOStorage::request(packet->id)->handle_touch(packet->x, packet->y, (bool) packet->type, packet->admin);
    delete packet;
}

void PacketHandler::handle(ConnectionContext* ctx, void* raw) {
    auto* packet = (ClientboundPacket*) raw;

    std::cout << "Received packet: 0x" << std::hex << (int) packet->getPacketID() << std::dec << std::endl;

    switch(packet->getPacketID()) {
        case 0xC3: handleEncryption(ctx, (ClientboundEncryption*) raw); break;
        case 0xB1: handleHandshake(ctx, (ClientboundHandshakePacket*) raw, token); break;
        case 0xB2: handleDisconnect(ctx, (ClientboundDisconnectPacket*) raw); break;
        case 0xB4: handleConnectionSuccess(ctx, (ClientboundConnectionSuccessPacket*) raw); break;
        case 0xC1: handlePalette(ctx, (ClientboundPalettePacket*) raw); break;
        case 0xB5: handlePing(ctx, (ClientboundPingPacket*) raw); break;
        case 0xB7: handleCreationRequest(ctx, (ClientboundCreationRequestPacket*) raw, io_factory); break;
        case 0xB9: handleDestroy(ctx, (ClientboundDestroyPacket*) raw); break;
        case 0xC2: handleTouch(ctx, (ClientboundTouchPacket*) raw); break;
    }
}