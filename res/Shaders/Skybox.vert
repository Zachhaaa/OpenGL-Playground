#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 CubeTexCoords;

uniform mat4 view, proj;

void main() {
	CubeTexCoords = aPos; 
	gl_Position = proj * view * (5.0 * vec4(aPos, 1.0)); 
}