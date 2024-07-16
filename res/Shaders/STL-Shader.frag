#version 330 core

out vec4 FragColor; 

// IMPORTANT: All Normal vectors are either unit vectors or zero vectors.
in vec3 Normal;
in vec3 FragPos;

struct Material {
	vec3 color;
	float ambient, diffuse, specular, shininess;
};
uniform Material material;
uniform vec3 LightCol;
uniform vec3 LightPos;
uniform vec3 ViewPos;

void main() {
	vec3 lightDir = normalize(LightPos - FragPos);
	float diffuse = material.diffuse * max(dot(Normal, lightDir), 0.0);

	vec3 viewDir = normalize(-ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);		
	float specular = material.specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	FragColor = vec4((material.ambient + diffuse + specular) * LightCol * material.color, 1.0);
}