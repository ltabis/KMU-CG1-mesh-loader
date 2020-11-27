#include "Renderer.hpp"

/* register a new callback in memory that will be called when a key is pressed. */
void CG::EventHandler::registerCallback(unsigned int key,
    CG::CGCallback callback
)
{
    auto key_index = _keyCallbacks.find(key);

    if (key_index == _keyCallbacks.end())
        _keyCallbacks.emplace(key, callback);
    else {
        CG_CONSOLE_WARN("callback '{}' already bound. overriding it.", key);
        _keyCallbacks[key] = callback;
    }
}

/* execute the callback assigned to a specific key. */
void CG::EventHandler::executeCallback(Renderer* renderer, int key, int scancode, int action, int mods)
{
    for (auto& [callbackKey, callback] : _keyCallbacks)
        if (key == callbackKey)
            callback(renderer, key, scancode, action, mods);
}

/* openGL error callback. will be called if any error is thrown by glew. */
static void GLAPIENTRY glewErrorCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
)
{
    CG_LOG_ERROR("OpenGL internal error: {}, type: 0x{}, severity: 0x{}, message: {}",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type,
        severity,
        message
    );
}

/* glfw error callback. will be called if any error is thrown by glfw */
static void glfwErrorCallback(int error, const char* description)
{
    CG_LOG_ERROR("Error '{}': {}", error, description);
}

/* initializing the Renderer object. glfw / glew */
CG::Renderer::Renderer(const std::string& windowName, int width, int height)
    : _view        { glm::mat4(1.f)               }
    , _projection  { glm::mat4(1.f)               }
    , _fov         { 45                           }
    , _aspectRatio { (float)width / (float)height }
{
    CG_LOG_INFO("Initializing OpenGL Renderer.");

    /* using modern opengl */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* setting error callback */
    glfwSetErrorCallback(glfwErrorCallback);

    /* Initialize glfw */
    if (!glfwInit())
        throw "Couldn't initialize glfw.";

    CG_LOG_INFO("Glfw initialized.");

    /* Create a windowed mode window and its OpenGL context */
    _window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

    if (!_window)
    {
        glfwTerminate();
        throw "Couldn't initialize glfw's window.";
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    /* disable Vsync, synchronises with monitor refresh rate */
    glfwSwapInterval(1);

    /* setting window resize callback */
    glfwSetWindowSizeCallback(_window, resize_callback);

    /* create the mvp matrix */
    createMVP();

    /* Initialize glew */
    if (glewInit() != GLEW_OK)
        throw "Couldn't initialize glew.";

    CG_LOG_INFO("Glew initialized.");

    /* Create an event handler instance to register key callbacks */
    _eventHandler = std::make_unique<EventHandler>(this);

    /* Initializing error debug callback */
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDebugMessageCallback(glewErrorCallback, 0);

    CG_LOG_INFO("Renderer ready.");
}

CG::Renderer::~Renderer()
{
    glfwTerminate();
}

void CG::Renderer::setAspectRatio(float width, float height)
{
    _aspectRatio = width / height;
    createProjectionMatrix(glm::radians(_fov), _aspectRatio, .1f, 500.f);
}

void CG::Renderer::setFov(float fov)
{
    _fov = fov;
    createProjectionMatrix(glm::radians(fov), _aspectRatio, .1f, 500.f);
}

void CG::Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CG::Renderer::clearColor(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
}

void CG::Renderer::draw(const VertexArray& vao, const IndexBuffer& ibo, const ShaderLoader& shader) const
{
    shader.use();
    vao.bind();
    ibo.bind();

    glDrawElements(GL_TRIANGLES, ibo.indices(), GL_UNSIGNED_INT, nullptr);
}

void CG::Renderer::drawLine(const VertexArray& vao, const IndexBuffer& ibo, const ShaderLoader& shader) const
{
    shader.use();
    vao.bind();
    ibo.bind();

    glDrawElements(GL_LINES, ibo.indices(), GL_UNSIGNED_INT, nullptr);
}

void CG::Renderer::draw(const AShape& shape, const ShaderLoader& shader) const
{
    draw(shape.vao(), shape.ibo(), shader);
}

void CG::Renderer::drawLine(const AShape& shape, const ShaderLoader& shader) const
{
    drawLine(shape.vao(), shape.ibo(), shader);
}

void CG::Renderer::drawWireFrame(const AShape& shape, const ShaderLoader& shader) const
{
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);

    draw(shape.vao(), shape.ibo(), shader);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
}

void CG::Renderer::pollEvents() const
{
    glfwPollEvents();
}

void CG::Renderer::swapBuffers() const
{
    glfwSwapBuffers(_window);
}

bool CG::Renderer::windowShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void CG::Renderer::createViewMatrix(const glm::vec3& campos, const glm::vec3& look, const glm::vec3& up)
{
    glm::vec3 z = glm::normalize(campos - look);
    glm::vec3 x = glm::normalize(glm::cross(up, z));
    glm::vec3 y = glm::normalize(glm::cross(z, x));

    glm::mat4 translation { glm::translate(glm::mat4(1.0f), -campos) };
    glm::mat4 rotation {
        { x.x, y.x, z.x, 0 },
        { x.y, y.y, z.y, 0 },
        { x.z, y.z, z.z, 0 },
        { 0,   0,   0,   1 }
    };

    _view = rotation * translation;
}

void CG::Renderer::createProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane)
{
    _projection = glm::mat4 {
        { 1 / (aspect * glm::tan(fovy / 2)), 0, 0, 0 },
        { 0, 1 / glm::tan(fovy / 2), 0, 0 },
        { 0, 0, -((farPlane + nearPlane) / (farPlane - nearPlane)), -1},
        { 0, 0, -((2 * farPlane * nearPlane) / (farPlane - nearPlane)), 0 }
    };
}

void CG::Renderer::createMVP()
{
    createViewMatrix(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    createProjectionMatrix(glm::radians(_fov), _aspectRatio, .1f, 500.f);
}

glm::mat4 CG::Renderer::viewMatrix() const
{
    return _view;
}

glm::mat4 CG::Renderer::projectionMatrix() const
{
    return _projection;
}


void CG::Renderer::registerKeyBindingCallback(
    unsigned int key,
    CGCallback callback
)
{
    _eventHandler->registerCallback(key, callback);
}