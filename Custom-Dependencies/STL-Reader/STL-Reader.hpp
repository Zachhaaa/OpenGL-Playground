#pragma once

#include <Windows.h>
#include <vector>
#include <glm/glm.hpp>

extern int StlVertexAttribSizes[];

struct StlVertex {
	glm::vec3 pos;
	glm::vec3 normal;
};

#define FILE_ERROR status = false; __debugbreak(); return;

class STL {
	bool status = true;
public:
	std::vector<StlVertex> vertices;

	STL(const wchar_t* filePath);
	STL() {}
	STL(const STL& a) : status(a.status), vertices(a.vertices) {}
	const STL& operator=(const STL& a) { 
		status = a.status;
		vertices = a.vertices;
		return a; 
	}
	bool getStatus() { return status; }
	/// @param cuttoffAngle units: radians
	void visualSmooth(float cutoffAngle); 
};