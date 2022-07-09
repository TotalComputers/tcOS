#pragma once

#include "../internal/shader.h"

class CommonShaders {
public:
    static Shader* defaultDisplayShader();
    static std::string u_defaultDisplayMatrix;

private:
    static Shader* defaultShader;

};