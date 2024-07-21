#pragma once

#include "App.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include <Timers.hpp>

inline void MyImGui() {
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);
	
	if (ImGui::Begin("Debug Menu")) {
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("FPS: %f", io.Framerate);
		ImGui::Text("CameraPos: (%f, %f, %f)", aPtr->cameraPos.x, aPtr->cameraPos.y, aPtr->cameraPos.z);
		ImGui::Text("CameraAngle: (%f, %f)", aPtr->cameraAngle.x, aPtr->cameraAngle.y);
		ImGui::Text("Viewport size: (%i, %i)", aPtr->viewport.getWidth(), aPtr->viewport.getWidth());

		ImGui::ColorPicker3("Light Color", (float*)&aPtr->g_LightCol);
		ImGui::SliderFloat3("Light Position", (float*)&aPtr->g_LightPos, -10.0f, 10.0f);
		ImGui::Text("Object Settings");
		ImGui::SliderFloat("Ambient", &aPtr->crate.Mambient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse", &aPtr->crate.Mdiffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular", &aPtr->crate.Mspecular, 0.0f, 1.0f);
		aPtr->crate.Mshininess = pow(2.0, (double)aPtr->shineExponent);
		char format[20];
		sprintf_s(format, 20, "%.1f", aPtr->crate.Mshininess);
		ImGui::SliderInt("Shininess", &aPtr->shineExponent, 0, 11, format);
		ImGui::SliderFloat3("Crate Position", (float*)&aPtr->cratePos, -10.0f, 10.0f);
		ImGui::SliderAngle("Smooth Cutoff Angle", &aPtr->smoothCutoffAngle, 0.0f, 180.0f);
		if (aPtr->smoothCutoffAngle != aPtr->prevSmoothCutoffAngle) {
			if (aPtr->smoothCutoffAngle == 0.0f) aPtr->mesh.subAllData((float*)aPtr->meshFile.vertices.data());
			else {
				PerfTimer a;

				PerfTimer b;
				aPtr->smoothedMesh = aPtr->meshFile;
				timers.cutoffAngleCopy = b.elapsed();

				PerfTimer c;
				aPtr->smoothedMesh.visualSmooth(aPtr->smoothCutoffAngle);
				timers.cutoffAngleSmooth = c.elapsed();

				PerfTimer d;
				aPtr->mesh.subAllData((float*)aPtr->smoothedMesh.vertices.data());
				timers.cutoffAngleSubData = d.elapsed();

				timers.cutoffAngleChange = a.elapsed();
			}
			aPtr->prevSmoothCutoffAngle = aPtr->smoothCutoffAngle;
		}

		ImGui::Text("Control Settings");
		ImGui::SliderFloat("Mouse Sensitivity", &aPtr->mouseSensitivity, 0.0001, 0.01);
		ImGui::SliderFloat("Move Speed", &aPtr->moveSpeed, 0.001, 0.01);
		ImGui::SliderInt("Swap Interval", &aPtr->swapInterval, -1, 10);
		if (aPtr->swapInterval != aPtr->prevSwapInterval) {
			aPtr->window.setSwapInterval(aPtr->swapInterval);
			aPtr->prevSwapInterval = aPtr->swapInterval;
		}
	}
	ImGui::End();
	

	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);

	if (ImGui::Begin("Timers")) {

		ImGui::Text("Cutoof Angle Change     %f", timers.cutoffAngleChange);
		ImGui::Text("Cutoof Angle Copy       %f", timers.cutoffAngleCopy);
		ImGui::Text("Cutoof Angle Smooth     %f", timers.cutoffAngleSmooth);
		ImGui::Text("Cutoof Angle SubData    %f", timers.cutoffAngleSubData);
		ImGui::Text("Clear Buffer Time       %f", timers.clearBufferTime);
	}
	ImGui::End();

	if (ImGui::Begin("Viewport")) {
		ImVec2 windowSize = ImGui::GetWindowSize();
		// TODO: Figure out how to accurately subtract the size of the title bar.
		windowSize.x -= 20;
		windowSize.y -= 45; 
		glm::mat4 proj = glm::perspective(
			glm::radians(c_DefaultFOV),
			windowSize.x / windowSize.y,
			c_NearClippingDistance,
			c_FarClippingDistance
		);
		aPtr->stlShdr.proj(proj);
		ImGui::Image(
			(void*)(intptr_t)aPtr->viewport.getTexID(),
			windowSize, 
			ImVec2(0, 1), 
			ImVec2(1, 0)
		);
	}
	ImGui::End(); 
	

}
glm::vec2 operator*(float c, ImVec2& a) { return glm::vec2(c * a.x, c * a.y); }
glm::vec2 swap(const glm::vec2& a) { return glm::vec2(a.y, a.x); }

inline float length(const ImVec2& v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

inline void render() {

	ULONGLONG currTime = GetTickCount64();
	float posDiff = aPtr->moveSpeed * (currTime - aPtr->prevTime);
	aPtr->prevTime = currTime;
	glm::vec2 keyVec(0.0f, 0.0f);
	if (ImGui::IsKeyDown(ImGuiKey_W)) keyVec.y += posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_A)) keyVec.x += -posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_S)) keyVec.y += -posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_D)) keyVec.x += posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_Space)) aPtr->cameraPos.y += -posDiff;
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) aPtr->cameraPos.y += posDiff;


	if (ImGui::IsKeyDown(ImGuiKey_E)) {
		ImGuiIO& io = ImGui::GetIO();
		aPtr->cameraAngle += swap(aPtr->mouseSensitivity * io.MouseDelta);
		if (aPtr->cameraAngle.x > glm::radians(90.0f))  aPtr->cameraAngle.x = glm::radians(90.0f);
		else if (aPtr->cameraAngle.x < glm::radians(-90.0f)) aPtr->cameraAngle.x = glm::radians(-90.0f);
		aPtr->sina = sin(aPtr->cameraAngle.y);
		aPtr->cosa = cos(aPtr->cameraAngle.y);
	}

	aPtr->cameraPos.x += -(aPtr->cosa * keyVec.x + aPtr->sina * keyVec.y);
	aPtr->cameraPos.z += (aPtr->cosa * keyVec.y - aPtr->sina * keyVec.x);
	if (ImGui::IsKeyDown(ImGuiKey_R)) {
		ImGuiIO& io = ImGui::GetIO();
		aPtr->orbit = aPtr->orbit * glm::eulerAngleY(aPtr->mouseSensitivity * io.MouseDelta.x);
		aPtr->orbit = glm::eulerAngleX(aPtr->mouseSensitivity * io.MouseDelta.y) * aPtr->orbit;
	}

	glm::mat4 model = glm::translate(aPtr->orbit, aPtr->cratePos);

	// Viewport Rendering
	aPtr->viewport.bind();

	aPtr->viewport.clear(); 

	aPtr->stlShdr.model(model);
	glm::mat4 view = glm::eulerAngleXY(aPtr->cameraAngle.x, aPtr->cameraAngle.y);
	view = glm::translate(view, aPtr->cameraPos); 
	aPtr->stlShdr.view(view);
	aPtr->stlShdr.lightCol(aPtr->g_LightCol);
	aPtr->stlShdr.lightPos(aPtr->g_LightPos);
	aPtr->stlShdr.viewPos(aPtr->cameraPos);
	aPtr->stlShdr.materialColor(aPtr->crate.Mcolor);
	aPtr->stlShdr.materialAmbient(aPtr->crate.Mambient);
	aPtr->stlShdr.materialDiffuse(aPtr->crate.Mdiffuse); 
	aPtr->stlShdr.materialSpecular(aPtr->crate.Mspecular);
	aPtr->stlShdr.materialShininess(aPtr->crate.Mshininess); 

	aPtr->mesh.render();

	aPtr->viewport.unbind();

	//Frame Input from Events
	// ImGui Rendering


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::ShowDemoWindow();

	MyImGui(); 

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	

	aPtr->window.swapBuffers();
	aPtr->window.clearBuffer();

}