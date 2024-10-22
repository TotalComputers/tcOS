#include "../protocol/protocol.h"
#include "pipeline.h"
#include <cmath>

using std::min;

bool PacketDefragmentation::onConnect(ConnectionContext*) {
    return true;
}

bool PacketDefragmentation::onDisconnect(ConnectionContext*) {
    return true;
}

bool PacketDefragmentation::decode(ConnectionContext*, void* in, std::vector<void*>& out) {
    ByteBuffer* inbuf = (ByteBuffer*) in;
    
    while(inbuf->readableBytes() > 0) {
        if (!buf) {
            buf = new ByteBuffer();
            remaining = inbuf->readVarInt();
        }
        int toWrite = min(remaining, (int) inbuf->readableBytes());
        buf->writeBytes(*inbuf, toWrite);
        remaining -= toWrite;
        if (remaining == 0) {
            out.push_back(buf);
            buf = nullptr;
        }
    }

    delete inbuf;

    return true;
}