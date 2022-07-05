#include <glad/glad.h>

#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include "client/pipeline/pipeline.h"

#include "graphics/internal/vao.h"
#include "graphics/internal/vbo.h"
#include "graphics/internal/shader.h"
#include "graphics/internal/glio.h"
#include "graphics/pbo_surface.h"

#include "common/thread_safety.h"

#include <iostream>
#include <thread>

class TestRenderer : public IRenderer {
public:
    TestRenderer() {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f,
        };

        vao = new VAO();
        vbo = new VBO();
        program = new Shader();

        vao->bind();
        vbo->setData((void*)vertices, sizeof(vertices));
        vao->attribPointer3f(0, 3, 0);

        VBO::unbind();
        VAO::unbind();

        program->setVertexFile  ("shaders/shader.vert");
        program->setFragmentFile("shaders/shader.frag");
        program->create();
    }

    ~TestRenderer() {
        delete vao;
        delete vbo;
        delete program;
    }

public:
    void render() override {
        GLWindow::clear(float(0xF5) / float(0xFF), float(0xDF) / float(0xFF), float(0x99) / float(0xFF), 1);

        program->bind();
        vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3); // TODO: Move this in another file
    }

private:
    VAO* vao;
    VBO* vbo;
    Shader* program;

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