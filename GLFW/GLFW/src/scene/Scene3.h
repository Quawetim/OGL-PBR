#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/light/Light.h"

///<summary>Сцена 3.</summary>
class Scene3 : public IScene
{
private:
	std::vector<std::shared_ptr<Object>> cubes_;
	std::vector<std::shared_ptr<Object>> spheres_;
	std::vector<std::shared_ptr<Object>> cylinders_;

	std::vector<std::shared_ptr<PointLight>> lights_;

	bool right_ = true; 

public:
	///<summary>Деструктор.</summary>
	~Scene3();

	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	void init(std::vector<std::shared_ptr<Model>> models);

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void render(float deltaTime, std::shared_ptr<Shader> shader, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
