#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"

///<summary>Простейший 3D-объект не содержит в себе никаких подобъектов.</summary>
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

	///<summary>Вершины меша.</summary>
    std::vector<QVertexData> vertices;
 
	///<summary>Индексы вершин.</summary>
    std::vector<unsigned int> indices;
 
	///<summary>Текстуры.</summary>
    std::vector<QTexture> textures;
 
	///<summary>Флаг использования diffuseMap.</summary>
    bool use_diffuse_map = true;

	///<summary>Флаг использования specularMap.</summary>
    bool use_specular_map = true;
 
	///<summary>Флаг использования normalMap.</summary>
    bool use_normal_map = true;

public:
	///<summary>Конструктор.</summary>
	Mesh() {};

    ///<summary>Конструктор.</summary>
    ///<param name = 'name'>Имя меша.</param>
    ///<param name = 'vertices'>Вершины меша.</param>
    ///<param name = 'indices'>Индексы вершин.</param>
    ///<param name = 'textures'>Текстуры.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

	///<summary>Отрисовка меша.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'material'>Материал.</param>
	void draw(const Shader shader, const QMaterial material);

    ///<summary>Задаёт флаг использования текстуры меша.</summary>
    ///<param name = 'type'>Тип текстуры.</param>
    ///<param name = 'use'>Использовать текстуру или нет.</param>
    void useTexture(const QTextureType type, const bool use);

    ///<summary>Задаёт мешу тестовую текстуру.</summary>
    ///<param name = 'texture'>Текстура.</param>
    void useTestTexture(const QTexture texture);
};