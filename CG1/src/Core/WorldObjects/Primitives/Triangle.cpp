#include "Triangle.hpp"

CG::Triangle::Triangle(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	: AShape()
{
	transform = Transform(position, rotation, scale);

	// creating a vertex buffer.
	_vbo = std::make_unique<VertexBuffer>(_vertices, sizeof(_vertices));

	// creating the layout of the buffer's data.
	_valo = std::make_unique<VertexArrayLayout>();
	_valo->push<float>(4);
	_valo->push<float>(3);

	// creating the vertex array, add the layout to it.
	_vao = std::make_unique<VertexArray>();
	_vao->addBuffer(*_vbo, *_valo);

	// creating the index buffer.
	_ibo = std::make_unique<IndexBuffer>(_indices, sizeof(_indices));
}

CG::Triangle::~Triangle()
{
}

const CG::VertexArray& CG::Triangle::vao() const
{
	return *_vao;
}

const CG::IndexBuffer& CG::Triangle::ibo() const
{
	return *_ibo;
}
