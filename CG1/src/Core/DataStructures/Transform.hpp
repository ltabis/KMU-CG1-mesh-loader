#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace CG {
	struct Transform {

		Transform(
			const glm::vec3& position = glm::vec3(0.f),
			const glm::vec3& rotation = glm::vec3(0.f),
			const glm::vec3& scale = glm::vec3(1.f)
		)
			: _position		  { position       }
			, _rotation		  { rotation       }
			, _scale		  { scale          }
			, _modelCache     { glm::mat4(1.f) }
			, _translationMat { glm::mat4(1.f) }
			, _rotationMat	  { glm::mat4(1.f) }
			, _scaleMat		  { glm::mat4(1.f) }
		{
			setPosition(position.x, position.y, position.z);
			setRotation(rotation.x, rotation.y, rotation.z, glm::mat4(1.f));
			setScale(scale.x, scale.y, scale.z);
			_updateModel();
		}

		void translate(float x, float y, float z)
		{
			auto tmp = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));

			_modelCache *= tmp;
			_translationMat *= tmp;
		}

		void rotate(float degree, float x, float y, float z)
		{
			auto tmp = glm::rotate(glm::mat4(1.f), glm::radians(degree), glm::vec3(x, y, z));

			_modelCache *= tmp;
			_rotationMat *= tmp;
		}

		void scale(float x, float y, float z)
		{
			auto tmp = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));

			_modelCache *= tmp;
			_scaleMat *= tmp;
		}

		void setPosition(float x, float y, float z)
		{
			_translationMat = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
			_position = glm::vec3(x, y, z);
			_updateModel();
		}

		void setRotation(float anglex, float angley, float anglez, const glm::mat4& pivot)
		{
			_rotationMat =
				  glm::rotate(glm::mat4(1.f), glm::radians(anglex), glm::vec3(pivot[0][0], pivot[0][1], pivot[0][2]))
				* glm::rotate(glm::mat4(1.f), glm::radians(angley), glm::vec3(pivot[1][0], pivot[1][1], pivot[1][2]))
				* glm::rotate(glm::mat4(1.f), glm::radians(anglez), glm::vec3(pivot[2][0], pivot[2][1], pivot[2][2]));
			_rotation = glm::vec3(anglex, angley, anglez);
			_updateModel();
		}

		void setScale(float x, float y, float z)
		{
			_scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(x, y, z));
			_scale = glm::vec3(x, y, z);
			_updateModel();
		}

		inline glm::mat4 model() const { return _modelCache; }
		inline glm::vec3 position() const { return _position; }
		inline glm::vec3 rotation() const { return _rotation; }
		inline glm::vec3 scale() const { return _scale; }

	private:

		// updates the model matrix in case of transform reset.
		void _updateModel()
		{
			_modelCache = _translationMat * _scaleMat * _rotationMat;
		}

		// visual data.
		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;

		// transform matrices.
		glm::mat4 _translationMat;
		glm::mat4 _rotationMat;
		glm::mat4 _scaleMat;

		// model cache.
		glm::mat4 _modelCache;
	};
}