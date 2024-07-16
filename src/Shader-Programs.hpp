#pragma once

#include <OpenGL-Manager.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ObjectShader : public Man::ShaderProgram {
	const char* uniNames[11] = {
			"u_Model",
			"u_View",
			"u_Proj",
			"u_LightCol",
			"u_LightPos",
			"u_ViewPos",
			"material.ambient",
			"material.diffuse",
			"material.shininess",
			"material.objColor",
			"material.specular"
	};
public:
	ObjectShader()
	{
		createProgram(
			L"res/Shaders/Object-Shader.vert", 
			L"res/Shaders/Object-Shader.frag", 
			uniNames, 
			sizeof(uniNames) / sizeof(char*)
		);
		if (!shaderStatus) { __debugbreak(); return; }

		bind();

		createTexture("res/Textures/Crate.png", GL_TEXTURE0, GL_REPEAT);
		if (!shaderStatus) { __debugbreak(); return; }
		materialColorTextureLoc(0);

		createTexture("res/Textures/Crate-Specular.png", GL_TEXTURE1, GL_REPEAT);
		if (!shaderStatus) { __debugbreak(); return; }
		materialSpecularTextureLoc(1);
	}

	void model(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[0], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void view(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[1], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void proj(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[2], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void lightCol(const glm::vec3& rgb) { GL_ERROR(glUniform3f(uniLocs[3], rgb.x, rgb.y, rgb.z)); }
	void lightPos(const glm::vec3& pos) { GL_ERROR(glUniform3f(uniLocs[4], pos.x, pos.y, pos.z)); }
	void viewPos(const glm::vec3& pos) { GL_ERROR(glUniform3f(uniLocs[5], pos.x, pos.y, pos.z)); }
	void materialAmbient(GLfloat ambient) { GL_ERROR(glUniform1f(uniLocs[6], ambient)); }
	void materialDiffuse(GLfloat diffuse) { GL_ERROR(glUniform1f(uniLocs[7], diffuse)); }
	void materialShininess(GLfloat shininess) { GL_ERROR(glUniform1f(uniLocs[8], shininess)); }
	void materialColorTextureLoc(GLint textureLoc) { GL_ERROR(glUniform1i(uniLocs[9], textureLoc)); }
	void materialSpecularTextureLoc(GLint textureLoc) { GL_ERROR(glUniform1i(uniLocs[10], textureLoc)); }
};
class LightShader : public Man::ShaderProgram {
	const char* uniNames[4] = {
		"u_Model",
		"u_View",
		"u_Proj",
		"u_LightCol"
	};
public:
	LightShader() {
		createProgram(
			L"res/Shaders/Light-Shader.vert", 
			L"res/Shaders/Light-Shader.frag", 
			uniNames, 
			sizeof(uniNames) / sizeof(char*)
		);
		if (!shaderStatus) { __debugbreak(); return; }
	}

	void model(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[0], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void view(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[1], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void proj(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[2], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void lightCol(const glm::vec3& rgb) { GL_ERROR(glUniform3f(uniLocs[3], rgb.x, rgb.y, rgb.z)); }
};
class StlShader : public Man::ShaderProgram {
	const char* uniNames[11] = {
		"model",
		"view",
		"proj",
		"LightCol",
		"LightPos",
		"ViewPos",
		"material.color",
		"material.ambient",
		"material.diffuse",
		"material.specular",
		"material.shininess"
	};
public: 
	StlShader() {
		createProgram(
			L"res/Shaders/STL-Shader.vert", 
			L"res/Shaders/STL-Shader.frag",
			uniNames,
			sizeof(uniNames) / sizeof(char*)
		);
		if (!shaderStatus) { __debugbreak(); return; }
	}
	void model(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[0], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void view(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[1], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void proj(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[2], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void lightCol(const glm::vec3& rgb) { GL_ERROR(glUniform3f(uniLocs[3], rgb.x, rgb.y, rgb.z)); }
	void lightPos(const glm::vec3& pos) { GL_ERROR(glUniform3f(uniLocs[4], pos.x, pos.y, pos.z)); }
	void viewPos(const glm::vec3& pos) { GL_ERROR(glUniform3f(uniLocs[5], pos.x, pos.y, pos.z)); }
	void materialColor(const glm::vec3& rgb) { GL_ERROR(glUniform3f(uniLocs[6], rgb.x, rgb.y, rgb.z)); }
	void materialAmbient(GLfloat ambient) { GL_ERROR(glUniform1f(uniLocs[7], ambient)); }
	void materialDiffuse(GLfloat diffuse) { GL_ERROR(glUniform1f(uniLocs[8], diffuse)); }
	void materialSpecular(GLfloat specular) { GL_ERROR(glUniform1f(uniLocs[9], specular)); }
	void materialShininess(GLfloat shininess) { GL_ERROR(glUniform1f(uniLocs[10], shininess)); }
};
class OutlineShader : public Man::ShaderProgram {
	const char* uniNames[3] = {
		"model",
		"view",
		"proj",
	};
public:
	OutlineShader() {
		createProgram(
			L"res/Shaders/Outline.vert",
			L"res/Shaders/Outline.frag",
			uniNames,
			sizeof(uniNames) / sizeof(char*)
		);
		if (!shaderStatus) { __debugbreak(); return; }
	}
	void model(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[0], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void view(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[1], 1, GL_FALSE, (GLfloat*)&matrix)); }
	void proj(const glm::mat4& matrix) { GL_ERROR(glUniformMatrix4fv(uniLocs[2], 1, GL_FALSE, (GLfloat*)&matrix)); }
};