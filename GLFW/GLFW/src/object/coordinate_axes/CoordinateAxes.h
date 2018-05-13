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

	///<summary>Матрица модели.</summary>
	glm::mat4 modelMatrix_;

	///<summary>Матрица проекции.</summary>
	glm::mat4 projectionMatrix_;
	
public:
	///<summary>Конструктор.</summary>
	CoordinateAxes();

	///<summary>Деструктор.</summary>
	~CoordinateAxes();

	///<summary>Возвращает VAO.</summary>
	unsigned int getVAO() const;

	///<summary>Возвращает матрицу модели.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>Возвращает матрицу проекции.</summary>
	glm::mat4 getProjectionMatrix() const;
};
