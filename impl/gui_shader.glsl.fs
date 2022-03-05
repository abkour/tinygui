#version 460 core 
layout(origin_upper_left) in vec4 gl_FragCoord;
layout(location = 0) out vec4 FragmentColor;

layout(std430, binding = 0) buffer ssbo 
{
    int idOfRectangle;
};

flat in int rect_id;

#include <C:\Users\flora\source\repos\tinygui\impl\colortable.data>
#include <C:\Users\flora\source\repos\tinygui\impl\extended.glsl.vs>

uniform float cursorX;
uniform float cursorY;

void main() {
    if(compareCoordinates(vec2(cursorX, cursorY))) {
        idOfRectangle = rect_id;
    }
    FragmentColor = vec4(colors[rect_id], 1.f);
}