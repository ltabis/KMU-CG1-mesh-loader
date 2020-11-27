#include "EditorAxis.hpp"

CG::EditorAxis::EditorAxis(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	// creating all axis.
	auto xAxis = std::make_unique<Line>(
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(2.f),
		glm::vec3(1.f, 0.f, 0.f)
	);
	xAxis->transform.rotate(90.f, 1.f, 0.f, 0.f);
	m_Axes.push_back(std::move(xAxis));

	auto yAxis = std::make_unique<Line>(
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(2.f),
		glm::vec3(0.f, 1.f, 0.f)
		);
	m_Axes.push_back(std::move(yAxis));

	auto zAxis = std::make_unique<Line>(
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(2.f),
		glm::vec3(0.f, 0.f, 1.f)
		);
	zAxis->transform.rotate(90.f, 0.f, 0.f, 1.f);
	m_Axes.push_back(std::move(zAxis));

	for (auto& axis : m_Axes) {
		axis->transform.rotate(rotation.x, 1.f, 0.f, 0.f);
		axis->transform.rotate(rotation.y, 0.f, 1.f, 0.f);
		axis->transform.rotate(rotation.z, 0.f, 0.f, 1.f);
		axis->transform.setPosition(position.x, position.y, position.z);
		axis->transform.scale(scale.x, scale.y, scale.z);
	}
}
