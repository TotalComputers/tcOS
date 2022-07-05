#pragma once

#include <cstdint>

class VAO {
public:
    VAO();
    ~VAO();

public:
    unsigned int getHandle();
    void bind();
    void attribPointer3f(unsigned, unsigned, unsigned long);
    static void unbind();

private:
    unsigned int handle;

};