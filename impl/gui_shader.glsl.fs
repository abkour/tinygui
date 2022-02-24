#version 460 core 
layout(location = 0) out vec4 FragmentColor;

flat in int rect_id;

const vec3 colors[3] = vec3[3]
(
    vec3(1.f, 0.f, 0.f),
    vec3(0.f, 1.f, 0.f),
    vec3(0.f, 0.f, 1.f),
    vec3(1.f, 1.f, 0.f) // If cursor hovers over rectangle we change color to yellow
);

void main() {
    FragmentColor = vec4(colors[rect_id], 1.f);
}