#include "Line.hpp"

CG::Line::Line(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec3& color)
	: AShape()
{
	transform = Transform(position, rotation, scale);

	_vertices[4] = color.x;
	_vertices[5] = color.y;
	_vertices[6] = color.z;
	_vertices[11] = color.x;
	_vertices[12] = color.y;
	_vertices[13] = color.z;

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

CG::Line::~Line()
{
}

const CG::VertexArray& CG::Line::vao() const
{
	return *_vao;
}

const CG::IndexBuffer& CG::Line::ibo() const
{
	return *_ibo;
}
