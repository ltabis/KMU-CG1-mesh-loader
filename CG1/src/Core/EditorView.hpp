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
		EditorView(int size, int nsquare, Renderer& renderer);

		inline const std::vector<std::unique_ptr<AShape>>& board() const { return m_Squares; };
		inline const std::vector<std::unique_ptr<AShape>>& models() const { return m_Models; };
		inline const std::vector<std::unique_ptr<AShape>>& axes() const { return m_Axes.axes(); };
		inline int nsquare() const { return m_Nsquare; }
		inline int size() const { return m_Size;  }

		void render(Renderer &renderer, GUI &gui);

	private:
		void createCheckerBoard();
		void renderGUI();

		int m_Size;
		int m_Nsquare;

		EditorAxis m_Axes;
		std::vector<std::unique_ptr<AShape>> m_Squares;
		std::vector<std::unique_ptr<AShape>> m_Models;

		NoClipCameraController m_Controller;

		// shaders.
		CG::ShaderLoader blueCheckerShader;
		CG::ShaderLoader lightBlueCheckerShader;
		CG::ShaderLoader colorShader;

		// useful variables for frame independent code and fps mode & help for the controller.
		float m_DeltaTime = 0.f;
		float m_LastFrame = 0.f;
		bool  m_FpsMode = false;
		bool  m_HelpOpened = true;
	};
}
