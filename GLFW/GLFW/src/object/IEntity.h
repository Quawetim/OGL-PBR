#pragma once
#include "..\includes\Includes.h"

class IEntity
{
protected:
	IEntity() {};

	///<summary>Имя модели.</summary>
	std::string name_;

	///<summary>Матрица перемещенния.</summary>
	glm::mat4 translationMatrix_;

	///<summary>Матрица вращения.</summary>
	glm::mat4 rotationMatrix_;

	///<summary>Матрица размера.</summary>
	glm::mat4 scaleMatrix_;

	///<summary>Позиция модели.</summary>
	glm::vec3 position_;

	///<summary>Угол поворота.</summary>
	double rotationAngle_;

	///<summary>Ось поворота.</summary>
	glm::vec3 rotationAxis_;

	///<summary>Коэффициент размера по каждой из осей.</summary>
	glm::vec3 scaleCoeffs_;	

public:
	///<summary>Двигает объект в направлении оси с заданной скоростью.</summary>
	///<param name = 'velocity_x'>Скорость по оси x.</param>
	///<param name = 'velocity_y'>Скорость по оси y.</param>
	///<param name = 'velocity_z'>Скорость по оси z.</param>
	void move(float deltaTime, const float velocity_x, const float velocity_y, const float velocity_z);

	///<summary>Вращает объект с заданной скоростью.</summary>
	///<param name = 'angle'>Скорость поворота в градусах.</param>
	///<param name = 'axis'>Ось вращения.</param>
	void rotate(float deltaTime, const double angle, const glm::vec3 axis);

	///<summary>Изменяет размер объекта с заданной скоростью.</summary>
	///<param name = 'velocity_xyz'>Скорость изменения размера по всем осям.</param>
	void scale(float deltaTime, const float velocity_xyz);

	///<summary>Изменяет размер объекта с заданной скоростью.</summary>
	///<param name = 'velocity_x'>Скорость изменения размера по X.</param>
	///<param name = 'velocity_y'>Скорость изменения размера по Y.</param>
	///<param name = 'velocity_z'>Скорость изменения размера по Z.</param>
	void scale(float deltaTime, const float velocity_x, const float velocity_y, const float velocity_z);

	///<summary>Задаёт позицию.</summary>
	///<param name = 'position'>Позиция.</param>
	void setPosition(const glm::vec3 position);

	///<summary>Задаёт поворот.</summary>
	///<param name = 'angle'>Угол поворота в градусах.</param>
	///<param name = 'axis'>Ось поворота.</param>
	void setRotation(const double angle, const glm::vec3 axis);

	///<summary>Задаёт размер от исходного.</summary>
	///<param name = 'scale'>Коэффициент размера.</param>
	void setScale(const glm::vec3 scale);

	///<summary>Возвращает имя.</summary>
	std::string getName() const;

	///<summary>Возвращает матрицу модели.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>Возвращает позицию.</summary>
	glm::vec3 getPosition() const;

	///<summary>Возвращает размер.</summary>
	glm::vec3 getScale() const;
};
