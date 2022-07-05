#version 330 core

layout (location = 0) in vec2 _in_pos;
layout (location = 1) in vec2 _in_texCoords;

out vec2 _pass_texCoords;

void main() {
    _pass_texCoords = _in_texCoords;
    gl_Position = vec4(_in_pos.x, _in_pos.y, 0.0, 1.0);
}