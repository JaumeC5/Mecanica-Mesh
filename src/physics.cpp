#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <vector>
#include <../include/GLFW/glfw3.h>
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
int Rtime;
float sphereRadius;
float distanceSprings;
float ellogation;


glm::vec3 *currMesh;
glm::vec3* lastMesh;
glm::vec3 * tempMesh;
glm::vec3 * finalMesh;
glm::vec3 f;
glm::vec3 centreSphere;
time_t theTime = time(0);
float gravity;
float sphereX;
float sphereY;
float sphereZ;
float RandomRadiusSphere;
float Ke;
float Kd;
int counter = 0;
const float ke = 900; //rigidez
const float kd = 50;// dumping


bool hasCollision(glm::vec3 Pt, glm::vec3 n, float d, glm::vec3 PtPost, int plane) { // Collision detector
	float getPos;

	getPos = ((glm::dot(n, Pt) + d) * (glm::dot(n, PtPost) + d));
	if (getPos <= 0){
		return true;
	}

	else { return false; }
}

void collidePlane(glm::vec3 n, float d, glm::vec3 &pos, glm::vec3 &prevPos) {

	/*std::cout << "Posicio anterior        " << prevPos.y << endl;
	std::cout << "Posicio actual          " << pos.y << endl;*/
	
	pos = pos - (1 + 0.8f) * (glm::dot(pos, n) + d) * n;
	prevPos = prevPos - (1 + 0.8f) * (glm::dot(prevPos, n) + d) * n;

	/*std::cout << "Posicio anterior rebot  " << tempPrevPos.y << endl;
	std::cout << "Posicio actual rebot	  " << tempPos.y << endl << endl;*/
}


float getTheAlpha(float a, float b, float c) { // test 1

	float res1 = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
	float res2 = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);

	//cout << res1 << "////" << res2 << endl;
	if (res1 <= 1 && res1 >= 0) return res1;
	if (res2 <= 1 && res2 >= 0) return res2;

}  

glm::vec3 getQ(glm::vec3 position, glm::vec3 lastP, glm::vec3 cS, float r) { 

	float a, b, c, alpha = 0;
	glm::vec3 dir = glm::normalize(position - lastP); // vector direcció normalitzat
	a = glm::dot(dir, dir); // esto = 1
	b = 2 * glm::dot(dir, lastP - cS);
	c = glm::dot(lastP - cS, lastP - cS) - r*r;

	float res1 = (-b + sqrt(pow(b, 2) - 4*a*c)) / (2 * a);
	float res2 = (-b - sqrt(pow(b, 2) - 4*a*c)) / (2 * a);

	if (res1 <= 1 && res1 >= 0) alpha = res1;
	if (res2 <= 1 && res2 >= 0) alpha = res2;


	glm::vec3 Q = lastP + (position - lastP)*alpha;
	
	return Q;
	

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

		if (ImGui::CollapsingHeader("Spring parameters"))
		{
			ImGui::SliderFloat("Damp Direct link Springs", &Kd, 47, 53);
			ImGui::SliderFloat("Damp Diagonal link Springs", &Kd, 47, 53);
			ImGui::SliderFloat("Damp Second link Springs", &Kd, 47, 53);

			ImGui::SliderFloat("Constant Direct link Springs", &Ke, 0, 1000);
			ImGui::SliderFloat("Consant Diagonal link Springs", &Ke, 0, 1000);
			ImGui::SliderFloat("Constant Second link Springs", &Ke, 0, 1000);

			ImGui::SliderFloat("Initial Rest distance of the springs", &distanceSprings, 0.3, 0.7);
			ImGui::SliderFloat("Max elogation", &ellogation, -0.5f, 0.5f);
		}
		if (ImGui::CollapsingHeader("Collisions"))
		{
			ImGui::Checkbox("Use collisions", &collision);
			ImGui::Checkbox("Use Sphere collider", &collisionSphere);
		}
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
	void regulateDist(Particle p);
	void calculateForce(Particle nextP, float dist);
	glm::vec3 ActualPos;
	glm::vec3 lastPos;
	glm::vec3 vel;
	glm::vec3 Force;
	glm::vec3 Q;

	bool collided = true;
	bool calculated = false;
	float alpha;
	int mass = 1;
};
void Particle::calculateForce(Particle nextP, float dist) {

	float d = glm::length(ActualPos - nextP.ActualPos);//Force += -((ke*(glm::length(ActualPos - nextP.ActualPos)) - dist) + kd*(glm::dot(vel - nextP.vel, (ActualPos - nextP.ActualPos))) / (glm::length(ActualPos - nextP.ActualPos))) * (ActualPos - nextP.ActualPos) / (glm::normalize(ActualPos - nextP.ActualPos));	 	// Provot // -kd*(glm::length(ActualPos - nextP.ActualPos) - dist)* ((ActualPos - nextP.ActualPos) / (glm::length(ActualPos - nextP.ActualPos)));
	glm::vec3 speed = vel - nextP.vel;
	glm::vec3 normalized = glm::normalize(ActualPos - nextP.ActualPos);
	float subResult = Ke*(d - dist) + glm::dot(Kd*speed, normalized);
	glm::vec3 res = -subResult * normalized;
	Force += res;
	// slides //-((ke*(glm::length(ActualPos - nextP.ActualPos))- dist) + kd*glm::dot(vel - nextP.vel ,(ActualPos - nextP.ActualPos)/(glm::length(ActualPos - nextP.ActualPos)))) * (ActualPos - nextP.ActualPos) / (glm::length(ActualPos - nextP.ActualPos));	 
}

void Particle::regulateDist(Particle p) {
	float d = glm::distance(ActualPos, p.ActualPos);
	float dMax = d + ellogation;
	float difference = d - dMax;

}
Particle* parVerts;

void sphereCollision(glm::vec3 Q, glm::vec3 &actPos,glm::vec3 &lastPos,  glm::vec3 cS) {

	glm::vec3 n = glm::normalize(Q - cS);
	//cout << n.x << " " << n.y << " " <<  n.z << " " << endl;

	float d = -glm::dot(n, Q);

	if (glm::length(actPos - centreSphere) <= RandomRadiusSphere) { 

		//cout << "life is good" << endl;
		
		// execute bounce
		collidePlane(n, d, actPos, lastPos);

	}


}

void PhysicsCleanup() {
	delete[] currMesh;
	delete[] lastMesh;
	delete[] tempMesh;
	delete[] finalMesh;

}

void PhysicsInit() {
	Kd = 50;
	Ke = 900;
	ellogation = 0;
	reset = false;
	Rtime = 0;
	distanceSprings = 0.5f;
	tempMesh = new glm::vec3[14 * 18];
	finalMesh = new glm::vec3[14 * 18];
	currMesh = new glm::vec3[14 * 18];
	lastMesh = new glm::vec3[14 * 18];
	parVerts = new Particle[14 * 18];

	for (int j = 0; j < ClothMesh::numRows; j++) {
		for (int i = 0; i < ClothMesh::numCols; i++) {
			if (i == 0 && j == 0) {
				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5, 9.5, -4.5);
			}
			else if (i == ClothMesh::numCols - 1 && j == 0) {
				currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5, 9.5, -4.5 + 1.7f);
			}
			currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5 + j*0.5f, 9.5, -4.5 + i*0.5f);// distancia horitzontal i vertical entre nodes = 0.5. Distancia doble = 1. Distància diagonal = sqrt(0.5*0.5 + 0.5*0.5);
			lastMesh[(j * ClothMesh::numCols + i)] = currMesh[(j * ClothMesh::numCols + i)];
			tempMesh[(j * ClothMesh::numCols + i)] = currMesh[(j * ClothMesh::numCols + i)];
			finalMesh[(j * ClothMesh::numCols + i)] = currMesh[(j * ClothMesh::numCols + i)];

			f = glm::vec3(0, -9.81, 0); // força externa, sa gravetat

			parVerts[j * ClothMesh::numCols + i].ActualPos = currMesh[(j * ClothMesh::numCols + i)];
			parVerts[j * ClothMesh::numCols + i].lastPos = lastMesh[(j * ClothMesh::numCols + i)];
			parVerts[j * ClothMesh::numCols + i].Force = glm::vec3(0, 0, 0);
			parVerts[j * ClothMesh::numCols + i].vel = glm::vec3(0, 0, 0);
			parVerts[j * ClothMesh::numCols + i].alpha = 0;


		}
	}
	cout << "Posició del node: " << parVerts[0].ActualPos.x << " " << parVerts[0].ActualPos.y << " " << parVerts[0].ActualPos.z << " " << endl;

	sphereX = -4 + rand() % 8;
	sphereY = 0 + rand() % 9;
	sphereZ = -4 + rand() % 8;
	centreSphere = glm::vec3(sphereX, sphereY, sphereZ);
	RandomRadiusSphere = 0.5f + rand() % 3;
	cout << "Centre: " << centreSphere.x << " " << centreSphere.y << " " << centreSphere.z << " " << endl << endl;
	cout << "Radius: " << RandomRadiusSphere << endl;


}

void calculateForces(int i, int j) { // calculate the forces of every node
									 //

	parVerts[(j * ClothMesh::numCols + i)].Force = f;

	if (i < ClothMesh::numCols - 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i + 1))], distanceSprings); }

	if (i > 0) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i - 1))], distanceSprings); }

	if (i < ClothMesh::numCols - 2) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i + 2))], distanceSprings * 2); }

	if (i > 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[(j * ClothMesh::numCols + (i - 2))], distanceSprings * 2); }

	if (j < ClothMesh::numRows - 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + i)], distanceSprings); }

	if (j > 0) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + i)], distanceSprings); }

	if (j < ClothMesh::numRows - 2) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 2)* ClothMesh::numCols + i)], distanceSprings * 2); }

	if (j > 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 2) * ClothMesh::numCols + i)], distanceSprings * 2); }

	if (i < ClothMesh::numCols - 1 && j < ClothMesh::numRows - 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + (i + 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2))); }

	if (i < ClothMesh::numCols - 1 && j > 0) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + (i + 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2))); }

	if (i > 0 && j > 0) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j - 1) * ClothMesh::numCols + (i - 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2))); }

	if (i > 0 && j < ClothMesh::numRows - 1) { parVerts[(j * ClothMesh::numCols + i)].calculateForce(parVerts[((j + 1) * ClothMesh::numCols + (i - 1))], sqrt(pow(distanceSprings, 2) + pow(distanceSprings, 2))); }

	parVerts[(j * ClothMesh::numCols + i)].Force += f;

}
void resetAll()
{
	theTime++;
	if (theTime > 242)
	{
		PhysicsCleanup();
		PhysicsInit();

		Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), RandomRadiusSphere);
		theTime = 0;
	}
	if (reset == true)
	{
		PhysicsCleanup();
		PhysicsInit();
		Sphere::updateSphere(glm::vec3(sphereX, sphereY, sphereZ), RandomRadiusSphere);
		theTime = 0;
	}
	centreSphere = glm::vec3(sphereX, sphereY, sphereZ);
}
void verletSprings(int i, int j, float dt)
{

	if (i == 0 && j == 0) {

		currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5, 9.5, -4.5);
	}
	else if (i == ClothMesh::numCols - 1 && j == 0) {

		currMesh[(j * ClothMesh::numCols + i)] = glm::vec3(-4.5, 9.5, 1.7f);
	}

	else{

		if (j == 17 && i == 13) {
			//cout << "pre: " << lastMesh[(j * ClothMesh::numCols + i)].y << " // " << currMesh[(j * ClothMesh::numCols + i)].y << " ----- " << " i: " << i << "  j: " << j << endl;
		}

		tempMesh[(j * ClothMesh::numCols + i)] = currMesh[(j*ClothMesh::numCols + i)]; // temporal. Emmagametza sa anterior currMesh
		//cout << currMesh[(j*ClothMesh::numCols + i)].y - lastMesh[(j*ClothMesh::numCols + i)].y << endl; // dona 0, perque currMesh - currMesh

		finalMesh[(j * ClothMesh::numCols + i)] = currMesh[(j*ClothMesh::numCols + i)] + (currMesh[(j*ClothMesh::numCols + i)] - lastMesh[(j*ClothMesh::numCols + i)]) + (parVerts[(j*ClothMesh::numCols + i)].Force)*(dt*dt);
		currMesh[(j * ClothMesh::numCols + i)] = finalMesh[(j * ClothMesh::numCols + i)]; // posició actual actualitzada
		lastMesh[(j * ClothMesh::numCols + i)] = tempMesh[(j * ClothMesh::numCols + i)]; // posició anterior, que ara conté currMesh


		//left plane
		collisionLeft = hasCollision(lastMesh[(j * ClothMesh::numCols + i)], glm::vec3(1, 0, 0), 5, currMesh[(j * ClothMesh::numCols + i)], 1);
		//right
		collisionRight = hasCollision(lastMesh[(j * ClothMesh::numCols + i)], glm::vec3(-1, 0, 0), 5, currMesh[(j * ClothMesh::numCols + i)], 2);
		//up
		collisionUp = hasCollision(lastMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, -1, 0), 10, currMesh[(j * ClothMesh::numCols + i)], 3);
		//down
		collisionDown = hasCollision(lastMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, 1, 0), 0, currMesh[(j * ClothMesh::numCols + i)], 4);
		//front
		collisionFront = hasCollision(lastMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, 0, -1), 5, currMesh[(j * ClothMesh::numCols + i)], 5);
		//back
		collisionBack = hasCollision(lastMesh[(j * ClothMesh::numCols + i)], glm::vec3(0, 0, 1), 5, currMesh[(j * ClothMesh::numCols + i)], 6);
		if (j == 17 && i == 13) {
			//cout << "act: " << lastMesh[(j * ClothMesh::numCols + i)].y << " // " << currMesh[(j * ClothMesh::numCols + i)].y << " ----- " << " i: " << i << "  j: " << j << endl;
		}
		
		if (collisionLeft) {
			collidePlane(glm::vec3(1, 0, 0), 5, currMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)]);
		}
		if (collisionRight) {
			collidePlane(glm::vec3(-1, 1, 0), 5, currMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)]);
		}
		if (collisionUp) {
			collidePlane(glm::vec3(0, -1, 0), 10, currMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)]);
		}
		if (collisionDown) {
			collidePlane(glm::vec3(0, 1, 0), 0, currMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)]);
		}
		if (collisionFront) {
			collidePlane(glm::vec3(0, 0, -1), 5, currMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)]);
		}
		if (collisionBack) {
			collidePlane(glm::vec3(0, 0, 1), 5, currMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)]);
		}

		parVerts[(j * ClothMesh::numCols + i)].Q = getQ(lastMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)], centreSphere, RandomRadiusSphere);
		sphereCollision(parVerts[(j * ClothMesh::numCols + i)].Q, currMesh[(j * ClothMesh::numCols + i)], lastMesh[(j * ClothMesh::numCols + i)], centreSphere);
	
		

		parVerts[(j * ClothMesh::numCols + i)].lastPos = lastMesh[(j * ClothMesh::numCols + i)];
		parVerts[(j * ClothMesh::numCols + i)].ActualPos = currMesh[(j * ClothMesh::numCols + i)];
		parVerts[(j * ClothMesh::numCols + i)].vel = (parVerts[(j * ClothMesh::numCols + i)].ActualPos - parVerts[(j * ClothMesh::numCols + i)].lastPos) / dt;

		

		/*parVerts[(j * ClothMesh::numCols + i)].alpha = getTheAlpha(parVerts[(j * ClothMesh::numCols + i)].ActualPos.x - parVerts[(j * ClothMesh::numCols + i)].lastPos.x + parVerts[(j * ClothMesh::numCols + i)].ActualPos.y - parVerts[(j * ClothMesh::numCols + i)].lastPos.y + parVerts[(j * ClothMesh::numCols + i)].ActualPos.z - parVerts[(j * ClothMesh::numCols + i)].lastPos.z,
		2 * (parVerts[(j * ClothMesh::numCols + i)].ActualPos.x - parVerts[(j * ClothMesh::numCols + i)].lastPos.x)*parVerts[(j * ClothMesh::numCols + i)].lastPos.x + 2 * (parVerts[(j * ClothMesh::numCols + i)].ActualPos.y - parVerts[(j * ClothMesh::numCols + i)].lastPos.y)*parVerts[(j * ClothMesh::numCols + i)].lastPos.y + 2 * (parVerts[(j * ClothMesh::numCols + i)].ActualPos.z - parVerts[(j * ClothMesh::numCols + i)].lastPos.z)*parVerts[(j * ClothMesh::numCols + i)].lastPos.z - 2 * (parVerts[(j * ClothMesh::numCols + i)].ActualPos.x - parVerts[(j * ClothMesh::numCols + i)].lastPos.x)*sphereX - 2 * (parVerts[(j * ClothMesh::numCols + i)].ActualPos.y - parVerts[(j * ClothMesh::numCols + i)].lastPos.y)*sphereY - 2 * (parVerts[(j * ClothMesh::numCols + i)].ActualPos.z - parVerts[(j * ClothMesh::numCols + i)].lastPos.z)*sphereZ,
		pow(parVerts[(j * ClothMesh::numCols + i)].lastPos.x, 2) + pow(parVerts[(j * ClothMesh::numCols + i)].lastPos.y, 2) + pow(parVerts[(j * ClothMesh::numCols + i)].lastPos.z, 2) - 2 * parVerts[(j * ClothMesh::numCols + i)].lastPos.x * sphereX - 2 * parVerts[(j * ClothMesh::numCols + i)].lastPos.y * sphereY - 2 * parVerts[(j * ClothMesh::numCols + i)].lastPos.z * sphereZ + pow(sphereX, 2) + pow(sphereY, 2) + pow(sphereZ, 2) - pow(RandomRadiusSphere, 2));
		//parVerts[(j * ClothMesh::numCols + i)].regulateDist(parVerts[(j + 1 * ClothMesh::numCols + i)]);*/
	}


}

void PhysicsUpdate(float dt) {
	//
	dt /= 10;
	sphereCollision(parVerts[0].Q, lastMesh[0], lastMesh[0], centreSphere);

	for (int x = 0; x < 10; x++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			for (int i = 0; i < ClothMesh::numCols; i++) {
				verletSprings(i, j, dt);
				calculateForces(i, j);
				Rtime = glfwGetTime();
			}
		}
	}

	resetAll();

	ClothMesh::updateClothMesh(&currMesh[0].x);
}// jaumecrack