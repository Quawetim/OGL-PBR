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
    ///<param name ='node'>Узел assimp.</param>
    ///<param name ='scene'>Сцена assimp.</param>
    void handleNode(aiNode *node, const aiScene *scene);

    ///<summary>Обработка меша модели.</summary>
    ///<param name ='mesh'>Меш assimp.</param>
    ///<param name ='scene'>Сцена assimp.</param>
    Mesh handleMesh(aiMesh *mesh, const aiScene *scene);

    ///<summary>Загрузка текстур модели.</summary>
    ///<param name ='material'>Материал assimp.</param>
    ///<param name ='type'>Тип текстуры assimp.</param>
    ///<param name ='textureType'>Тип текстуры в шейдере.</param>
    std::vector<QTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, QTextureType textureType);

public:
    ///<summary>Конструктор.</summary>
    ///<param name ='path'>Путь к модели.</param>
    Model(std::string path);

    ///<summary>Отрисовка модели.</summary>
    ///<param name ='shader'>Шейдер.</param>
    void drawModel(Shader shader);

    ///<summary>Задаёт ambient цвет всем мешам модели в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт diffuse цвет всем мешам модели в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт specular цвет всем мешам модели в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт силу (яркость) блика всем мешам модели.</summary>
    ///<param name = 'value'>Значение.</param>
    void setShinePower(const float value);

    ///<summary>Задаёт флаг использования текстуры меша name.</summary>
    ///<param name = 'mesh_name'>Имя меша.</param>
    ///<param name = 'texture_type'>Тип текстуры флага.</param>
    ///<param name = 'use'>Использовать текстуру или нет.</param>
    void setTextureFlag(const std::string mesh_name, const QTextureType texture_type, const bool use);

    ///<summary>Задаёт флаг использования текстуры всех мешей модели.</summary>
    ///<param name = 'type'>Тип текстуры.</param>
    ///<param name = 'use'>Использовать текстуру или нет.</param>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>Задаёт всем мешам тестовую текстуру.</summary>
    ///<param name = 'texture'>Текстура.</param>
    void setTestTexture(QTexture texture);
};