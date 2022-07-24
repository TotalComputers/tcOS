#include <glad/glad.h>
#include "cursor.h"
#include "../internal/window.h"
#include "std_shaders.h"

CursorElement::CursorElement(PositionedElement* parent, Shader* cursorShader)
        : CachedElement(0, 0, 30, 30, 4), cursorShader(cursorShader) {
    setDisplayShader(CommonShaders::BlendDisplay::Get(), CommonShaders::BlendDisplay::uMatrix());
    setParent(parent);
    cache(3);
    bindLayer(0, -1);
    bindLayer(1, -1);
    bindLayer(2, -1);
}

void CursorElement::thisChanged() {
    CachedElement::thisChanged();
    lastActive = glfwGetTime();
    opacity = 1;
}

void CursorElement::render(int layer) {
    GLWindow::clear(0, 0, 0, 0);
    if (layer == 0) {
        return;
    }
    VAO* vao = new VAO();
    VBO* vbo = new VBO();

    unsigned numSides = 20;
    unsigned numVertices = numSides * 2 + 2;
    auto* vertices = (float*) malloc(sizeof(float) * numVertices);

    double twoPi = 2 * 3.14159265358979323846;
    double inc = twoPi / numSides;
    float radius = (float) layer / (1.f + (float) layer);
    int i = 0;

    for (double angle = 0.f; angle <= twoPi; angle += inc) {
        vertices[i++] = (float) (radius * cos(angle));
        vertices[i++] = (float) (radius * sin(angle));
    }

    vao->bind();
    vbo->setData((void*) vertices, sizeof(float) * numVertices);
    vao->attribPointer2f(0, 2, 0);
    
    cursorShader->bind();
    vao->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, (int) numVertices);

    VAO::unbind();
    VBO::unbind();

    delete vao;
    delete vbo;
    free(vertices);
}

void CursorElement::display() {
    if (opacity <= 0) return;
    double diff = glfwGetTime() - lastActive;
    CommonShaders::BlendDisplay::Get()->bind();
    if (diff > 3) {
        bindLayer(1, 1);
        bindLayer(0, 0);
        bindLayer(2, -1);
        opacity -= (float) ((diff*diff) / 45);
        Shader::uniformFloat((int) CommonShaders::BlendDisplay::uBlendFactorLoc(), opacity);
    } else {
        diff = glfwGetTime() - lastClicked;
        if (state > 0) {
            state -= (float) (diff * 10);
            if (state < 0) {
                state = 0;
            }
        }
        bindLayer(0, -1);
        bindLayer(1, 0);
        bindLayer(2, 1);
        Shader::uniformFloat((int) CommonShaders::BlendDisplay::uBlendFactorLoc(), state);
    }
    CachedElement::display();
}

void CursorElement::handleTouch(int x, int y, bool, bool) {
    set((float) x - getWidth() / 2, (float) y - getHeight() / 2, getWidth(), getHeight());
    lastClicked = glfwGetTime();
    state = 1;
}

void CursorElement::handleMove(int x, int y) {
    set((float) x - getWidth() / 2, (float) y - getHeight() / 2, getWidth(), getHeight());
}