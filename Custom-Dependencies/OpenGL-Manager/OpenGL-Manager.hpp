#pragma once

#ifdef _DEBUG
#define GL_ERROR(x) x; \
	if (glGetError()) __debugbreak()
#else
#define GL_ERROR(x) x
#endif