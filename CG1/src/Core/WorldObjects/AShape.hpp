#pragma once

#include "DataStructures/IndexBuffer.hpp"
#include "DataStructures/VertexArray.hpp"
#include "DataStructures/Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace CG {
	class AShape {
	public:
		AShape() = default;
		virtual ~AShape() = default;

		Transform transform;

		virtual const VertexArray& vao() const = 0;
		virtual const IndexBuffer& ibo() const = 0;

	protected:
		std::unique_ptr<VertexBuffer> _vbo;
		std::unique_ptr<VertexArray> _vao;
		std::unique_ptr<VertexArrayLayout> _valo;
		std::unique_ptr<IndexBuffer> _ibo;
	};
}