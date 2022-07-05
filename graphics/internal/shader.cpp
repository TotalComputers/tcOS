#include <glad/glad.h>
#include "shader.h"
#include "../../common/file.h"
#include <iostream>

Shader::Shader() {}

Shader::~Shader() {
    glDeleteProgram(handle);
}

void Shader::setVertexFile(const std::string& path) {
    setVertexSource(readFile(path));
}

void Shader::setFragmentFile(const std::string& path) {
    setFragmentSource(readFile(path));
}

void Shader::setVertexSource(const std::string& src) {
    vertexSource = src;
}

void Shader::setFragmentSource(const std::string& src) {
    fragmentSource = src;
}

unsigned int Shader::getHandle() {
    return handle;
}

unsigned createShader(unsigned type, const std::string& src) {
    const char* c_src = src.c_str();
    unsigned handle = glCreateShader(type);
    glShaderSource(handle, 1, &c_src, 0);
    glCompileShader(handle);

    int res;
    char what[1024];
    glGetShaderiv(handle, GL_COMPILE_STATUS, &res);
    if(!res) {
        glGetShaderInfoLog(handle, 1024, 0, what);
        std::cerr << "Unable to compile shader (" << type << ")" << std::endl << " > " << what << std::endl;
        return 0;
    }
    return handle;
}

void Shader::create() {
    unsigned vertex, fragment;

    vertex = createShader(GL_VERTEX_SHADER, vertexSource);
    fragment = createShader(GL_FRAGMENT_SHADER, fragmentSource);

    handle = glCreateProgram();
    glAttachShader(handle, vertex);
    glAttachShader(handle, fragment);
    glLinkProgram(handle);

    int res;
    char what[1024];
    glGetProgramiv(handle, GL_LINK_STATUS, &res);
    if(!res) {
        glGetProgramInfoLog(handle, 1024, 0, what);
        std::cerr << "Unable to link program: " << std::endl << " > " << what << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::bind() {
    glUseProgram(handle);
}

void Shader::unbind() {
    glUseProgram(0);
}