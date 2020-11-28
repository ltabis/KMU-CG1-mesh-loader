// main.cpp
// Application entry point.

#include "Profiling/Breakpoint.hpp"
#include "Callbacks.hpp"
#include "EditorView.hpp"

//#include <assimp/Importer.hpp>      // C++ importer interface
//#include <assimp/scene.h>           // Output data structure
//#include <assimp/postprocess.h>     // Post processing flags

//int main(void)
//{
//    CG::Logger::Init();
//
//    // Create an instance of the Importer class
//    Assimp::Importer importer;
//    // And have it read the given file with some example postprocessing
//    // Usually - if speed is not the most important aspect for you - you'll 
//    // propably to request more postprocessing than we do in this example.
//    const aiScene* scene = importer.ReadFile("../meshes/bunny.obj",
//        aiProcess_CalcTangentSpace |
//        aiProcess_Triangulate |
//        aiProcess_JoinIdenticalVertices |
//        aiProcess_SortByPType);
//
//    // If the import failed, report it
//    if (!scene)
//    {
//        CG_LOG_ERROR("Couldn't load the scene");
//        return false;
//    }
//    return true;
//}

int main(void)
{
	CG::Logger::Init();

	// Creating the renderer and GUI elements.
	CG::Renderer renderer("Checker board", 1920, 1080);
	CG::GUI gui(renderer.window(), CG::GUI::Style::DARK);

	// Creating the checker board with its shaders.
	CG::EditorView editor(50, 16);
	CG::ShaderLoader blueCheckerShader;
	CG::ShaderLoader lightBlueCheckerShader;
	CG::ShaderLoader colorShader;

	// Creating the camera controller.
	CG::NoClipCameraController controller(
		renderer.window(),
		glm::vec3(-20.f, 20.f, 20.f),
		glm::vec3(0.f)
	);
	controller.speed = 0.1f;

	// laoding shaders.
	blueCheckerShader.load("./res/shaders/checker.shader");
	blueCheckerShader.attach("triangle");
	blueCheckerShader.attach("color_blue");
	blueCheckerShader.createExecutable();

	lightBlueCheckerShader.load("./res/shaders/checker.shader");
	lightBlueCheckerShader.attach("triangle");
	lightBlueCheckerShader.attach("color_light_blue");
	lightBlueCheckerShader.createExecutable();

	colorShader.load("./res/shaders/color.shader");
	colorShader.attach("triangle");
	colorShader.attach("color");
	colorShader.createExecutable();

	// assigning callbacks for the renderer.
	renderer.registerKeyBindingCallback(GLFW_KEY_ESCAPE, escape_callback);
	
	// useful variables for frame independent code and fps mode & help for the controller.
	float deltaTime = 0.f;
	float lastFrame = 0.f;
	bool  fpsMode = false;
	bool  helpOpened = true;

	while (!renderer.windowShouldClose()) {

		float currentFrame = (float)glfwGetTime();

		// computing deltatime;
		deltaTime = currentFrame - deltaTime;
		lastFrame = currentFrame;

		controller.update(deltaTime);

		// cleaning the screen and starting new frame for imgui.
		renderer.pollEvents();
		gui.newFrame();
		renderer.clear();

		// camera controller imgui settings.
		ImGui::Begin("Camera Controller");
		// control over the projection matrix.
		if (ImGui::InputFloat("camera speed", &controller.speed, 1))
			CG_CONSOLE_INFO("Controller speed set to {}", controller.speed);
		if (ImGui::InputFloat("camera sensitivity speed", &controller.sensitivity, 1))
			CG_CONSOLE_INFO("Controller sensitivity set to {}", controller.speed);
		if (ImGui::Button("Help"))
			helpOpened = !helpOpened;

		if (helpOpened) {
			ImGui::BeginChild("Tips");
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "How to use the controller.");
			ImGui::Text("Use the mouse to look around.");
			ImGui::Text("Use 'W' 'A' 'S' 'D' to move the camera.");
			ImGui::Text("Use 'Space' and 'Left Shift' to move the camera up or down.");
			ImGui::Text("Press mouse right click to enter fps mode and reclick to get out");
			ImGui::EndChild();
		}
		ImGui::End();


		// updating the fps mode if mouse clicked.
		if (glfwGetMouseButton(renderer.window(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			fpsMode = !fpsMode;
			if (fpsMode)
				glfwSetInputMode(renderer.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(renderer.window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		const auto& board = editor.board();
		const auto& axes = editor.axes();

		for (auto& axis : axes) {
			colorShader.setUniform("u_mvp", controller.view() * axis->transform.model());
			renderer.drawLine(*axis, colorShader);
		}

		// Rendering the floor.
		for (int x = 0, it = 0; x < editor.nsquare(); ++x, it += editor.nsquare()) {
			for (int z = 0, i = x; z < editor.nsquare(); ++z, ++i) {

				int idx = it + z;

				// rendering one of two plane with a different color.
				if (i % 2 == 1) {
					blueCheckerShader.setUniform("u_mvp", controller.view() * board[idx]->transform.model());
					renderer.draw(*(board[idx]), blueCheckerShader);
				} else {
					lightBlueCheckerShader.setUniform("u_mvp", controller.view() * board[idx]->transform.model());
					renderer.draw(*(board[idx]), lightBlueCheckerShader);
				}
			}
		}

		// draw all elements from the gui.
		gui.drawDebugUI();
		gui.renderGUI();
		renderer.swapBuffers();
	}

	return 0;
}