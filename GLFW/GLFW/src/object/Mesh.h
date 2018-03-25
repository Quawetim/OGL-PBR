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
    bool use_diffuse_map = true;

	///<summary>���� ������������� specularMap.</summary>
    bool use_specular_map = true;
 
	///<summary>���� ������������� normalMap.</summary>
    bool use_normal_map = true;

public:
	///<summary>�����������.</summary>
	Mesh() {};

    ///<summary>�����������.</summary>
    ///<param name = 'name'>��� ����.</param>
    ///<param name = 'vertices'>������� ����.</param>
    ///<param name = 'indices'>������� ������.</param>
    ///<param name = 'textures'>��������.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

	///<summary>��������� ����.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'material'>��������.</param>
	void draw(const Shader shader, const QMaterial material);

    ///<summary>����� ���� ������������� �������� ����.</summary>
    ///<param name = 'type'>��� ��������.</param>
    ///<param name = 'use'>������������ �������� ��� ���.</param>
    void useTexture(const QTextureType type, const bool use);

    ///<summary>����� ���� �������� ��������.</summary>
    ///<param name = 'texture'>��������.</param>
    void useTestTexture(const QTexture texture);
};