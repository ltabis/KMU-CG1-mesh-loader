#pragma once

#include "DataStructures/Texture.hpp"
#include "DataStructures/Vertex.hpp"
#include "WorldObjects/AShape.hpp"

namespace CG {
	class Mesh : public AShape
	{
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<std::shared_ptr<Texture>> m_Textures;
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, const glm::vec3& position = glm::vec3(0.f), const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f));
		~Mesh() = default;

		const VertexArray& vao() const override;
		const IndexBuffer& ibo() const override;
		const std::vector<std::shared_ptr<Texture>>& textures() const;
	};
}