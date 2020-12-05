#include "EditorCameraController.hpp"

CG::EditorCameraController::EditorCameraController(
    GLFWwindow *window,
    const glm::vec3& position,
    const glm::vec3& point
)
    : m_Window { window }
    , m_Viewer {
        position,
        point,
        glm::vec3(0.f, 1.f, 0.f),
        90.f,
        1920.f / 1080.f,
}
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwGetCursorPos(m_Window, &m_LastMouseXPosition, &m_LastMouseYPosition);
}

CG::EditorCameraController::~EditorCameraController()
{
}

void CG::EditorCameraController::setFieldOfView(float fov)
{
    m_Viewer.setFieldOfView(fov);
}

void CG::EditorCameraController::setAspectRatio(float width, float height)
{
    m_Viewer.setAspectRatio(width / height);
}

void CG::EditorCameraController::update(float deltaTime)
{
    glfwGetCursorPos(m_Window, &cx, &cy);

    _computeCameraInputs(deltaTime);
    _computeCameraRotation(deltaTime);
}

glm::mat4 CG::EditorCameraController::projectionView() const
{
    return m_Viewer.projection() * view();
}

glm::vec3 CG::EditorCameraController::position() const
{
    return m_Viewer.getViewPoint();
}

glm::mat4 CG::EditorCameraController::projection() const
{
    return m_Viewer.projection();
}

glm::mat4 CG::EditorCameraController::view() const
{
    return glm::lookAt(m_Viewer.getViewPoint(), m_Viewer.getViewCenter(), m_Viewer.getUpVector());
}

void CG::EditorCameraController::_computeCameraInputs(float deltaTime)
{
    // getting buttons states.
    int stateL = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
    int stateR = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT);
    int stateM = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE);
    
    if (stateL == GLFW_PRESS)
        lbutton_down = true;
    else if (stateL == GLFW_RELEASE)
        lbutton_down = false;

    if (stateR == GLFW_PRESS)
        rbutton_down = true;
    else if (stateR == GLFW_RELEASE)
        rbutton_down = false;

    if (stateM == GLFW_PRESS)
        mbutton_down = true;
    else if (stateM == GLFW_RELEASE)
        mbutton_down = false;
}

void CG::EditorCameraController::_computeCameraRotation(float deltaTime)
{
    if (lbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(1920);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(1080);
        m_Viewer.rotate(fractionChangeX, fractionChangeY);
    }
    else if (mbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(1920);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(1080);
        m_Viewer.zoom(fractionChangeY);
    }
    else if (rbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(1920);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(1080);
        m_Viewer.translate(-fractionChangeX, -fractionChangeY, 1);
    }
    m_lastMouseX = cx;
    m_lastMouseY = cy;
}
