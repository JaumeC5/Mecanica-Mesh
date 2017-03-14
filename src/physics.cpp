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
	if(show_test_window) {//
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}
float arr[14][18];
void PhysicsInit() {
	/*int counter = 0;
	for (int i = 0; i < ClothMesh::numCols; i++){
		for (int j = 0; j < ClothMesh::numRows; j++) {
			arr[i][j] = 1;
		}
		
		//TODO
	}

	

	for (int i = 0; i < ClothMesh::numCols; i++) {
		for (int j = 0; j < ClothMesh::numRows; j++) {
			std::cout << arr[i][j];
		}
		std::cout << std::endl;

		//TODO
	}*/

}
void PhysicsUpdate(float dt) {
	//TODO	}

	ClothMesh::updateClothMesh(*arr);
}
void PhysicsCleanup() {
	//TODO
}