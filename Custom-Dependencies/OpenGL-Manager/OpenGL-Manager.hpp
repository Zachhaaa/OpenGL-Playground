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

const GLint vertexAttribSizes[] = {3, 3, 2};
struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 textureCoord;
};

namespace Man {
	class Geometry {
		GLsizei m_NumTriangles;
	public:
		Geometry(Vertex vertexData[], GLsizei m_NumTriangles)
			: m_NumTriangles(m_NumTriangles)
		{
			GLuint vertexBuffer, vertexArray;
			
			GL_ERROR(glGenVertexArrays(1, &vertexArray));
			GL_ERROR(glBindVertexArray(vertexArray));

			GL_ERROR(glGenBuffers(1, &vertexBuffer));
			GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
		
			glBufferData(GL_ARRAY_BUFFER, m_NumTriangles * sizeof(Vertex), vertexData, GL_STATIC_DRAW);
			GLint offset = 0;
			for (unsigned i = 0; i < sizeof(vertexAttribSizes) / sizeof(GLint); ++i) {
				GL_ERROR(glVertexAttribPointer(i, vertexAttribSizes[i], GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offset * sizeof(float))));
				GL_ERROR(glEnableVertexAttribArray(i));
				offset += vertexAttribSizes[i];
			}
		}
		void render() { GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, m_NumTriangles)); }
	};
	class ShaderProgram {

		void createShader(GLenum type, const wchar_t* fileName) {
			HANDLE file = CreateFile(
				fileName,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (file == INVALID_HANDLE_VALUE) {
				shaderStatus = false;
				__debugbreak();
				return;
			}
			unsigned long long fileSize;
			GetFileSizeEx(file, (PLARGE_INTEGER)&fileSize);
			char* shaderSrc = new char[fileSize + 1];

			DWORD bytesRead;
			if (!ReadFile(file, shaderSrc, (DWORD)fileSize, &bytesRead, NULL)) {
				shaderStatus = false;
				__debugbreak();
				return;
			}

			shaderSrc[bytesRead] = '\0';

			CloseHandle(file);



			GLuint shdr = GL_ERROR(glCreateShader(type));
			if (!shdr) {
				shaderStatus = false;
				__debugbreak();
				return;
			}
			GL_ERROR(glShaderSource(shdr, 1, &shaderSrc, NULL));
			GL_ERROR(glCompileShader(shdr));

			GLint success;
			constexpr unsigned c_logSize = 512;
			char infoLog[c_logSize];
			GL_ERROR(glGetShaderiv(shdr, GL_COMPILE_STATUS, &success));
			if (!success) {
				GL_ERROR(glGetShaderInfoLog(shdr, c_logSize, NULL, infoLog));
				shaderStatus = false;
				__debugbreak();
				return;
			}

			GL_ERROR(glAttachShader(shdrID, shdr));

			GL_ERROR(glDeleteShader(shdr));
			delete[] shaderSrc;
		}
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

		void createTexture(const char* file, GLenum textureSlot,GLint wrapMethod) {
			GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMethod));
			GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMethod));
			GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			int width, height, nrChannels;
			unsigned char* img = stbi_load(file, &width, &height, &nrChannels, 0);

			if (!img) {
				__debugbreak();
				shaderStatus = false;
				return;
			}
			if (nrChannels != 4) {
				__debugbreak();
				shaderStatus = false; 
				return;
			}
			unsigned int texture;
			GL_ERROR(glActiveTexture(textureSlot));
			GL_ERROR(glGenTextures(1, &texture));
			GL_ERROR(glBindTexture(GL_TEXTURE_2D, texture));

			GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
			GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
			stbi_image_free(img);
		}

	public: 
		void createProgram(const wchar_t* vertexShaderFile, const wchar_t* fragmentShaderFile, const char* uniformNames[], unsigned namesSize)
		{
			uniLocs = new GLint[namesSize];

			shdrID = GL_ERROR(glCreateProgram());
			createShader(GL_VERTEX_SHADER, vertexShaderFile);
			if (!shaderStatus) return;
			createShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
			if (!shaderStatus) return;

			GL_ERROR(glLinkProgram(shdrID));
			GLint success;
			constexpr unsigned c_logSize = 512;
			char infoLog[c_logSize];
			GL_ERROR(glGetProgramiv(shdrID, GL_LINK_STATUS, &success));
			if (!success) {
				GL_ERROR(glGetProgramInfoLog(shdrID, c_logSize, NULL, infoLog));
				shaderStatus = false;
				__debugbreak();
				return;
			}
			for (GLint* loc = uniLocs; loc < uniLocs + namesSize; ++loc, ++uniformNames) {
				*loc = getUniform(*uniformNames); 
			}
			 
		}

		void bind() { GL_ERROR(glUseProgram(shdrID)); }
		bool getShaderStatus() { return shaderStatus; }

		~ShaderProgram() {
			if(uniLocs) delete[] uniLocs; 
		}
	};
}