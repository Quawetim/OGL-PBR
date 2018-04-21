#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/light/Light.h"

///<summary>Тестовая сцена.</summary>
class TestScene : public IScene
{
private:
	std::vector<Object*> objects_;

	bool drawLights_ = true;
	std::vector<std::shared_ptr<PointLight>> lights_;

public:
	///<summary>Деструктор.</summary>
	TestScene() {};

	///<summary>Деструктор.</summary>
	~TestScene();

	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	void init(std::vector<Model*> models);

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void render(Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
