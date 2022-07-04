#include "../protocol/protocol.h"
#include "pipeline.h"

bool PacketLengthPrefixer::encode(ConnectionContext*, void* in, void*& out) {
    out = new ByteBuffer();

    ByteBuffer* inbuf = (ByteBuffer*)in;
    ByteBuffer* outbuf = (ByteBuffer*)out;

    outbuf->writeVarInt(inbuf->readableBytes());
    outbuf->writeBytes(inbuf->data);

    delete inbuf;

    return true;
}