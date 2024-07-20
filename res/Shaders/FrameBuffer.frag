#version 460 core

out vec4 fragColor;

in vec3 texCoord;

uniform sampler2D tex;

void main() {
	fragColor = texture(tex, texCoord);
}