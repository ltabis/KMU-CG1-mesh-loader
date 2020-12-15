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

		void translate(float x, float y, float z);
		void rotate(float degree, float x, float y, float z);
		void scale(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setRotation(float anglex, float angley, float anglez, const glm::mat4& pivot);
		void setScale(float x, float y, float z);

	private:
		Transform m_Transform;
		std::vector<std::unique_ptr<AShape>> m_Axes;
	};
}
