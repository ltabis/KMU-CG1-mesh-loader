#pragma once

#include "Renderer.hpp"

static void escape_callback(CG::Renderer* renderer, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        glfwSetWindowShouldClose(renderer->window(), GLFW_TRUE);
}

static void spacebar_callback(CG::Renderer* renderer, int key, int scancode, int action, int mods)
{
    //if (action == GLFW_PRESS)
    //    CG_CONSOLE_INFO("spacebar pressed");
}
