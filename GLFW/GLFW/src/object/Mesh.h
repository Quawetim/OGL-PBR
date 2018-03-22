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

    // ������� ������
    glm::mat4 modelMatrix;
    
    // ������� ������������
    glm::mat4 translationMatrix;

    // ������� ��������
    glm::mat4 rotationMatrix;

    // ������� �������
    glm::mat4 scaleMatrix;

    // ������� ������� ����
    glm::vec3 position;

    // ���� ��������
    double rotationAngle;

    // ��� ��������
    glm::vec3 rotationAxis;

    // ����������� ������� �� ������ �� ����
    glm::vec3 scale;

    // ���� ��������� diffuseMap
    bool diffuseMap_flag = true;

    // ���� ��������� specularMap
    bool specularMap_flag = true;

    // ���� ��������� normalMap
    bool normalMap_flag = true;

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

    ///<summary>������� ��� � ����������� ��� � �������� ���������.</summary>
    ///<param name = 'velocityX'>�������� �� ��� x.</param>
    ///<param name = 'velocityY'>�������� �� ��� y.</param>
    ///<param name = 'velocityZ'>�������� �� ��� z.</param>
    void moveMesh(const float velocityX, const float velocityY, const float velocityZ);

    ///<summary>������� ��� � �������� ���������.</summary>
    ///<param name = 'angle'>�������� �������� � ��������.</param>
    ///<param name = 'axis'>��� ��������.</param>
    void rotateMesh(const double angle, const glm::vec3 axis);

    ///<summary>�������� ������ ���� � �������� ���������.</summary>
    ///<param name = 'scaleXYZ'>�������� ��������� ������� �� ���� ����.</param>
    void scaleMesh(const float scaleXYZ);

    ///<summary>�������� ������ ���� � �������� ���������.</summary>
    ///<param name = 'scaleX'>�������� ��������� ������� �� X.</param>
    ///<param name = 'scaleY'>�������� ��������� ������� �� Y.</param>
    ///<param name = 'scaleZ'>�������� ��������� ������� �� Z.</param>
    void scaleMesh(const float scaleX, const float scaleY, const float scaleZ);

    ///<summary>����� ambient ���� ���� � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� diffuse ���� ���� � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� specular ���� ���� � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� ���� (�������) �����.</summary>
    ///<param name = 'value'>��������.</param>
    void setShinePower(const float value);

    ///<summary>����� ���� ������������� �������� ����.</summary>
    ///<param name = 'type'>��� ��������.</param>
    ///<param name = 'use'>������������ �������� ��� ���.</param>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>����� ���� �������� ��������.</summary>
    ///<param name = 'texture'>��������.</param>
    void setTestTexture(QTexture texture);

    ///<summary>����� ������� ����.</summary>
    ///<param name = 'position'>�������.</param>
    void setPosition(glm::vec3 position);

    ///<summary>����� ������� ����.</summary>
    ///<param name = 'angle'>���� �������� � ��������.</param>
    ///<param name = 'axis'>��� ��������.</param>
    void setRotation(const double angle, const glm::vec3 axis);

    ///<summary>����� ������ ���� �� ���������.</summary>
    ///<param name = 'scale'>����������� �������.</param>
    void setScale(const glm::vec3 scale);

    ///<summary>���������� ��� ����.</summary>
    std::string getName() const;

    ///<summary>���������� ������� ������.</summary>
    glm::mat4 getModelMatrix();

    ///<summary>���������� ������� ����.</summary>
    glm::vec3 getPosition() const;
};