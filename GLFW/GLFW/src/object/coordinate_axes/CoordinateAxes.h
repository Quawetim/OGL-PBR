#pragma once
#include "../../includes/Includes.h"

///<summary>Объект координатные оси.
///<para>x - красная.</para>
///<para>y - зелёная.</para>
///<para>z - синяя.</para>
///</summary>
class CoordinateAxes
{
private:
	///<summary>Vertex array object.</summary>
	unsigned int VAO_;

	///<summary>Vertex buffer object.</summary>
	unsigned int VBO_;

	///<summary>Вершины.</summary>
	std::vector<QVertexData> vertices_;

	///<summary>Индексы.</summary>
	std::vector<unsigned int> indices_;

	///<summary>Матрица модели.</summary>
	glm::mat4 modelMatrix_;

	///<summary>Матрица проекции.</summary>
	glm::mat4 projectionMatrix_;
	
public:
	///<summary>Конструктор.</summary>
	CoordinateAxes();

	///<summary>Деструктор.</summary>
	~CoordinateAxes();

	///<summary>Отрисовка осей.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	void draw(const Shader shader, const glm::mat4 view_matrix);
};
