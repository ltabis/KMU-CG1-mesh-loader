#pragma once

#include <glm/gtx/string_cast.hpp>
#include "EditorAxis.hpp"

#include "WorldObjects/Primitives/Plane.hpp"
#include "WorldObjects/Complex/Cube.hpp"
#include "NoClipCameraController.hpp"

namespace CG {
	class EditorView
	{
	public:
		// size: full size of the checkered floor.
		// nsquare: number of squares for each side.
		EditorView(int size, int nsquare);

		inline const std::vector<std::unique_ptr<AShape>>& board() const { return m_Squares; };
		inline const std::vector<std::unique_ptr<AShape>>& models() const { return m_Models; };
		inline const std::vector<std::unique_ptr<AShape>>& axes() const { return m_Axes.axes(); };
		inline int nsquare() const { return m_Nsquare; }
		inline int size() const { return m_Size;    }
	private:
		int m_Size;
		int m_Nsquare;

		EditorAxis m_Axes;
		std::vector<std::unique_ptr<AShape>> m_Squares;
		std::vector<std::unique_ptr<AShape>> m_Models;
	};
}
