#include "TestMenu.hpp"

CG::Test::TestMenu::TestMenu(std::shared_ptr<Renderer>& renderer, std::shared_ptr <GUI> gui)
	: _deltaTime { 0 }
{
	if (!renderer)
		return;

	_renderer = std::shared_ptr<Renderer>(renderer);
	_gui      = std::shared_ptr<GUI>(gui);
}

CG::Test::TestMenu::~TestMenu()
{
}

void CG::Test::TestMenu::onRender()
{
	float currentFrame = (float)glfwGetTime();

	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;

	_renderer->pollEvents();
	_gui->newFrame();
	_renderer->clear();

	if (_currentTest != "") {
		_tests[_currentTest]->onUpdate(_deltaTime);
		_tests[_currentTest]->onRender();
		ImGui::Begin("Go back to menu");
		if (ImGui::Button("<-")) {
			_tests[_currentTest]->onStop();
			_currentTest = "";
		}
		ImGui::End();
	} else {
		ImGui::Begin("Test selection menu");
		for (auto& it : _tests)
			if (ImGui::Button(it.first.c_str())) {
				_currentTest = it.first;
				_tests[_currentTest]->onReset();
				break;
			}
		ImGui::End();
	}

	_gui->drawDebugUI();
	_gui->renderGUI();
	_renderer->swapBuffers();
}