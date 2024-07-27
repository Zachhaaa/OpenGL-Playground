#include "OpenGL-Manager.hpp"

#include <cstring>

Man::Geometry::Geometry(float* vertexData, GLsizei m_NumVertices, int attribSizes[], unsigned numAttribs, unsigned vertexSize, 
	unsigned *indexData, unsigned indexDataSize)
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
	if (indexData) {
		GLuint indexID;
		GL_ERROR(glGenBuffers(1, &indexID));
		GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID));
		GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_STATIC_DRAW));
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
	int width, height, nrChannels;
	unsigned char* img = stbi_load(file, &width, &height, &nrChannels, 0);

	if (!img) {
		__debugbreak();
		shaderStatus = false;
		return;
	}
	// TODO: Improve the handling of channels for different textures
	if (nrChannels != 4) {
		__debugbreak();
		shaderStatus = false;
		return;
	}
	unsigned int texture;
	GL_ERROR(glGenTextures(1, &texture));
	GL_ERROR(glActiveTexture(textureSlot));
	GL_ERROR(glBindTexture(GL_TEXTURE_2D, texture));

	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMethod));
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMethod));
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
	GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
	stbi_image_free(img);
}

void Man::ShaderProgram::createCubeMap(const char* pathToImages, const char* fileType, GLenum textureSlot) {
	GLuint texID;
	GL_ERROR(glGenTextures(1, &texID));
	GL_ERROR(glActiveTexture(textureSlot));
	GL_ERROR(glBindTexture(GL_TEXTURE_CUBE_MAP, texID));

	GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	constexpr unsigned fileNameBuffSize = 12; 
	char imgNames[][fileNameBuffSize] = {"right", "left", "top", "bottom", "front", "back"};
	// The following line is the sizes of each string in imgNames respectively
	constexpr unsigned rc = 5, lc = 4, tc = 3, bmc = 6, bkc = 4, fc = 5;

	strncpy_s(imgNames[0] +  rc, fileNameBuffSize -  rc, fileType, fileNameBuffSize -  rc);
	strncpy_s(imgNames[1] +  lc, fileNameBuffSize -  lc, fileType, fileNameBuffSize -  lc);
	strncpy_s(imgNames[2] +  tc, fileNameBuffSize -  tc, fileType, fileNameBuffSize -  tc);
	strncpy_s(imgNames[3] + bmc, fileNameBuffSize - bmc, fileType, fileNameBuffSize - bmc);
	strncpy_s(imgNames[4] +  fc, fileNameBuffSize -  fc, fileType, fileNameBuffSize -  fc);
	strncpy_s(imgNames[5] + bkc, fileNameBuffSize - bkc, fileType, fileNameBuffSize - bkc);

	unsigned pathToImagesLen = strlen(pathToImages);
	unsigned filePathSize = pathToImagesLen + fileNameBuffSize;
	char* filePath = new char[filePathSize];
	strcpy_s(filePath, filePathSize, pathToImages);

	// Stuff in loop:
	for (int i = 0; i < 6; i++) {
		strcpy_s(filePath + pathToImagesLen, fileNameBuffSize, imgNames[i]);
		int width, height, nChannels;
		unsigned char* img = stbi_load(filePath, &width, &height, &nChannels, 0);
		// TODO: Add better Channel support. Note modifying the shader to.
		if (img == nullptr || nChannels != 3) {
			__debugbreak();
			return;
		}

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			img);
		stbi_image_free(img);
	}
	delete[] filePath;
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

Man::FrameBuffer::FrameBuffer(int widthIn, int heightIn) : width(widthIn), height(heightIn) {
	GL_ERROR(glGenFramebuffers(1, &fbID));
	GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, fbID));
	
	GL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

	GL_ERROR(glGenTextures(1, &texID));
	GL_ERROR(glActiveTexture(GL_TEXTURE0));
	GL_ERROR(glBindTexture(GL_TEXTURE_2D, texID));

	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height , 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

	GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0));

	GLuint rbo;
	GL_ERROR(glGenRenderbuffers(1, &rbo));
	GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

	GL_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GL_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
	GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fbStatus = false;
		__debugbreak();
		return;
	} 
	GL_ERROR(glViewport(0, 0, width, height));
}