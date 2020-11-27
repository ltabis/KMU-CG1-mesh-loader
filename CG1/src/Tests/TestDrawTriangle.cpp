#include "TestDrawTriangle.hpp"

CG::Test::TestDrawTriangle::TestDrawTriangle()
	: ATest  {}
{}

void CG::Test::TestDrawTriangle::onStart()
{
	_vbo = std::make_unique<VertexBuffer>(_data, sizeof(_data));

	// create the layout of the data.
	_layout = std::make_unique<VertexArrayLayout>();
	_layout->push<float>(4);
	_layout->push<float>(3);

	// initialise the vertex array.
	_vao = std::make_unique<VertexArray>();
	_vao->addBuffer(*_vbo, *_layout);

	// create indices order.
	_ibo = std::make_unique<IndexBuffer>(_indices, sizeof(_indices));

	// load shaders.
	_shaders = std::make_unique<ShaderLoader>();
	_shaders->load("./res/shaders/basic.shader");
	_shaders->attach("colored_triangle_vertex");
	_shaders->attach("vertices_colors");
	_shaders->createExecutable();
	_shaders->setUniform("u_projection", glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f));
}

CG::Test::TestDrawTriangle::~TestDrawTriangle()
{

}

void CG::Test::TestDrawTriangle::onUpdate(float deltaTime)
{
	(void)deltaTime;
	_renderer->pollEvents();
}

void CG::Test::TestDrawTriangle::onRender()
{
	_renderer->draw(*_vao, *_ibo, *_shaders);
}

void CG::Test::TestDrawTriangle::onStop()
{
}

void CG::Test::TestDrawTriangle::onReset()
{
}