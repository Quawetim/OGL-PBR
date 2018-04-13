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
    std::vector<VertexData> vertices_;
 
	///<summary>������� ������.</summary>
    std::vector<unsigned int> indices_;
 
	///<summary>��������.</summary>
    std::vector<Texture> textures_;

	///<summary>�����������.</summary>
	Mesh();

	///<summary>�����������.</summary>
	///<param name = 'name'>��� ����.</param>
	///<param name = 'vertices'>������� ����.</param>
	///<param name = 'indices'>������� ������.</param>
	///<param name = 'textures'>��������.</param>
	Mesh(std::string name, std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

public:
	///<summary>����������.</summary>
	~Mesh();

	///<summary>���������� ����� VAO.</summary>
	unsigned int getVAO() const;

	///<summary>���������� ������ ������� �������� ������.</summary>
	unsigned int getIndicesSize() const;

	///<summary>���������� ��������, ����������� � ����.</summary>
	const std::vector<Texture>& getTextures() const;
};