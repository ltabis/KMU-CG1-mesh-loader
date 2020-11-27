#include "TestBackgroundColorChange.hpp"

CG::Test::TestBackgroundColorChange::TestBackgroundColorChange()
	: ATest  {}
	, _color { 0 }
{}

void CG::Test::TestBackgroundColorChange::onStart()
{
}

CG::Test::TestBackgroundColorChange::~TestBackgroundColorChange()
{
}

void CG::Test::TestBackgroundColorChange::onUpdate(float deltaTime)
{
	(void)deltaTime;
	_renderer->pollEvents();
}

void CG::Test::TestBackgroundColorChange::onRender()
{
	ImGui::Begin("Change background color");
	ImGui::ColorEdit4("background color", _color);
	ImGui::End();

	_renderer->clearColor(
		_color[0],
		_color[1],
		_color[2],
		_color[3]
	);
}

void CG::Test::TestBackgroundColorChange::onStop()
{
	_renderer->clearColor(.0f, .0f, .0f, .0f);
}

void CG::Test::TestBackgroundColorChange::onReset()
{
	std::memset(_color, 0, sizeof(_color));
}
