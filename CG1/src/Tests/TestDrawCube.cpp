#include "TestDrawCube.hpp"

CG::Test::TestDrawCube::TestDrawCube()
	: _rotation			 { glm::vec3(0.f) }
	, _translation		 { glm::vec3(0.f) }
	, _scale			 { glm::vec3(1.f) }
	, _fov				 { 45             }
{
	_vbo = std::make_unique<VertexBuffer>(_cubeVertices, sizeof(_cubeVertices));
	_dataLayout = std::make_unique<VertexArrayLayout>();

	_dataLayout->push<float>(4);
	_dataLayout->push<float>(3);

	_vao = std::make_unique<VertexArray>();
	_vao->addBuffer(*_vbo, *_dataLayout);

	_ibo = std::make_unique<IndexBuffer>(_cubeIndices, sizeof(_cubeIndices));
	_sloader = std::make_unique<ShaderLoader>();

	_sloader->load("./res/shaders/square.shader");
	_sloader->attach("triangle");
	_sloader->attach("color");
	_sloader->createExecutable();
}

CG::Test::TestDrawCube::~TestDrawCube()
{
}

void CG::Test::TestDrawCube::onStart()
{
	_sloader->setUniform("u_mvp", glm::mat4(1.f));
}

void CG::Test::TestDrawCube::onUpdate(float deltaTime)
{
	_renderer->pollEvents();
}

void CG::Test::TestDrawCube::onRender()
{
	bool updated = false;
	
	ImGui::Begin("Projection transformation");
	// control over the projection matrix.
	if (ImGui::SliderFloat("FOV", &_fov, 45, 120, "%.1f"))
		_renderer->setFov(_fov);
	ImGui::End();

	ImGui::Begin("Model transformation");
	
	// control over the model matrix.
	if (ImGui::SliderFloat3("translation", &_translation[0], -10, 10, "%.1f"))
		updated = true;
	if (ImGui::SliderFloat3("rotation", &_rotation[0], 0, 360, "%.1f"))
		updated = true;
	if (ImGui::SliderFloat3("scale", &_scale[0], 1, 100, "%.1f"))
		updated = true;
	if (ImGui::Button("Undo transformation"))
		_matrixStack.pop();

	ImGui::End();

	// clamping scale value (to always be initialised to 1 at the start).
	_scale = glm::clamp(_scale, glm::vec3(1, 1, 1), glm::vec3(100, 100, 100));

	if (updated) {
		CG_CONSOLE_INFO("Pushing to stack.");
		_matrixStack.setPosition(_translation[0], _translation[1], _translation[2]);
		_matrixStack.setScale(_scale[0], _scale[1], _scale[2]);
		_matrixStack.setRotation(_rotation[0], _rotation[1], _rotation[2], glm::mat4(1.f));
		_matrixStack.push();
	}

	glm::mat4 mvp = _renderer->projectionMatrix() * _renderer->viewMatrix() * _matrixStack.get();
	_sloader->setUniform("u_mvp", mvp);
	_renderer->draw(*_vao, *_ibo, *_sloader);
}

void CG::Test::TestDrawCube::onStop()
{
}

void CG::Test::TestDrawCube::onReset()
{
	// reseting the translation uniform.
	_sloader->setUniform("u_mvp", glm::translate(glm::mat4(1.f), glm::vec3(0.f)));
	
	// reseting transform.
	_matrixStack.reset();
	_translation = glm::vec3(0.f);
	_rotation = glm::vec3(0.f);
	_scale = glm::vec3(1.f);

}
