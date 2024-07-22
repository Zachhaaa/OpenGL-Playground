int StlVertexAttribSizes[] = { 3, 3 };

#include "STL-Reader.hpp"
#include <Timers.hpp>

struct VertGroup {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 normalSum;
	unsigned count;
	VertGroup() {}
	VertGroup(const glm::vec3& pos, const glm::vec3& normal, unsigned count)
		: pos(pos), normal(normal), normalSum(normal), count(count) {}
	void calcAverageNormal() {
		if (normalSum == glm::vec3(0.0f, 0.0f, 0.0f)) return;
		normalSum /= (float)count;
		normalSum = glm::normalize(normalSum);
	}

};

class StlTriangle {
	unsigned size = 0;
public:
	float data[12];
	bool push_back(float num) {
		data[size] = num;
		++size;
		if (size < 12) return true;
		size = 0;
		return false;
	}
};

STL::STL(const wchar_t* filePath) {
	HANDLE file = CreateFile(
		filePath,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file == INVALID_HANDLE_VALUE) { FILE_ERROR }

	constexpr unsigned headerSize = 5;
	char buff[headerSize];
	DWORD readCheck;
	if (!ReadFile(file, buff, headerSize, &readCheck, NULL)) { FILE_ERROR }
	if (readCheck != headerSize) { FILE_ERROR }
	bool isFileAscii = true;
	char asciiHeader[] = "solid";
	for (int i = 0; i < headerSize; ++i) {
		if (buff[i] != asciiHeader[i]) { isFileAscii = false; break; }
	}

	if (isFileAscii) {
		constexpr unsigned buffSize = 100000;
		char* buffer = new char[buffSize];

		DWORD bytesRead;
		if (!ReadFile(file, buffer, buffSize, &bytesRead, NULL)) { FILE_ERROR }
		constexpr int overlapSpace = 25;
		StlTriangle triangle;
		for (char* i = buffer, *end = buffer + bytesRead - overlapSpace; true;) {
			for (; *i != '.'; ++i) {
				if (i < end) continue;
				if (bytesRead < buffSize) {
					CloseHandle(file);
					delete[] buffer;
					return;
				}
				SetFilePointer(file, -overlapSpace, NULL, FILE_CURRENT);
				if (!ReadFile(file, buffer, buffSize, &bytesRead, NULL)) { FILE_ERROR }
				i = buffer;
				end = buffer + bytesRead - overlapSpace;
				continue;
			}
			float sign = i[-2] == '-' ? -1.0f : 1.0f;
			float value = i[-1] - '0';
			++i;
			for (float multiplier = 0.1; *i != 'e'; ++i, multiplier *= 0.1) { value += multiplier * (*i - '0'); }

			value *= sign;
			float fMult = i[1] == '-' ? 0.1f : 10.0f;
			i += 2;
			int exDigits = 0;
			for (; *i != ' ' && *i != '\r' && *i != '\n'; ++i, ++exDigits) {};
			++exDigits;
			unsigned mult = 1;
			unsigned exponent = 0;
			for (int j = 1; j < exDigits; ++j, mult *= 10)
			{
				exponent += mult * (i[-j] - '0');
			}
			for (int j = 1; j < 128; j <<= 1, fMult *= fMult) {
				if (exponent & j) {
					value *= fMult;
				}
			}
			if (triangle.push_back(value)) continue;
			if (vertices.size() == vertices.capacity()) vertices.reserve(300);

			StlVertex a;

			a.normal.x = triangle.data[0];
			a.normal.y = triangle.data[1];
			a.normal.z = triangle.data[2];
			a.normal = glm::normalize(a.normal);
			a.pos.x = triangle.data[3];
			a.pos.y = triangle.data[4];
			a.pos.z = triangle.data[5];
			vertices.push_back(a);

			a.pos.x = triangle.data[6];
			a.pos.y = triangle.data[7];
			a.pos.z = triangle.data[8];
			vertices.push_back(a);

			a.pos.x = triangle.data[9];
			a.pos.y = triangle.data[10];
			a.pos.z = triangle.data[11];
			vertices.push_back(a);
		}
	}
	else {
		SetFilePointer(file, 80, NULL, FILE_BEGIN);
		DWORD bytesRead;
		unsigned facetCount;
		if (!ReadFile(file, &facetCount, sizeof(unsigned), &bytesRead, NULL)) { FILE_ERROR }

		vertices.reserve(3 * facetCount);
		constexpr unsigned sizeofFacet = sizeof(float) * 12 + 2;

		char* fileData = new char[sizeofFacet * facetCount];


		if (!ReadFile(file, fileData, sizeofFacet * facetCount, &bytesRead, NULL)) { FILE_ERROR }

		StlVertex a;
		for (char* facetPtr = fileData, *end = fileData + bytesRead; facetPtr < end; facetPtr += sizeofFacet) {

			a.normal.x = *(float*)(facetPtr + 0 * sizeof(float));
			a.normal.y = *(float*)(facetPtr + 1 * sizeof(float));
			a.normal.z = *(float*)(facetPtr + 2 * sizeof(float));
			a.pos.x = *(float*)(facetPtr + 3 * sizeof(float));
			a.pos.y = *(float*)(facetPtr + 4 * sizeof(float));
			a.pos.z = *(float*)(facetPtr + 5 * sizeof(float));
			vertices.push_back(a);

			a.pos.x = *(float*)(facetPtr + 6 * sizeof(float));
			a.pos.y = *(float*)(facetPtr + 7 * sizeof(float));
			a.pos.z = *(float*)(facetPtr + 8 * sizeof(float));
			vertices.push_back(a);

			a.pos.x = *(float*)(facetPtr + 9 * sizeof(float));
			a.pos.y = *(float*)(facetPtr + 10 * sizeof(float));
			a.pos.z = *(float*)(facetPtr + 11 * sizeof(float));
			vertices.push_back(a);
		}

		delete[] fileData;

	}
}
class VertGroupArray {
public:
	VertGroup* arrayStart;
	VertGroup* data;
	unsigned size = 0;
	unsigned capacity;

	VertGroupArray(unsigned capacity) : capacity(capacity) {
		arrayStart = new VertGroup[capacity];
		data = arrayStart + capacity;
	}
	void push_front(const StlVertex& v) {
		constexpr unsigned capIncrease = 100;
		if (size == capacity) {
			VertGroup* newData = new VertGroup[capacity  + capIncrease];
			memcpy(newData + capIncrease, data, capacity * sizeof(VertGroup));
			delete[] arrayStart;
			arrayStart = newData;
			data = newData + capIncrease;
			capacity += capIncrease;
		}
		++size;
		--data;
		data->pos       = v.pos;
		data->normal    = v.normal;
		data->normalSum = v.normal;
		data->count     = 1;
	}
	~VertGroupArray() {
		delete[] arrayStart;
	}
};

namespace My {
	float dot(const glm::vec3& a, const glm::vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
}
// TODO: optimize the following function even more
// ideas:
//  1. keep a list of what vertices have the same position
//  2. maybe multithread that?
//  3. maybe even a compute shader?
void STL::visualSmooth(float cutoffAngle) { 
	std::vector<long long> vertNormalMap;
	vertNormalMap.reserve(vertices.size());
	VertGroupArray vertGroups((unsigned)vertices.size() / 3);

	float cosCuttoffAngle = cos(cutoffAngle);
	for (StlVertex* vPtr = vertices.data(), *vEnd = vertices.data() + vertices.size(); vPtr < vEnd; ++vPtr) {
		long long i = vertGroups.size;
		for (VertGroup* gPtr = vertGroups.data, *gEnd = vertGroups.data + vertGroups.size; gPtr < gEnd; ++gPtr, --i) {
			if (vPtr->pos == gPtr->pos && My::dot(gPtr->normal, vPtr->normal) > cosCuttoffAngle) {
				gPtr->normalSum += vPtr->normal;
				++gPtr->count;
				vertNormalMap.push_back(i);
				goto posMatch;
			}
		}

		vertGroups.push_front(*vPtr); 
		vertNormalMap.push_back(vertGroups.size);

		posMatch:;
	}
	for (VertGroup* gPtr = vertGroups.data, *end = vertGroups.data + vertGroups.size; gPtr < end; ++gPtr) {
		gPtr->calcAverageNormal();
	}
	long long* mPtr = vertNormalMap.data();
	for (StlVertex* vPtr = vertices.data(), *end = vertices.data() + vertices.size(); vPtr < end; ++vPtr, ++mPtr) {
		vPtr->normal = vertGroups.data[vertGroups.size - *mPtr].normalSum; 
	}
}

