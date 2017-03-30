#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <time.h>
#include <ctime>
#include <vector>
using namespace std;
bool show_test_window = false;
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
const float ke = 1.5f; //rigidez
const float kd = 1.5f;
class Particle {
public:
	void calculateForce(Particle nextP, float dist);
	glm::vec3 ActualPos;
	glm::vec3 lastPos;
	glm::vec3 vel;
	glm::vec3 OriginalPos;
	glm::vec3 Force = f;
	int mass = 1;
};
void Particle::calculateForce(Particle nextP, float dist) {
		
	 // se iguala a sa força externa. Motius obvis
	Force += -(ke * (glm::length(ActualPos - nextP.ActualPos)));//-(ke * (glm::length(ActualPos - nextP.ActualPos) - dist) +glm::dot(kd*(vel - nextP.vel), (ActualPos / nextP.ActualPos) / glm::length(ActualPos - nextP.ActualPos))) *(ActualPos / nextP.ActualPos) / glm::length(ActualPos - nextP.ActualPos);
		
}
	Particle* parVerts;
void PhysicsInit() {
	
	tempMesh = new glm::vec3[14 * 18];
	finalMesh = new glm::vec3[14 * 18];
	currMesh = new glm::vec3[14 * 18];
	lastMesh = new glm::vec3[14 * 18];

	parVerts = new Particle[14 * 18];
		for (int i = 0; i < ClothMesh::numCols; i++) {
			for (int j = 0; j < ClothMesh::numRows; j++) {
				if (i == 0 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -9.5 + i*0.5f);
				}
				else if (i == ClothMesh::numCols-1 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -9.5 + i*0.5f);
				}


				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + j*0.5f, 9.5, -4.5 + i*0.5f); // distancia horitzontal i vertical entre nodes = 0.5. Distancia doble = 1. Distància diagonal = sqrt(0.5*0.5 + 0.5*0.5);
				f = glm::vec3(0, -9.81, 0); // força externa, sa gravetat
				parVerts[j * ClothMesh::numCols + i].OriginalPos = currMesh[(j * ClothMesh::numCols + i)];
				parVerts[j * ClothMesh::numCols + i].lastPos = lastMesh[(j * ClothMesh::numCols + i)];
				parVerts[j * ClothMesh::numCols + i].Force = glm::vec3(0, 0, 0);
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
			
			if(i < ClothMesh::numCols-1){ // fuerza a la derecha
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i + 1))], 0.5f);
			}
			
			if (i > 0) { // fuerza a la izquierda
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i - 1))], 0.5f);
			}

			if (i < ClothMesh::numCols-2) { // fuerza a las dos de la derecha
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i + 2))], 1);
			}

			if (i > 1) {// fuerza a las dos de la izquierda
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i - 2))], 1);
			}

			if (j < ClothMesh::numRows-1) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + i)], 0.5);
			}

			if (j > 0) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + i)], 0.5);
			}
			
			
			if (j < ClothMesh::numRows-2) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 2)* ClothMesh::numCols + i)], 1);
			}
			
			if (j > 1) {
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 2) * ClothMesh::numCols + i)], 1);
			}
			
			if (i < ClothMesh::numCols-1 && j < ClothMesh::numRows-1) { // diagonal derecha abajo
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + (i + 1))], 0.707f);
			}

			if (i < ClothMesh::numCols-1 && j > 0) { // diagonal derecha arriba
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + (i + 1))], 0.707f);
			}

			if (i > 0 && j > 0) { // diagonal izquierda arriba
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + (i - 1))], 0.707f);
			}

			if (i > 0 && j < ClothMesh::numRows-1) { // diagonal izquierda abajo
				parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + (i - 1))], 0.707f);
			}

			tempMesh[(j * ClothMesh::numCols + i)] = currMesh[(j * ClothMesh::numCols + i)];


			if (i == 0 && j == 0) {

				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + 0.5f, 9.5, -4.5 + 0.5f);
				

			}
			else if (i == ClothMesh::numCols - 1 && j == 0) {

				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + 0.5f, 9.5, 1.f);
				

			}

			else {


				lastMesh[(j * ClothMesh::numCols + i)] = tempMesh[(j * ClothMesh::numCols + i)];
				//cout << "/////////////" << endl;
				//cout << lastMesh[0].x << endl; //aixo no toca donar lo mateix que currMesh, ja que lastMesh se suposa que emmagametza sa posicio anterior
				//cout << currMesh[0].x << endl;


				finalMesh[(j * ClothMesh::numCols + i)] = currMesh[(j*ClothMesh::numCols + i)] + (currMesh[(j*ClothMesh::numCols + i)] - lastMesh[(j*ClothMesh::numCols + i)]) + ((parVerts[(j*ClothMesh::numCols + i)].Force))*(dt*dt);
				currMesh[(j * ClothMesh::numCols + i)] = finalMesh[(j * ClothMesh::numCols + i)];

				parVerts[(j * ClothMesh::numCols + i)].lastPos = lastMesh[(j * ClothMesh::numCols + i)];
				parVerts[(j * ClothMesh::numCols + i)].ActualPos = currMesh[(j * ClothMesh::numCols + i)];

				parVerts[(j * ClothMesh::numCols + i)].vel = (parVerts[(j * ClothMesh::numCols + i)].ActualPos - parVerts[(j * ClothMesh::numCols + i)].lastPos) / dt;
			}
		}
	}
	cout << "/////////////" << endl;
	cout << currMesh[1].x << " " <<lastMesh[1].x << endl;
	cout << parVerts[1].ActualPos.x <<" "  << parVerts[1].lastPos.x<< endl;
	//cout << "/////////////" << endl;
	ClothMesh::updateClothMesh(&currMesh[0].x);



	//delete [] currMesh;
}

void PhysicsCleanup() {
	delete[] currMesh;
}