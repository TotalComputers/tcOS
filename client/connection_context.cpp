#include "connection_context.h"
#include "client.h"
#include <memory>
#include <iostream>

ConnectionContext::ConnectionContext(Pipeline* pipeline, uv_stream_t* stream)
    : pipeline(pipeline), stream(stream) {}

void ConnectionContext::write(const void* object) {
    void* src = (void*)object;
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

static ConnectionContext* _async_tmp_current_ctx;
void ConnectionContext::write_async(const void* object) {
    uv_async_t async;

    _async_tmp_current_ctx = this;
    _async_tmp = (void*)object;
    uv_async_init(tcp_get_uv_loop(), &async, [](uv_async_t* handle) {
        ConnectionContext* ctx = _async_tmp_current_ctx;
        const void* object = ctx->_async_tmp;
        ctx->write(object);
    });

    uv_async_send(&async);
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