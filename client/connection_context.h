#pragma once

#include "pipeline.h"
#include <uv.h>

class ConnectionContext {
public:
    ConnectionContext(Pipeline*, uv_stream_t*);

    void write(void*);
    void read(ByteBuffer&);

public:
    Pipeline* pipeline;
    uv_stream_t* stream;

};