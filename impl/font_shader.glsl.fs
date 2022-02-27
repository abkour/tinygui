#version 460 core
layout(location = 0) out vec4 FragmentColor;

in vec2 texCoord;

uniform sampler2D inputText;
uniform vec3 FontColor;

void main() {
    vec4 alpha = vec4(1.f, 1.f, 1.f, texture(inputText, texCoord));
    FragmentColor = vec4(FontColor, 1.f) * alpha;
}