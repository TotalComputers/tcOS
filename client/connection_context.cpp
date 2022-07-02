#include "connection_context.h"
#include "client.h"
#include <memory>
#include <iostream>

ConnectionContext::ConnectionContext(Pipeline* pipeline, uv_stream_t* stream)
    : pipeline(pipeline), stream(stream) {}

void ConnectionContext::write(void* object) {
    void* src = object;
    void* dst;
    pipeline->forEach([this, &src, &object, &dst](AbstractHandler* handler, int) {
        if(handler->isOutboundHandler()) {
            bool res = ((OutboundHandler*)handler)->encode(this, src, dst);
            src = dst;
            return res;
        }
        return true;
    });

    if(!src) return;
    ByteBuffer* buf = (ByteBuffer*)src;

    uv_buf_t buffer;
    buffer.base = (char*)buf->data.data();
    buffer.len = buf->data.size();

    uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));
    uv_write(req, stream, &buffer, 1, [](uv_write_t* req, int status) {
        if(status) {
            std::cout << "Unable to write data: " << uv_strerror(status);
            return;
        }
        free(req);
    });
}

void ConnectionContext::read(ByteBuffer& buf) {
    std::vector<void*> src;
    std::vector<void*> dst;

    src.push_back(&buf);

    pipeline->forEach([this, &src, &dst](AbstractHandler* handler, int){
        if(handler->isInboundHandler()) {
            InboundHandler* outbound = ((InboundHandler*)handler);
            for(void*& obj : src) {
                if(!outbound->decode(this, obj, dst))
                    return false;
            }
            src = std::vector<void*>(dst);
            dst.clear();
        }
        return true;
    });
}