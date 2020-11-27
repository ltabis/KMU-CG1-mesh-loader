#include "TestShapeSpawner.hpp"

CG::Test::TestShapeSpawner::TestShapeSpawner()
	: _fov { 45 }
{
	_sloader = std::make_unique<ShaderLoader>();

	_sloader->load("./res/shaders/square.shader");
	_sloader->attach("triangle");
	_sloader->attach("color");
	_sloader->createExecutable();
}

CG::Test::TestShapeSpawner::~TestShapeSpawner()
{
}

void CG::Test::TestShapeSpawner::onStart()
{
}

void CG::Test::TestShapeSpawner::onUpdate(float deltaTime)
{
	_renderer->pollEvents();
}

void CG::Test::TestShapeSpawner::onRender()
{
	ImGui::Begin("Projection transformation");
	// control over the projection matrix.
	if (ImGui::SliderFloat("FOV", &_fov, 45, 120, "%.1f"))
		_renderer->setFov(_fov);
	ImGui::End();

	ImGui::Begin("Spawner controller");

	if (ImGui::Button("Create triangle"))
		_shapes.push_back(std::make_unique<Triangle>(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)));
	if (ImGui::Button("Create plane"))
		_shapes.push_back(std::make_unique<Plane>(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)));
	if (ImGui::Button("Create cube"))
		_shapes.push_back(std::make_unique<Cube>(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)));

	if (ImGui::TreeNode("Shapes")) {
		unsigned int id = 0;
		for (auto shape = _shapes.begin(); shape != _shapes.end(); ++id) {
			if (ImGui::TreeNode(std::to_string(id).c_str())) {

				glm::vec3 translation = glm::vec3(0.f);
				glm::vec3 rotation = glm::vec3(0.f);
				glm::vec3 scale = glm::vec3(1.f);

				if (ImGui::InputFloat3("position", &translation[0], 3))
					(*shape)->transform.setPosition(translation.x, translation.y, translation.z);
				if (ImGui::SliderFloat3("rotation", &rotation[0], 0, 360, "%.1f"))
					(*shape)->transform.setRotation(rotation.x, rotation.y, rotation.z, glm::mat4(1.f));
				if (ImGui::SliderFloat3("scale", &scale[0], 1, 100, "%.1f"))
					(*shape)->transform.setScale(scale.x, scale.y, scale.z);

				if (ImGui::Button("delete"))
					shape = _shapes.erase(shape);
				else
					++shape;

				ImGui::TreePop();
			} else
				++shape;
		}
		ImGui::TreePop();
	}
	ImGui::End();

	for (auto& shape : _shapes) {
		glm::mat4 mvp = _renderer->projectionMatrix() * _renderer->viewMatrix() * shape->transform.model();
		_sloader->setUniform("u_mvp", mvp);
		_renderer->draw(*shape, *_sloader);
	}
}

void CG::Test::TestShapeSpawner::onStop()
{
}

void CG::Test::TestShapeSpawner::onReset()
{
	// reseting the translation uniform.
	_sloader->setUniform("u_mvp", glm::mat4(1.f));
}
