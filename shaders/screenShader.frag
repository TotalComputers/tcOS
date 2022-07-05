#version 330

out vec4 _color;

in vec2 _pass_texCoords;

uniform sampler2D u_screen;

void main() {
    vec3 color = texture(u_screen, _pass_texCoords).rgb;
    _color = vec4(color, 1.0);
}