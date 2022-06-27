#version 460 core 

out vec4 FragmentColor;

in vec4 color;

void main() {
	FragmentColor = vec4(color.xyz / vec3(255.f), 1.f);
}