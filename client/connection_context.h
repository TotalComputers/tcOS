#pragma once

#include "pipeline.h"
#include <uv.h>

class ConnectionContext {
public:
    ConnectionContext(Pipeline*, uv_stream_t*);

    void write(const void*);
    void write_async(const void*);
    void read(ByteBuffer&);

public:
    Pipeline* pipeline;
    uv_stream_t* stream;

private:
    void* _async_tmp;

};