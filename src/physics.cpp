#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>
#include <cstdio>
#include <glm\gtc\matrix_transform.hpp>
#include <time.h>
#include <ctime>
bool show_test_window = false;

float sphereX; //z
float sphereY;
float sphereZ;
namespace ClothMesh {
	extern const int numCols;
	extern const int numRows;
	extern const int numVerts;
	extern void updateClothMesh(float* array_data);

}
namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(5.f, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
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

void PhysicsInit() {
	srand(time(NULL));
	int counterX = 0;
	int counterY = 0;
	int counter = 0;
	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {

			arr[(j * ClothMesh::numCols + i) * 3] = i; //x 
			arr[(j * ClothMesh::numCols + i) * 3 + 1] = j; //y
			arr[(j * ClothMesh::numCols + i) * 3 + 2] = i; //z

		}

		//TODO
	}
	sphereX = (((float)rand() / RAND_MAX) *4.5 - 4.5); //z
	sphereY = (((float)rand() / RAND_MAX) *0.5 + 5.f);
	sphereZ = (((float)rand() / RAND_MAX) *4.5f - 4.5f);
	Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), 1.f);
	//ClothMesh::updateClothMesh(arr);
}

void PhysicsUpdate(float dt) {
	//TODO	}
	std::srand(std::time(NULL));

	float counter = 0;
	
	
}
void PhysicsCleanup() {
	//TODO
}