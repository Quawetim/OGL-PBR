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

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'vertices'>Вершины меша.</param>
    ///<param name = 'indices'>Индексы вершин.</param>
    ///<param name = 'textures'>Текстуры.</param>
    Mesh(std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

    ///<summary>Отрисовка меша.</summary>
    ///<param name = 'shader'>Шейдер.</param>
    void drawMesh(Shader shader);

    ///<summary>Возвращает вершины меша.</summary>
    std::vector<QVertexData> getVertices();

    ///<summary>Возвращает индексы вершин меша.</summary>
    std::vector<unsigned int> getIndices();

    ///<summary>Возвращает текстуры меша.</summary>
    std::vector<QTexture> getTextures();
};