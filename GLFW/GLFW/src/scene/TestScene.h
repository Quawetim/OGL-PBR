#pragma once
#include "../includes/Includes.h"
#include "IScene.h"

///<summary>Тестовая сцена.</summary>
class TestScene : public IScene
{
private:
	Mesh rectangle_;
	std::vector<Object> objects_;

public:
	///<summary>Деструктор.</summary>
	~TestScene();

	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	void init(std::vector<Model*> models);

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'projection_matrix'>Матрица проекции.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
