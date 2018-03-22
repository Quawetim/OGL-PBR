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
    bool use_diffuse_map_flag = true;

	///<summary>Флаг использования specularMap.</summary>
    bool use_specular_map_flag = true;
 
	///<summary>Флаг использования normalMap.</summary>
    bool use_normal_map_flag = true;

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'name'>Имя меша.</param>
    ///<param name = 'vertices'>Вершины меша.</param>
    ///<param name = 'indices'>Индексы вершин.</param>
    ///<param name = 'textures'>Текстуры.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

	///<summary>Отрисовка меша.</summary>
	///<param name = 'shader'>Шейдер.</param>
	void draw(const Shader shader);

	///<summary>Отрисовка меша с заданными цветами.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'ambientColor'>Ambient цвет.</param>
	///<param name = 'diffuseColor'>Diffuse цвет.</param>
	///<param name = 'specularColor'>Specular цвет.</param>
	///<param name = 'shinePower'>Сила (яркость) блика.</param>
	void draw(const Shader shader, const glm::vec3 ambientColor, const glm::vec3 diffuseColor, const glm::vec3 specularColor, const float shinePower);

    ///<summary>Задаёт ambient цвет меша в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт specular цвет меша в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт силу (яркость) блика.</summary>
    ///<param name = 'value'>Значение.</param>
    void setShinePower(const float value);

    ///<summary>Задаёт флаг использования текстуры меша.</summary>
    ///<param name = 'type'>Тип текстуры.</param>
    ///<param name = 'use'>Использовать текстуру или нет.</param>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>Задаёт мешу тестовую текстуру.</summary>
    ///<param name = 'texture'>Текстура.</param>
    void setTestTexture(const QTexture texture);
};