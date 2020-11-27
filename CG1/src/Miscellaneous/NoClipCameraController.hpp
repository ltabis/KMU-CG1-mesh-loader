#pragma once

#include "Camera.hpp"
#include "Renderer.hpp"

namespace CG {
	class NoClipCameraController
	{
	public:

		NoClipCameraController(
			GLFWwindow* _window,
			const glm::vec3& position = glm::vec3(0.f),
			const glm::vec3& point = glm::vec3(0.f)
		);
		~NoClipCameraController();

		void setFieldOfView(float fov);
		void setAspectRatio(float width, float height);
		void update(float deltaTime);
		glm::mat4 view() const;

		float speed = 1.f;
		float sensitivity = 0.1f;

	private:

		GLFWwindow* _window;
		CG::Camera _camera;

		double _lastMouseXPosition = 0;
		double _lastMouseYPosition = 0;

		float _pitch = 0.f;
		float _yaw = 0.f;
		// Not really needed, but would be cool to implement.
		float _roll = 0.f;

		void _computeCameraTranslation(float deltaTime);
		void _computeCameraRotation(float deltaTime);
	};
}