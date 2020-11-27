// Renderer.hpp
// Renderer class declaration.

#pragma once

// using the crt library in debug mode
// to find memory leaks.
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_CLIENTBLOCK
#endif /* ! _DEBUG */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.hpp"
#include "Components/GUI.hpp"
#include "Components/ShaderLoader.hpp"
#include "DataStructures/VertexArray.hpp"
#include "DataStructures/IndexBuffer.hpp"
#include "WorldObjects/AShape.hpp"

namespace glm
{
	// recreate your propre classes
	// it will be better in memory.
	// (vector class is bloated)
	typedef vec2 point2;
	typedef vec3 point3;
}

namespace CG
{
	class Renderer;
	class EventHandler;
	typedef void (*CGCallback)(Renderer* renderer, int key, int scancode, int action, int mods);

	class Renderer
	{
	private:
		GLFWwindow* _window;
		std::unique_ptr<EventHandler> _eventHandler;
		glm::mat4 _view;
		glm::mat4 _projection;

		float _fov;
		float _aspectRatio;

	public:
		friend void input_handler(GLFWwindow* window, int key, int scancode, int action, int mods);

		Renderer(const std::string &windowName = "Window", int width = 640, int height = 480);
		~Renderer();

		void setAspectRatio(float width, float height);
		void setFov(float fov);

		void clear() const;
		void clearColor(float r, float g, float b, float a) const;
		void draw(const VertexArray& vao, const IndexBuffer& ibo, const ShaderLoader& shader) const;
		void draw(const AShape& shape, const ShaderLoader& shader) const;
		void drawLine(const VertexArray& vao, const IndexBuffer& ibo, const ShaderLoader& shader) const;
		void drawLine(const AShape& shape, const ShaderLoader& shader) const;
		void drawWireFrame(const AShape& shape, const ShaderLoader& shader) const;
		void pollEvents() const;
		void swapBuffers() const;
		bool windowShouldClose();

		void createMVP();
		void createViewMatrix(const glm::vec3& campos, const glm::vec3& look, const glm::vec3& up);
		void createProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane);

		glm::mat4 viewMatrix() const;
		glm::mat4 projectionMatrix() const;

		void registerKeyBindingCallback(unsigned int key, CGCallback callback);
		GLFWwindow* window() { return _window; };
	};

	// forward declarations.
	void input_handler(GLFWwindow* window, int key, int scancode, int action, int mods);

	class EventHandler
	{
	private:
		std::string _configPath;
		std::unordered_map<unsigned int, CGCallback> _keyCallbacks;
	public:

		EventHandler(Renderer* renderer, const std::string& configPath = "")
			: _configPath{ configPath }
		{
			if (!renderer || !renderer->window())
				throw "Couldn't initialize the event handler, window null";

			glfwSetKeyCallback(renderer->window(), input_handler);
			glfwSetWindowUserPointer(renderer->window(), renderer);
		}

		// TODO: implemente.
		void saveBindings() const;
		void loadBindings() const;
		void registerCallback(unsigned int, CGCallback callback);
		void executeCallback(Renderer* renderer, int key, int scancode, int action, int mods);
	};

	// CALLBACKS //
	static void input_handler(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		void* data = glfwGetWindowUserPointer(window);

		if (!data)
			return;

		Renderer *renderer = static_cast<CG::Renderer*>(data);
		renderer->_eventHandler->executeCallback(renderer, key, scancode, action, mods);
	}

	static void resize_callback(GLFWwindow* window, int width, int height)
	{
		void* data = glfwGetWindowUserPointer(window);

		if (!data)
			return;

		Renderer* renderer = static_cast<CG::Renderer*>(data);

		renderer->setAspectRatio((float)width, (float)height);
		glViewport(0, 0, width, height);
	}
}
