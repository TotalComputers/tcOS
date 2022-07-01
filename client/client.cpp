#include "client.h"
#include <cstdlib>

static uv_loop_t* loop = uv_default_loop();

static void on_close(uv_handle_t* handle)
{
  printf("closed\n");
}

static void alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf) {
	*buf = uv_buf_init((char*)malloc(size), size);
}

static void on_read(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf)
{
    if(nread >= 0) {
        printf("read: %s\n", buf->base);
    }
    else {
        uv_close((uv_handle_t*)tcp, on_close);
    }

    free(buf->base);
}

static void on_connect(uv_connect_t* con, int status) {
    if(status < 0) {
        printf("Connection refused\n");
        return;
    }

    uv_stream_t* stream = con->handle;
    free(con);
    uv_read_start(stream, alloc_cb, on_read);
}

void tcp_connect(const char* host, int port) {
    uv_tcp_t* tcp = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, tcp);
    sockaddr_in dst;
    uv_ip4_addr(host, port, &dst);
    uv_connect_t* con = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    uv_tcp_connect(con, tcp, (const sockaddr*)&dst, on_connect);
    uv_run(loop, UV_RUN_DEFAULT);
}