#include "../protocol/protocol.h"
#include "pipeline.h"

bool PacketLengthPrefixer::encode(ConnectionContext*, void* in, void*& out) {
    out = new ByteBuffer();

    auto* inbuf = (ByteBuffer*) in;
    auto* outbuf = (ByteBuffer*) out;

    outbuf->writeVarInt((int) inbuf->readableBytes());
    outbuf->writeBytes(inbuf->data);

    delete inbuf;

    return true;
}