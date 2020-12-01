#include "EditorView.hpp"

CG::EditorView::EditorView(int size, int nsquare, Renderer &renderer)
	: m_Size	   { size															 }
	, m_Nsquare    { nsquare														 }
	, m_Axes       { glm::vec3(0.f), glm::vec3(0.f), glm::vec3(20.f)				 }
	, m_Controller { renderer.window(), glm::vec3(-20.f, 20.f, 20.f), glm::vec3(0.f) }
{
	createCheckerBoard();

	// Creating the camera controller.
	CG::NoClipCameraController controller(
		renderer.window(),
		glm::vec3(-20.f, 20.f, 20.f),
		glm::vec3(0.f)
	);
	controller.speed = 0.1f;

	// laoding shaders.
	blueCheckerShader.load("./res/shaders/checker.shader");
	blueCheckerShader.attach("triangle");
	blueCheckerShader.attach("color_blue");
	blueCheckerShader.createExecutable();

	lightBlueCheckerShader.load("./res/shaders/checker.shader");
	lightBlueCheckerShader.attach("triangle");
	lightBlueCheckerShader.attach("color_light_blue");
	lightBlueCheckerShader.createExecutable();

	colorShader.load("./res/shaders/color.shader");
	colorShader.attach("triangle");
	colorShader.attach("color");
	colorShader.createExecutable();
}

void CG::EditorView::createCheckerBoard()
{
	// computing components for each plane size.
	float min = -static_cast<float>(m_Size) / 2;
	float max = static_cast<float>(m_Size) / 2;

	// computing sizes of each squares.
	float sideSize = (max - min) / static_cast<float>(m_Nsquare);

	float squareSize = sideSize * sideSize;

	CG_CONSOLE_INFO("number of squares: {} x {}", sideSize, sideSize);
	CG_CONSOLE_INFO("square size: {}", squareSize);

	// creating the floor.
	float xpos = min;
	for (int x = 0; x < m_Nsquare; ++x, xpos += sideSize) {
		float zpos = min;
		for (int z = 0; z < m_Nsquare; ++z, zpos += sideSize) {
			m_Squares.push_back(std::make_unique<Plane>(
				glm::vec3(xpos * sideSize, 0.f, zpos * sideSize),
				glm::vec3(90.f, 0.f, 0.f),
				glm::vec3(squareSize, 0.f, squareSize)
				));
		}
	}
}

void CG::EditorView::render(Renderer& renderer, GUI& gui)
{
	float currentFrame = static_cast<float>(glfwGetTime());

	// computing m_DeltaTime;
	m_DeltaTime = currentFrame - m_DeltaTime;
	m_LastFrame = currentFrame;

	m_Controller.update(m_DeltaTime);

	// cleaning the screen and starting new frame for imgui.
	renderer.pollEvents();
	gui.newFrame();
	renderer.clear();

	renderGUI();

	// updating the fps mode if mouse clicked.
	if (glfwGetMouseButton(renderer.window(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		m_FpsMode = !m_FpsMode;
		if (m_FpsMode)
			glfwSetInputMode(renderer.window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(renderer.window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	const auto& axes = m_Axes.axes();

	for (auto& axis : axes) {
		colorShader.setUniform("u_mvp", m_Controller.view() * axis->transform.model());
		renderer.drawLine(*axis, colorShader);
	}

	// Rendering the floor.
	for (int x = 0, it = 0; x < m_Nsquare; ++x, it += m_Nsquare) {
		for (int z = 0, i = x; z < m_Nsquare; ++z, ++i) {

			int idx = it + z;

			// rendering one of two plane with a different color.
			if (i % 2 == 1) {
				blueCheckerShader.setUniform("u_mvp", m_Controller.view() * m_Squares[idx]->transform.model());
				renderer.draw(*(m_Squares[idx]), blueCheckerShader);
			}
			else {
				lightBlueCheckerShader.setUniform("u_mvp", m_Controller.view() * m_Squares[idx]->transform.model());
				renderer.draw(*(m_Squares[idx]), lightBlueCheckerShader);
			}
		}
	}

	// draw all elements from the gui.
	gui.drawDebugUI();
	gui.renderGUI();
	renderer.swapBuffers();
}

void CG::EditorView::renderGUI()
{
	// camera controller imgui settings.
	ImGui::Begin("Camera Controller");
	// control over the projection matrix.
	if (ImGui::InputFloat("camera speed", &m_Controller.speed, 1))
		CG_CONSOLE_INFO("Controller speed set to {}", m_Controller.speed);
	if (ImGui::InputFloat("camera sensitivity speed", &m_Controller.sensitivity, 1))
		CG_CONSOLE_INFO("Controller sensitivity set to {}", m_Controller.speed);
	if (ImGui::Button("Help"))
		m_HelpOpened = !m_HelpOpened;

	if (m_HelpOpened) {
		ImGui::BeginChild("Tips");
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "How to use the controller.");
		ImGui::Text("Use the mouse to look around.");
		ImGui::Text("Use 'W' 'A' 'S' 'D' to move the camera.");
		ImGui::Text("Use 'Space' and 'Left Shift' to move the camera up or down.");
		ImGui::Text("Press mouse right click to enter fps mode and reclick to get out");
		ImGui::EndChild();	
	}
	ImGui::End();
}