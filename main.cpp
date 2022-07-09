#include <glad/glad.h>

#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include "client/pipeline/pipeline.h"

#include "graphics/internal/vao.h"
#include "graphics/internal/vbo.h"
#include "graphics/internal/shader.h"
#include "graphics/internal/glio.h"
#include "graphics/ui/std_shaders.h"
#include "graphics/ui/cursor.h"
#include "graphics/pbo_surface.h"
#include "graphics/multisampling.h"
#include "graphics/utils.h"

#include "common/thread_safety.h"

#include <iostream>
#include <thread>

class TestRenderer : public IRenderer {
public:
    TestRenderer(GLWindow* window) {
        cursorShader = new Shader();
        cursorShader->setVertexFile("shaders/cursor.vert");
        cursorShader->setFragmentFile("shaders/cursor.frag");
        cursorShader->create();
        cursor = new CursorElement(window->getElement(), cursorShader);
        window->addInputHandler(cursor);

        screenShader = new Shader();
        screenShader->setVertexFile  ("shaders/screenShader.vert");
        screenShader->setFragmentFile("shaders/screenShader.frag");
        screenShader->create();

        screenShader->bind();
        screenShader->uniformInt("u_screen", 0);

        Shader::unbind();

        msaa = new Multisampling(window, 4);
        msaa->createScreenVAO();
    }

    ~TestRenderer() {
        delete cursor;
        delete cursorShader;
        delete screenShader;
        delete msaa;
    }

public:
    void render() override {
        msaa->beforeRender();

        GLUtils::enableTransparency();
        GLWindow::clear(float(0xF5) / float(0xFF), float(0xDF) / float(0xFF), float(0x99) / float(0xFF), 1);

        cursor->display();

        msaa->afterRender();
        msaa->render(screenShader);
    }

private:
    CursorElement* cursor;
    Shader* screenShader, *cursorShader;
    Multisampling* msaa;

};

class TestWindowFactory : public IWindowFactory {
public:
    GLWindow* createWindow(int w, int h, std::string t) override {
        GLWindow* window = new GLWindow(w, h, t);
        window->create();
        // window->headless();
        window->setSurface(new PBOSurface(window));
        window->setRenderer(new TestRenderer(window));
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