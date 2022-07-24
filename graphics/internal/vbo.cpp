#include <glad/glad.h>
#include "vbo.h"

VBO::VBO() {
    glGenBuffers(1, &handle);
}

VBO::~VBO() {
    glDeleteBuffers(1, &handle);
}

unsigned int VBO::getHandle() const {
    return handle;
}

void VBO::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VBO::setData(void* data, unsigned long long size) const {
    bind();
    glBufferData(GL_ARRAY_BUFFER, (int) size, data, GL_STATIC_DRAW);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}