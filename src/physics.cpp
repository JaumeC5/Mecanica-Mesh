#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <time.h>
#include <ctime>
#include <vector>
using namespace std;
bool show_test_window = false;
float xForce = 0, yForce = -9.81, zForce = 0;

bool reset = false;

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

		if (ImGui::Button("Reset")) {
			reset = true;
		}

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
const float k = 1.f; //rigidez
class Particle {
public:
	void calculateForce(Particle nextP);
	glm::vec3 ActualPos;
	glm::vec3 lastPos;
	glm::vec3 vel;
	glm::vec3 OriginalPos;
	glm::vec3 Force;
};
void Particle::calculateForce(Particle nextP) {
	Force -= (k*(abs(ActualPos - OriginalPos) - )
		// Calculat forces L12 es sa distancia inicial, que varia depenen d on se trobi s altra particula. en diagonal se fa pitagoras, ect
		//k*(ActualPos - nextP.ActualPos - (sqrt(pow(OriginalPos.x - nextP.OriginalPos.x, 2) + pow(OriginalPos.y - nextP.OriginalPos.y, 2) + pow(OriginalPos.z - nextP.OriginalPos.z, 2)))*((ActualPos - nextP.ActualPos) / sqrt(pow(ActualPos.x - nextP.ActualPos.x, 2) + pow(ActualPos.y - nextP.ActualPos.y, 2) + pow(ActualPos.z - nextP.ActualPos.z, 2))));
}
	Particle* parVerts;
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
	parVerts = new Particle[14 * 18];
		for (int i = 0; i < ClothMesh::numCols; i++) {
			for (int j = 0; j < ClothMesh::numRows; j++) {
				if (i == 0 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -4.5 + i*0.5f);
				}
				else if (i == ClothMesh::numCols-1 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -4.5 + i*0.5f);
				}
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + j*0.5f, 9.5, -4.5 + i*0.5f);
					lastMesh[(j * ClothMesh::numCols + i)] = currMesh[(j * ClothMesh::numCols + i)];
					f = glm::vec3(0, -9.81, 0);
					parVerts[j * ClothMesh::numCols + i].OriginalPos = currMesh[(j * ClothMesh::numCols + i)];
		}
	}
	//ClothMesh::updateClothMesh(currMesh);
}
void PhysicsUpdate(float dt) {
	//TODO	
	float sphereX = -4 + rand() % 8;
	float sphereY = 1 +rand () % 9;
	float sphereZ = -4 + rand() % 8;

	 if (counter >= 400) { //this is 20 seconds.
		PhysicsInit();
		Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), 1);
		counter = 0;
	}
	else counter += 1;

	//position update
	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			tempMesh = currMesh;
			if (i == 0 && j == 0) {
				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + 0.5f, 9.5, -4.5 + 0.5f);
			}
			else if (i == ClothMesh::numCols - 1 && j == 0) {
				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + 0.5f, 9.5,  0.5f);
			}
			finalMesh[(j * ClothMesh::numCols + i)] = currMesh[(j*ClothMesh::numCols + i)] + (currMesh[(j*ClothMesh::numCols + i)] - lastMesh[(j*ClothMesh::numCols + i)]) + (f)*(dt*dt);
			lastMesh[(j * ClothMesh::numCols + i)] = tempMesh[(j * ClothMesh::numCols + i)];
			currMesh[(j * ClothMesh::numCols + i)] = finalMesh[(j * ClothMesh::numCols + i)];
			parVerts[(j * ClothMesh::numCols + i)].lastPos = lastMesh[(j * ClothMesh::numCols + i)];

			parVerts[(j * ClothMesh::numCols + i)].ActualPos = currMesh[(j * ClothMesh::numCols + i)];
			parVerts[(j * ClothMesh::numCols + i)].vel = (parVerts[(j * ClothMesh::numCols + i)].ActualPos - parVerts[(j * ClothMesh::numCols + i)].lastPos) / dt;


			if(i < ClothMesh::numCols){
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + i + 1)]);
			}

			if (i > 0) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + i - 1)]);
			}

			if (i < ClothMesh::numCols-1) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + i + 2)]);
			}

			if (i > 1) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + i - 2)]);
			}

			if (j < ClothMesh::numRows) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j+1 * ClothMesh::numCols + i)]);
			}

			if (j > 0) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j - 1 * ClothMesh::numCols + i)]);
			}

			if (j < ClothMesh::numRows-1) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j + 2* ClothMesh::numCols + i)]);
			}

			if (j > 1) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j - 2 * ClothMesh::numCols + i)]);
			}
			
			if (i < ClothMesh::numCols && j < ClothMesh::numRows) { // diagonal derecha abajo
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j + 1 * ClothMesh::numCols + i + 1)]);
			}

			if (i < ClothMesh::numCols && j > 0) { // diagonal derecha arriba
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j - 1 * ClothMesh::numCols + i + 1)]);
			}

			if (i > 0 && j > 0) { // diagonal izquierda arriba
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j - 1 * ClothMesh::numCols + i - 1)]);
			}

			if (i > 0 && j < ClothMesh::numRows) { // diagonal izquierda abajo
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j + 1 * ClothMesh::numCols + i - 1)]);
			}

		}
	}
	cout << parVerts[0].Force.x << endl;
	ClothMesh::updateClothMesh(&currMesh[0].x);


	}

void PhysicsCleanup() {
	//TODO
	delete [] currMesh;
}