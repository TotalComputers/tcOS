#include <glad/glad.h>
#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &handle);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &handle);
}

unsigned int VAO::getHandle() {
    return handle;
}

void VAO::bind() {
    glBindVertexArray(handle);
}

void VAO::attribPointer3f(unsigned id, unsigned stride, unsigned long offset) {
    glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offset);
    glEnableVertexAttribArray(id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}