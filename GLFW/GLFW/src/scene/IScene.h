#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"
#include "../object/light/Light.h"

///<summary>Класс-контенер для объектов aka сцена, уровень.</summary>
class IScene
{
protected:
	///<summary>Список объектов.</summary>
	std::vector<std::shared_ptr<Object>> objects_;

	///<summary>Список источников освещения.</summary>
	std::vector<std::shared_ptr<PointLight>> lights_;

	///<summary>Флаг движения объектов.</summary>
	bool objectsMoving_ = false;

	///<summary>Флаг движения источников освещения.</summary>
	bool lightsMoving_ = false;

	///<summary>Флаг отображения источников освещения.</summary>
	bool lightsVisible_ = false;

	template<typename Type> int getSign(Type value) { return (value > 0) ? 1 : -1; }

public:
	///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
	///<param name = 'models'>Список моделей.</param>
	virtual void init(std::vector<std::shared_ptr<Model>> models) = 0;

	///<summary>Отрисовка сцены.</summary>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	virtual void render(float deltaTime, std::shared_ptr<Shader> shader, const glm::mat4 view_matrix, const glm::vec3 camera_position) = 0;

	///<summary>Переключатель движения объектов.</summary>
	///<param name = 'show'>Вкл/выкл.</param>
	void moveObjects(const bool move) { this->objectsMoving_ = move; }

	///<summary>Переключатель движения источников освещения.</summary>
	///<param name = 'show'>Вкл/выкл.</param>
	void moveLights(const bool move) { this->lightsMoving_ = move; }

	///<summary>Переключатель отображения источников освещения.</summary>
	///<param name = 'show'>Вкл/выкл.</param>
	void showLights(const bool show) { this->lightsVisible_ = show; }

	///<summary>Возвращает флаг движения объектов.</summary>
	bool getMoveObjects() { return this->objectsMoving_; }

	///<summary>Возвращает флаг движения источников освещения.</summary>
	bool getMoveLights() { return this->lightsMoving_; }

	///<summary>Возвращает флаг отображения источников освещения.</summary>
	bool getShowLights() { return this->lightsVisible_; }
};
