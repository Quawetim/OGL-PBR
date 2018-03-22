#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"

///<summary>���������� 3D-������ �� �������� � ���� ������� �����������.</summary>
class Mesh : public IEntity
{
	friend class Model;
private: 
	///<summary>Vertex array object.</summary>
    unsigned int VAO;
 
	///<summary>Vertex buffer object.</summary>
    unsigned int VBO;

	///<summary>Element buffer object.</summary>
    unsigned int EBO;

	///<summary>������� ����.</summary>
    std::vector<QVertexData> vertices;
 
	///<summary>������� ������.</summary>
    std::vector<unsigned int> indices;
 
	///<summary>��������.</summary>
    std::vector<QTexture> textures;
 
	///<summary>���� ������������� diffuseMap.</summary>
    bool use_diffuse_map_flag = true;

	///<summary>���� ������������� specularMap.</summary>
    bool use_specular_map_flag = true;
 
	///<summary>���� ������������� normalMap.</summary>
    bool use_normal_map_flag = true;

public:
    ///<summary>�����������.</summary>
    ///<param name = 'name'>��� ����.</param>
    ///<param name = 'vertices'>������� ����.</param>
    ///<param name = 'indices'>������� ������.</param>
    ///<param name = 'textures'>��������.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

	///<summary>��������� ����.</summary>
	///<param name = 'shader'>������.</param>
	void draw(const Shader shader);

	///<summary>��������� ���� � ��������� �������.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'ambientColor'>Ambient ����.</param>
	///<param name = 'diffuseColor'>Diffuse ����.</param>
	///<param name = 'specularColor'>Specular ����.</param>
	///<param name = 'shinePower'>���� (�������) �����.</param>
	void draw(const Shader shader, const glm::vec3 ambientColor, const glm::vec3 diffuseColor, const glm::vec3 specularColor, const float shinePower);

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
};