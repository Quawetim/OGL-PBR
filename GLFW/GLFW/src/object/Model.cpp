#include "Model.h"
#include "..\texture_loader\TextureLoader.h"

///<summary>Конструктор.</summary>
Model::Model()
{
	this->translationMatrix_ = glm::mat4(1.0f);
	this->rotationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
	this->position_ = glm::vec3(0.0f);
	this->rotationAngle_ = 0.0f;
	this->rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scaleCoeffs_ = glm::vec3(1.0f);

	this->name_ = "not loaded";
	this->dir_ = "";
}

///<summary>Конструктор.</summary>
///<param name = 'path'>Путь к модели.</param>
Model::Model(std::string path)
{
    this->translationMatrix_ = glm::mat4(1.0f);
    this->rotationMatrix_ = glm::mat4(1.0f);
    this->scaleMatrix_ = glm::mat4(1.0f);
    this->position_ = glm::vec3(0.0f);
    this->rotationAngle_ = 0.0f;
    this->rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
    this->scaleCoeffs_ = glm::vec3(1.0f);

	std::string s = "resources/3dmodels/" + path;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(s, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
		std::string msg = "Can't load model. " + std::string(importer.GetErrorString());
        logger.log(__FUNCTION__, ErrorType::error, msg);
        return;
    }

    int dot_pos = s.find_last_of('.');
    int last_slash_pos = s.find_last_of('/');

    this->name_ = s.substr(last_slash_pos + 1, dot_pos - last_slash_pos - 1);
    this->dir_ = s.substr(0, last_slash_pos);

    handleNode(scene->mRootNode, scene);
}

///<summary>Обработка узла модели.</summary>
///<param name = 'node'>Узел assimp.</param>
///<param name = 'scene'>Сцена assimp.</param>
void Model::handleNode(const aiNode *node, const aiScene *scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes_.push_back(handleMesh(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        handleNode(node->mChildren[i], scene);
    }
}

///<summary>Обработка меша модели.</summary>
///<param name = 'mesh'>Меш assimp.</param>
///<param name = 'scene'>Сцена assimp.</param>
std::shared_ptr<Mesh> Model::handleMesh(const aiMesh *mesh, const aiScene *scene)
{
    std::string name;
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

	bool computeTB = true;

    name = mesh->mName.C_Str();

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData vertex;

        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
        {
            vertex.textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.textureCoords = glm::vec2(0.0f, 0.0f);
        }

        if (mesh->mTangents)
        {
            vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        }
        else
        {
			if (!computeTB) computeTB = true;
        }

        if (mesh->mBitangents)
        {
            vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
        else
        {
			if (!computeTB) computeTB = true;
        }

        vertices.push_back(vertex);
    }

	if (computeTB) computeTangentsBitangents(vertices);

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        if (face.mNumIndices > 0)
        {
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        else
        {
            std::string mesh_name = mesh->mName.C_Str();
            std::string msg = "Indices of not found. MESH:" + mesh_name;
            logger.log(__FUNCTION__, ErrorType::error, msg);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<std::shared_ptr<Texture>> albedoMap = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::albedo);
        textures.insert(textures.end(), albedoMap.begin(), albedoMap.end());

        std::vector<std::shared_ptr<Texture>> smoothnessMap = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::smoothness);
        textures.insert(textures.end(), smoothnessMap.begin(), smoothnessMap.end());

		std::vector<std::shared_ptr<Texture>> metallicMap = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::metallic);
		textures.insert(textures.end(), metallicMap.begin(), metallicMap.end());

        std::vector<std::shared_ptr<Texture>> normalMap = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::normal);
        textures.insert(textures.end(), normalMap.begin(), normalMap.end());

		std::vector<std::shared_ptr<Texture>> ambientOcclusionMap = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::ambientOcclusion);
		textures.insert(textures.end(), ambientOcclusionMap.begin(), ambientOcclusionMap.end());
    }

    return std::shared_ptr<Mesh>(new Mesh(name, vertices, indices, textures));
}

///<summary>Загрузка текстур модели.</summary>
///<param name = 'material'>Материал assimp.</param>
///<param name = 'type'>Тип текстуры assimp.</param>
///<param name = 'texture_type'>Тип текстуры в шейдере.</param>
std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const TextureType texture_type)
{
	std::vector<std::shared_ptr<Texture>> textures;

	for (size_t i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString s;
		bool skip_loading = false;

		material->GetTexture(type, i, &s);

		for (unsigned int j = 0; j < loadedTextures_.size(); j++)
		{
			if (std::strcmp(loadedTextures_[j]->getName().data(), s.C_Str()) == 0)
			{
				textures.push_back(loadedTextures_[j]);
				skip_loading = true;
				break;
			}
		}

		if (!skip_loading)
		{
			std::shared_ptr<Texture> texture(new Texture(std::string(dir_ + "/" + s.C_Str()), texture_type));
			texture->setName(s.C_Str());

			textures.push_back(texture);
			loadedTextures_.push_back(texture);
		}
	}

	return textures;
}

///<summary>Вычисляет касательную и бикасательную для каждой из вершин.</summary>
///<param name = 'vertices'>Вершины.</param>
void Model::computeTangentsBitangents(std::vector<VertexData> &vertices)
{
	float r;
	glm::vec3 vertexPos1, vertexPos2, vertexPos3;
	glm::vec3 edge1, edge2;

	glm::vec2 texCoords1, texCoords2, texCoords3;
	glm::vec2 deltaTexCoords1, deltaTexCoords2;

	glm::vec3 normal, tangent, bitangent;

	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		normal = vertices[i].normal;

		vertexPos1 = vertices[i].position;
		vertexPos2 = vertices[i + 1].position;
		vertexPos3 = vertices[i + 2].position;

		texCoords1 = vertices[i].textureCoords;
		texCoords2 = vertices[i + 1].textureCoords;
		texCoords3 = vertices[i + 2].textureCoords;

		edge1 = vertexPos2 - vertexPos1;
		edge2 = vertexPos3 - vertexPos1;

		deltaTexCoords1 = texCoords2 - texCoords1;
		deltaTexCoords2 = texCoords3 - texCoords1;

		r = 1.0f / (deltaTexCoords1.x * deltaTexCoords2.y - deltaTexCoords1.y * deltaTexCoords2.x);

		tangent = r * (edge1 * deltaTexCoords2.y - edge2 * deltaTexCoords1.y);
		tangent = glm::normalize(tangent);

		bitangent = r * (edge2 * deltaTexCoords1.x - edge1 * deltaTexCoords2.x);
		bitangent = glm::normalize(bitangent);	

		vertices[i].tangent = tangent;
		vertices[i + 1].tangent = tangent;
		vertices[i + 2].tangent = tangent;

		vertices[i].bitangent = bitangent;
		vertices[i + 1].bitangent = bitangent;
		vertices[i + 2].bitangent = bitangent;
	}
	
	/*
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].tangent = glm::normalize(vertices[i].tangent - vertices[i].normal * glm::dot(normal, vertices[i].tangent));

		if (glm::dot(glm::cross(vertices[i].normal, vertices[i].tangent), vertices[i].bitangent) < 0) vertices[i].tangent *= -1;
	}
	*/
}

///<summary>Возвращает меши, из которых состоит модель.</summary>
const std::vector<std::shared_ptr<Mesh>> Model::getMeshes() const
{
	return this->meshes_;
}

///<summary>
///<para>Возвращает меш по имени.</para>
///<para>Если не найден, возвращает первый меш.</para>
///</summary>
///<param name = 'name'>Имя меша.</param>
const std::shared_ptr<Mesh> Model::getMeshByName(std::string name) const
{
	for (size_t i = 0; i < this->meshes_.size(); i++)
	{
		if (meshes_[i]->name_ == name) return meshes_[i];
	}

	std::string msg = "Mesh name: " + name + " not found.";
	logger.log(__FUNCTION__, ErrorType::warning, msg);

	return meshes_[0];
}