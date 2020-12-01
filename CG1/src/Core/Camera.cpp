#include "Camera.hpp"

CG::Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float width, float height, float nearPlane, float farPlane, float fov, CameraType type)
	: m_Position	{ position       }
    , m_Front		{ front          }
	, m_Up			{ up             }
    , m_Fov         { fov            }
    , m_NearPlane   { nearPlane      }
    , m_FarPlane    { farPlane       }
    , m_AspectRatio { width / height }
    , m_Type        { type           }
{
    _createProjectionMatrix();
    _createViewMatrix();
}

void CG::Camera::_createProjectionMatrix()
{
    m_Projection = glm::mat4{
        { 1 / (m_AspectRatio * glm::tan(m_Fov / 2)), 0, 0, 0 },
        { 0, 1 / glm::tan(m_Fov / 2), 0, 0 },
        { 0, 0, -((m_FarPlane + m_NearPlane) / (m_FarPlane - m_NearPlane)), -1},
        { 0, 0, -((2 * m_FarPlane * m_NearPlane) / (m_FarPlane - m_NearPlane)), 0 }
    };
}

void CG::Camera::_createViewMatrix()
{
    // finding the x, y and z axis of the camera using cross product.
    glm::vec3 z = glm::normalize(-m_Front);
    glm::vec3 x = glm::normalize(glm::cross(m_Up, z));
    glm::vec3 y = glm::normalize(glm::cross(z, x));

    glm::mat4 translation{ glm::translate(glm::mat4(1.0f), -m_Position) };
    glm::mat4 rotation{
        { x.x, y.x, z.x, 0 },
        { x.y, y.y, z.y, 0 },
        { x.z, y.z, z.z, 0 },
        { 0,   0,   0,   1 }
    };

    m_View = rotation * translation;
}

CG::Camera::~Camera()
{
}

void CG::Camera::translate(const glm::vec3& translation)
{
    m_Position += translation;
    _createViewMatrix();
}

void CG::Camera::rotate(const glm::vec3& rotation)
{
    m_Front += rotation;
    m_View *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
             glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
             glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0, 0, 1));
}

void CG::Camera::setFieldOfView(float fov)
{
    m_Fov = fov;
    _createProjectionMatrix();
}

void CG::Camera::setAspectRatio(float width, float height)
{
    m_AspectRatio = width / height;
    _createProjectionMatrix();
}

void CG::Camera::setPosition(const glm::vec3& position)
{
    m_Position = position;
    _createViewMatrix();
}

void CG::Camera::setFront(const glm::vec3& front)
{
    m_Front = front;
    _createViewMatrix();
}

glm::mat4 CG::Camera::view() const
{
    return m_View;
}

glm::mat4 CG::Camera::projection() const
{
    return m_Projection;
}
glm::mat4 CG::Camera::projectionView() const
{
    return m_Projection * m_View;
}

glm::vec3 CG::Camera::position() const
{
    return m_Position;
}

glm::vec3 CG::Camera::front() const
{
    return m_Front;
}

glm::vec3 CG::Camera::up() const
{
    return m_Up;
}
