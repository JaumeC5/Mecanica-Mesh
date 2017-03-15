#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <iostream>

bool show_test_window = false;

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


}
void PhysicsCleanup() {
	//TODO
}