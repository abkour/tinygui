#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 texCoord;

uniform mat4 projection;	// orthograpgic projection
uniform mat4 transformMatrix;

void main() {
	gl_Position = transformMatrix * projection * vec4(aPos, 0.f, 1.f);
	texCoord = aUV;
}