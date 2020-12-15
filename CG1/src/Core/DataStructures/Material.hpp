#pragma once

#include <glm/vec3.hpp>

namespace CG
{
	struct Material
	{
		glm::vec3 ambiantColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;

		float shininess;
		float opacity;

		Material()
			: ambiantColor  { glm::vec3(1.f) }
			, diffuseColor  { glm::vec3(1.f) }
			, specularColor { glm::vec3(1.f) }
			, shininess     { 1.f            }
			, opacity       { 1.f            }
		{}
	};
}