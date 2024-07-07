#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "GlobalVariables.hpp"

inline void renderMyWindow() {
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
	


	ImGui::Begin("Debug Menu");
	if (ImGui::CollapsingHeader("Camera & Window Info")) {
		ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", camPos.x, camPos.y, camPos.z);
		ImGui::Text("Camera Angle: (%.2f, %.2f)", camAng.y, camAng.x);
		ImGui::Text("Window Size: (%u, %u)", windowWidth, windowHeight);
	}
	if (ImGui::CollapsingHeader("Light Settings")) {
		ImGui::Text("Light Color");
		ImGui::ColorPicker3("Light Color", (float*)&lightCol);
	}
	if (ImGui::CollapsingHeader("Object Color Settings")) {
		ImGui::SliderFloat("Ambient", &objAmbient, 0.0f, 1.0);
		ImGui::SliderFloat("Diffuse", &objDiffuse, 0.0f, 1.0);
		ImGui::SliderFloat("Shininess", &objShininess, 1.0f, 512); 
	}
	ImGui::Text("fps: %.3f", ImGui::GetIO().Framerate);
	ImGui::End();
}

inline void render() {
	using namespace glm;

	if (isMinimized) return;

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
	GL_ERROR(glUniform3f(ObjUni.u_LightCol, lightCol.x, lightCol.y, lightCol.z));
	GL_ERROR(glUniform1f(ObjUni.u_Material.ambient, objAmbient));
	GL_ERROR(glUniform1f(ObjUni.u_Material.diffuse, objDiffuse));
	GL_ERROR(glUniform1f(ObjUni.u_Material.shininess, objShininess));

	GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));

	GL_ERROR(glUseProgram(lightProg));
	
	GL_ERROR(glUniform3f(LightUni.u_LightCol, lightCol.x, lightCol.y, lightCol.z));

	model = translate(mat4(1.0f), lightPos);
	GL_ERROR(glUniformMatrix4fv(LightUni.u_Model, 1, GL_FALSE, &model[0][0]));
	GL_ERROR(glUniformMatrix4fv(LightUni.u_View,  1, GL_FALSE, &view[0][0]));
	GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	GL_ERROR(SwapBuffers(dc));

 	GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}