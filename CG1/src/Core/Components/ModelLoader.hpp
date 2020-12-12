#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_filebrowser.hpp>
#include "Logger.hpp"

#include "WorldObjects/Complex/Model.hpp"

namespace CG
{
	class ModelLoader : public ImGui::FileBrowser
	{
	public:
		ModelLoader();
		~ModelLoader() = default;

		void render();
		void importModel();
		const std::vector<std::pair<std::string, std::unique_ptr<Model>>>& models() const { return m_Models; };
	private:
		std::vector<std::pair<std::string, std::unique_ptr<Model>>> m_Models;
	};
}