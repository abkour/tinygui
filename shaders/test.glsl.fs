#version 460 core 
layout(location = 0) out vec4 FragmentColor;

in vec4 color;

void main() {
    FragmentColor = vec4(color.xyz, 1.f);
}