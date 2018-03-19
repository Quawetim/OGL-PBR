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

    // Вершины меша
    std::vector<QVertexData> vertices;

    // Индексы вершин
    std::vector<unsigned int> indices;

    // Текстуры
    std::vector<QTexture> textures;

    // Цвет по-умолчанию
    glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'vertices'>Вершины меша.</param>
    ///<param name = 'indices'>Индексы вершин.</param>
    ///<param name = 'textures'>Текстуры.</param>
    Mesh(std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

    ///<summary>Отрисовка меша.</summary>
    ///<param name = 'shader'>Шейдер.</param>
    void drawMesh(Shader shader);

    ///<summary>Задаёт цвет меша в RGB формате.</summary>
    ///<para name = 'red'>Красная компонента цвета.</para>
    ///<para name = 'green'>Зелёная компонента цвета.</para>
    ///<para name = 'blue'>Синяя компонента цвета.</para>
    void setMeshColor(unsigned char red, unsigned char green, unsigned char blue);
};