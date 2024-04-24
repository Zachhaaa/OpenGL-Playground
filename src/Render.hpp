#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "GlobalVariables.hpp"

inline void render() {
	using namespace glm;
	GL_ERROR(glUseProgram(objProg));

	model = mat4(1.0f);
	view  = eulerAngleXY(camAng.x, camAng.y);
	view  = translate(view, camPos);
	GL_ERROR(glUniformMatrix4fv(u_ObjModel, 1, GL_FALSE, &model[0][0]));
	GL_ERROR(glUniformMatrix4fv(u_ObjView,  1, GL_FALSE, &view[0][0]));
	GL_ERROR(glUniform3f(u_ViewPos, camPos.x, camPos.y, camPos.z));
	GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));

	GL_ERROR(glUseProgram(lightProg));
	
	model = translate(mat4(1.0f), lightPos);
	GL_ERROR(glUniformMatrix4fv(u_LightModel, 1, GL_FALSE, &model[0][0]));
	GL_ERROR(glUniformMatrix4fv(u_LightView,  1, GL_FALSE, &view[0][0]));
	GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));

	SwapBuffers(dc);
	GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}