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
    std::shared_ptr<Model> model = std::make_shared<Model>(currentModelPath);

    //for (auto &model : m_ModelCache)
    //    if (currentModelPath == model) {
    //        m_Models.emplace_back(
    //            currentModelPath,
    //            model
    //        );
    //    }

    m_Models.push_back(
        model
    );
    m_ModelCache.push_back(
        model
    );
}
