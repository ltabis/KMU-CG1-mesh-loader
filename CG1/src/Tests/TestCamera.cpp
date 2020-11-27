#include "TestCamera.hpp"

CG::Test::TestCamera::TestCamera()
	: _fov { 45 }
{
	_cube = std::make_unique<Cube>(
		glm::vec3(0.f, 0.f, -3.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	);

	_sloader = std::make_unique<ShaderLoader>();
	_camera = std::make_unique<Camera>(
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f),
		glm::vec3(0.f, 1.f, 0.f),
		1920.f,
		1080.f
	);

	_sloader->load("./res/shaders/square.shader");
	_sloader->attach("triangle");
	_sloader->attach("color");
	_sloader->createExecutable();
}

CG::Test::TestCamera::~TestCamera()
{
}

void CG::Test::TestCamera::onStart()
{
	_sloader->setUniform("u_mvp", glm::mat4(1.f));
}

void CG::Test::TestCamera::onUpdate(float deltaTime)
{
}

void CG::Test::TestCamera::onRender()
{
	bool updated = false;
	
	ImGui::Begin("Camera controls");
	// control over the projection matrix.
	if (ImGui::InputFloat("FOV", &_fov, 1)) {
		_camera->setFieldOfView(_fov);
		CG_CONSOLE_INFO("fov: {}", _fov);
	}
	if (ImGui::InputFloat2("Aspect Ratio", &_aspectRatio[0], 1))
		_renderer->setAspectRatio(_aspectRatio.x, _aspectRatio.y);
	
	ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Transform");

	glm::vec3 translation = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);

	if (ImGui::SliderFloat3("translate", &translation[0], -1.f, 1.f))
		_camera->translate(translation);
	if (ImGui::SliderFloat3("rotate", &rotation[0], -1.f, 1.f))
		_camera->rotate(rotation);

	ImGui::End();

	glm::mat4 mvp = _camera->view() * _cube->transform.model();
	_sloader->setUniform("u_mvp", mvp);
	_renderer->draw(*_cube, *_sloader);
}

void CG::Test::TestCamera::onStop()
{
}

void CG::Test::TestCamera::onReset()
{
	// reseting the translation uniform.
	_sloader->setUniform("u_mvp", glm::translate(glm::mat4(1.f), glm::vec3(0.f)));

	// TODO: reset cam position and rotation.
}
