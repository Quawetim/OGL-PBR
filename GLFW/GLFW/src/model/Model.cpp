#include "Model.h"
#include "..\texture_loader\TextureLoader.h"

///<summary>�����������.</summary>
///<para name ='path'>���� � ������.</para>
Model::Model(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        logger.log("Model", QErrorType::error, std::string("Can't load model. " + std::string(importer.GetErrorString())));
        return;
    }

    this->dir = path.substr(0, path.find_last_of('/'));

    handleNode(scene->mRootNode, scene);
}

///<summary>��������� ������.</summary>
///<para name ='shader'>������.</para>
void Model::drawModel(Shader shader)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].drawMesh(shader);
    }
}

///<summary>��������� ���� ������.</summary>
///<para name ='node'>���� assimp.</para>
///<para name ='scene'>����� assimp.</para>
void Model::handleNode(aiNode *node, const aiScene *scene)
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

///<summary>��������� ���� ������.</summary>
///<para name ='mesh'>��� assimp.</para>
///<para name ='scene'>����� assimp.</para>
Mesh Model::handleMesh(aiMesh *mesh, const aiScene *scene)
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
            logger.log("handleMesh", QErrorType::error, msg);
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

///<summary>�������� ������� ������.</summary>
///<para name ='material'>�������� assimp.</para>
///<para name ='type'>��� �������� assimp.</para>
///<para name ='textureType'>��� �������� � �������.</para>
std::vector<QTexture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, QTextureType textureType)
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
  
            texture.id = QTextureLoader::loadTexture(std::string(dir + "/" + s.C_Str()));
            texture.type = textureType;
            texture.path = s.C_Str();

            textures.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }

    return textures;
}

///<summary>����� ambient ���� ���� ����� ������ � RGB �������.</summary>
///<para name = 'red'>������� ���������� �����.</para>
///<para name = 'green'>������ ���������� �����.</para>
///<para name = 'blue'>����� ���������� �����.</para>
void Model::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setAmbientColor(red, green, blue);
    }
}

///<summary>����� diffuse ���� ���� ����� ������ � RGB �������.</summary>
///<para name = 'red'>������� ���������� �����.</para>
///<para name = 'green'>������ ���������� �����.</para>
///<para name = 'blue'>����� ���������� �����.</para>
void Model::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setDiffuseColor(red, green, blue);
    }
}

///<summary>����� specular ���� ���� ����� ������ � RGB �������.</summary>
///<para name = 'red'>������� ���������� �����.</para>
///<para name = 'green'>������ ���������� �����.</para>
///<para name = 'blue'>����� ���������� �����.</para>
void Model::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setSpecularColor(red, green, blue);
    }
}

///<summary>����� ���� (�������) ����� ���� ����� ������.</summary>
///<para name = 'value'>��������.</para>
void Model::setShinePower(const float value)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setShinePower(value);
    }
}

///<summary>����� ���� ������������� �������� ���� name.</summary>
///<para name = 'mesh_name'>��� ����.</para>
///<para name = 'texture_type'>��� �������� �����.</para>
///<para name = 'use'>������������ �������� ��� ���.</para>
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

    std::string msg = "Mesh " + mesh_name + " not found.";
    logger.log("setTextureFlag", QErrorType::warning, msg);
}

///<summary>����� ���� ������������� �������� ���� ����� ������.</summary>
///<para name = 'type'>��� ��������.</para>
///<para name = 'use'>������������ �������� ��� ���.</para>
void Model::setTextureFlag(const QTextureType type, const bool use)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setTextureFlag(type, use);
    }
}

///<summary>����� ���� ����� �������� ��������.</summary>
///<para name = 'texture'>��������.</para>
void Model::setTestTexture(QTexture texture)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setTestTexture(texture);
    }
}