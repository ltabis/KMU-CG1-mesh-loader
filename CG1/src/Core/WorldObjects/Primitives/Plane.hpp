#pragma once

#include "WorldObjects/AShape.hpp"

namespace CG {
	class Plane : public AShape
	{
	private:

		float _vertices[28] = {
			-0.5f, -0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f,    0.0f,  0.0f, 1.0f
		};

		unsigned int _indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

	public:
		Plane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		~Plane();

		const VertexArray& vao() const override;
		const IndexBuffer& ibo() const override;
	};
}

