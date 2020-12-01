#include "NoClipCameraController.hpp"

CG::NoClipCameraController::NoClipCameraController(
    GLFWwindow *window,
    const glm::vec3& position,
    const glm::vec3& point
)
    : _window { window }
    , _camera {
        position,
        point,
        glm::vec3(0.f, 1.f, 0.f),
        1920.f,
        1080.f,
        .1f,
        500.f,
        90.f
    }
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwGetCursorPos(_window, &_lastMouseXPosition, &_lastMouseYPosition);
}

CG::NoClipCameraController::~NoClipCameraController()
{
}

void CG::NoClipCameraController::setFieldOfView(float fov)
{
    _camera.setFieldOfView(fov);
}

void CG::NoClipCameraController::setAspectRatio(float width, float height)
{
    _camera.setAspectRatio(width, height);
}

void CG::NoClipCameraController::update(float deltaTime)
{
    _computeCameraTranslation(deltaTime);
    _computeCameraRotation(deltaTime);
}

glm::mat4 CG::NoClipCameraController::projectionView() const
{
    return _camera.projectionView();
}

glm::vec3 CG::NoClipCameraController::position() const
{
    return _camera.position();
}

glm::mat4 CG::NoClipCameraController::projection() const
{
    return _camera.projection();
}

glm::mat4 CG::NoClipCameraController::view() const
{
    return _camera.view();
}

void CG::NoClipCameraController::_computeCameraTranslation(float deltaTime)
{
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _camera.translate(glm::normalize(_camera.front()) * speed * deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _camera.translate(glm::normalize(-_camera.front()) * speed * deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _camera.translate(glm::normalize(glm::cross(_camera.front(), _camera.up())) * speed * deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _camera.translate(-glm::normalize(glm::cross(_camera.front(), _camera.up())) * speed * deltaTime);

    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        _camera.translate(glm::normalize(_camera.up()) * speed * deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        _camera.translate(glm::normalize(-_camera.up()) * speed * deltaTime);
}

void CG::NoClipCameraController::_computeCameraRotation(float deltaTime)
{
    double xpos;
    double ypos;

    glfwGetCursorPos(_window, &xpos, &ypos);

    float mouse_x_offset = static_cast<float>(xpos - _lastMouseXPosition) * sensitivity;
    float mouse_y_offset = static_cast<float>(_lastMouseYPosition - ypos) * sensitivity;

    _lastMouseXPosition = xpos;
    _lastMouseYPosition = ypos;

    _yaw += mouse_x_offset;
    _pitch += mouse_y_offset;

    // applying constraints on the y axis.
    if (_pitch > 89.0f)
        _pitch = 89.0f;
    if (_pitch < -89.0f)
        _pitch = -89.0f;

    glm::vec3 direction;
    direction.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
    direction.y = std::sin(glm::radians(_pitch));
    direction.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));

    _camera.setFront(glm::normalize(direction));
}
