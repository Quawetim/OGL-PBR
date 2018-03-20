#include "Model.h"
#include "..\texture_loader\TextureLoader.h"

///<summary>�����������.</summary>
///<param name ='path'>���� � ������.</param>
Model::Model(std::string path)
{
    this->modelMatrix = glm::mat4(1.0f);
    this->translationMatrix = glm::mat4(1.0f);
    this->rotationMatrix = glm::mat4(1.0f);
    this->scaleMatrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
    this->rotationAngle = 0.0f;
    this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->scale = glm::vec3(1.0f);

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        logger.log("Model", QErrorType::error, std::string("Can't load model. " + std::string(importer.GetErrorString())));
        return;
    }

    int dot_pos = path.find_last_of('.');
    int last_slash_pos = path.find_last_of('/');

    this->name = path.substr(last_slash_pos + 1, dot_pos - last_slash_pos - 1);
    this->dir = path.substr(0, last_slash_pos);

    handleNode(scene->mRootNode, scene);
}

///<summary>��������� ���� ������.</summary>
///<param name ='node'>���� assimp.</param>
///<param name ='scene'>����� assimp.</param>
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
///<param name ='mesh'>��� assimp.</param>
///<param name ='scene'>����� assimp.</param>
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

///<summary>��������� ������.</summary>
///<param name ='shader'>������.</param>
void Model::drawModel(Shader shader)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].drawMesh(shader);
    }
}

///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
///<param name = 'velocityX'>�������� �� ��� x.</param>
///<param name = 'velocityY'>�������� �� ��� y.</param>
///<param name = 'velocityZ'>�������� �� ��� z.</param>
void Model::moveModel(const float velocityX, const float velocityY, const float velocityZ)
{
    this->position += glm::vec3(velocityX * deltaTime, velocityY * deltaTime, velocityZ * deltaTime);
    this->translationMatrix = glm::translate(this->position);
}

///<summary>������� ������ � �������� ���������.</summary>
///<param name = 'angle'>�������� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
void Model::rotateModel(const double angle, const glm::vec3 axis)
{
    this->rotationAngle += angle * deltaTime;
    this->rotationAxis = axis;

    if (this->rotationAngle == 360) this->rotationAngle = 0;
    else
    {
        if (this->rotationAngle > 360) this->rotationAngle -= 360;
    }

    this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>�������� ������ ������ � �������� ���������.</summary>
///<param name = 'scaleXYZ'>�������� ��������� ������� �� ���� ����.</param>
void Model::scaleModel(const float scaleXYZ)
{
    this->scale += glm::vec3(scaleXYZ * (float)deltaTime);

    if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Model::scaleModel", QErrorType::error, "Scale = 0");
    else
    {
        if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Model::scaleModel", QErrorType::warning, "Scale < 0");
    }

    this->scaleMatrix = glm::scale(this->scale);
}

///<summary>�������� ������ ������ � �������� ���������.</summary>
///<param name = 'scaleX'>�������� ��������� ������� �� X.</param>
///<param name = 'scaleY'>�������� ��������� ������� �� Y.</param>
///<param name = 'scaleZ'>�������� ��������� ������� �� Z.</param>
void Model::scaleModel(const float scaleX, const float scaleY, const float scaleZ)
{
    this->scale += glm::vec3(scaleX * deltaTime, scaleY * deltaTime, scaleZ * deltaTime);

    if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Model::scaleModel", QErrorType::error, "Scale = 0");
    else
    {
        if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Model::scaleModel", QErrorType::warning, "Scale < 0");
    }

    this->scaleMatrix = glm::scale(this->scale);
}

///<summary>�������� ������� ������.</summary>
///<param name ='material'>�������� assimp.</param>
///<param name ='type'>��� �������� assimp.</param>
///<param name ='textureType'>��� �������� � �������.</param>
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
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Model::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setAmbientColor(red, green, blue);
    }
}

///<summary>����� diffuse ���� ���� ����� ������ � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Model::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setDiffuseColor(red, green, blue);
    }
}

///<summary>����� specular ���� ���� ����� ������ � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Model::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setSpecularColor(red, green, blue);
    }
}

///<summary>����� ���� (�������) ����� ���� ����� ������.</summary>
///<param name = 'value'>��������.</param>
void Model::setShinePower(const float value)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setShinePower(value);
    }
}

///<summary>����� ���� ������������� �������� ���� name.</summary>
///<param name = 'mesh_name'>��� ����.</param>
///<param name = 'texture_type'>��� �������� �����.</param>
///<param name = 'use'>������������ �������� ��� ���.</param>
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
///<param name = 'type'>��� ��������.</param>
///<param name = 'use'>������������ �������� ��� ���.</param>
void Model::setTextureFlag(const QTextureType type, const bool use)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setTextureFlag(type, use);
    }
}

///<summary>����� ���� ����� �������� ��������.</summary>
///<param name = 'texture'>��������.</param>
void Model::setTestTexture(QTexture texture)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setTestTexture(texture);
    }
}

///<summary>����� ������� ������.</summary>
///<param name = 'position'>�������.</param>
void Model::setPosition(glm::vec3 position)
{
    this->position = position;
    this->translationMatrix = glm::translate(this->position);
}

///<summary>����� ������� ������.</summary>
///<param name = 'angle'>���� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
void Model::setRotation(const double angle, const glm::vec3 axis)
{
    this->rotationAngle = angle;
    this->rotationAxis = axis;

    if (this->rotationAngle == 360) this->rotationAngle = 0;
    else
    {
        if (this->rotationAngle > 360) this->rotationAngle -= 360;
    }

    this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>����� ������ ������ �� ���������.</summary>
///<param name = 'scale'>����������� �������.</param>
void Model::setScale(const glm::vec3 scale)
{
    this->scale = scale;

    if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Model::setScale", QErrorType::error, "Scale = 0");
    else
    {
        if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Model::setScale", QErrorType::warning, "Scale < 0");
    }

    this->scaleMatrix = glm::scale(this->scale);
}

///<summary>���������� ��� ������.</summary>
std::string Model::getName() const
{
    return this->name;
}

///<summary>���������� ������� ������.</summary>
glm::mat4 Model::getModelMatrix()
{
    this->modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    return this->modelMatrix;
}

///<summary>���������� ������� ������.</summary>
glm::vec3 Model::getPosition() const
{
    return this->position;
}