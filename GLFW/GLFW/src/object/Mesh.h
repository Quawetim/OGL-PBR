#pragma once
#include "..\includes\Includes.h"

///<summary>���������� 3D-������ �� �������� � ���� ������� �����������.</summary>
class Mesh
{
	friend class Model;
private: 
	///<summary>Vertex array object.</summary>
    unsigned int VAO;
 
	///<summary>Vertex buffer object.</summary>
    unsigned int VBO;

	///<summary>Element buffer object.</summary>
    unsigned int EBO;
 
	///<summary>��� ����.</summary>
    std::string meshName;

	///<summary>������� ����.</summary>
    std::vector<QVertexData> vertices;
 
	///<summary>������� ������.</summary>
    std::vector<unsigned int> indices;
 
	///<summary>��������.</summary>
    std::vector<QTexture> textures;
     
	///<summary>������� ������������.</summary>
    glm::mat4 translationMatrix;
 
	///<summary>������� ��������.</summary>
    glm::mat4 rotationMatrix;
 
	///<summary>������� �������.</summary>
    glm::mat4 scaleMatrix;
 
	///<summary>������� ����.</summary>
    glm::vec3 position;

	///<summary>���� ��������.</summary>
    double rotationAngle;
 
	///<summary>��� ��������.</summary>
    glm::vec3 rotationAxis;
 
	///<summary>����������� ������� �� ������ �� ����.</summary>
    glm::vec3 scale;
 
	///<summary>���� ��������� diffuseMap.</summary>
    bool diffuseMap_flag = true;

	///<summary>���� ��������� specularMap.</summary>
    bool specularMap_flag = true;
 
	///<summary>���� ��������� normalMap.</summary>
    bool normalMap_flag = true;
 
	///<summary>Ambient ����.</summary>
    glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
 
	///<summary>Diffuse ����.</summary>
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
 
	///<summary>Specular ����.</summary>
    glm::vec3 specularColor = glm::vec3(0.9f, 0.9f, 0.9f);
 
	///<summary>���� (�������) �����.</summary>
    float shinePower = 8.0f;

	///<summary>��������� ����.</summary>
	///<param name = 'shader'>������.</param>
	void drawMesh(const Shader shader);

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

public:
    ///<summary>�����������.</summary>
    ///<param name = 'name'>��� ����.</param>
    ///<param name = 'vertices'>������� ����.</param>
    ///<param name = 'indices'>������� ������.</param>
    ///<param name = 'textures'>��������.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

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
    void setTestTexture(const QTexture texture);

    ///<summary>����� ������� ����.</summary>
    ///<param name = 'position'>�������.</param>
    void setPosition(const glm::vec3 position);

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
    glm::mat4 getModelMatrix() const;

    ///<summary>���������� ������� ����.</summary>
    glm::vec3 getPosition() const;
};