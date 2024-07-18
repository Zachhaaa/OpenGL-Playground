#version 460 core

out vec4 lightColor;
		
uniform vec3 u_LightCol;
		
void main() {
	lightColor = vec4(u_LightCol, 1.0f);
}