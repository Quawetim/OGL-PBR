#include "IEntity.h"

///<summary>Двигает объект в направлении оси с заданной скоростью.</summary>
///<param name = 'velocity_x'>Скорость по оси x.</param>
///<param name = 'velocity_y'>Скорость по оси y.</param>
///<param name = 'velocity_z'>Скорость по оси z.</param>
void IEntity::move(const float velocity_x, const float velocity_y, const float velocity_z)
{
	this->position_ += glm::vec3(velocity_x * deltaTime, velocity_y * deltaTime, velocity_z * deltaTime);
	this->translationMatrix_ = glm::translate(this->position_);
}

///<summary>Вращает объект с заданной скоростью.</summary>
///<param name = 'angle'>Скорость поворота в градусах.</param>
///<param name = 'axis'>Ось вращения.</param>
void IEntity::rotate(const double angle, const glm::vec3 axis)
{
	this->rotationAngle_ += angle * deltaTime;
	this->rotationAxis_ = axis;

	if (this->rotationAngle_ == 360) this->rotationAngle_ = 0;
	else
	{
		if (this->rotationAngle_ > 360) this->rotationAngle_ -= 360;
	}

	this->rotationMatrix_ = glm::rotate(static_cast<float>(glm::radians(this->rotationAngle_)), this->rotationAxis_);
}

///<summary>Изменяет размер объекта с заданной скоростью.</summary>
///<param name = 'velocity_xyz'>Скорость изменения размера по всем осям.</param>
void IEntity::scale(const float velocity_xyz)
{
	this->scaleCoeffs_ += glm::vec3(velocity_xyz * deltaTime);

	if (this->scaleCoeffs_.x == 0 || this->scaleCoeffs_.y == 0 || this->scaleCoeffs_.z == 0) logger.log("IEntity::scaleObject", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scaleCoeffs_.x < 0 || this->scaleCoeffs_.y < 0 || this->scaleCoeffs_.z < 0) logger.log("IEntity::scaleObject", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>Изменяет размер объекта с заданной скоростью.</summary>
///<param name = 'velocity_x'>Скорость изменения размера по X.</param>
///<param name = 'velocity_y'>Скорость изменения размера по Y.</param>
///<param name = 'velocity_z'>Скорость изменения размера по Z.</param>
void IEntity::scale(const float velocity_x, const float velocity_y, const float velocity_z)
{
	this->scaleCoeffs_ += glm::vec3(velocity_x * deltaTime, velocity_y * deltaTime, velocity_z * deltaTime);

	if (this->scaleCoeffs_.x == 0 || this->scaleCoeffs_.y == 0 || this->scaleCoeffs_.z == 0) logger.log("IEntity::scaleModel", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scaleCoeffs_.x < 0 || this->scaleCoeffs_.y < 0 || this->scaleCoeffs_.z < 0) logger.log("IEntity::scaleModel", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>Задаёт позицию сущности.</summary>
///<param name = 'position'>Позиция.</param>
void IEntity::setPosition(const glm::vec3 position)
{
	this->position_ = position;
	this->translationMatrix_ = glm::translate(this->position_);
}

///<summary>Задаёт поворот сущности.</summary>
///<param name = 'angle'>Угол поворота в градусах.</param>
///<param name = 'axis'>Ось поворота.</param>
void IEntity::setRotation(const double angle, const glm::vec3 axis)
{
	this->rotationAngle_ = angle;
	this->rotationAxis_ = axis;

	if (this->rotationAngle_ == 360) this->rotationAngle_ = 0;
	else
	{
		if (this->rotationAngle_ > 360) this->rotationAngle_ -= 360;
	}

	this->rotationMatrix_ = glm::rotate(static_cast<float>(glm::radians(this->rotationAngle_)), this->rotationAxis_);
}

///<summary>Задаёт размер сущности от исходного.</summary>
///<param name = 'scale'>Коэффициент размера.</param>
void IEntity::setScale(const glm::vec3 scale)
{
	this->scaleCoeffs_ = scale;

	if (this->scaleCoeffs_.x == 0 || this->scaleCoeffs_.y == 0 || this->scaleCoeffs_.z == 0) logger.log("IEntity::setScale", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scaleCoeffs_.x < 0 || this->scaleCoeffs_.y < 0 || this->scaleCoeffs_.z < 0) logger.log("IEntity::setScale", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>Возвращает имя модели.</summary>
std::string IEntity::getName() const
{
	return this->name_;
}

///<summary>Возвращает матрицу модели.</summary>
glm::mat4 IEntity::getModelMatrix() const
{
	return this->translationMatrix_ * this->rotationMatrix_ * this->scaleMatrix_;
}

///<summary>Возвращает позицию модели.</summary>
glm::vec3 IEntity::getPosition() const
{
	return this->position_;
}

///<summary>Возвращает размер.</summary>
glm::vec3 IEntity::getScale() const
{
	return this->scaleCoeffs_;
}