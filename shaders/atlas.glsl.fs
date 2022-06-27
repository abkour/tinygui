#version 460 core 
layout(location = 0) out vec4 FragmentColor;

uniform sampler2D tex0;

in vec2 TexCoord;

void main() {
    FragmentColor = texture(tex0, TexCoord);
}