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
#include "graphics/multisampling.h"
#include "graphics/utils.h"

#include "common/thread_safety.h"

#include <iostream>
#include <thread>

class TestRenderer : public IRenderer {
public:
    TestRenderer(GLWindow* window) {
        float vertices[] = {
            -0.5f, -0.5f,  0.0f,
             0.5f, -0.5f,  0.0f,
             0.0f,  0.5f,  0.0f,
        };

        vao = new VAO();
        vbo = new VBO();

        vao->bind();
        vbo->setData((void*)vertices, sizeof(vertices));
        vao->attribPointer3f(0, 3, 0);

        VBO::unbind();
        VAO::unbind();

        program = new Shader();
        program->setVertexFile  ("shaders/shader.vert");
        program->setFragmentFile("shaders/shader.frag");
        program->create();

        screenShader = new Shader();
        screenShader->setVertexFile  ("shaders/screenShader.vert");
        screenShader->setFragmentFile("shaders/screenShader.frag");
        screenShader->create();

        program->bind();
        u_colorMul = program->uniformLocation("u_colorMul");

        screenShader->bind();
        screenShader->uniformInt("u_screen", 0);

        Shader::unbind();

        msaa = new Multisampling(window, 4);
        msaa->createScreenVAO();
    }

    ~TestRenderer() {
        delete vao;
        delete vbo;
        delete program;
        delete screenShader;
        delete msaa;
    }

public:
    void render() override {
        msaa->beforeRender();

        GLUtils::enableTransparency();

        GLWindow::clear(float(0xF5) / float(0xFF), float(0xDF) / float(0xFF), float(0x99) / float(0xFF), 1);

        program->bind();
        Shader::uniformFloat(u_colorMul, (sin(glfwGetTime()) / 2.0f) + 0.5f);
        vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3); // TODO: Move this in another file

        msaa->afterRender();
        msaa->render(screenShader);
    }

private:
    VAO* vao;
    VBO* vbo;
    Shader* program;
    Shader* screenShader;
    Multisampling* msaa;

    unsigned int u_colorMul;

};

class TestWindowFactory : public IWindowFactory {
public:
    GLWindow* createWindow(int w, int h, std::string t) override {
        GLWindow* window = new GLWindow(w, h, t);
        window->create();
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