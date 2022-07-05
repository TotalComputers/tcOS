#pragma once

class VBO {
public:
    VBO();
    ~VBO();

public:
    unsigned int getHandle();
    void bind();
    void setData(void*, size_t);
    static void unbind();

private:
    unsigned int handle;

};