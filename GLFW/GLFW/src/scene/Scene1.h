#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/light/Light.h"

///<summary>Сцена 1.</summary>
class Scene1 : public IScene
{
private:
	std::vector<Object*> cubes_;
	std::vector<Object*> spheres_;
	std::vector<Object*> cylinders_;
	std::vector<Object*> objects_;
	bool decrease_;

	std::vector<std::shared_ptr<PointLight>> lights_;

public:
	///<summary>Деструктор.</summary>
	~Scene1();

	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	void init(std::vector<Model*> models);

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void render(float deltaTime, Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
