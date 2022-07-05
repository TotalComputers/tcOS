#pragma once

class VBO {
public:
    VBO();
    ~VBO();

public:
    unsigned int getHandle();
    void bind();
    void setData(void*, unsigned long long);
    static void unbind();

private:
    unsigned int handle;

};