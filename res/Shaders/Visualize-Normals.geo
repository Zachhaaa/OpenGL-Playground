#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 Normal[];

void addNormalGeometry(int index) {
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + 0.5 * vec4(Normal[index], 0.0); 
	EmitVertex(); 
	EndPrimitive(); 
}

void main() {
	addNormalGeometry(0);
	addNormalGeometry(1);
	addNormalGeometry(2);
}