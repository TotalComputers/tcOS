#include "std_shaders.h"

Shader*      CommonShaders::DefaultDisplay::shader;
unsigned int CommonShaders::DefaultDisplay::u_matLoc;

Shader* CommonShaders::DefaultDisplay::Get() {
    if(shader) return shader;
    shader = new Shader();
    shader->setVertexFile("shaders/common/displayDefault.vert");
    shader->setFragmentFile("shaders/common/displayDefault.frag");
    shader->create();
    shader->bind();
    shader->uniformInt("u_texture", 0);
    u_matLoc = shader->uniformLocation("u_mat");
    Shader::unbind();
    return shader;
}

unsigned int CommonShaders::DefaultDisplay::uMatrixLoc() {
    Get();
    return u_matLoc;
}

std::string CommonShaders::DefaultDisplay::uMatrix() {
    return "u_mat";
}

Shader*      CommonShaders::BlendDisplay::shader;
unsigned int CommonShaders::BlendDisplay::u_matLoc;
unsigned int CommonShaders::BlendDisplay::u_blendFactorLoc;

Shader* CommonShaders::BlendDisplay::Get() {
    if(shader) return shader;
    shader = new Shader();
    shader->setVertexFile("shaders/common/blendDisplay.vert");
    shader->setFragmentFile("shaders/common/blendDisplay.frag");
    shader->create();
    shader->bind();
    shader->uniformInt("u_texture1", 0);
    shader->uniformInt("u_texture2", 1);
    shader->uniformFloat("u_blendFactor", 1.f);
    u_matLoc = shader->uniformLocation("u_mat");
    u_blendFactorLoc = shader->uniformLocation("u_blendFactor");
    return shader;
}

std::string CommonShaders::BlendDisplay::uBlendFactor() {
    return "u_blendFactor";
}

unsigned int CommonShaders::BlendDisplay::uBlendFactorLoc() {
    return u_blendFactorLoc;
}

std::string CommonShaders::BlendDisplay::uMatrix() {
    return "u_mat";
}

unsigned int CommonShaders::BlendDisplay::uMatrixLoc() {
    return u_matLoc;
}
