#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model, view, proj;

void main() {
	gl_Position = proj * view * model * vec4(0.2 * normalize(aPos) + aPos, 1.0);
}