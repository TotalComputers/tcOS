#include "../protocol/protocol.h"
#include "pipeline.h"

bool PacketEncoder::encode(ConnectionContext* ctx, void* src, void*& dst) {
    dst = new ByteBuffer();
    ByteBuffer* buf = (ByteBuffer*)dst;
    ServerboundPacket* packet = (ServerboundPacket*)src;

    buf->writeByte(0x0A);
    buf->writeByte(0x1F);
    buf->writeByte(packet->getPacketID());

    ByteBuffer tmp;
    packet->write(tmp);

    buf->writeInt(tmp.readableBytes());
    buf->writeBytes(tmp.data);

    return true;
};