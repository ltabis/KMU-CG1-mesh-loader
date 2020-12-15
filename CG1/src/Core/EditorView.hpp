#pragma once

#include <glm/gtx/string_cast.hpp>
#include "Components/ModelLoader.hpp"
#include "EditorAxis.hpp"
#include "../Callbacks.hpp"

#include "WorldObjects/Primitives/Plane.hpp"
#include "WorldObjects/Complex/Cube.hpp"
#include "EditorCameraController.hpp"

namespace CG {
	class EditorView
	{
	public:
		// size: full size of the checkered floor.
		// nsquare: number of squares for each side.
		EditorView(int size, int nsquare, Renderer* renderer);

		inline const std::vector<std::unique_ptr<AShape>>& axes() const { return m_Axes.axes(); };
		inline int nsquare() const { return m_Nsquare; }
		inline int size() const { return m_Size;  }

		void start(CG::GUI &gui);

	private:
		void render(GUI &gui);

		void createCheckerBoard();
		void renderGUI();
		void renderFloor();
		void renderAxis();
		void renderModels();

		void renderGuiEnvironment();
		void renderGuiHierarchy();
		void renderGuiDockSpace();
		void renderGuiMenuBar();

		int m_Size;
		int m_Nsquare;

		char m_ModelPath[500];

		EditorAxis m_Axes;
		bool m_ObjectSelected;
		unsigned int m_SelectedModel = 0;

		ModelLoader m_ModelLoader;

		std::vector<std::unique_ptr<AShape>> m_Squares;

		EditorCameraController m_Controller;

		// shaders.
		ShaderLoader m_BlueCheckerShader;
		ShaderLoader m_LightBlueCheckerShader;
		ShaderLoader m_AxisShader;

		// Create a shader for each objects.
		ShaderLoader m_BlinnPhongShader;

		glm::vec3 m_AmbiantLightColor;
		glm::vec3 m_ObjectColor;
		glm::vec3 m_LightPos;

		// useful variables for frame independent code and fps mode & help for the controller.
		float m_DeltaTime = 0.f;
		float m_LastFrame = 0.f;

		// ImGui settings.
		ImGuiWindowFlags m_WindowFlags;

		// Renderer
		std::unique_ptr<Renderer> m_Renderer;
	};
}
