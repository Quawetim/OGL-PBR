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

    // ������� ����
    std::vector<QVertexData> vertices;

    // ������� ������
    std::vector<unsigned int> indices;

    // ��������
    std::vector<QTexture> textures;

public:
    ///<summary>�����������.</summary>
    ///<param name = 'vertices'>������� ����.</param>
    ///<param name = 'indices'>������� ������.</param>
    ///<param name = 'textures'>��������.</param>
    Mesh(std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

    ///<summary>��������� ����.</summary>
    ///<param name = 'shader'>������.</param>
    void drawMesh(Shader shader);

    ///<summary>���������� ������� ����.</summary>
    std::vector<QVertexData> getVertices();

    ///<summary>���������� ������� ������ ����.</summary>
    std::vector<unsigned int> getIndices();

    ///<summary>���������� �������� ����.</summary>
    std::vector<QTexture> getTextures();
};