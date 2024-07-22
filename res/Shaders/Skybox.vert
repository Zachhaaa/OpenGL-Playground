#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 CubeTexCoords;

uniform mat4 view, proj;

void main() {
	CubeTexCoords = aPos; 
	vec4 pos = proj * view * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}