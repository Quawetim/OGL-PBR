#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../texture_loader/TextureLoader.h"
#include "../object/Object.h"

///<summary>Тестовая сцена.</summary>
class TestScene : public IScene
{
private:
	Mesh rectangle;
	Object Nanosuit;

public:
	///<summary>Деструктор.</summary>
	~TestScene();

	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	void init(std::vector<Model*> models);

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'shader'></param>
	void render(const Shader shader, const glm::mat4 P, const glm::mat4 V);
};
