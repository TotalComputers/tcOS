#pragma once

class VBO {
public:
    VBO();
    ~VBO();

public:
    [[nodiscard]] unsigned int getHandle() const;
    void bind() const;
    void setData(void*, unsigned long long) const;
    static void unbind();

private:
    unsigned int handle = 0;

};