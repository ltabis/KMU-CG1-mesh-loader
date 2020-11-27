#pragma once

#include "WorldObjects/AShape.hpp"

namespace CG {
	class Cube : public AShape
	{
	private:

		float _vertices[56] = {
			// front
		   -1.0, -1.0, 1.0, 1.0,  	1.0, 0.0, 0.0,
			1.0, -1.0, 1.0, 1.0,	0.0, 1.0, 0.0,
			1.0,  1.0, 1.0, 1.0,	0.0, 0.0, 1.0,
		   -1.0,  1.0, 1.0,	1.0,	1.0, 1.0, 1.0,
		   // back
		  -1.0, -1.0, -1.0, 1.0, 	1.0, 0.0, 0.0,
		   1.0, -1.0, -1.0, 1.0,	0.0, 1.0, 0.0,
		   1.0,  1.0, -1.0, 1.0,	0.0, 0.0, 1.0,
		  -1.0,  1.0, -1.0, 1.0,	1.0, 1.0, 1.0
		};

		unsigned int _indices[36] = {
			0, 1, 2,		2, 3, 0,		1, 5, 6,
			6, 2, 1,		7, 6, 5,		5, 4, 7,
			4, 0, 3,		3, 7, 4,		4, 5, 1,
			1, 0, 4,		3, 2, 6,		6, 7, 3,
		};

	public:
		Cube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		~Cube();

		const VertexArray& vao() const override;
		const IndexBuffer& ibo() const override;
	};
}

