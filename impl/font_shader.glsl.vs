#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 0) in vec2 aUV;

out vec2 texCoord;

uniform vec2 screen_resolution;

vec2 mapFromWindowToClipSpace(vec2 windowCoordinates) {
	return windowCoordinates / screen_resolution;
}

void main() {
	vec2 screenPositionQuad = mapFromWindowToClipSpace(aPos);
	gl_Position = vec4(screenPositionQuad, 0.f, 1.f);
	texCoord = aUv;
}