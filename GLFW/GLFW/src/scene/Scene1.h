#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/Object.h"

///<summary>Сцена 1.</summary>
class Scene1 : public IScene
{
private:
	std::vector<Object> cubes;
	std::vector<Object> spheres;
	std::vector<Object> cylinders;

public:
	///<summary>Деструктор.</summary>
	~Scene1();

	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	void init(std::vector<Model*> models);

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'shader'></param>
	void render(const Shader shader, const glm::mat4 P, const glm::mat4 V);
};
