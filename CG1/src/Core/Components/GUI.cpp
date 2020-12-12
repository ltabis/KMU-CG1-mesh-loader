// GUI.cpp
// GUI class implementation

#include "GUI.hpp"

// TODO: add this tooltip to elements.
//static void HelpMarker(const char* desc)
//{
//	ImGui::TextDisabled("(?)");
//	if (ImGui::IsItemHovered())
//	{
//		ImGui::BeginTooltip();
//		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
//		ImGui::TextUnformatted(desc);
//		ImGui::PopTextWrapPos();
//		ImGui::EndTooltip();
//	}
//}

/* initilializing Imgui and setting a theme. */
CG::GUI::GUI(GLFWwindow* window, CG::GUI::Style style)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	if (style == Style::CLASSIC)
		ImGui::StyleColorsClassic();
	else
		ImGui::StyleColorsDark();
}

CG::GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

/* creating a new frame for the gui. */
void CG::GUI::newFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

/* draw elements on screen. */
/* each window / widgets should be sepparated into functions. */
void CG::GUI::drawUI()
{
	drawDebugUI();
}

void CG::GUI::drawDebugUI()
{
	ImGui::Begin("Tools");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Separator();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Logs");
	ImGui::SameLine();
	if (ImGui::Button("Clear")) CG::Logger::clearConsoleLogs();
	ImGui::BeginChild("scrolling");
	ImGui::TextUnformatted(CG::Logger::buffer().c_str());
	ImGui::EndChild();
	ImGui::End();
}

/* renders the ui. */
void CG::GUI::renderGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CG::GUI::clearLogs()
{
	CG::Logger::clearConsoleLogs();
}