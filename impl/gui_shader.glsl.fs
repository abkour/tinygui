#version 460 core 
layout(origin_upper_left) in vec4 gl_FragCoord;
layout(location = 0) out vec4 FragmentColor;

layout(std430, binding = 0) buffer ssbo 
{
    int idOfRectangle;
};

flat in int rect_id;

const vec3 colors[4] = vec3[4]
(
    vec3(1.f, 0.f, 0.f),
    vec3(0.f, 1.f, 0.f),
    vec3(0.f, 0.f, 1.f),
    vec3(1.f, 1.f, 0.f) // If cursor hovers over rectangle we change color to yellow
);

uniform float cursorX;
uniform float cursorY;

void main() {
    if(vec2(cursorX, cursorY) + vec2(0.5) == gl_FragCoord.xy) {
        if(rect_id == 2) {
            idOfRectangle = 42;
        } else {
            idOfRectangle = 0;
        }
    }
    FragmentColor = vec4(colors[rect_id], 1.f);
}