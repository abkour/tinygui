#version 460 core
layout(location = 0) out vec4 FragmentColor;

in vec2 texCoord;

uniform sampler2D inputText;
uniform vec3 fontColor;

void main() {
    vec4 alpha = vec4(fontColor, texture(inputText, texCoord).r);
    FragmentColor = vec4(1.f) * alpha;
}