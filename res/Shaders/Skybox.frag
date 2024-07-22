#version 460 core

in vec3 CubeTexCoords;

out vec4 FragColor;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, CubeTexCoords); 
}