#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "GlobalVariables.hpp"

inline void renderMyWindow() {
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
	


	ImGui::Begin("Debug Menu");
	ImGui::Text("Hello, World");
	if (ImGui::CollapsingHeader("Object")) {
		ImGui::Text("This is a collapsing header");
	}
	ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", camPos.x, camPos.y, camPos.z);
	ImGui::Text("Camera Angle: (%.2f, %.2f)", camAng.y, camAng.x);
	ImGui::Text("Window Size: (%u, %u)", windowWidth, windowHeight);
	/*ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");*/
	ImGui::End();
}

inline void render() {
	using namespace glm;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	renderMyWindow();

	GL_ERROR(glUseProgram(objProg));

	model = mat4(1.0f);
	view  = eulerAngleXY(camAng.x, camAng.y);
	view  = translate(view, camPos);
	GL_ERROR(glUniformMatrix4fv(ObjUni.u_Model, 1, GL_FALSE, &model[0][0]));
	GL_ERROR(glUniformMatrix4fv(ObjUni.u_View,  1, GL_FALSE, &view[0][0]));
	GL_ERROR(glUniform3f(ObjUni.u_ViewPos, camPos.x, camPos.y, camPos.z));
	GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));

	GL_ERROR(glUseProgram(lightProg));
	
	model = translate(mat4(1.0f), lightPos);
	GL_ERROR(glUniformMatrix4fv(LightUni.u_Model, 1, GL_FALSE, &model[0][0]));
	GL_ERROR(glUniformMatrix4fv(LightUni.u_View,  1, GL_FALSE, &view[0][0]));
	GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SwapBuffers(dc);

	GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}