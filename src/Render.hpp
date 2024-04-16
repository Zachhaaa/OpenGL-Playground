#pragma once

#include "GlobalVariables.hpp"

inline void render() {
	model = glm::mat4(1.0f);
	view = glm::translate(glm::mat4(1.0f), camPos);
	glUniformMatrix4fv(u_Model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(u_View, 1, GL_FALSE, &view[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	SwapBuffers(dc);
	glClear(GL_COLOR_BUFFER_BIT);
}