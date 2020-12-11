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
		GLFWwindow* m_Window;
		std::unique_ptr<EventHandler> m_EventHandler;
		
	public:
		friend void input_handler(GLFWwindow* window, int key, int scancode, int action, int mods);

		Renderer(const std::string &windowName = "Window", int width = 640, int height = 480);
		~Renderer();

		void clear() const;
		void clearColor(float r, float g, float b, float a) const;
		void draw(const VertexArray& vao, const IndexBuffer& ibo, const ShaderLoader& shader) const;
		void draw(const AShape& shape, const ShaderLoader& shader) const;
		void drawLine(const VertexArray & vao, const IndexBuffer & ibo, const ShaderLoader & shader) const;
		void drawLine(const AShape & shape, const ShaderLoader & shader) const;
		void drawWireFrame(const AShape& shape, const ShaderLoader& shader) const;
		void pollEvents() const;
		void swapBuffers() const;
		bool windowShouldClose();

		void registerKeyBindingCallback(unsigned int key, CGCallback callback);
		GLFWwindow* window() { return m_Window; };
	};

	// forward declarations.
	void input_handler(GLFWwindow* window, int key, int scancode, int action, int mods);

	class EventHandler
	{
	private:
		std::string m_ConfigPath;
		std::unordered_map<unsigned int, CGCallback> m_KeyCallbacks;
	public:

		EventHandler(Renderer* renderer, const std::string& configPath = "")
			: m_ConfigPath{ configPath }
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
		renderer->m_EventHandler->executeCallback(renderer, key, scancode, action, mods);
	}

	static void resize_callback(GLFWwindow* window, int width, int height)
	{
		void* data = glfwGetWindowUserPointer(window);

		if (!data)
			return;

		Renderer* renderer = static_cast<CG::Renderer*>(data);

		glViewport(0, 0, width, height);
	}
}
