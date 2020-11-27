#include "Camera.hpp"

CG::Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float width, float height, float nearPlane, float farPlane, float fov, CameraType type)
	: _position		        { position             }
    , _front		        { front                }
	, _up			        { up                   }
    , _fov                  { fov                  }
    , _nearPlane            { nearPlane            }
    , _farPlane             { farPlane             }
    , _aspectRatio          { width / height       }
    , _type                 { type                 }
{
    _createProjectionMatrix();
    _createViewMatrix();
}

void CG::Camera::_createProjectionMatrix()
{
    _projection = glm::mat4{
        { 1 / (_aspectRatio * glm::tan(_fov / 2)), 0, 0, 0 },
        { 0, 1 / glm::tan(_fov / 2), 0, 0 },
        { 0, 0, -((_farPlane + _nearPlane) / (_farPlane - _nearPlane)), -1},
        { 0, 0, -((2 * _farPlane * _nearPlane) / (_farPlane - _nearPlane)), 0 }
    };
}

void CG::Camera::_createViewMatrix()
{
    // finding the x, y and z axis of the camera using cross product.
    glm::vec3 z = glm::normalize(-_front);
    glm::vec3 x = glm::normalize(glm::cross(_up, z));
    glm::vec3 y = glm::normalize(glm::cross(z, x));

    glm::mat4 translation{ glm::translate(glm::mat4(1.0f), -_position) };
    glm::mat4 rotation{
        { x.x, y.x, z.x, 0 },
        { x.y, y.y, z.y, 0 },
        { x.z, y.z, z.z, 0 },
        { 0,   0,   0,   1 }
    };

    _view = rotation * translation;
}

CG::Camera::~Camera()
{
}

void CG::Camera::translate(const glm::vec3& translation)
{
    _position += translation;
    _createViewMatrix();
}

void CG::Camera::rotate(const glm::vec3& rotation)
{
    _front += rotation;
    _view *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
             glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
             glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0, 0, 1));
}

void CG::Camera::setFieldOfView(float fov)
{
    _fov = fov;
    _createProjectionMatrix();
}

void CG::Camera::setAspectRatio(float width, float height)
{
    _aspectRatio = width / height;
    _createProjectionMatrix();
}

void CG::Camera::setPosition(const glm::vec3& position)
{
    _position = position;
    _createViewMatrix();
}

void CG::Camera::setFront(const glm::vec3& front)
{
    _front = front;
    _createViewMatrix();
}

glm::mat4 CG::Camera::view() const
{
	return _projection * _view;
}

glm::vec3 CG::Camera::position() const
{
    return _position;
}

glm::vec3 CG::Camera::front() const
{
    return _front;
}

glm::vec3 CG::Camera::up() const
{
    return _up;
}
