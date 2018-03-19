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

    // ��� ����
    std::string name;

    // ������� ����
    std::vector<QVertexData> vertices;

    // ������� ������
    std::vector<unsigned int> indices;

    // ��������
    std::vector<QTexture> textures;

    bool diffuseMap_flag = true, specularMap_flag = true, normalMap_flag = true;

    // Ambient ����
    glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);

    // Diffuse ����
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);

    // Specular ����
    glm::vec3 specularColor = glm::vec3(0.9f, 0.9f, 0.9f);

    // ���� (�������) �����
    float shinePower = 8.0f;

public:
    ///<summary>�����������.</summary>
    ///<param name = 'name'>��� ����.</param>
    ///<param name = 'vertices'>������� ����.</param>
    ///<param name = 'indices'>������� ������.</param>
    ///<param name = 'textures'>��������.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

    ///<summary>��������� ����.</summary>
    ///<param name = 'shader'>������.</param>
    void drawMesh(Shader shader);

    ///<summary>����� ambient ���� ���� � RGB �������.</summary>
    ///<para name = 'red'>������� ���������� �����.</para>
    ///<para name = 'green'>������ ���������� �����.</para>
    ///<para name = 'blue'>����� ���������� �����.</para>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� diffuse ���� ���� � RGB �������.</summary>
    ///<para name = 'red'>������� ���������� �����.</para>
    ///<para name = 'green'>������ ���������� �����.</para>
    ///<para name = 'blue'>����� ���������� �����.</para>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� specular ���� ���� � RGB �������.</summary>
    ///<para name = 'red'>������� ���������� �����.</para>
    ///<para name = 'green'>������ ���������� �����.</para>
    ///<para name = 'blue'>����� ���������� �����.</para>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� ���� (�������) �����.</summary>
    ///<para name = 'value'>��������.</para>
    void setShinePower(const float value);

    ///<summary>����� ���� ������������� �������� ����.</summary>
    ///<para name = 'type'>��� ��������.</para>
    ///<para name = 'use'>������������ �������� ��� ���.</para>
    void Mesh::setTextureFlag(const QTextureType type, const bool use);

    ///<summary>����� ���� �������� ��������.</summary>
    ///<para name = 'texture'>��������.</para>
    void setTestTexture(QTexture texture);

    ///<summary>���������� ��� ����.</summary>
    std::string getName() const;
};