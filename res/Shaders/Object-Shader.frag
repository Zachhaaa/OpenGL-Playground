#version 460 core

out vec4 FragColor;
		
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
		
struct Material {
	sampler2D objColor;
	sampler2D specular;
	float ambient, diffuse, shininess;
};
		
uniform Material material;
uniform vec3 u_LightCol;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
		
void main() {
	vec3 lightDir = normalize(u_LightPos - FragPos);
	float diff = max(dot(Normal, lightDir), 0.0);
	float diffuse = diff * material.diffuse;
		  
	vec3 viewDir = normalize(-u_ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = spec * texture(material.specular, TexCoord);
		  
	FragColor = specular + (material.ambient + diffuse) * vec4(u_LightCol, 1.0) * texture(material.objColor, TexCoord);
}