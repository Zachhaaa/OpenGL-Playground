#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model, view, proj; 
mat3 normalModel = mat3(model); 

void main() {
	vec4 pos = vec4(aPos, 1.0);

	gl_Position = proj * view * model * pos; 
	FragPos = vec3(model * pos); 
	Normal = normalModel * aNormal; 
}