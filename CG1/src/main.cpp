﻿// main.cpp
// Application entry point.

#include "Profiling/Breakpoint.hpp"
#include "EditorView.hpp"

int main(void)
{
	CG::Logger::Init();

	// Creating the renderer and GUI elements.
	CG::Renderer *renderer = new CG::Renderer("Checker board", 1920, 1080);
	CG::GUI gui(renderer->window(), CG::GUI::Style::DARK);
	
	// Creating the checker board with its shaders.
	CG::EditorView editor(50, 16, renderer);

	editor.start(gui);

	return 0;
}