#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "WorldObjects/Primitives/Mesh.hpp"

namespace CG {
	class Model
	{
	private:
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		std::vector<std::shared_ptr<Texture>> m_CachedTextures;
		std::string m_ModelPath;
		std::string m_DirectoryPath;

		Transform transform;

		void loadModel(const aiScene* scene, const aiNode* node);
		std::vector<std::shared_ptr<Texture>> loadMaterial(const aiMaterial* material, aiTextureType type, const std::string& typeName);
		void createMesh(const aiScene* scene, unsigned int meshIndex);
	public:
		Model(const std::string& modelPath, const glm::vec3& position = glm::vec3(0.f), const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f));
		~Model() = default;

		inline const std::vector<std::unique_ptr<Mesh>>& meshes() const { return m_Meshes; };

		void translate(float x, float y, float z);
		void rotate(float degree, float x, float y, float z);
		void scale(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setRotation(float anglex, float angley, float anglez, const glm::mat4& pivot);
		void setScale(float x, float y, float z);
		inline glm::mat4 model() const { return transform.model(); }
	};
}