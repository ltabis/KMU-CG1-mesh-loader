#include "EditorView.hpp"

CG::EditorView::EditorView(int size, int nsquare)
	: m_Size	{ size											  }
	, m_Nsquare { nsquare										  }
	, m_Axes    { glm::vec3(0.f), glm::vec3(0.f), glm::vec3(20.f) }
{
	// computing components for each plane size.
	float min = -static_cast<float>(size) / 2;
	float max =  static_cast<float>(size) / 2;

	// computing sizes of each squares.
	float sideSize = (max - min) / static_cast<float>(m_Nsquare);

	float squareSize = sideSize * sideSize;

	CG_CONSOLE_INFO("number of squares: {} x {}", sideSize, sideSize);
	CG_CONSOLE_INFO("square size: {}", squareSize);

	// creating the floor.
	float xpos = min;
	for (int x = 0; x < m_Nsquare; ++x, xpos += sideSize) {
		float zpos = min;
		for (int z = 0; z < m_Nsquare; ++z, zpos += sideSize) {
			m_Squares.push_back(std::make_unique<Plane>(
				glm::vec3(xpos * sideSize, 0.f, zpos * sideSize),
				glm::vec3(90.f, 0.f, 0.f),
				glm::vec3(squareSize, 0.f, squareSize)
			));
		}
	}
}