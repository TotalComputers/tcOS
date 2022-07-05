#pragma once

#include <cstdint>

class VAO {
public:
    VAO();
    ~VAO();

public:
    unsigned int getHandle();
    void bind();
    void attribPointerf(unsigned, unsigned, unsigned, long long);
    void attribPointer4f(unsigned, unsigned, long long);
    void attribPointer3f(unsigned, unsigned, long long);
    void attribPointer2f(unsigned, unsigned, long long);
    static void unbind();

private:
    unsigned int handle;

};