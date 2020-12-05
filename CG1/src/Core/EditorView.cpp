#include "EditorView.hpp"

CG::EditorView::EditorView(int size, int nsquare, Renderer &renderer)
	: m_Size	   { size															 }
	, m_Nsquare    { nsquare														 }
	, m_Axes       { glm::vec3(0.f), glm::vec3(0.f), glm::vec3(20.f)				 }
	, m_Controller { renderer.window(), glm::vec3(-20.f, 20.f, 20.f), glm::vec3(0.f) }

	, m_AmbiantLightColor { glm::vec3(1.f) }
	, m_ObjectColor		  { glm::vec3(.2f) }
	, m_LightPos          { glm::vec3(5.f) }
{
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

	m_BlinnPhongShader.load("./res/shaders/phong-frag.shader");
	m_BlinnPhongShader.attach("triangle");
	m_BlinnPhongShader.attach("color");
	m_BlinnPhongShader.createExecutable();

	m_AxisShader.load("./res/shaders/color.shader");
	m_AxisShader.attach("triangle");
	m_AxisShader.attach("color");
	m_AxisShader.createExecutable();
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
	renderAxis(renderer);
	renderFloor(renderer);
	renderModels(renderer);

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
		CG_CONSOLE_INFO("Controller sensitivity set to {}", m_Controller.sensitivity);

	ImGui::Begin("Scene");
	ImGui::InputText("Model path", m_ModelPath, sizeof(m_ModelPath));
	ImGui::SameLine();
	if (ImGui::Button("Import"))
		importModel();
	ImGui::End();
}

void CG::EditorView::renderFloor(Renderer& renderer)
{
	// Rendering the floor.
	for (int x = 0, it = 0; x < m_Nsquare; ++x, it += m_Nsquare) {
		for (int z = 0, i = x; z < m_Nsquare; ++z, ++i) {

			int idx = it + z;

			// rendering one of two plane with a different color.
			if (i % 2 == 1) {
				m_BlueCheckerShader.setUniform("u_mvp", m_Controller.projectionView() * m_Squares[idx]->transform.model());
				renderer.draw(*(m_Squares[idx]), m_BlueCheckerShader);
			} else {
				m_LightBlueCheckerShader.setUniform("u_mvp", m_Controller.projectionView() * m_Squares[idx]->transform.model());
				renderer.draw(*(m_Squares[idx]), m_LightBlueCheckerShader);
			}
		}
	}
}

void CG::EditorView::renderAxis(Renderer& renderer)
{
	const auto& axes = m_Axes.axes();

	for (auto& axis : axes) {
		m_AxisShader.setUniform("u_mvp", m_Controller.projectionView() * axis->transform.model());
		renderer.drawLine(*axis, m_AxisShader);
	}
}

void CG::EditorView::renderModels(Renderer& renderer)
{
	for (auto& [_, model] : m_Models)
		for (auto& mesh : model->meshes()) {
			glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(m_Controller.view() * mesh->transform.model())));

			m_BlinnPhongShader.setUniform("u_mvp", m_Controller.projectionView() * mesh->transform.model());
			m_BlinnPhongShader.setUniform("u_view", m_Controller.view());
			m_BlinnPhongShader.setUniform("u_modelView", m_Controller.view() * mesh->transform.model());
			m_BlinnPhongShader.setUniform("u_normalMat", normalMat);

			renderer.draw(*mesh, m_BlinnPhongShader);
		}
}

void CG::EditorView::importModel()
{
	std::unique_ptr<Model> model = std::make_unique<Model>(m_ModelPath);

	if (!model->isInitialized()) {
		CG_CONSOLE_CRITICAL("Couldn't load the model.");
		return;
	}

	m_Models.emplace_back(
		m_ModelPath,
		std::move(model)
	);

	m_BlinnPhongShader.setUniform("u_ambiantLightColor", m_AmbiantLightColor);
	m_BlinnPhongShader.setUniform("u_objectColor", m_ObjectColor);
	m_BlinnPhongShader.setUniform("u_lightPos", m_LightPos);
}