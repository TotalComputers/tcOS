#version 330 core

out vec4 _color;

in vec2 _pass_texCoords;

uniform sampler2D u_texture;

void main() {
    _color = texture(u_texture, _pass_texCoords);
}