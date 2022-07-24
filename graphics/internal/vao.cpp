#include <glad/glad.h>
#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &handle);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &handle);
}

unsigned int VAO::getHandle() const {
    return handle;
}

void VAO::bind() const {
    glBindVertexArray(handle);
}

void VAO::attribPointerf(unsigned id, unsigned count, unsigned stride, long long offset) {
    glVertexAttribPointer(id, (int) count, GL_FLOAT, GL_FALSE, (int) (stride * sizeof(float)), (void*) (offset * sizeof(float)));
    glEnableVertexAttribArray(id);
}

void VAO::attribPointer3f(unsigned id, unsigned stride, long long offset) {
    attribPointerf(id, 3, stride, offset);
}

void VAO::attribPointer4f(unsigned id, unsigned stride, long long offset) {
    attribPointerf(id, 4, stride, offset);
}

void VAO::attribPointer2f(unsigned id, unsigned stride, long long offset) {
    attribPointerf(id, 2, stride, offset);
}

void VAO::unbind() {
    glBindVertexArray(0);
}