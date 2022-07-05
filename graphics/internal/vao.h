#pragma once

class VAO {
public:
    VAO();
    ~VAO();

public:
    unsigned int getHandle();
    void bind();
    void attribPointer3f(unsigned, unsigned, unsigned);
    static void unbind();

private:
    unsigned int handle;

};