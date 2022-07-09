#include "std_shaders.h"

Shader* CommonShaders::defaultShader;
std::string CommonShaders::u_defaultDisplayMatrix = "u_mat";
Shader* CommonShaders::defaultDisplayShader() {
    if(defaultShader) return defaultShader;
    defaultShader = new Shader();
    defaultShader->setVertexFile("shaders/common/displayDefault.vert");
    defaultShader->setFragmentFile("shaders/common/displayDefault.frag");
    defaultShader->create();
    defaultShader->bind();
    defaultShader->uniformInt("u_texture", 0);
    Shader::unbind();
    return defaultShader;
}