#version 330 core

out vec4 _color;

in vec2 _pass_texCoords;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float u_blendFactor;

void main() {
    _color = mix(texture(u_texture1, _pass_texCoords), texture(u_texture2, _pass_texCoords), vec4(u_blendFactor));
}