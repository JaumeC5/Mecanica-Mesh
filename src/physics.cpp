#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <time.h>
#include <ctime>
bool show_test_window = false;
float xForce = 0, yForce = -9.81, zForce = 0;

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
float *currMesh; // mesh array
float *lastMesh;
float *tempMesh;
float *finalMesh;



float counterX = 0;
float counterY = 0;
int counter = 0;
void PhysicsInit() {
	//gravity
	//muelles
	currMesh = new float[14 * 18 * 3];
	lastMesh = new float[14 * 18 * 3];
	tempMesh = new float[14 * 18 * 3];
	finalMesh = new float[14 * 18 * 3];
		float counter = 0;
		for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			currMesh[(j * ClothMesh::numCols + i) * 3] = -4.5+j*0.5f; //x 
			currMesh[(j * ClothMesh::numCols + i) * 3 + 1] = 9.5; //y 
			currMesh[(j * ClothMesh::numCols + i) * 3 + 2] = -4.5 + i*0.5f; //z
			
			lastMesh[(j * ClothMesh::numCols + i) * 3] = currMesh[(j * ClothMesh::numCols + i) * 3];
			lastMesh[(j * ClothMesh::numCols + i) * 3 + 1] = currMesh[(j * ClothMesh::numCols + i) * 3 + 1];
			lastMesh[(j * ClothMesh::numCols + i) * 3 + 2] = currMesh[(j * ClothMesh::numCols + i) * 3 + 2];
		}
		//TODO
	}
	//ClothMesh::updateClothMesh(currMesh);
	
}
void PhysicsUpdate(float dt) {
	//TODO	
	float sphereX = -4 + rand() % 8;
	float sphereY = 1 +rand () % 9;
	float sphereZ = -4 + rand() % 8;
	if (counter >= 400) { //this is 20 seconds.

		Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), 1);
		counter = 0;
	}
	else counter += 1;

	//position update
	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			tempMesh = currMesh;

			finalMesh[(j * ClothMesh::numCols + i) * 3] = currMesh[(j * ClothMesh::numCols + i) * 3] + (currMesh[(j * ClothMesh::numCols + i) * 3] - lastMesh[(j * ClothMesh::numCols + i) * 3]) + (xForce / 1)*(dt*dt);
			finalMesh[(j * ClothMesh::numCols + i) * 3+1] = currMesh[(j * ClothMesh::numCols + i) * 3+1] + (currMesh[(j * ClothMesh::numCols + i) * 3+1] - lastMesh[(j * ClothMesh::numCols + i) * 3+1]) + (yForce / 1)*(dt*dt);
			finalMesh[(j * ClothMesh::numCols + i) * 3+2] = currMesh[(j * ClothMesh::numCols + i) * 3+2] + (currMesh[(j * ClothMesh::numCols + i) * 3+2] - lastMesh[(j * ClothMesh::numCols + i) * 3+2]) + (zForce / 1)*(dt*dt);

			lastMesh[(j * ClothMesh::numCols + i) * 3] = tempMesh[(j * ClothMesh::numCols + i) * 3];
			lastMesh[(j * ClothMesh::numCols + i) * 3+1] = tempMesh[(j * ClothMesh::numCols + i) * 3+1];
			lastMesh[(j * ClothMesh::numCols + i) * 3+2] = tempMesh[(j * ClothMesh::numCols + i) * 3+2];
		
		    currMesh[(j * ClothMesh::numCols + i) * 3] = finalMesh[(j * ClothMesh::numCols + i) * 3];
			currMesh[(j * ClothMesh::numCols + i+1) * 3+1] = finalMesh[(j * ClothMesh::numCols + i) * 3+1]; // quitar el +1
			currMesh[(j * ClothMesh::numCols + i) * 3+2] = finalMesh[(j * ClothMesh::numCols + i) * 3+2];

		}
	}

	ClothMesh::updateClothMesh(currMesh);


	}

void PhysicsCleanup() {
	//TODO
}