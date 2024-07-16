#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
		
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
		
uniform mat4 u_Model, u_View, u_Proj;
		
void main() {
	gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
	FragPos = vec3(u_Model * vec4(aPos, 1.0));
	Normal = vec3(u_Model * vec4(aNormal, 0.0));
	TexCoord = aTexCoord;
}