#pragma once
#include "..\includes\Includes.h"

class Mesh
{
private:
    // Vertex array object
    unsigned int VAO;

    // Vertex buffer object
    unsigned int VBO;

    // Element buffer object
    unsigned int EBO;

    // Имя меша
    std::string name;

    // Вершины меша
    std::vector<QVertexData> vertices;

    // Индексы вершин
    std::vector<unsigned int> indices;

    // Текстуры
    std::vector<QTexture> textures;

    bool diffuseMap_flag = true, specularMap_flag = true, normalMap_flag = true;

    // Ambient цвет
    glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);

    // Diffuse цвет
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);

    // Specular цвет
    glm::vec3 specularColor = glm::vec3(0.9f, 0.9f, 0.9f);

    // Сила (яркость) блика
    float shinePower = 8.0f;

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'name'>Имя меша.</param>
    ///<param name = 'vertices'>Вершины меша.</param>
    ///<param name = 'indices'>Индексы вершин.</param>
    ///<param name = 'textures'>Текстуры.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

    ///<summary>Отрисовка меша.</summary>
    ///<param name = 'shader'>Шейдер.</param>
    void drawMesh(Shader shader);

    ///<summary>Задаёт ambient цвет меша в RGB формате.</summary>
    ///<para name = 'red'>Красная компонента цвета.</para>
    ///<para name = 'green'>Зелёная компонента цвета.</para>
    ///<para name = 'blue'>Синяя компонента цвета.</para>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
    ///<para name = 'red'>Красная компонента цвета.</para>
    ///<para name = 'green'>Зелёная компонента цвета.</para>
    ///<para name = 'blue'>Синяя компонента цвета.</para>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт specular цвет меша в RGB формате.</summary>
    ///<para name = 'red'>Красная компонента цвета.</para>
    ///<para name = 'green'>Зелёная компонента цвета.</para>
    ///<para name = 'blue'>Синяя компонента цвета.</para>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт силу (яркость) блика.</summary>
    ///<para name = 'value'>Значение.</para>
    void setShinePower(const float value);

    ///<summary>Задаёт флаг использования текстуры меша.</summary>
    ///<para name = 'type'>Тип текстуры.</para>
    ///<para name = 'use'>Использовать текстуру или нет.</para>
    void Mesh::setTextureFlag(const QTextureType type, const bool use);

    ///<summary>Задаёт мешу тестовую текстуру.</summary>
    ///<para name = 'texture'>Текстура.</para>
    void setTestTexture(QTexture texture);

    ///<summary>Возвращает имя меша.</summary>
    std::string getName() const;
};