#pragma once
#include "..\Object.h"

class Skybox : public Object
{
private:
	///<summary>Модель.</summary>
	Model model_;

	///<summary>Идентификатор текстуры.</summary>
	unsigned int cubeMapID;

public:
	///<summary>Конструктор.</summary>
	///<param name = 'size'>Размер скайбокса.</param>
	Skybox(const float size);

	///<summary>Отрисовка скайбокса.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'projection_matrix'>Матрица проекции.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void draw(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};