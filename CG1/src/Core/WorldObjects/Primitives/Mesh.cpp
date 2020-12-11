#include "Mesh.hpp"

CG::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	: AShape     {          }
	, m_Vertices { vertices }
	, m_Indices  { indices  }
	, m_Textures { textures }
{
	transform = Transform(position, rotation, scale);

	// creating a vertex buffer.
	_vbo = std::make_unique<VertexBuffer>(m_Vertices);

	// creating the layout of the buffer's data.
	_valo = std::make_unique<VertexArrayLayout>();
	_valo->push<float>(4);
	_valo->push<float>(3);
	_valo->push<float>(2);

	// creating the vertex array, add the layout to it.
	_vao = std::make_unique<VertexArray>();
	_vao->addBuffer(*_vbo, *_valo);

	// creating the index buffer.
	_ibo = std::make_unique<IndexBuffer>(m_Indices);
}

const CG::VertexArray& CG::Mesh::vao() const
{
	return *_vao;
}

const CG::IndexBuffer& CG::Mesh::ibo() const
{
	return *_ibo;
}

const std::vector<std::shared_ptr<CG::Texture>>& CG::Mesh::textures() const
{
	return m_Textures;
}
