#include "Model.h"
#include "..\texture_loader\TextureLoader.h"

///<summary>Конструктор.</summary>
///<param name = 'path'>Путь к модели.</param>
Model::Model(std::string path)
{
    this->translationMatrix = glm::mat4(1.0f);
    this->rotationMatrix = glm::mat4(1.0f);
    this->scaleMatrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
    this->rotationAngle = 0.0f;
    this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->scaleCoefficients = glm::vec3(1.0f);

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
		std::string msg = "Can't load model. " + std::string(importer.GetErrorString());
        logger.log("Model::Model", QErrorType::error, msg);
        return;
    }

    int dot_pos = path.find_last_of('.');
    int last_slash_pos = path.find_last_of('/');

    this->name = path.substr(last_slash_pos + 1, dot_pos - last_slash_pos - 1);
    this->dir = path.substr(0, last_slash_pos);

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
        this->meshes.push_back(handleMesh(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        handleNode(node->mChildren[i], scene);
    }
}

///<summary>Обработка меша модели.</summary>
///<param name = 'mesh'>Меш assimp.</param>
///<param name = 'scene'>Сцена assimp.</param>
Mesh Model::handleMesh(const aiMesh *mesh, const aiScene *scene)
{
    std::string name;
    std::vector<QVertexData> vertices;
    std::vector<unsigned int> indices;
    std::vector<QTexture> textures;

    name = mesh->mName.C_Str();

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        QVertexData vertex;

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
            vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        if (mesh->mBitangents)
        {
            vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
        else
        {
            vertex.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

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
            logger.log("Model::handleMesh", QErrorType::error, msg);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<QTexture> diffuseMap = loadMaterialTextures(material, aiTextureType_DIFFUSE, QTextureType::diffuse);
        textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());

        std::vector<QTexture> specularMap = loadMaterialTextures(material, aiTextureType_SPECULAR, QTextureType::specular);
        textures.insert(textures.end(), specularMap.begin(), specularMap.end());

        std::vector<QTexture> normalMap = loadMaterialTextures(material, aiTextureType_NORMALS, QTextureType::normal);
        textures.insert(textures.end(), normalMap.begin(), normalMap.end());
    }

    return Mesh(name, vertices, indices, textures);
}

///<summary>Загрузка текстур модели.</summary>
///<param name = 'material'>Материал assimp.</param>
///<param name = 'type'>Тип текстуры assimp.</param>
///<param name = 'textureType'>Тип текстуры в шейдере.</param>
std::vector<QTexture> Model::loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const QTextureType textureType)
{
	std::vector<QTexture> textures;

	for (size_t i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString s;
		bool skip_loading = false;

		material->GetTexture(type, i, &s);

		for (unsigned int j = 0; j < loaded_textures.size(); j++)
		{
			if (std::strcmp(loaded_textures[j].path.data(), s.C_Str()) == 0)
			{
				textures.push_back(loaded_textures[j]);
				skip_loading = true;
				break;
			}
		}

		if (!skip_loading)
		{
			QTexture texture;

			texture.id = TextureLoader::loadTexture(std::string(dir + "/" + s.C_Str()));
			texture.type = textureType;
			texture.path = s.C_Str();

			textures.push_back(texture);
			loaded_textures.push_back(texture);
		}
	}

	return textures;
}

///<summary>Отрисовка объекта.
///<para>Если задан флаг "model_colors_flag", то все меши рисуются с цветом модели, </para>
///<para>иначе - с заданным цветом меша.</para>
///</summary>
///<param name = 'shader'>Шейдер.</param>
void Model::draw(const Shader shader)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
		if (model_colors_flag) this->meshes[i].draw(shader, this->ambientColor, this->diffuseColor, this->specularColor, this->shinePower);
        else this->meshes[i].draw(shader);
    }
}

///<summary>Отрисовка модели с заданными цветами.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'ambientColor'>Ambient цвет.</param>
///<param name = 'diffuseColor'>Diffuse цвет.</param>
///<param name = 'specularColor'>Specular цвет.</param>
///<param name = 'shinePower'>Сила (яркость) блика.</param>
void Model::draw(const Shader shader, const glm::vec3 ambientColor, const glm::vec3 diffuseColor, const glm::vec3 specularColor, const float shinePower)
{
	for (size_t i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].draw(shader, ambientColor, diffuseColor, specularColor, shinePower);
	}
}

///<summary>Задаёт флаг использования цвета этой модели для всех мешей, 
///<para>принадлежащих модели.</para>
///<para>Приоритет ниже флага текстур и ниже флага объекта.</para>
///</summary>
void Model::useModelColors()
{
	this->model_colors_flag = true;
}

///<summary>Задаёт флаг использования текстуры меша name.</summary>
///<param name = 'mesh_name'>Имя меша.</param>
///<param name = 'texture_type'>Тип текстуры флага.</param>
///<param name = 'use'>Использовать текстуру или нет.</param>
void Model::setTextureFlag(const std::string mesh_name, const QTextureType texture_type, const bool use)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        if (this->meshes[i].getName() == mesh_name)
        {
            this->meshes[i].setTextureFlag(texture_type, use);
            return;
        }
    }

    std::string msg = "Mesh \"" + mesh_name + "\" not found.";
    logger.log("Model::setTextureFlag", QErrorType::warning, msg);
}

///<summary>Задаёт флаг использования текстуры всех мешей модели.</summary>
///<param name = 'type'>Тип текстуры.</param>
///<param name = 'use'>Использовать текстуру или нет.</param>
void Model::setTextureFlag(const QTextureType type, const bool use)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setTextureFlag(type, use);
    }
}

///<summary>Задаёт всем мешам тестовую текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void Model::setTestTexture(const QTexture texture)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setTestTexture(texture);
    }
}

///<summary>Возвращает флаг использования цвета этой модели для всех мешей, 
///<para>принадлежащих модели.</para>
///</summary>
bool Model::getModelColorsFlag() const
{
	return this->model_colors_flag;
}