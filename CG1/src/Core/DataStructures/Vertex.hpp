#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace CG
{
	struct Vertex
	{
		Vertex(const glm::vec3 vCoords, const glm::vec3 vNormal, const glm::vec2 vTextureCoords)
			: position		{ glm::vec4(vCoords, 1.f) }
			, normal		{ vNormal				  }
			, textureCoords	{ vTextureCoords		  }
		{}

		glm::vec4 position;
		glm::vec3 normal;
		glm::vec2 textureCoords;
	};
}