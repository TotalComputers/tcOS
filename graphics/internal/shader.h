#pragma once

#include <string>

class Shader {
public: 
    Shader();
    ~Shader();

public:
    void setVertexFile(const std::string&);
    void setFragmentFile(const std::string&);
    void setVertexSource(const std::string&);
    void setFragmentSource(const std::string&);
    unsigned int getHandle();
    void create();
    void bind();
    static void unbind();

private:
    std::string vertexSource, fragmentSource;
    unsigned int handle;

};