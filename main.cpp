#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include "client/pipeline/pipeline.h"

#include "graphics/internal/glio.h"
#include "graphics/pbo_surface.h"

#include "common/thread_safety.h"

#include <iostream>
#include <thread>

class TestRenderer : public IRenderer {
public:
    void render() override {
        GLWindow::clear(0.5f, 0, 1, 1);
    }

};

class TestWindowFactory : public IWindowFactory {
public:
    GLWindow* createWindow(int w, int h, std::string t) override {
        GLWindow* window = new GLWindow(w, h, t);
        window->create();
        window->setSurface(new PBOSurface(window));
        window->setRenderer(new TestRenderer());
        return window;
    }

};

int main() {
    glfwInit();

    std::string ip;
    std::string token;
    int port;

    std::cout << "Enter IP (Without port): ";
    std::cin >> ip;
    std::cout << "Enter port: ";
    std::cin >> port;
    std::cout << "Enter token: ";
    std::cin >> token;
    
    std::thread([=]() {
        protocol_registerPackets();

        Pipeline* pipeline = new Pipeline();

        pipeline->addLast("packet_decoder", new PacketDecoder());
        pipeline->addLast("packet_encoder", new PacketEncoder());
        pipeline->addLast("packet_handler", new PacketHandler(token, new GLIOFactory(new TestWindowFactory())));

        tcp_set_pipeline(pipeline);    

        tcp_connect(ip.c_str(), port);
    }).detach();

    run_loop();
}