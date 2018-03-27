#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/Object.h"
#include "../texture_loader/TextureLoader.h"

///<summary>Сцена 1.</summary>
class Scene1 : public IScene
{
private:
	std::vector<Object> cubes_;
	std::vector<Object> spheres_;
	std::vector<Object> cylinders_;
	bool decrease_;

public:
	///<summary>Деструктор.</summary>
	~Scene1();

	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	void init(std::vector<Model*> models);

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'projection_matrix'>Матрица проекции.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	void render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix);
};
