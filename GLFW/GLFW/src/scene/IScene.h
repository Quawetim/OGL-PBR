#pragma once
#include "../includes/Includes.h"
#include "../object/Model.h"

///<summary>Класс-контенер для объектов aka сцена, уровень.</summary>
class IScene
{
public:
	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	virtual void init(std::vector<Model*> models) = 0;

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'shader'></param>
	virtual void render(const Shader shader, const glm::mat4 P, const glm::mat4 V) = 0;
};
