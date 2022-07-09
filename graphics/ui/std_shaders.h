#pragma once

#include "../internal/shader.h"

class CommonShaders {
public:
    class DefaultDisplay {
    public:
        static Shader* Get();
        static std::string uMatrix();
        static unsigned int uMatrixLoc();

    private:
        static Shader* shader;
        static unsigned int u_matLoc;

    };

    class BlendDisplay {
    public:
        static Shader* Get();
        static std::string uMatrix();
        static unsigned int uMatrixLoc();
        static std::string uBlendFactor();
        static unsigned int uBlendFactorLoc();

    private:
        static Shader* shader;
        static unsigned int u_matLoc;
        static unsigned int u_blendFactorLoc;

    };

};