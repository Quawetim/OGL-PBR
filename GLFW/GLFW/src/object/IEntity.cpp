#include "IEntity.h"

///<summary>Двигает объект в направлении оси с заданной скоростью.</summary>
///<param name = 'velocityX'>Скорость по оси x.</param>
///<param name = 'velocityY'>Скорость по оси y.</param>
///<param name = 'velocityZ'>Скорость по оси z.</param>
void IEntity::move(const float velocityX, const float velocityY, const float velocityZ)
{
	this->position += glm::vec3(velocityX * deltaTime, velocityY * deltaTime, velocityZ * deltaTime);
	this->translationMatrix = glm::translate(this->position);
}

///<summary>Вращает объект с заданной скоростью.</summary>
///<param name = 'angle'>Скорость поворота в градусах.</param>
///<param name = 'axis'>Ось вращения.</param>
void IEntity::rotate(const double angle, const glm::vec3 axis)
{
	this->rotationAngle += angle * deltaTime;
	this->rotationAxis = axis;

	if (this->rotationAngle == 360) this->rotationAngle = 0;
	else
	{
		if (this->rotationAngle > 360) this->rotationAngle -= 360;
	}

	this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>Изменяет размер объекта с заданной скоростью.</summary>
///<param name = 'scaleXYZ'>Скорость изменения размера по всем осям.</param>
void IEntity::scale(const float scaleXYZ)
{
	this->scaleCoefficients += glm::vec3(scaleXYZ * (float)deltaTime);

	if (this->scaleCoefficients.x == 0 || this->scaleCoefficients.y == 0 || this->scaleCoefficients.z == 0) logger.log("IEntity::scaleObject", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scaleCoefficients.x < 0 || this->scaleCoefficients.y < 0 || this->scaleCoefficients.z < 0) logger.log("IEntity::scaleObject", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix = glm::scale(this->scaleCoefficients);
}

///<summary>Изменяет размер объекта с заданной скоростью.</summary>
///<param name = 'scaleX'>Скорость изменения размера по X.</param>
///<param name = 'scaleY'>Скорость изменения размера по Y.</param>
///<param name = 'scaleZ'>Скорость изменения размера по Z.</param>
void IEntity::scale(const float scaleX, const float scaleY, const float scaleZ)
{
	this->scaleCoefficients += glm::vec3(scaleX * deltaTime, scaleY * deltaTime, scaleZ * deltaTime);

	if (this->scaleCoefficients.x == 0 || this->scaleCoefficients.y == 0 || this->scaleCoefficients.z == 0) logger.log("IEntity::scaleModel", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scaleCoefficients.x < 0 || this->scaleCoefficients.y < 0 || this->scaleCoefficients.z < 0) logger.log("IEntity::scaleModel", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix = glm::scale(this->scaleCoefficients);
}

///<summary>Задаёт ambient цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void IEntity::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void IEntity::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт specular цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void IEntity::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт силу (яркость) блика.</summary>
///<param name = 'value'>Значение.</param>
void IEntity::setShinePower(const float value)
{
	this->shinePower = value;
}

///<summary>Задаёт позицию сущности.</summary>
///<param name = 'position'>Позиция.</param>
void IEntity::setPosition(const glm::vec3 position)
{
	this->position = position;
	this->translationMatrix = glm::translate(this->position);
}

///<summary>Задаёт поворот сущности.</summary>
///<param name = 'angle'>Угол поворота в градусах.</param>
///<param name = 'axis'>Ось поворота.</param>
void IEntity::setRotation(const double angle, const glm::vec3 axis)
{
	this->rotationAngle = angle;
	this->rotationAxis = axis;

	if (this->rotationAngle == 360) this->rotationAngle = 0;
	else
	{
		if (this->rotationAngle > 360) this->rotationAngle -= 360;
	}

	this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>Задаёт размер сущности от исходного.</summary>
///<param name = 'scale'>Коэффициент размера.</param>
void IEntity::setScale(const glm::vec3 scale)
{
	this->scaleCoefficients = scale;

	if (this->scaleCoefficients.x == 0 || this->scaleCoefficients.y == 0 || this->scaleCoefficients.z == 0) logger.log("IEntity::setScale", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scaleCoefficients.x < 0 || this->scaleCoefficients.y < 0 || this->scaleCoefficients.z < 0) logger.log("IEntity::setScale", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix = glm::scale(this->scaleCoefficients);
}

///<summary>Возвращает имя модели.</summary>
std::string IEntity::getName() const
{
	return this->name;
}

///<summary>Возвращает матрицу модели.</summary>
glm::mat4 IEntity::getModelMatrix() const
{
	return this->translationMatrix * this->rotationMatrix * this->scaleMatrix;
}

///<summary>Возвращает позицию модели.</summary>
glm::vec3 IEntity::getPosition() const
{
	return this->position;
}