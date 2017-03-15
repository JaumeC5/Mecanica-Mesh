#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <time.h>
#include <ctime>
bool show_test_window = false;

namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(5.f, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}
namespace ClothMesh {
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
	extern void updateClothMesh(float* array_data);
}

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window) {//
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}
float arr[14 * 18 * 3];
float counterX = 0;
float counterY = 0;
int counter = 0;
void PhysicsInit() {
	//gravity
	//muelles
		float counter = 0;
		for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			arr[(j * ClothMesh::numCols + i) * 3] = -4.5+j*0.5f; //x 
			arr[(j * ClothMesh::numCols + i) * 3 + 1] = 9.5; //y 
			arr[(j * ClothMesh::numCols + i) * 3 + 2] = -4.5 + i*0.5f; //z
		}
		//TODO
	}
	ClothMesh::updateClothMesh(arr);
	
}
void PhysicsUpdate(float dt) {
	//TODO	}
	float sphereX = -4 + rand() % 8;
	float sphereY = 1 +rand () % 9;
	float sphereZ = -4 + rand() % 8;


	if (counter >= 400) { //this is 20 seconds.

		Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), 1);
		counter = 0;
	}
	else counter += 1;
	}

void PhysicsCleanup() {
	//TODO
}