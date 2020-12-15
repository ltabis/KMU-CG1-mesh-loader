#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "WorldObjects/Primitives/Mesh.hpp"

namespace CG {
	class Model
	{
	private:
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::vector<std::shared_ptr<Texture>> m_CachedTextures;
		std::string m_ModelName;
		std::string m_ModelPath;
		std::string m_DirectoryPath;

		Transform transform;

		void loadModel(const aiScene* scene, const aiNode* node);
		Material loadMaterial(const aiMaterial* material);
		std::vector<std::shared_ptr<Texture>> loadTexture(const aiMaterial* material, aiTextureType type, const std::string& typeName);
		void createMesh(const aiScene* scene, unsigned int meshIndex);
	public:
		Model(const std::string& modelPath, const glm::vec3& position = glm::vec3(0.f), const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f));
		Model(const Model& model);
		~Model() = default;

		inline const std::vector<std::shared_ptr<Mesh>>& meshes() const { return m_Meshes; };
		inline std::string name() const { return m_ModelName; };
		inline std::string path() const { return m_ModelPath; };
		inline std::string directory() const { return m_DirectoryPath; };

		void translate(float x, float y, float z);
		void rotate(float degree, float x, float y, float z);
		void scale(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setRotation(float anglex, float angley, float anglez, const glm::mat4& pivot);
		void setScale(float x, float y, float z);

		inline glm::vec3 position() const { return transform.position(); };
		inline glm::vec3 rotation() const { return transform.rotation(); };
		inline glm::vec3 scale() const { return transform.scale(); };

		inline glm::mat4 model() const { return transform.model(); }
	};
}