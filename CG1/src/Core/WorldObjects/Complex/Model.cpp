#include "Model.hpp"

CG::Model::Model(const std::string &modelPath, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	: m_ModelPath { modelPath							 }
	, transform   { Transform(position, rotation, scale) }
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(modelPath, aiProcessPreset_TargetRealtime_Quality);

    // TODO: add more details to the error.
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        CG_CONSOLE_ERROR("Couldn't load '{}' model: {}", modelPath, importer.GetErrorString());
        return;
    }

    // saving the directory to load additional resources.
    auto lastPathIndex = modelPath.find_last_of('\\');
    lastPathIndex = lastPathIndex == std::string::npos ? 0 : lastPathIndex + 1;

    m_DirectoryPath = modelPath.substr(0, lastPathIndex) + '\\';
    m_ModelName = modelPath.substr(lastPathIndex, modelPath.find_last_of('.') - lastPathIndex);

    m_Meshes.reserve(scene->mNumMeshes);
    loadModel(scene, scene->mRootNode);
}

CG::Model::Model(const Model& model)
    : m_CachedTextures { model.m_CachedTextures        }
    , m_ModelName      { model.m_ModelName + " - Copy" }
    , m_ModelPath      { model.m_ModelPath             }
    , m_DirectoryPath  { model.m_DirectoryPath         }
    , transform        {                               }
{
    for (auto& mesh : model.meshes())
        m_Meshes.push_back(std::make_shared<Mesh>(*mesh));
}

void CG::Model::translate(float x, float y, float z)
{
    transform.translate(x, y, z);

    for (auto& mesh : m_Meshes)
        mesh->transform.translate(x, y, z);
}

void CG::Model::rotate(float degree, float x, float y, float z)
{
    transform.rotate(degree, x, y, z);

    for (auto& mesh : m_Meshes)
        mesh->transform.rotate(degree, x, y, z);
}

void CG::Model::scale(float x, float y, float z)
{
    transform.scale(x, y, z);

    for (auto& mesh : m_Meshes)
        mesh->transform.scale(x, y, z);
}

void CG::Model::setPosition(float x, float y, float z)
{
    transform.setPosition(x, y, z);

    for (auto& mesh : m_Meshes)
        mesh->transform.setPosition(x, y, z);
}

void CG::Model::setRotation(float anglex, float angley, float anglez, const glm::mat4& pivot)
{
    transform.setRotation(anglex, angley, anglez, pivot);

    for (auto& mesh : m_Meshes)
        mesh->transform.setRotation(anglex, angley, anglez, pivot);
}

void CG::Model::setScale(float x, float y, float z)
{
    transform.setScale(x, y, z);

    for (auto& mesh : m_Meshes)
        mesh->transform.setScale(x, y, z);
}

void CG::Model::loadModel(const aiScene* scene, const aiNode* node)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        createMesh(scene, node->mMeshes[i]);

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
        loadModel(scene, node->mChildren[i]);
}

CG::Material CG::Model::loadMaterial(const aiMaterial* assimpMaterial)
{
    Material newMaterial;
    aiColor3D color = { 0, 0, 0 };

    // retreiving material data.
    if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        newMaterial.diffuseColor = glm::vec3(color.r, color.g, color.b);
    if (assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        newMaterial.specularColor = glm::vec3(color.r, color.g, color.b);
    if (assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        newMaterial.ambiantColor = glm::vec3(color.r, color.g, color.b);
    
    assimpMaterial->Get(AI_MATKEY_OPACITY, newMaterial.opacity);
    assimpMaterial->Get(AI_MATKEY_SHININESS, newMaterial.shininess);

    return newMaterial;
}

std::vector<std::shared_ptr<CG::Texture>> CG::Model::loadTexture(const aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    aiString path;
    bool skipTextureLoading;
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
        material->GetTexture(type, i, &path);
        skipTextureLoading = false;

        // checking if the texture haven't been already loaded beforehand.
        for (auto &cachedTexture : m_CachedTextures)
            if (!std::strcmp(path.C_Str(), cachedTexture->path().c_str())) {
                textures.push_back(cachedTexture);
                skipTextureLoading = true;
                break;
            }

        // loading and caching a new texture.
        if (!skipTextureLoading) {
            std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(m_DirectoryPath + path.C_Str(), typeName);
            textures.push_back(newTexture);
            m_CachedTextures.push_back(newTexture);
        }
    }

    return textures;
}

void CG::Model::createMesh(const aiScene* scene, unsigned int meshIndex)
{
    const aiMesh* mesh = scene->mMeshes[meshIndex];
    std::vector<Vertex> vertices;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<unsigned int> indices;
    Material material;

    // reserving vectors allows us to prevent vectors doubling in size.
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    // loading vertices.
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

        glm::vec3 vertex = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        glm::vec3 normal = glm::vec3(0.f);
        glm::vec2 textureCoords = glm::vec2(0.f);

        if (mesh->HasNormals())
            normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        if (mesh->HasTextureCoords(0))
            textureCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

        vertices.push_back({ vertex, normal, textureCoords });
    }

    // loading indecies.
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // loading textures.
    if (mesh->mMaterialIndex >= 0) {

        aiMaterial* assimpMaterial = scene->mMaterials[mesh->mMaterialIndex];

        auto diffuse = loadTexture(assimpMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
        auto specular = loadTexture(assimpMaterial, aiTextureType_SPECULAR, "texture_specular");

        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
        textures.insert(textures.end(), specular.begin(), specular.end());

        material = loadMaterial(assimpMaterial);
    }

    // creating the meshes using all data gathered for this iteration.
    std::shared_ptr<Mesh> meshObject = std::make_shared<Mesh>(
        vertices, indices, textures, material,
        transform.position(), transform.rotation(), transform.scale()
    );

    // pushing the mesh into our data collection.
    m_Meshes.push_back(meshObject);
}