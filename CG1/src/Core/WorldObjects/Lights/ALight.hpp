#pragma once

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "DataStructures/Transform.hpp"

namespace CG
{
	struct ALight
	{
		ALight()
			: ambiantColor  { glm::vec3(1.f) }
			, diffuseColor  { glm::vec3(1.f) }
			, specularColor { glm::vec3(1.f) }
			, intensity		{ glm::vec3(1.f) } {}

		ALight(
			const glm::vec3& ambiantColor,
			const glm::vec3& diffuseColor,
			const glm::vec3& specularColor,
			const glm::vec3& intensity
		)
			: ambiantColor  { ambiantColor  }
			, diffuseColor  { diffuseColor  }
			, specularColor { specularColor }
			, intensity     { intensity     } {}

		glm::vec3 ambiantColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		glm::vec3 intensity;

		Transform transform;
	};
}