#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace CG {
	class Camera
	{
	public:

		enum class CameraType { PERSPECTIVE, ORTHOGRAPHIC };

		Camera(
			const glm::vec3& position,
			const glm::vec3& front,
			const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f),
			float width = 800.f,
			float height = 800.f,
			float nearPlane = .1f,
			float farPlane = 500.f,
			float fov = 45.f,
			CameraType type = CameraType::PERSPECTIVE);
		~Camera();

		void translate(const glm::vec3& translation);
		void rotate(const glm::vec3& position);

		void setPosition(const glm::vec3& position);
		void setFront(const glm::vec3& front);

		void setFieldOfView(float fov);
		void setAspectRatio(float width, float height);

		glm::mat4 view() const;

		glm::vec3 position() const;
		glm::vec3 front() const;
		glm::vec3 up() const;

	private:
		CameraType _type;

		glm::mat4 _view;
		glm::mat4 _projection;

		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _up;

		float _fov;
		float _nearPlane;
		float _farPlane;
		float _aspectRatio;

		void _createViewMatrix();
		void _createProjectionMatrix();
	};
}