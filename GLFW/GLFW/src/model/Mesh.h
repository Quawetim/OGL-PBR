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

    // ���� ��-���������
    glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);

public:
    ///<summary>�����������.</summary>
    ///<param name = 'vertices'>������� ����.</param>
    ///<param name = 'indices'>������� ������.</param>
    ///<param name = 'textures'>��������.</param>
    Mesh(std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

    ///<summary>��������� ����.</summary>
    ///<param name = 'shader'>������.</param>
    void drawMesh(Shader shader);

    ///<summary>����� ���� ���� � RGB �������.</summary>
    ///<para name = 'red'>������� ���������� �����.</para>
    ///<para name = 'green'>������ ���������� �����.</para>
    ///<para name = 'blue'>����� ���������� �����.</para>
    void setMeshColor(unsigned char red, unsigned char green, unsigned char blue);
};