#include "ModelLoader.hpp"

CG::ModelLoader::ModelLoader()
	: ImGui::FileBrowser {}
{
	SetTitle("Import model");
	SetTypeFilters({ ".obj" });
}

void CG::ModelLoader::render()
{
    Display();

    if (HasSelected()) {
        importModel();
        ClearSelected();
    }
}

void CG::ModelLoader::importModel()
{
    std::string currentModelPath = GetSelected().string();
    Model* model = new Model(currentModelPath);

    m_Models.emplace_back(
        currentModelPath,
        std::move(model)
    );
}
