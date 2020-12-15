#pragma once

#include <glm/gtx/string_cast.hpp>
#include "Components/ModelLoader.hpp"
#include "EditorAxis.hpp"
#include "../Callbacks.hpp"

#include "WorldObjects/Lights/PointLight.hpp"
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
		enum class ObjectType { NONE, MODEL, LIGHT };

		void render(GUI &gui);

		void deleteObject();

		void createCheckerBoard();
		void renderGUI();
		void renderFloor();
		void renderAxis();
		void renderModels();
		void uploadLights();

		// // GUI
		void renderGuiEnvironment();
		void renderGuiDockSpace();
		void renderGuiMenuBar();

		// Hierarchy
		void renderGuiHierarchy();

		// Inspector.
		void renderGuiInspector();
		void renderGuiInspectorModels();
		void renderGuiInspectorLights();

		void reloadShader(std::unique_ptr<ShaderLoader>& shader, const std::string& shaderPath);

		int m_Size;
		int m_Nsquare;

		EditorAxis m_Axes;
		ObjectType m_SelectedObjectType = ObjectType::NONE;
		unsigned int m_SelectedObject = 0;

		ModelLoader m_ModelLoader;

		std::vector<std::unique_ptr<ALight>> m_Lights;
		std::vector<std::unique_ptr<AShape>> m_Squares;

		EditorCameraController m_Controller;

		// shaders.
		std::unique_ptr<ShaderLoader> m_BlueCheckerShader;
		std::unique_ptr<ShaderLoader> m_LightBlueCheckerShader;
		std::unique_ptr<ShaderLoader> m_AxisShader;

		// Create a shader for each objects.
		std::unique_ptr<ShaderLoader> m_ModelShader;

		// useful variables for frame independent code and fps mode & help for the controller.
		float m_DeltaTime = 0.f;
		float m_LastFrame = 0.f;

		// ImGui settings.
		ImGuiWindowFlags m_WindowFlags;

		// Renderer
		std::unique_ptr<Renderer> m_Renderer;
	};
}
