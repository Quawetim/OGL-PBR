#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"

///<summary>Класс-контенер для объектов aka сцена, уровень.</summary>
class IScene
{
protected:
	bool objectsMoving_ = false;
	bool lightsMoving_ = false;
	bool lightsVisible_ = false;

	template<typename Type> int getSign(Type value) { return (value > 0) ? 1 : -1; }

public:
	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	virtual void init(std::vector<Model*> models) = 0;

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	virtual void render(float deltaTime, Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position) = 0;

	void moveObjects(const bool move) { this->objectsMoving_ = move; }

	void moveLights(const bool move) { this->lightsMoving_ = move; }

	void showLights(const bool show) { this->lightsVisible_ = show; }

	bool getMoveObjects() { return this->objectsMoving_; }

	bool getMoveLights() { return this->lightsMoving_; }

	bool getShowLights() { return this->lightsVisible_; }
};
