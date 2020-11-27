#pragma once

#include "WorldObjects/AShape.hpp"

namespace CG {
	class Triangle : public AShape
	{
	private:

		float _vertices[28] = {
			 0.0f,  0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
		};

		unsigned int _indices[3] = {
			0, 1, 2
		};

	public:
		Triangle(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		~Triangle();

		const VertexArray& vao() const override;
		const IndexBuffer& ibo() const override;
	};
}

