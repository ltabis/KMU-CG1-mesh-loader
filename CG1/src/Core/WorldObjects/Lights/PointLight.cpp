#include "PointLight.hpp"

CG::PointLight::PointLight()
	: ALight {}
{}

CG::PointLight::PointLight(
	const glm::vec3& ambiantColor,
	const glm::vec3& diffuseColor,
	const glm::vec3& specularColor,
	const glm::vec3& intensity,
	const glm::vec3& position
)
	: CG::ALight { 
		ambiantColor,
		diffuseColor,
		specularColor,
		intensity
	}
{
	transform.setPosition(position.x, position.y, position.z);
}