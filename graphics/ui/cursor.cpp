#include <glad/glad.h>
#include "cursor.h"
#include "../internal/vao.h"
#include "../internal/vbo.h"
#include "../internal/window.h"
#include "std_shaders.h"

CursorElement::CursorElement(PositionedElement* parent, Shader* cursorShader)
        : CachedElement(0, 0, 30, 30, 4), cursorShader(cursorShader) {
    setDisplayShader(CommonShaders::BlendDisplay::Get(), CommonShaders::BlendDisplay::uMatrix());
    setParent(parent);
    cache(2);
    bindLayer(1, 1);
}

void CursorElement::thisChanged() {
    CachedElement::thisChanged();
    lastActive = glfwGetTime();
    opacity = 1;
}

void CursorElement::render(int layer) {
    GLWindow::clear(0, 0, 0, 0);
    if(layer == 0) return;
    VAO* vao = new VAO();
    VBO* vbo = new VBO();

    unsigned numSides = 20;
    unsigned numVertices = numSides * 2 + 2;
    float* vertices = (float*)malloc(sizeof(float) * numVertices);

    double twoPi = 2 * 3.14159265358979323846;
    double inc = twoPi / numSides;
    float radius = (float)layer / 2.f;
    int i = 0;

    for(double angle = 0.f; angle <= twoPi; angle += inc) {
        vertices[i++] = radius * cos(angle);
        vertices[i++] = radius * sin(angle);
    }

    vao->bind();
    vbo->setData((void*)vertices, sizeof(float) * numVertices);
    vao->attribPointer2f(0, 2, 0);
    
    cursorShader->bind();
    vao->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);

    VAO::unbind();
    VBO::unbind();

    delete vao;
    delete vbo;
    free(vertices);
}

void CursorElement::display() {
    if(opacity <= 0) return;
    double diff = glfwGetTime() - lastActive;
    if(diff > 3) {
        bindLayer(0, 0);
        opacity -= (float)((diff*diff) / 45);
    }
    CommonShaders::BlendDisplay::Get()->bind();
    Shader::uniformFloat(CommonShaders::BlendDisplay::uBlendFactorLoc(), opacity);
    CachedElement::display();
}

void CursorElement::handleTouch(int x, int y, bool, bool) {
    set(x - getWidth() / 2, y - getHeight() / 2, getWidth(), getHeight());
}

void CursorElement::handleMove(int x, int y) {
    set(x - getWidth() / 2, y - getHeight() / 2, getWidth(), getHeight());
}