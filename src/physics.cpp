#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <time.h>
#include <ctime>
#include <vector>
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
//float *currMesh; // mesh array
//float *lastMesh;
//float *tempMesh;
//float *finalMesh;
glm::vec3 *currMesh;	
glm::vec3* lastMesh;
glm::vec3 * tempMesh;
glm::vec3 * finalMesh; 
glm::vec3 f;
int counter = 0;
class spring {
	glm::vec3 originalPos = glm::vec3();
	//float originalPos[3];
	glm::vec3 vel = glm::vec3();
	//float v[3];
	const float ke = 1;
	const float kd = 1;
	const float L = 0.5;
	float f;
public:
	float pos[3];
	//void Update();
	void fuerza(spring nextNode) {
		//f = (ke*(sqrt(pow(pos[0] - nextNode.pos[0], 2) + pow(pos[1] - nextNode.pos[1], 2) + pow(pos[2] - nextNode.pos[3], 2))-L) + kd*() )
		originalPos = glm::vec3();
		
	}
};

void PhysicsInit() {
	//gravity
	//muelles
	//currMesh = new float[14 * 18 * 3];
	//lastMesh = new float[14 * 18 * 3];
	//tempMesh = new float[14 * 18 * 3];
	//finalMesh = new float[14 * 18 * 3];
	tempMesh = new glm::vec3[14 * 18];
	finalMesh = new glm::vec3[14 * 18];
	currMesh = new glm::vec3[14 * 18];
	lastMesh = new glm::vec3[14 * 18];

		for (int i = 0; i < ClothMesh::numCols; i++) {
			for (int j = 0; j < ClothMesh::numRows; j++) {

				if (i == 0 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -4.5 + i*0.5f);
				}
				else if (i == ClothMesh::numCols-1 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -4.5 + i*0.5f);

				}

				else {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + j*0.5f, 9.5, -4.5 + i*0.5f);

					/*	currMesh[(j * ClothMesh::numCols + i) * 3] = -4.5+j*0.5f; //x
						currMesh[(j * ClothMesh::numCols + i) * 3 + 1] = 9.5; //y
						currMesh[(j * ClothMesh::numCols + i) * 3 + 2] = -4.5 + i*0.5f; //z

						*/
					lastMesh[(j * ClothMesh::numCols + i)] = currMesh[(j * ClothMesh::numCols + i)];
				}
				f = glm::vec3(0, -9.81, 0);
/*			
			lastMesh[(j * ClothMesh::numCols + i) * 3] = currMesh[(j * ClothMesh::numCols + i) * 3];
			lastMesh[(j * ClothMesh::numCols + i) * 3 + 1] = currMesh[(j * ClothMesh::numCols + i) * 3 + 1];
			lastMesh[(j * ClothMesh::numCols + i) * 3 + 2] = currMesh[(j * ClothMesh::numCols + i) * 3 + 2];*/
			
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
			finalMesh[(j * ClothMesh::numCols + i)] = currMesh[(j*ClothMesh::numCols + i)] + (currMesh[(j*ClothMesh::numCols + i)] - lastMesh[(j*ClothMesh::numCols + i)]) + (f / glm::vec3(1,1,1))*(dt*dt);
			lastMesh[(j * ClothMesh::numCols + i)] = tempMesh[(j * ClothMesh::numCols + i)];
			currMesh[(j * ClothMesh::numCols + i)] = finalMesh[(j * ClothMesh::numCols + i)];
		}
	}


/*	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			tempMesh = currMesh;
			finalMesh[(j * ClothMesh::numCols + i) * 3] = currMesh[(j * ClothMesh::numCols + i) * 3] + (currMesh[(j * ClothMesh::numCols + i) * 3] - lastMesh[(j * ClothMesh::numCols + i) * 3]) + (xForce / 1)*(dt*dt);
			finalMesh[(j * ClothMesh::numCols + i) * 3+1] = currMesh[(j * ClothMesh::numCols + i) * 3+1] + (currMesh[(j * ClothMesh::numCols + i) * 3+1] - lastMesh[(j * ClothMesh::numCols + i) * 3+1]) + (yForce / 1)*(dt*dt);
			finalMesh[(j * ClothMesh::numCols + i) * 3+2] = currMesh[(j * ClothMesh::numCols + i) * 3+2] + (currMesh[(j * ClothMesh::numCols + i) * 3+2] - lastMesh[(j * ClothMesh::numCols + i) * 3+2]) + (zForce / 1)*(dt*dt);
			
			//finalMesh[(0 * ClothMesh::numCols + 0) * 3] = currMesh[(j * ClothMesh::numCols + i) * 3] = -4.5;
			//finalMesh[(ClothMesh::numRows-1 * ClothMesh::numCols + 0) * 3] = currMesh[(j * ClothMesh::numCols + i) * 3] = -4.5;


			lastMesh[(j * ClothMesh::numCols + i) * 3] = tempMesh[(j * ClothMesh::numCols + i) * 3];
			lastMesh[(j * ClothMesh::numCols + i) * 3+1] = tempMesh[(j * ClothMesh::numCols + i) * 3+1];
			lastMesh[(j * ClothMesh::numCols + i) * 3+2] = tempMesh[(j * ClothMesh::numCols + i) * 3+2];
		
		    currMesh[(j * ClothMesh::numCols + i) * 3] = finalMesh[(j * ClothMesh::numCols + i) * 3];
			currMesh[(j * ClothMesh::numCols + i) * 3+1] = finalMesh[(j * ClothMesh::numCols + i) * 3+1]; // quitar el +1
			currMesh[(j * ClothMesh::numCols + i) * 3+2] = finalMesh[(j * ClothMesh::numCols + i) * 3+2];
		}
	}*/

	ClothMesh::updateClothMesh(&currMesh[0].x);


	}

void PhysicsCleanup() {
	//TODO
	
}