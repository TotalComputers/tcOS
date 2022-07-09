#include <glad/glad.h>
#include "element.h"
#include <iostream>
#include <glm/ext.hpp>
#include <algorithm>

PositionedElement::PositionedElement(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h) {
    parent = nullptr;
}

void PositionedElement::parentChanged(PositionedElement*) {}
void PositionedElement::childChanged(PositionedElement*) {}
void PositionedElement::thisChanged() {}

void PositionedElement::setParent(PositionedElement* parent) {
    if(parent) {
        this->parent = parent;
        parent->children.push_back(this);
        parent->childChanged(this);
    } else if(this->parent) {
        this->parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
        this->parent->childChanged(this);
        this->parent = nullptr;
    }
    parentChanged(parent);
}

PositionedElement* PositionedElement::getParent() {
    return parent;
}

std::vector<PositionedElement*> PositionedElement::getChildren() {
    return children;
}

void PositionedElement::addChild(PositionedElement* child) {
    if(child) child->setParent(this);
}

void PositionedElement::removeChild(PositionedElement* child) {
    if(child) child->setParent(nullptr);
}

void PositionedElement::removeChild(int idx) {
    removeChild(children[idx]);
}

float PositionedElement::getX() {
    return x;
}

float PositionedElement::getY() {
    return y;
}

float PositionedElement::getWidth() {
    return w;
}

float PositionedElement::getHeight() {
    return h;
}

float PositionedElement::getLeft() {
    if(!parent) return x;
    return x + parent->getLeft();
}

float PositionedElement::getTop() {
    if(!parent) return y;
    return y + parent->getTop();
}

float PositionedElement::getRight() {
    return getLeft() + w;
}

float PositionedElement::getBottom() {
    return getTop() + h;
}

void PositionedElement::setX(float x) {
    this->x = x;
    notifyChanges();
}

void PositionedElement::setY(float y) {
    this->y = y;
    notifyChanges();
}

void PositionedElement::setWidth(float w) {
    this->w = w;
    notifyChanges();
}

void PositionedElement::setHeight(float h) {
    this->h = h;
    notifyChanges();
}

void PositionedElement::set(float x, float y, float w, float h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    notifyChanges();
}

void PositionedElement::notifyChanges() {
    thisChanged();
    if(parent) parent->childChanged(this);
    for(PositionedElement* child : children) 
        child->parentChanged(this);
}

glm::mat4 PositionedElement::ortho() {
    if(parent) return parent->ortho();
    return glm::ortho(getLeft(), getRight(), getBottom(), getTop(), 0.0f, 1.0f);
}

glm::mat4 PositionedElement::model() {
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(getLeft(), getTop(), 0.f));
    model = glm::scale(model, glm::vec3(getWidth(), getHeight(), 1.f));
    return model;
}

CachedElement::~CachedElement() {
    recache(0);
}

IRenderableElement::IRenderableElement(float x, float y, float w, float h)
    : PositionedElement(x, y, w, h) {}

CachedElement::CachedElement(float x, float y, float w, float h, unsigned char samples)
    : IRenderableElement(x, y, w, h), numSamples(samples) {}

int CachedElement::getSamples() {
    return numSamples;
}

void CachedElement::setSamples(unsigned char samples) {
    numSamples = samples;
}

void CachedElement::cache(int layers) {
    const int prevSize = this->layers.size();
    this->layers.resize(prevSize + layers);
    for(int i = 0; i < layers; i++) {
        if(numSamples <= 1) {
            unsigned framebuffer;
            glGenFramebuffers(1, &framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            unsigned texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)getWidth(), (int)getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
            unsigned rbo;
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)getWidth(), (int)getHeight());
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Unable to create framebuffer" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glViewport(0, 0, (int)getWidth(), (int)getHeight());
            render(i);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            deletable[rbo] = glDeleteRenderbuffers;
            deletable[framebuffer] = glDeleteFramebuffers;
            deletable[texture] = glDeleteTextures;
            this->layers[prevSize + i] = texture;
        } else {
            unsigned framebuffer;
            glGenFramebuffers(1, &framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            unsigned int msTex;
            glGenTextures(1, &msTex);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msTex);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GL_RGBA, (int)getWidth(), (int)getHeight(), GL_TRUE);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msTex, 0);
            unsigned rbo;
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, GL_DEPTH24_STENCIL8, (int)getWidth(), (int)getHeight());
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Unable to create framebuffer" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            unsigned intermediate;
            glGenFramebuffers(1, &intermediate);
            glBindFramebuffer(GL_FRAMEBUFFER, intermediate);
            unsigned texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)getWidth(), (int)getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Unable to create intermediate framebuffer" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glViewport(0, 0, (int)getWidth(), (int)getHeight());
            render(i);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate);
            glBlitFramebuffer(0, 0, (int)getWidth(), (int)getHeight(), 0, 0, (int)getWidth(), (int)getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            deletable[msTex] = glDeleteTextures;
            deletable[rbo] = glDeleteRenderbuffers;
            deletable[framebuffer] = glDeleteFramebuffers;
            deletable[texture] = glDeleteTextures;
            deletable[intermediate] = glDeleteFramebuffers;
            this->layers[prevSize + i] = texture;
        }
    }
}

void CachedElement::parentChanged(PositionedElement* parent) {
    IRenderableElement::parentChanged(parent); // call base
    matrix = ortho() * model();
}

void CachedElement::thisChanged() {
    IRenderableElement::thisChanged(); // call base
    matrix = ortho() * model();
}

void CachedElement::display() {
    if(!displayShader) return;
    displayShader->bind();
    getRenderVAO()->bind();
    for(int i = 0; i < binding.size(); i++) {
        int tex = binding[i] - 1;
        if(tex < 0) continue;
        glActiveTexture(GL_TEXTURE0 + tex);
        glBindTexture(GL_TEXTURE_2D, layers[i]);
    }
    displayShader->uniformMat4f(matrixLoc, matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CachedElement::recache(int layers) {
    auto it = deletable.begin();
    while(it != deletable.end()) {
        it->second(1, &it->first);
        it++;
    }
    deletable.clear();
    this->layers.clear();
    cache(layers);
}

void CachedElement::bindLayer(int id, int tex) {
    if(id + 1 > binding.size())
        binding.resize(id + 1);
    binding[id] = tex + 1;
}

void CachedElement::setDisplayShader(Shader* shader, const std::string& uniform) {
    displayShader = shader;
    displayShader->bind();
    matrixLoc = displayShader->uniformLocation(uniform);
    Shader::unbind();
}

Shader* CachedElement::getDisplayShader() {
    return displayShader;
}

VAO* CachedElement::vao;
VBO* CachedElement::vbo;

VAO* CachedElement::getRenderVAO() {
    if(vao) return vao;

    const float screenVertices[] = {
        0.0f, 1.0f,  0.0f, 1.0f,
        0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 1.0f,
    };

    vao = new VAO();
    vbo = new VBO();

    vao->bind();
    vbo->setData((void*)screenVertices, sizeof(screenVertices));
    vao->attribPointer2f(0, 4, 0);
    vao->attribPointer2f(1, 4, 2);
    
    VAO::unbind();
    VBO::unbind();

    return vao;
}