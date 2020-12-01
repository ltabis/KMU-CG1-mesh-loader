#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "WorldObjects/Primitives/Mesh.hpp"

namespace CG {
	class Model
	{
	private:
		std::vector<std::unique_ptr<AShape>> m_Meshes;
		std::string m_ModelPath;
		bool m_isInitialized;

		void loadModel(const aiScene* scene, aiNode* node);
		void createMesh(const aiScene* scene, unsigned int meshIndex);
	public:
		Model(const std::string& modelPath, const glm::vec3& position = glm::vec3(0.f), const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f));
		~Model();

		bool isInitialized() const { return m_isInitialized; };
		inline const std::vector<std::unique_ptr<AShape>>& meshes() const { return m_Meshes; };

		Transform transform;
	};
}