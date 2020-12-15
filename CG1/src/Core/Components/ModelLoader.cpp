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

    for (auto &model : m_ModelCache)
        if (currentModelPath == model->path()) {
            m_Models.push_back(std::make_shared<Model>(*model));
            return;
        }

    std::shared_ptr<Model> newModel = std::make_shared<Model>(currentModelPath);

    m_Models.push_back(newModel);
    m_ModelCache.push_back(newModel);
}
