#include "Model.h"
#include "..\texture_loader\TextureLoader.h"

///<summary>Конструктор.</summary>
///<param name ='path'>Путь к модели.</param>
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

///<summary>Обработка узла модели.</summary>
///<param name ='node'>Узел assimp.</param>
///<param name ='scene'>Сцена assimp.</param>
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

///<summary>Обработка меша модели.</summary>
///<param name ='mesh'>Меш assimp.</param>
///<param name ='scene'>Сцена assimp.</param>
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

///<summary>Отрисовка модели.</summary>
///<param name ='shader'>Шейдер.</param>
void Model::drawModel(Shader shader)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].drawMesh(shader);
    }
}

///<summary>Двигает модель в направлении оси с заданной скоростью.</summary>
///<param name = 'velocityX'>Скорость по оси x.</param>
///<param name = 'velocityY'>Скорость по оси y.</param>
///<param name = 'velocityZ'>Скорость по оси z.</param>
void Model::moveModel(const float velocityX, const float velocityY, const float velocityZ)
{
    this->position += glm::vec3(velocityX * deltaTime, velocityY * deltaTime, velocityZ * deltaTime);
    this->translationMatrix = glm::translate(this->position);
}

///<summary>Вращает модель с заданной скоростью.</summary>
///<param name = 'angle'>Скорость поворота в градусах.</param>
///<param name = 'axis'>Ось вращения.</param>
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

///<summary>Изменяет размер модели с заданной скоростью.</summary>
///<param name = 'scaleXYZ'>Скорость изменения размера по всем осям.</param>
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

///<summary>Изменяет размер модели с заданной скоростью.</summary>
///<param name = 'scaleX'>Скорость изменения размера по X.</param>
///<param name = 'scaleY'>Скорость изменения размера по Y.</param>
///<param name = 'scaleZ'>Скорость изменения размера по Z.</param>
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

///<summary>Загрузка текстур модели.</summary>
///<param name ='material'>Материал assimp.</param>
///<param name ='type'>Тип текстуры assimp.</param>
///<param name ='textureType'>Тип текстуры в шейдере.</param>
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

///<summary>Задаёт ambient цвет всем мешам модели в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Model::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setAmbientColor(red, green, blue);
    }
}

///<summary>Задаёт diffuse цвет всем мешам модели в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Model::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setDiffuseColor(red, green, blue);
    }
}

///<summary>Задаёт specular цвет всем мешам модели в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Model::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setSpecularColor(red, green, blue);
    }
}

///<summary>Задаёт силу (яркость) блика всем мешам модели.</summary>
///<param name = 'value'>Значение.</param>
void Model::setShinePower(const float value)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setShinePower(value);
    }
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

    std::string msg = "Mesh " + mesh_name + " not found.";
    logger.log("setTextureFlag", QErrorType::warning, msg);
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
void Model::setTestTexture(QTexture texture)
{
    for (size_t i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].setTestTexture(texture);
    }
}

///<summary>Задаёт позицию модели.</summary>
///<param name = 'position'>Позиция.</param>
void Model::setPosition(glm::vec3 position)
{
    this->position = position;
    this->translationMatrix = glm::translate(this->position);
}

///<summary>Задаёт поворот модели.</summary>
///<param name = 'angle'>Угол поворота в градусах.</param>
///<param name = 'axis'>Ось поворота.</param>
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

///<summary>Задаёт размер модели от исходного.</summary>
///<param name = 'scale'>Коэффициент размера.</param>
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

///<summary>Возвращает имя модели.</summary>
std::string Model::getName() const
{
    return this->name;
}

///<summary>Возвращает матрицу модели.</summary>
glm::mat4 Model::getModelMatrix()
{
    this->modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    return this->modelMatrix;
}

///<summary>Возвращает позицию модели.</summary>
glm::vec3 Model::getPosition() const
{
    return this->position;
}