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

    ///<summary>Задаёт ambient цвет всем мешам модели в RGB формате.</summary>
    ///<para name = 'red'>Красная компонента цвета.</para>
    ///<para name = 'green'>Зелёная компонента цвета.</para>
    ///<para name = 'blue'>Синяя компонента цвета.</para>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт diffuse цвет всем мешам модели в RGB формате.</summary>
    ///<para name = 'red'>Красная компонента цвета.</para>
    ///<para name = 'green'>Зелёная компонента цвета.</para>
    ///<para name = 'blue'>Синяя компонента цвета.</para>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт specular цвет всем мешам модели в RGB формате.</summary>
    ///<para name = 'red'>Красная компонента цвета.</para>
    ///<para name = 'green'>Зелёная компонента цвета.</para>
    ///<para name = 'blue'>Синяя компонента цвета.</para>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт силу (яркость) блика всем мешам модели.</summary>
    ///<para name = 'value'>Значение.</para>
    void setShinePower(const float value);

    ///<summary>Задаёт флаг использования текстуры меша name.</summary>
    ///<para name = 'mesh_name'>Имя меша.</para>
    ///<para name = 'texture_type'>Тип текстуры флага.</para>
    ///<para name = 'use'>Использовать текстуру или нет.</para>
    void setTextureFlag(const std::string mesh_name, const QTextureType texture_type, const bool use);

    ///<summary>Задаёт флаг использования текстуры всех мешей модели.</summary>
    ///<para name = 'type'>Тип текстуры.</para>
    ///<para name = 'use'>Использовать текстуру или нет.</para>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>Задаёт всем мешам тестовую текстуру.</summary>
    ///<para name = 'texture'>Текстура.</para>
    void setTestTexture(QTexture texture);
};