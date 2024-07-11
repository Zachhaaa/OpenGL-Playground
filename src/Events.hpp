#pragma once

#include "App.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

inline void windowResize(LPARAM lParam) {
	if (lParam == 0) return;
	GL_ERROR(glViewport(0, 0, LOWORD(lParam), HIWORD(lParam)));
	aPtr->proj = glm::perspective(
		glm::radians(c_DefaultFOV),
		(float)LOWORD(lParam) / HIWORD(lParam),
		c_NearClippingDistance,
		c_FarClippingDistance
	);
	aPtr->objShdr.bind();
	aPtr->objShdr.proj(aPtr->proj);

	aPtr->lghtShdr.bind();
	aPtr->lghtShdr.proj(aPtr->proj);
}
inline void MyImGuiWindow() {
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
	ImGui::Begin("Debug Menu");
	ImGui::Text("CameraPos: (%f, %f, %f)", aPtr->cameraPos.x, aPtr->cameraPos.y, aPtr->cameraPos.z);
	ImGui::Text("CameraAngle: (%f, %f)", aPtr->cameraAngle.x, aPtr->cameraAngle.y);

	ImGui::ColorPicker3("Light Color", (float*)&aPtr->g_LightCol);
	ImGui::SliderFloat3("Light Position", (float*)&aPtr->g_LightPos, -10.0f, 10.0f);
	ImGui::Text("Object sdSettings");
	ImGui::SliderFloat("Ambient", &aPtr->crate.Mambient, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse", &aPtr->crate.Mdiffuse, 0.0f, 1.0f);
	aPtr->crate.Mshininess = pow(2.0, (double)aPtr->shineExponent);
	char format[20];
	sprintf_s(format, 20, "%.1f", aPtr->crate.Mshininess);
	ImGui::SliderInt("Shininess", &aPtr->shineExponent, 0, 11, format);
	ImGui::SliderFloat3("Crate Position", (float*)&aPtr->cratePos, -10.0f, 10.0f);
	ImGui::SliderFloat3("Crate Rotation Vector", (float*)&aPtr->crateRotateVec, -1.0f, 1.0f);
	ImGui::SliderAngle("Crate Angle", &aPtr->crateAngle);

	ImGui::End();
}
glm::vec2 operator*(float c, ImVec2& a) { return glm::vec2(c * a.x, c * a.y); }
glm::vec2 swap(const glm::vec2& a) { return glm::vec2(a.y, a.x); }

inline void render() {

	//Frame Input from Events
	ULONGLONG currTime = GetTickCount64();
	float posDiff = c_MoveSpeed * (currTime - aPtr->prevTime);
	aPtr->prevTime = currTime;
	glm::vec2 keyVec(0.0f, 0.0f);
	if (ImGui::IsKeyDown(ImGuiKey_W)) keyVec.y += posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_A)) keyVec.x += -posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_S)) keyVec.y += -posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_D)) keyVec.x += posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_Space)) aPtr->cameraPos.y += -posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) aPtr->cameraPos.y += posDiff;


	if (ImGui::IsKeyDown(ImGuiKey_MouseMiddle)) {
		ImGuiIO& io = ImGui::GetIO();
		aPtr->cameraAngle += swap(c_MouseSensitiviy * io.MouseDelta);
		if (aPtr->cameraAngle.x > glm::radians(90.0f))  aPtr->cameraAngle.x = glm::radians(90.0f);
		else if (aPtr->cameraAngle.x < glm::radians(-90.0f)) aPtr->cameraAngle.x = glm::radians(-90.0f);
		aPtr->sina = sin(aPtr->cameraAngle.y);
		aPtr->cosa = cos(aPtr->cameraAngle.y);
	}

	aPtr->cameraPos.x += -(aPtr->cosa * keyVec.x + aPtr->sina * keyVec.y);
	aPtr->cameraPos.z += (aPtr->cosa * keyVec.y - aPtr->sina * keyVec.x);

	// ImGui Rendering

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	MyImGuiWindow(); 

	// Viewport Rendering
	aPtr->objShdr.bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), aPtr->cratePos);
	model = glm::rotate(model, aPtr->crateAngle, aPtr->crateRotateVec);
	aPtr->objShdr.model(model);
	aPtr->objShdr.viewPos(aPtr->cameraPos);
	aPtr->view = glm::eulerAngleXY(aPtr->cameraAngle.x, aPtr->cameraAngle.y);
	aPtr->view = glm::translate(aPtr->view, aPtr->cameraPos);
	aPtr->objShdr.view(aPtr->view);
	aPtr->objShdr.lightCol(aPtr->g_LightCol);
	aPtr->objShdr.lightPos(aPtr->g_LightPos);
	aPtr->objShdr.materialAmbient(aPtr->crate.Mambient);
	aPtr->objShdr.materialDiffuse(aPtr->crate.Mdiffuse);
	aPtr->objShdr.materialShininess(aPtr->crate.Mshininess);

	aPtr->box.render();

	aPtr->lghtShdr.bind();

	aPtr->lghtShdr.model(glm::translate(glm::mat4(1.0f), aPtr->g_LightPos));
	aPtr->lghtShdr.lightCol(aPtr->g_LightCol);
	aPtr->lghtShdr.view(aPtr->view);

	aPtr->box.render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	aPtr->window.swapBuffers();
	aPtr->window.clearBuffer();
}