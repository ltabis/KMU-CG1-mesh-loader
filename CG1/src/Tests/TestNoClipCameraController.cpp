#include "TestNoClipCameraController.hpp"

CG::Test::TestNoClipCameraController::TestNoClipCameraController()
	: _fov		  { 90	  }
	, _helpOpened { false }
	, _fpsMode    { false }
{
	_cube = std::make_unique<Cube>(
		glm::vec3(0.f, 0.f, -3.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	);

	_sloader = std::make_unique<ShaderLoader>();

	_sloader->load("./res/shaders/square.shader");
	_sloader->attach("triangle");
	_sloader->attach("color");
	_sloader->createExecutable();
}

CG::Test::TestNoClipCameraController::~TestNoClipCameraController()
{
}

void CG::Test::TestNoClipCameraController::onStart()
{
	if (!_controller.get())
		_controller = std::make_unique<NoClipCameraController>(
			_renderer->window(),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f)
		);

	_sloader->setUniform("u_mvp", glm::mat4(1.f));
}

void CG::Test::TestNoClipCameraController::onUpdate(float deltaTime)
{
	_controller->update(deltaTime);
}

void CG::Test::TestNoClipCameraController::onRender()
{
	ImGui::Begin("Camera Controller");
	// control over the projection matrix.
	if (ImGui::InputFloat("FOV", &_fov, 1)) {
		_controller->setFieldOfView(_fov);
		CG_CONSOLE_INFO("fov set to {}", _fov);
	}
	if (ImGui::InputFloat2("Aspect Ratio", &_aspectRatio[0], 1))
		_controller->setAspectRatio(_aspectRatio.x, _aspectRatio.y);
	if (ImGui::Button("Help"))
		_helpOpened = !_helpOpened;

	if (_helpOpened) {
		ImGui::BeginChild("Tips");
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "How to use the controller.");
		ImGui::Text("Use the mouse to look around.");
		ImGui::Text("Use 'W' 'A' 'S' 'D' to move the camera.");
		ImGui::Text("Use 'Space' and 'Left Shift' to move the camera up or down.");
		ImGui::Text("Press mouse right click to enter fps mode and reclick to get out");
		ImGui::EndChild();
	}
	ImGui::End();

	if (glfwGetMouseButton(_renderer->window(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		_fpsMode = !_fpsMode;
		if (_fpsMode)
			glfwSetInputMode(_renderer->window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(_renderer->window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}


	glm::mat4 mvp = _controller->view() * _cube->transform.model();
	_sloader->setUniform("u_mvp", mvp);
	_renderer->drawWireFrame(*_cube, *_sloader);
}

void CG::Test::TestNoClipCameraController::onStop()
{
}

void CG::Test::TestNoClipCameraController::onReset()
{
	// reseting the translation uniform.
	_sloader->setUniform("u_mvp", glm::translate(glm::mat4(1.f), glm::vec3(0.f)));

	// TODO: reset cam position and rotation.
}
