#include <glad/glad.h>
#include "shader.h"
#include "../../common/file.h"
#include <glm/gtc/type_ptr.hpp>
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

void Shader::uniformBool(int loc, bool value) {
    Shader::uniformInt(loc, (int)value);
}

void Shader::uniformInt(int loc, int value) {
    glUniform1i(loc, value);
}

void Shader::uniformFloat(int loc, float value) {
    glUniform1f(loc, value);
}

void Shader::uniformVec2f(int loc, const glm::vec2& value) {
    glUniform2f(loc, value.x, value.y);
}

void Shader::uniformVec3f(int loc, const glm::vec3& value) {
    glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::uniformVec4f(int loc, const glm::vec4& value) {
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::uniformMat2f(int loc, const glm::mat2& value) {
    glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::uniformMat3f(int loc, const glm::mat3& value) {
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::uniformMat4f(int loc, const glm::mat4& value) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::uniformBool(const std::string& name, bool value) {
    Shader::uniformBool(uniformLocation(name), value);
}

void Shader::uniformInt(const std::string& name, int value) {
    Shader::uniformInt(uniformLocation(name), value);
}

void Shader::uniformFloat(const std::string& name, float value) {
    Shader::uniformFloat(uniformLocation(name), value);
}

void Shader::uniformVec2f(const std::string& name, const glm::vec2& value) {
    Shader::uniformVec2f(uniformLocation(name), value);
}

void Shader::uniformVec3f(const std::string& name, const glm::vec3& value) {
    Shader::uniformVec3f(uniformLocation(name), value);
}

void Shader::uniformVec4f(const std::string& name, const glm::vec4& value) {
    Shader::uniformVec4f(uniformLocation(name), value);
}

void Shader::uniformMat2f(const std::string& name, const glm::mat2& value) {
    Shader::uniformMat2f(uniformLocation(name), value);
}

void Shader::uniformMat3f(const std::string& name, const glm::mat3& value) {
    Shader::uniformMat3f(uniformLocation(name), value);
}

void Shader::uniformMat4f(const std::string& name, const glm::mat4& value) {
    Shader::uniformMat4f(uniformLocation(name), value);
}

unsigned int Shader::uniformLocation(const std::string& name) {
    return glGetUniformLocation(handle, name.c_str());
}