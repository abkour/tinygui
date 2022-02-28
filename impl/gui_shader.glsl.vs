#version 460 core 
layout(location = 0) in vec2 aPos;
layout(location = 1) in int aId;

//#include <mystatement.lib>

flat out int rect_id;

void main() {
	gl_Position = vec4(aPos, 0.f, 1.f);
	rect_id = aId;
}