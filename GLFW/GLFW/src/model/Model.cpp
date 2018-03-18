#include "Model.h"
#include "..\texture_loader\TextureLoader.h"

extern Logger logger;

///<summary>Конструктор.</summary>
///<para name ='path'>Путь к модели.</para>
Model::Model(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        logger.log("Model", QErrorType::error, std::string("Can't load model. " + std::string(importer.GetErrorString())));
        return;
    }

    dir = path.substr(0, path.find_last_of('/'));

    handleNode(scene->mRootNode, scene);
}

///<summary>Отрисовка модели.</summary>
///<para name ='shader'>Шейдер.</para>
void Model::drawModel(Shader shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].drawMesh(shader);
    }
}

///<summary>Обработка узла модели.</summary>
///<para name ='node'>Узел assimp.</para>
///<para name ='scene'>Сцена assimp.</para>
void Model::handleNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(handleMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        handleNode(node->mChildren[i], scene);
    }
}

///<summary>Обработка меша модели.</summary>
///<para name ='mesh'>Меш assimp.</para>
///<para name ='scene'>Сцена assimp.</para>
Mesh Model::handleMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<QVertexData> vertices;
    std::vector<unsigned int> indices;
    std::vector<QTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
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

        if (mesh->mBitangents)
        {
            vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
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

    return Mesh(vertices, indices, textures);
}

///<summary>Загрузка текстур модели.</summary>
///<para name ='material'>Материал assimp.</para>
///<para name ='type'>Тип текстуры assimp.</para>
///<para name ='textureType'>Тип текстуры в шейдере.</para>
std::vector<QTexture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, QTextureType textureType)
{
    std::vector<QTexture> textures;
    TextureLoader texture_loader;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString s;
        bool skip = false;

        material->GetTexture(type, i, &s);

        for (unsigned int j = 0; j < loaded_textures.size(); j++)
        {
            if (std::strcmp(loaded_textures[j].path.data(), s.C_Str()) == 0)
            {
                textures.push_back(loaded_textures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            QTexture texture;
  
            texture.id = texture_loader.loadTexture(std::string(dir + "/" + s.C_Str()));
            texture.type = textureType;
            texture.path = s.C_Str();

            textures.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }

    return textures;
}