#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"

///<summary>Простейший 3D-объект не содержит в себе никаких подобъектов.</summary>
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

	///<summary>Вершины меша.</summary>
    std::vector<VertexData> vertices_;
 
	///<summary>Индексы вершин.</summary>
    std::vector<unsigned int> indices_;
 
	///<summary>Текстуры.</summary>
    std::vector<Texture> textures_;

	///<summary>Конструктор.</summary>
	Mesh();

	///<summary>Конструктор.</summary>
	///<param name = 'name'>Имя меша.</param>
	///<param name = 'vertices'>Вершины меша.</param>
	///<param name = 'indices'>Индексы вершин.</param>
	///<param name = 'textures'>Текстуры.</param>
	Mesh(std::string name, std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

public:
	///<summary>Деструктор.</summary>
	~Mesh();

	///<summary>Возвращает номер VAO.</summary>
	unsigned int getVAO() const;

	///<summary>Возвращает размер вектора индексов вершин.</summary>
	unsigned int getIndicesSize() const;

	///<summary>Возвращает текстуры, привязанные к мешу.</summary>
	const std::vector<Texture>& getTextures() const;
};