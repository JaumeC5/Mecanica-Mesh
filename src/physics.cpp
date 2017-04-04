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

bool collisionLeft;
bool collisionRight;
bool collisionUp;
bool collisionDown;
bool collisionFront;
bool collisionBack;
bool collision;
bool collisionSphere;

float distanceSprings;
float ellogation;
glm::vec3 *currMesh;
glm::vec3* lastMesh;
glm::vec3 * tempMesh;
glm::vec3 * finalMesh;
glm::vec3 f;
float gravity;

float damp;

int counter = 0;
const float ke = 1.f; //rigidez
const float kd = 1.f; // dumping
bool hasCollision(glm::vec3 Pt, glm::vec3 n, float d, glm::vec3 PtPost, int plane) { // Collision detector
	float getPos;

	getPos = ((glm::dot(n, Pt) + d) * (glm::dot(n, PtPost) + d));

	if (getPos <= 0) {
		cout << plane << ": true xd" << endl;
		return true;
	}
	else { return false; }
}

void secondGradeEquation(float a, float b, float c) {
	float disc, x1, x2, xi, xr;
	disc = pow(b, 2.0) - 4 * a*c;
	if (disc>0.0) {
		x1 = ((-b + sqrt(disc)) / (2.0*a));
		x2 = ((-b - sqrt(disc)) / (2.0*a));
	}
	else {
		if (disc == 0.0) {
			x1 = (-b) / (2.0*a);
		}
		else {
			xr = (-b / (2.0*a));
			xi = (sqrt(-disc) / (2.0*a));
		}
	}
}
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
		if (ImGui::Button("0"))
		{
			gravity = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("-9.81"))
		{
			gravity = -9.81;
		}ImGui::SameLine();
		ImGui::Text("Gravity Acceleration");
		if (ImGui::CollapsingHeader("Spring parameters"))
		{
			ImGui::SliderFloat("Damp Direct link Springs", &damp, 0,5);
			ImGui::SliderFloat("Damp Diagonal link Springs", &damp, 0, 5);
			ImGui::SliderFloat("Damp Second link Springs", &damp, 0, 5);

			ImGui::SliderFloat("Initial Rest distance of the springs", &distanceSprings, 0.3, 0.7);
			ImGui::SliderFloat("Max elogation", &ellogation, 0.5, 1);
			//Constant and damping term of direct-link springs (stretch).
			//Constant and damping term of diagonal - link springs(shear).
			//Constant and damping term of second - link springs(bend).
			//Max % of accepted ellongation of links.
			//Initial Rest distance of the springs between the points of the mesh.		
		}
		if (ImGui::CollapsingHeader("Collisions"))
		{
			ImGui::Checkbox("Use collisions", &collision);
			ImGui::Checkbox("Use Sphere collider", &collisionSphere);
		}
		//0
		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window) {//
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window) {//
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}//a
}

class Particle {
public:
	void regulateDist(Particle p) {
		float d= glm::distance(ActualPos, p.ActualPos);
		if (d < 0.5) {
 		}
	}
	void calculateForce(Particle nextP, float dist);
	glm::vec3 ActualPos;
	glm::vec3 lastPos;
	glm::vec3 vel;
	glm::vec3 Force;
	glm::vec3 dump;
	int mass = 1;
};
void Particle::calculateForce(Particle nextP, float dist) {
	//dump = kd * vel;
	Force += -((ke*(glm::length(ActualPos - nextP.ActualPos)) - dist) + damp*glm::dot(vel - nextP.vel, (ActualPos - nextP.ActualPos) / (glm::length(ActualPos - nextP.ActualPos)))) * (ActualPos - nextP.ActualPos) / (glm::length(ActualPos - nextP.ActualPos));	 	// Provot // -kd*(glm::length(ActualPos - nextP.ActualPos) - dist)* ((ActualPos - nextP.ActualPos) / (glm::length(ActualPos - nextP.ActualPos)));
																																																																	// slides //-((ke*(glm::length(ActualPos - nextP.ActualPos))- dist) + kd*glm::dot(vel - nextP.vel ,(ActualPos - nextP.ActualPos)/(glm::length(ActualPos - nextP.ActualPos)))) * (ActualPos - nextP.ActualPos) / (glm::length(ActualPos - nextP.ActualPos));	 
}
Particle* parVerts;

void sphereCollision(glm::vec3 actPos, glm::vec3 nextPos, float r, glm::vec3 cS) {

	glm::vec3 Q = glm::vec3(0, 0, 0); // punt de contacte amb l'esfera
	float alpha = 0.f;

	Q = actPos - ((nextPos - actPos) * alpha);
}

void PhysicsInit() {
	damp = 1.1f;
	distanceSprings = 0.5f;
	tempMesh = new glm::vec3[14 * 18];
	finalMesh = new glm::vec3[14 * 18];
	currMesh = new glm::vec3[14 * 18];
	lastMesh = new glm::vec3[14 * 18];
	parVerts = new Particle[14 * 18];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			for (int i = 0; i < ClothMesh::numCols; i++) {
				if (i == 0 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -9.5 + i*0.5f);
				}
				else if (i == ClothMesh::numCols - 1 && j == 0) {
					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(1, 1, -9.5 + i*0.5f);
				}
				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + j*0.5f, 9.5, -4.5 + i*0.5f);// distancia horitzontal i vertical entre nodes = 0.5. Distancia doble = 1. Dist�ncia diagonal = sqrt(0.5*0.5 + 0.5*0.5);
				lastMesh[(j * ClothMesh::numCols + i)] = glm::vec3(0, 0, 0);
				tempMesh[(j * ClothMesh::numCols + i)] = glm::vec3(0, 0, 0);
				finalMesh[(j * ClothMesh::numCols + i)] = glm::vec3(0, 0, 0);

				f = glm::vec3(0, -9.81, 0); // for�a externa, sa gravetat

				parVerts[j * ClothMesh::numCols + i].ActualPos = currMesh[(j * ClothMesh::numCols + i)];
				parVerts[j * ClothMesh::numCols + i].lastPos = lastMesh[(j * ClothMesh::numCols + i)];
				parVerts[j * ClothMesh::numCols + i].Force = glm::vec3(0, 0, 0);
				parVerts[j * ClothMesh::numCols + i].vel = glm::vec3(0, 0, 0);

			}
		}
	}
	//ClothMesh::updateClothMesh(currMesh);
}

void calculateForces(int i, int j) { // calculate the forces of every node
	if (i < ClothMesh::numCols - 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i + 1))], distanceSprings); }
	if (i > 0) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i - 1))], distanceSprings);}
	if (i < ClothMesh::numCols - 2) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i + 2))], distanceSprings * 2);}
	if (i > 1) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i - 2))], distanceSprings * 2);}
	if (j < ClothMesh::numRows - 1) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + i)], distanceSprings);}
	if (j > 0) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + i)], distanceSprings);}
	if (j < ClothMesh::numRows - 2) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 2)* ClothMesh::numCols + i)], distanceSprings * 2);}
	if (j > 1) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 2) * ClothMesh::numCols + i)], distanceSprings * 2);}
	if (i < ClothMesh::numCols - 1 && j < ClothMesh::numRows - 1) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + (i + 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2)));}
	if (i < ClothMesh::numCols - 1 && j > 0) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + (i + 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2)));}
	if (i > 0 && j > 0) {parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + (i - 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2)));}
	if (i > 0 && j < ClothMesh::numRows - 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + (i - 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2)));}
}

void PhysicsCleanup() {
	delete[] currMesh;
	delete[] lastMesh;
	delete[] tempMesh;
	delete[] finalMesh;

}
void PhysicsUpdate(float dt) {
	//TODO	
	float sphereX = -4 + rand() % 8;
	float sphereY = 1 + rand() % 9;
	float sphereZ = -4 + rand() % 8;

	if (counter >= 800) { //this is 20 seconds.
		PhysicsCleanup();
		PhysicsInit();
		Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), 1);
		counter = 0;
	}
	if (reset)
	{
		PhysicsCleanup();
		PhysicsInit();
		Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), 1);
		counter = 0;
	}
	
	//position update
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			for (int i = 0; i < ClothMesh::numCols; i++) {
				
				tempMesh[(j * ClothMesh::numCols + i)] = currMesh[(j * ClothMesh::numCols + i)];


				if (i == 0 && j == 0) {

					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + 0.5f, 9.5, -4.5 + 0.3f);


				}
				else if (i == ClothMesh::numCols - 1 && j == 0) {

					currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + 0.5f, 9.5, 1.7f);


				}

				else {


					lastMesh[(j * ClothMesh::numCols + i)] = tempMesh[(j * ClothMesh::numCols + i)];
					//cout << "/////////////" << endl;
					//cout << lastMesh[0].x << endl; 
					//cout << currMesh[0].x << endl;
					finalMesh[(j * ClothMesh::numCols + i)] = currMesh[(j*ClothMesh::numCols + i)] + (currMesh[(j*ClothMesh::numCols + i)] - lastMesh[(j*ClothMesh::numCols + i)]) + ((f + parVerts[(j*ClothMesh::numCols + i)].Force))*(dt*dt);
					//colisons
					//left plane
					collisionLeft = hasCollision(currMesh[(j * ClothMesh::numCols + i)], glm::vec3(1, 0, 0), 5, finalMesh[(j * ClothMesh::numCols + i)], 1);
					//right
					collisionRight =hasCollision(currMesh[(j * ClothMesh::numCols + i)], glm::vec3(-1, 0, 0), 5, finalMesh[(j * ClothMesh::numCols + i)], 2);
					//up
					collisionUp = hasCollision(currMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, -1, 0), 5, finalMesh[(j * ClothMesh::numCols + i)], 3);
					//down
					collisionDown= hasCollision(currMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, 1, 0), 5, finalMesh[(j * ClothMesh::numCols + i)], 4);
					//front
					collisionFront = hasCollision(currMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, 0, -1), 5, finalMesh[(j * ClothMesh::numCols + i)], 5);
					//back
					collisionBack = hasCollision(currMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, 0, 1), 5, finalMesh[(j * ClothMesh::numCols + i)], 6);

					currMesh[(j * ClothMesh::numCols + i)] = finalMesh[(j * ClothMesh::numCols + i)];

					parVerts[(j * ClothMesh::numCols + i)].lastPos = lastMesh[(j * ClothMesh::numCols + i)];
					parVerts[(j * ClothMesh::numCols + i)].ActualPos = currMesh[(j * ClothMesh::numCols + i)];

					parVerts[(j * ClothMesh::numCols + i)].vel = (parVerts[(j * ClothMesh::numCols + i)].ActualPos - parVerts[(j * ClothMesh::numCols + i)].lastPos) / dt;
				}

				calculateForces(i, j);

				if (collisionLeft)
				{

				}
				if (collisionBack)
				{

				}
				if (collisionFront)
				{

				}
				if (collisionUp)
				{

				}
				if (collisionDown)
				{

				}
				if (collisionRight)
				{

				}
			}
		}
	}

	//cout << "/////////////" << endl;
	cout << dt << endl;
	//cout << glm::length(parVerts[1].ActualPos.x - parVerts[2].ActualPos.x) << endl;
	//cout << parVerts[1].Force.x <<" " << parVerts[1].Force.y <<" " << parVerts[1].Force.z<< endl;
	//secondGradeEquation(pow(currMesh[0].x - lastMesh[0].x, 2) + pow(currMesh[0].y - lastMesh[0].y, 2) + pow(currMesh[0].z - lastMesh[0].z, 2),2*(currMesh[0].x - lastMesh[0].x)* )
	ClothMesh::updateClothMesh(&currMesh[0].x);



	//delete [] currMesh;
}


