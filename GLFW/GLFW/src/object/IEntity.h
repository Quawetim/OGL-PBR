#pragma once
#include "..\includes\Includes.h"

class IEntity
{
protected:
	///<summary>Имя модели.</summary>
	std::string name;

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
	glm::vec3 scaleCoefficients;

	///<summary>Ambient цвет.</summary>
	glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);

	///<summary>Diffuse цвет.</summary>
	glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);

	///<summary>Specular цвет.</summary>
	glm::vec3 specularColor = glm::vec3(0.9f, 0.9f, 0.9f);

	///<summary>Сила (яркость) блика.</summary>
	float shinePower = 8.0f;

public:
	///<summary>Отрисовка сущности.</summary>
	///<param name = 'shader'>Шейдер.</param>
	virtual void draw(const Shader shader) = 0;

	///<summary>Двигает объект в направлении оси с заданной скоростью.</summary>
	///<param name = 'velocityX'>Скорость по оси x.</param>
	///<param name = 'velocityY'>Скорость по оси y.</param>
	///<param name = 'velocityZ'>Скорость по оси z.</param>
	void move(const float velocityX, const float velocityY, const float velocityZ);

	///<summary>Вращает объект с заданной скоростью.</summary>
	///<param name = 'angle'>Скорость поворота в градусах.</param>
	///<param name = 'axis'>Ось вращения.</param>
	void rotate(const double angle, const glm::vec3 axis);

	///<summary>Изменяет размер объекта с заданной скоростью.</summary>
	///<param name = 'scaleXYZ'>Скорость изменения размера по всем осям.</param>
	void scale(const float scaleXYZ);

	///<summary>Изменяет размер объекта с заданной скоростью.</summary>
	///<param name = 'scaleX'>Скорость изменения размера по X.</param>
	///<param name = 'scaleY'>Скорость изменения размера по Y.</param>
	///<param name = 'scaleZ'>Скорость изменения размера по Z.</param>
	void scale(const float scaleX, const float scaleY, const float scaleZ);

	///<summary>Задаёт ambient цвет меша в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	virtual void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue) = 0;

	///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	virtual void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue) = 0;

	///<summary>Задаёт specular цвет меша в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	virtual void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue) = 0;

	///<summary>Задаёт силу (яркость) блика.</summary>
	///<param name = 'value'>Значение.</param>
	virtual void setShinePower(const float value) = 0;

	///<summary>Задаёт позицию сущности.</summary>
	///<param name = 'position'>Позиция.</param>
	virtual void setPosition(const glm::vec3 position);

	///<summary>Задаёт поворот сущности.</summary>
	///<param name = 'angle'>Угол поворота в градусах.</param>
	///<param name = 'axis'>Ось поворота.</param>
	virtual void setRotation(const double angle, const glm::vec3 axis);

	///<summary>Задаёт размер сущности от исходного.</summary>
	///<param name = 'scale'>Коэффициент размера.</param>
	virtual void setScale(const glm::vec3 scale);

	///<summary>Возвращает имя модели.</summary>
	std::string getName() const;

	///<summary>Возвращает матрицу модели.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>Возвращает позицию модели.</summary>
	glm::vec3 getPosition() const;
};
