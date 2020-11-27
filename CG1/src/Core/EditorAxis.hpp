#pragma once

#include "WorldObjects/Primitives/Line.hpp"

namespace CG {
	class EditorAxis
	{
	public:
		EditorAxis(
			const glm::vec3& position = glm::vec3(0.f),
			const glm::vec3& rotation = glm::vec3(0.f),
			const glm::vec3& scale = glm::vec3(1.f));
		~EditorAxis() = default;

		inline const std::vector<std::unique_ptr<AShape>>& axes() const { return m_Axes; };

	private:
		std::vector<std::unique_ptr<AShape>> m_Axes;
	};
}
