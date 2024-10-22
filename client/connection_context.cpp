#include "connection_context.h"
#include "client.h"
#include <memory>
#include <iostream>

ConnectionContext::ConnectionContext(Pipeline* pipeline, uv_stream_t* stream)
    : pipeline(pipeline), stream(stream) {}

void write_cb(uv_write_t* req, int status) {
    if (status) {
        std::cout << "Unable to write data: " << uv_strerror(status);
        return;
    }
    free(req);
}

static uv_buf_t async_tmp;
static uv_stream_t* async_stream;
static ByteBuffer* async_buf;

void ConnectionContext::write(void* object, bool async) {
    void* src = (void*) object;
    void* dst;
    pipeline->forEach([&](AbstractHandler* handler, int) {
        if (handler->isOutboundHandler()) {
            bool res = ((OutboundHandler*) handler)->encode(this, src, dst);
            src = dst;
            return res;
        }
        return true;
    });

    if (!src) {
        return;
    }
    auto* buf = (ByteBuffer*) src;

    uv_buf_t buffer;
    buffer.base = (char*) buf->data.data();
    buffer.len = buf->data.size();

    if (!async) {
        auto* req = (uv_write_t*) malloc(sizeof(uv_write_t));
        uv_write(req, stream, &buffer, 1, write_cb);
        delete buf;
    } else {
        async_buf = buf;
        async_tmp = buffer;
        async_stream = stream;

        auto* async = (uv_async_t*) malloc(sizeof(uv_async_t));

        uv_async_init(uv_default_loop(), async, [](uv_async_t* handle) {
            auto* req = (uv_write_t*) malloc(sizeof(uv_write_t));
            uv_write(req, async_stream, &async_tmp, 1, write_cb);
            delete async_buf;
            free(handle);
        });

        uv_async_send(async);
    }

}

void ConnectionContext::read(ByteBuffer* buf) {
    std::vector<void*> src;
    std::vector<void*> dst;

    src.push_back(buf);

    pipeline->forEach([this, &src, &dst](AbstractHandler* handler, int){
        if (handler->isInboundHandler()) {
            InboundHandler* outbound = ((InboundHandler*) handler);
            for (void*& obj : src) {
                if (!outbound->decode(this, obj, dst)) {
                    return false;
                }
            }
            src = std::vector<void*>(dst);
            dst.clear();
        }
        return true;
    });
}