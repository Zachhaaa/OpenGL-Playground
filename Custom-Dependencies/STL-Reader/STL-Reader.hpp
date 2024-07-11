#pragma once

#include <Windows.h>
#include <vector>

class STL {
public:
	std::vector<float> vertices;

	STL(const wchar_t* filePath) {
		HANDLE file = CreateFile(
			filePath,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (file == INVALID_HANDLE_VALUE) {
			__debugbreak();
			return;
		}
		constexpr unsigned buffSize= 100000;
		char* buffer = new char[buffSize];

		DWORD bytesRead;
		if (!ReadFile(file, buffer, buffSize, &bytesRead, NULL)) {
			__debugbreak();
			return;
		}
		constexpr int overlapSpace = 25;
		for (char* i = buffer, *end = buffer + bytesRead - overlapSpace; true;) {
			for (; *i != '.'; ++i) {
				if (i < end) continue;
				if (bytesRead < buffSize) {
					CloseHandle(file);
					delete[] buffer;
					return;
				}
				SetFilePointer(file, -overlapSpace, NULL, FILE_CURRENT); 
				if (!ReadFile(file, buffer, buffSize, &bytesRead, NULL)) {
					__debugbreak();
					return;
				}
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
			if(vertices.size() == vertices.capacity()) vertices.reserve(1200);
			vertices.emplace_back(value);
		}
	}
};