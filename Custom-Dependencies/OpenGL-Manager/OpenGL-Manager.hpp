#pragma once

#include <Windows.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include <stb_image.h>
#include <stdio.h>

#ifdef _DEBUG
#define GL_ERROR(x) x; \
	if (glGetError()) __debugbreak()
#else
#define GL_ERROR(x) x
#endif

namespace Man {
	class Geometry {
		GLuint bufferID; 
		GLuint vertexArray;
		GLsizei m_NumVertices;
		GLsizei vertexSize;
	public:
		Geometry(float *vertexData, GLsizei m_NumVertices, int attribSizes[], unsigned numAttribs, unsigned vertexSize);
		void bind() { GL_ERROR(glBindVertexArray(vertexArray)); }
		/// requires the buffer/arrays to be bound (use .bind()).
		void render() { GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, m_NumVertices)); }
		/// Does not require the buffer to be bound.
		void subData(float *data, GLintptr offset, GLsizeiptr size) { GL_ERROR(glNamedBufferSubData(bufferID, offset, size, data)); }
		/// Does not require the buffer to be bound.
		void subAllData(float* data) { GL_ERROR(glNamedBufferSubData(bufferID, 0, m_NumVertices * vertexSize, data)); }
		/// Does not require the buffer to be bound.
		void changeData(float* data, GLsizeiptr size) { GL_ERROR(glNamedBufferData(bufferID, size * vertexSize, data, GL_DYNAMIC_DRAW)); }
	};
	class ShaderProgram {

		void createShader(GLenum type, const wchar_t* fileName);
		GLint getUniform(const char* name) {
			GLint loc = GL_ERROR(glGetUniformLocation(shdrID, name));
			if (loc == -1) {
				shaderStatus = false;
				__debugbreak();
			}
			return loc;
		}
	protected: 
		bool shaderStatus = true;
		GLuint shdrID = 0;
		GLint *uniLocs = nullptr;

		void createTexture(const char* file, GLenum textureSlot, GLint wrapMethod);

	public: 
		void createProgram(const wchar_t* vertexShaderFile, const wchar_t* fragmentShaderFile, const char* uniformNames[], unsigned namesSize);

		void bind() { GL_ERROR(glUseProgram(shdrID)); }
		bool getShaderStatus() { return shaderStatus; }

		~ShaderProgram() {
			if(uniLocs) delete[] uniLocs; 
		}
	};
}