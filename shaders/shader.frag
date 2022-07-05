#version 330 core

out vec4 _color;

uniform float u_colorMul;

void main() {
    _color = vec4(vec3(0.29f, 0.52f, 0.45f), u_colorMul);
}