#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"

///<summary>���������� 3D-������ �� �������� � ���� ������� �����������.</summary>
class Mesh : public IEntity
{
	friend class Model;
private: 
	///<summary>Vertex array object.</summary>
    unsigned int VAO_;
 
	///<summary>Vertex buffer object.</summary>
    unsigned int VBO_;

	///<summary>Element buffer object.</summary>
    unsigned int EBO_;

	///<summary>������� ����.</summary>
    std::vector<QVertexData> vertices_;
 
	///<summary>������� ������.</summary>
    std::vector<unsigned int> indices_;
 
	///<summary>��������.</summary>
    std::vector<QTexture> textures_;
 
	///<summary>���� ������������� diffuseMap.</summary>
    bool useDiffuseMaps_;

	///<summary>���� ������������� specularMap.</summary>
    bool useSpecularMaps_;
 
	///<summary>���� ������������� normalMap.</summary>
    bool useNormalMaps_;

public:
	///<summary>�����������.</summary>
	Mesh();

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