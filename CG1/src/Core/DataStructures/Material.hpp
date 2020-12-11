#pragma once

#include <glm/vec3.hpp>

namespace CG
{
	struct Material
	{
	public:
		glm::vec3 m_AmbiantColor;
		glm::vec3 m_DiffuseColor;
		glm::vec3 m_SpecularColor;

		float m_Shininess;
	private:
	};
}