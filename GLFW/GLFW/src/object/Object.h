#pragma once
#include "..\includes\Includes.h"
#include "Model.h"

///<summary>Основная сущность. Контейнер моделей.</summary>
class Object
{
	///<summary>Имя модели.</summary>
	std::string objectName;
 
	///<summary>Модели, из которых состоит объект.</summary>
	std::vector<Model*> models;									// Мб лучше список? Который можно отсортировать по приоритету отрисовки...

	///<summary>Матрица перемещенния.</summary>
	glm::mat4 translationMatrix;

	///<summary>Матрица вращения.</summary>
	glm::mat4 rotationMatrix;

	///<summary>Матрица размера.</summary>
	glm::mat4 scaleMatrix;

	///<summary>Позиция модели.</summary>
	glm::vec3 position;

	///<summary>Угол поворота.</summary>
	double rotationAngle;

	///<summary>Ось поворота.</summary>
	glm::vec3 rotationAxis;

	///<summary>Коэффициент размера по каждой из осей.</summary>
	glm::vec3 scale;

public:
	///<summary>Конструктор.</summary>
	Object() {};

	///<summary>Конструктор.</summary>
	///<param name = 'name'>Имя объекта.</param>
	///<param name = 'model'>Модель, составляющая объек.</param>
	Object(std::string name, Model *model);

	///<summary>Конструктор.</summary>
	///<param name = 'name'>Имя объекта.</param>
	///<param name = 'models'>Список моделей, составляющих объект.</param>
	Object(std::string name, std::vector<Model*> models);

	///<summary>Деструктор.</summary>
	~Object();

	///<summary>Отрисовка объекта.</summary>
	///<param name ='shader'>Шейдер.</param>
	virtual void draw(Shader shader);

	///<summary>Двигает объект в направлении оси с заданной скоростью.</summary>
	///<param name = 'velocityX'>Скорость по оси x.</param>
	///<param name = 'velocityY'>Скорость по оси y.</param>
	///<param name = 'velocityZ'>Скорость по оси z.</param>
	void moveObject(const float velocityX, const float velocityY, const float velocityZ);

	///<summary>Вращает объект с заданной скоростью.</summary>
	///<param name = 'angle'>Скорость поворота в градусах.</param>
	///<param name = 'axis'>Ось вращения.</param>
	void rotateObject(const double angle, const glm::vec3 axis);

	///<summary>Изменяет размер объекта с заданной скоростью.</summary>
	///<param name = 'scaleXYZ'>Скорость изменения размера по всем осям.</param>
	void scaleObject(const float scaleXYZ);

	///<summary>Изменяет размер объекта с заданной скоростью.</summary>
	///<param name = 'scaleX'>Скорость изменения размера по X.</param>
	///<param name = 'scaleY'>Скорость изменения размера по Y.</param>
	///<param name = 'scaleZ'>Скорость изменения размера по Z.</param>
	void scaleObject(const float scaleX, const float scaleY, const float scaleZ);

	///<summary>Задаёт позицию объекта.</summary>
	///<param name = 'position'>Позиция.</param>
	void setPosition(const glm::vec3 position);

	///<summary>Задаёт поворот объекта.</summary>
	///<param name = 'angle'>Угол поворота в градусах.</param>
	///<param name = 'axis'>Ось поворота.</param>
	void setRotation(const double angle, const glm::vec3 axis);

	///<summary>Задаёт размер объекта от исходного.</summary>
	///<param name = 'scale'>Коэффициент размера.</param>
	void setScale(const glm::vec3 scale);

	///<summary>Добавляет модель к объекту.</summary>
	///<param name = 'model'>Модель.</param>
	void addModel(Model *model);

	///<summary>Извлекает модель из объекта пл имени.</summary>
	///<param name = 'name'>Имя извлекаемой модели.</param>
	Model* deleteModel(const std::string name);

	///<summary>Возвращает имя модели.</summary>
	std::string getName() const;

	///<summary>Возвращает матрицу модели.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>Возвращает позицию модели.</summary>
	glm::vec3 getPosition() const;
};

class StarDestroyer : Object
{

};

class StarFighter : Object
{

};

class ImperialStarDestroyer : StarDestroyer
{

};

class Venator : StarDestroyer
{

};

class TieFighter : StarFighter
{

};

class ARC170 : StarFighter
{

};