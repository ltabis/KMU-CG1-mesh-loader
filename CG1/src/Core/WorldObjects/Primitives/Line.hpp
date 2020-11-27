#pragma once

#include "WorldObjects/AShape.hpp"

namespace CG {
	class Line : public AShape
	{
	private:

		float _vertices[14] = {
			 0.0f, 10.0f, 0.0f, 1.0f,	1.f, 1.f, 1.f,
			 0.0f, 0.0f, 0.0f, 1.0f,	1.f, 1.f, 1.f
		};

		unsigned int _indices[2] = {
			0, 1
		};

	public:
		Line(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec3& color = glm::vec3(1.f));
		~Line();

		const VertexArray& vao() const override;
		const IndexBuffer& ibo() const override;
	};
}

