#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 texCoord;

uniform mat4 projection;	// orthograpgic projection

void main() {
	gl_Position = projection * vec4(aPos, 0.f, 1.f);
	texCoord = aUV;
}