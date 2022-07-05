#pragma once

#include "internal/window.h"
#include "internal/shader.h"
#include "internal/vao.h"
#include "internal/vbo.h"

class Multisampling {
public:
    Multisampling(GLWindow*, int);
    ~Multisampling();

public:
    void createScreenVAO();
    void beforeRender();
    void afterRender();
    void render(Shader*);
    unsigned int getFramebufferHandle();
    unsigned int getScreenTexture();

private:
    GLWindow* window;
    unsigned int framebuffer;
    unsigned int msColorAttachment;
    unsigned int rbo;
    unsigned int intermediateFbo;
    unsigned int screenTexture;
    VAO* screenVao;
    VBO* screenVbo;

};