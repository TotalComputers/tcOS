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
    void beforeRender() const;
    void afterRender();
    void render(Shader*);
    [[nodiscard]] unsigned int getFramebufferHandle() const;
    [[nodiscard]] unsigned int getScreenTexture() const;

private:
    GLWindow* window = nullptr;
    unsigned int framebuffer = 0;
    unsigned int msColorAttachment = 0;
    unsigned int rbo = 0;
    unsigned int intermediateFbo = 0;
    unsigned int screenTexture = 0;
    VAO* screenVao = nullptr;
    VBO* screenVbo = nullptr;

};