#pragma once
#include "..\includes\Includes.h"
#include "Mesh.h"

class Model
{
private:
    // Меши, из который состоит модель.
    std::vector<Mesh> meshes;

    // Уже загруженные в память текстуры
    std::vector<QTexture> loaded_textures;

    // Директория в которой лежит модель
    std::string dir;

    ///<summary>Обработка узла модели.</summary>
    ///<para name ='node'>Узел assimp.</para>
    ///<para name ='scene'>Сцена assimp.</para>
    void handleNode(aiNode *node, const aiScene *scene);

    ///<summary>Обработка меша модели.</summary>
    ///<para name ='mesh'>Меш assimp.</para>
    ///<para name ='scene'>Сцена assimp.</para>
    Mesh handleMesh(aiMesh *mesh, const aiScene *scene);

    ///<summary>Загрузка текстур модели.</summary>
    ///<para name ='material'>Материал assimp.</para>
    ///<para name ='type'>Тип текстуры assimp.</para>
    ///<para name ='textureType'>Тип текстуры в шейдере.</para>
    std::vector<QTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, QTextureType textureType);

public:
    ///<summary>Конструктор.</summary>
    ///<para name ='path'>Путь к модели.</para>
    Model(std::string path);

    ///<summary>Отрисовка модели.</summary>
    ///<para name ='shader'>Шейдер.</para>
    void drawModel(Shader shader);
};