#include "Window-Manager.hpp"

BOOL(WINAPI* wglSwapIntervalEXT)(int) = (WGLSWAPINTERVALEXT)wglGetProcAddress("wglSwapIntervalEXT");