#include "EditorView.hpp"

CG::EditorView::EditorView(int size, int nsquare, Renderer* m_Renderer)
	: m_Size	       { size															    }
	, m_Nsquare        { nsquare														    }
	, m_Axes           { glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)						}
	, m_ObjectSelected { false																}
	, m_Controller     { m_Renderer->window(), glm::vec3(-20.f, 20.f, 20.f), glm::vec3(0.f) }

	, m_Renderer   { std::move(m_Renderer) }
{

	// assigning callbacks for the renderer.
	m_Renderer->registerKeyBindingCallback(GLFW_KEY_ESCAPE, escape_callback);

	// creating the checkboard data.
	createCheckerBoard();

	// setting the speed of the controller.
	m_Controller.speed = 0.1f;

	// laoding shaders.
	m_BlueCheckerShader.load("./res/shaders/checker.shader");
	m_BlueCheckerShader.attach("triangle");
	m_BlueCheckerShader.attach("color_blue");
	m_BlueCheckerShader.createExecutable();

	m_LightBlueCheckerShader.load("./res/shaders/checker.shader");
	m_LightBlueCheckerShader.attach("triangle");
	m_LightBlueCheckerShader.attach("color_light_blue");
	m_LightBlueCheckerShader.createExecutable();

	m_ModelShader.load("./res/shaders/phong-frag-texture.shader");
	m_ModelShader.attach("triangle");
	m_ModelShader.attach("color");
	m_ModelShader.createExecutable();

	m_AxisShader.load("./res/shaders/color.shader");
	m_AxisShader.attach("triangle");
	m_AxisShader.attach("color");
	m_AxisShader.createExecutable();

	// setting ImGui dock config flags.
	m_WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
	m_WindowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	// TODO: delete this.
	m_Lights.push_back(std::make_unique<PointLight>(
		
	));
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

void CG::EditorView::start(CG::GUI &gui)
{
	while (!m_Renderer->windowShouldClose())
		render(gui);
}

void CG::EditorView::render(GUI& gui)
{
	float currentFrame = static_cast<float>(glfwGetTime());

	// computing m_DeltaTime;
	m_DeltaTime = currentFrame - m_DeltaTime;
	m_LastFrame = currentFrame;

	m_Controller.update(m_DeltaTime);

	// cleaning the screen and starting new frame for imgui.
	m_Renderer->pollEvents();
	gui.newFrame();
	m_Renderer->clear();

	renderGUI();
	renderAxis();
	renderFloor();

	uploadLights();
	renderModels();

	// draw all elements from the gui.
	gui.drawDebugUI();
	gui.renderGUI();
	m_Renderer->swapBuffers();
}

void CG::EditorView::renderGUI()
{
	renderGuiDockSpace();
	renderGuiEnvironment();
	renderGuiHierarchy();
	m_ModelLoader.render();
}

void CG::EditorView::renderFloor()
{
	// Rendering the floor.
	for (int x = 0, it = 0; x < m_Nsquare; ++x, it += m_Nsquare) {
		for (int z = 0, i = x; z < m_Nsquare; ++z, ++i) {

			int idx = it + z;

			// rendering one of two plane with a different color.
			if (i % 2 == 1) {
				m_BlueCheckerShader.setUniform("u_mvp", m_Controller.projectionView() * m_Squares[idx]->transform.model());
				m_Renderer->draw(*(m_Squares[idx]), m_BlueCheckerShader);
			} else {
				m_LightBlueCheckerShader.setUniform("u_mvp", m_Controller.projectionView() * m_Squares[idx]->transform.model());
				m_Renderer->draw(*(m_Squares[idx]), m_LightBlueCheckerShader);
			}
		}
	}
}

void CG::EditorView::renderAxis()
{
	const auto& axes = m_Axes.axes();

	for (auto& axis : axes) {
		m_AxisShader.setUniform("u_mvp", m_Controller.projectionView() * axis->transform.model());
		m_Renderer->drawLine(*axis, m_AxisShader);
	}
}

void CG::EditorView::renderModels()
{
	// rendering all models.
	for (auto& model : m_ModelLoader.models())

		// rendering all meshes.
		for (auto& mesh : model->meshes()) {

			unsigned int slot = 0;

			// textures.
			for (auto& texture : mesh->textures()) {
				slot = texture->slot();
				texture->bind();
				std::string uniform = "u_" + texture->type() + std::to_string(slot + 1);
				m_ModelShader.setUniform(uniform, static_cast<int>(slot));
			}

			// maths.
			glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(m_Controller.view() * mesh->transform.model())));

			m_ModelShader.setUniform("u_mvp", m_Controller.projectionView() * mesh->transform.model());
			m_ModelShader.setUniform("u_view", m_Controller.view());
			m_ModelShader.setUniform("u_modelView", m_Controller.view() * mesh->transform.model());
			m_ModelShader.setUniform("u_normalMat", normalMat);

			// material.
			m_ModelShader.setUniform("u_material.AmbiantColor", mesh->material.ambiantColor);
			m_ModelShader.setUniform("u_material.DiffuseColor", mesh->material.diffuseColor);
			m_ModelShader.setUniform("u_material.SpecularColor", mesh->material.specularColor);
			m_ModelShader.setUniform("u_material.shininess", mesh->material.shininess);
			m_ModelShader.setUniform("u_material.opacity", mesh->material.opacity);

			m_Renderer->draw(*mesh, m_ModelShader);
		}
}

void CG::EditorView::uploadLights()
{
	// setting all lights data.
	for (unsigned int i = 0; i < m_Lights.size(); ++i) {
		std::string uniformName = "u_lights[" + std::to_string(i) + "]";

		m_ModelShader.setUniform(uniformName + ".AmbiantColor", m_Lights[i]->ambiantColor);
		m_ModelShader.setUniform(uniformName + ".DiffuseColor", m_Lights[i]->diffuseColor);
		m_ModelShader.setUniform(uniformName + ".SpecularColor", m_Lights[i]->specularColor);
		m_ModelShader.setUniform(uniformName + ".Position", glm::vec4(m_Lights[i]->transform.position(), 1.f));
		m_ModelShader.setUniform(uniformName + ".Intensity", m_Lights[i]->intensity);
	}
}

void CG::EditorView::renderGuiDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Dockspace", nullptr, m_WindowFlags);
	ImGuiID dockspace_id = ImGui::GetID("Dockspace");
	ImGui::PopStyleVar(3);
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	// rendering additional viewport widjets.
	renderGuiMenuBar();

	ImGui::End();
}

void CG::EditorView::renderGuiMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Import model", NULL))
				m_ModelLoader.Open();
			ImGui::Separator();
			if (ImGui::MenuItem("Close", NULL))
				glfwSetWindowShouldClose(m_Renderer->window(), GLFW_TRUE);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void CG::EditorView::renderGuiEnvironment()
{
	// camera controller imgui settings.
	ImGui::Begin("Camera Controller");
	// control over the projection matrix.
	if (ImGui::InputFloat("camera speed", &m_Controller.speed, 1))
		CG_CONSOLE_INFO("Controller speed set to {}", m_Controller.speed);
	if (ImGui::InputFloat("camera sensitivity", &m_Controller.sensitivity, 1))
		CG_CONSOLE_INFO("Controller sensitivity set to {}", m_Controller.sensitivity);
	ImGui::End();
}

void CG::EditorView::renderGuiHierarchy()
{
	auto& models = m_ModelLoader.models();

	ImGui::Begin("Scene");
	for (unsigned int i = 0; i < models.size(); ++i) {

		bool isModelSelected = m_ObjectSelected && m_SelectedModel == i ? true : false;
		if (ImGui::Selectable(models[i]->name().c_str(), &isModelSelected)) {
			glm::vec3 position = models[i]->position();
			m_Axes.setPosition(position.x, position.y, position.z);
			m_ObjectSelected = true;
			m_SelectedModel = i;
		}
	}
	ImGui::End();
}
