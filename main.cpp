#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include "client/pipeline/pipeline.h"

#include "graphics/internal/vao.h"
#include "graphics/internal/vbo.h"
#include "graphics/internal/shader.h"
#include "graphics/internal/glio.h"
#include "graphics/ui/std_shaders.h"
#include "graphics/pbo_surface.h"
#include "graphics/multisampling.h"
#include "graphics/utils.h"

#include "common/thread_safety.h"

#include <iostream>
#include <thread>

class TestElement : public CachedElement {
public:
    TestElement(float x, float y, float w, float h)
        : CachedElement(x, y, w, h) {}

    void render(int layer) override {
        std::cout << "Rendering layer: " << layer << std::endl;
        GLWindow::clear(r, g, b, a);
    }

public:
    float r = 1, g = 0, b = 0, a = 1;

};

class TestRenderer : public IRenderer {
public:
    TestRenderer(GLWindow* window) {
        element = new TestElement(100.f, 100.f, 100.f, 100.f);
        element->setDisplayShader(CommonShaders::defaultDisplayShader(), CommonShaders::u_defaultDisplayMatrix);
        element->setParent(window->getElement());
        element->cache(1);
        element->bindLayer(0, 0);

        element2 = new TestElement(20.f, 20.f, 20.f, 20.f);
        element2->r = 0.f;
        element2->g = 1.f;
        element2->setDisplayShader(CommonShaders::defaultDisplayShader(), CommonShaders::u_defaultDisplayMatrix);
        element2->setParent(element);
        element2->cache(1);
        element2->bindLayer(0, 0);

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
        delete element;
        delete element2;
        delete screenShader;
        delete msaa;
    }

public:
    void render() override {
        element->setX(100.f + (float)(std::sin(glfwGetTime()) + 1.f) * 70);
        element2->setY(20.f + (float)(std::sin(glfwGetTime()) + 1.f) * 5);

        msaa->beforeRender();

        GLUtils::enableTransparency();
        GLWindow::clear(float(0xF5) / float(0xFF), float(0xDF) / float(0xFF), float(0x99) / float(0xFF), 1);

        element->display();
        element2->display();

        msaa->afterRender();
        msaa->render(screenShader);
    }

private:
    TestElement* element, *element2;
    Shader* screenShader;
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