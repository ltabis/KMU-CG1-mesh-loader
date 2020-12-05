#pragma once

#include "Camera.hpp"
#include "Renderer.hpp"

#include "../Dependencies/Viewer/Viewer.h"

namespace CG {
	class EditorCameraController
	{
	public:

		EditorCameraController(
			GLFWwindow* _window,
			const glm::vec3& position = glm::vec3(0.f),
			const glm::vec3& point = glm::vec3(0.f)
		);
		~EditorCameraController();

		void setFieldOfView(float fov);
		void setAspectRatio(float width, float height);
		void update(float deltaTime);

		glm::mat4 view() const;
		glm::mat4 projection() const;
		glm::mat4 projectionView() const;

		glm::vec3 position() const;

		float speed = 1.f;
		float sensitivity = 0.1f;

	private:

		GLFWwindow* m_Window;
		Viewer m_Viewer;

		double m_LastMouseXPosition = 0;
		double m_LastMouseYPosition = 0;

		bool lbutton_down;
		bool rbutton_down;
		bool mbutton_down;
		double m_lastMouseX;
		double m_lastMouseY;
		double cx, cy;

		void _computeCameraInputs(float deltaTime);
		void _computeCameraRotation(float deltaTime);

	};
}