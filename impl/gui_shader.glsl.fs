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

//#include <C:\Users\flora\source\repos\tinygui\impl\extended.glsl.vs>

bool compareCoordinates(vec2 cursor) {
    return cursor + vec2(0.5) == gl_FragCoord.xy;
}

void main() {
   
    if(compareCoordinates(vec2(cursorX, cursorY))) {
        idOfRectangle = rect_id;
    }
    /*
    if(vec2(cursorX, cursorY) + vec2(0.5) == gl_FragCoord.xy) {
         idOfRectangle = rect_id;
    }*/
    FragmentColor = vec4(colors[rect_id], 1.f);
}