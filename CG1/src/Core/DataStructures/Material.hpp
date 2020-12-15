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
			: ambiantColor  { glm::vec3(.2f) }
			, diffuseColor  { glm::vec3(0.f) }
			, specularColor { glm::vec3(0.f) }
			, shininess     { 180.f          }
			, opacity       { 1.f            }
		{}

		Material(const Material& mat)
			: ambiantColor   { mat.ambiantColor  }
			, diffuseColor   { mat.diffuseColor  }
			, specularColor  { mat.specularColor }
			, shininess      { mat.shininess     }
			, opacity        { mat.opacity       }
		{}
	};
}