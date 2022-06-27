#version 460 core 
layout(location = 0) out vec4 FragmentColor;

uniform vec3 graphColor;

void main() {
    FragmentColor = vec4(graphColor, 1.f);
}