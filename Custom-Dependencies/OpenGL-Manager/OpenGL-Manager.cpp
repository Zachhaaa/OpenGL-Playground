#include "OpenGL-Manager.hpp"

Man::Geometry::Geometry(float* vertexData, GLsizei m_NumVertices, int attribSizes[], unsigned numAttribs, unsigned vertexSize)
	: m_NumVertices(m_NumVertices), vertexSize(vertexSize)
{

	GL_ERROR(glGenVertexArrays(1, &vertexArray));
	GL_ERROR(glBindVertexArray(vertexArray));

	GL_ERROR(glGenBuffers(1, &bufferID));
	GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, bufferID));

	glBufferData(GL_ARRAY_BUFFER, m_NumVertices * vertexSize, vertexData, GL_DYNAMIC_DRAW);
	GLint offset = 0;
	for (unsigned i = 0; i < numAttribs; ++i) {
		GL_ERROR(glVertexAttribPointer(i, attribSizes[i], GL_FLOAT, GL_FALSE, vertexSize, (void*)(offset * sizeof(float))));
		GL_ERROR(glEnableVertexAttribArray(i));
		offset += attribSizes[i];
	}
}

void Man::ShaderProgram::createShader(GLenum type, const wchar_t* fileName) {
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

void Man::ShaderProgram::createTexture(const char* file, GLenum textureSlot, GLint wrapMethod) {
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

void Man::ShaderProgram::createProgram(const wchar_t* vertexShaderFile, const wchar_t* fragmentShaderFile, const char* uniformNames[], unsigned namesSize)
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