#include <glad/glad.h>
#include "multisampling.h"
#include <iostream>

Multisampling::Multisampling(GLWindow* window, int samples) 
  : window(window) { 
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glGenTextures(1, &msColorAttachment);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msColorAttachment);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, window->getWidth(), window->getHeight(), GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msColorAttachment, 0);
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, window->getWidth(), window->getHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Unable to create framebuffer" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glGenFramebuffers(1, &intermediateFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFbo);
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window->getWidth(), window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Unable to create intermediate framebuffer" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Multisampling::~Multisampling() {
    glDeleteFramebuffers(1, &framebuffer);
    framebuffer = 0;
    glDeleteTextures(1, &msColorAttachment);
    msColorAttachment = 0;
    glDeleteRenderbuffers(1, &rbo);
    rbo = 0;
    glDeleteFramebuffers(1, &intermediateFbo);
    intermediateFbo = 0;
    glDeleteTextures(1, &screenTexture);
    screenTexture = 0;
    if (screenVao) {
        delete screenVao;
        screenVao = nullptr;
    }
    if (screenVbo) {
        delete screenVbo;
        screenVbo = nullptr;
    }
}

void Multisampling::createScreenVAO() {
    const float screenVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
    };

    screenVao = new VAO();
    screenVbo = new VBO();

    screenVao->bind();
    screenVbo->setData((void*) screenVertices, sizeof(screenVertices));
    screenVao->attribPointer2f(0, 4, 0);
    screenVao->attribPointer2f(1, 4, 2);
    
    VAO::unbind();
    VBO::unbind();
}

void Multisampling::beforeRender() const {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Multisampling::afterRender() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFbo);
    glBlitFramebuffer(0, 0, window->getWidth(), window->getHeight(), 0, 0, window->getWidth(), window->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Multisampling::render(Shader* screenShader) {
    if (screenVao) {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        screenShader->bind();
        screenVao->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        VAO::unbind();
        Shader::unbind();
    }
}

unsigned int Multisampling::getFramebufferHandle() const {
    return intermediateFbo;
}

unsigned int Multisampling::getScreenTexture() const {
    return screenTexture;
}