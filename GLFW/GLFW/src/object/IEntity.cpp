#include "IEntity.h"

///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
///<param name = 'velocity_x'>�������� �� ��� x.</param>
///<param name = 'velocity_y'>�������� �� ��� y.</param>
///<param name = 'velocity_z'>�������� �� ��� z.</param>
void IEntity::move(const float velocity_x, const float velocity_y, const float velocity_z)
{
	this->position_ += glm::vec3(velocity_x * deltaTime, velocity_y * deltaTime, velocity_z * deltaTime);
	this->translationMatrix_ = glm::translate(this->position_);
}

///<summary>������� ������ � �������� ���������.</summary>
///<param name = 'angle'>�������� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
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

///<summary>�������� ������ ������� � �������� ���������.</summary>
///<param name = 'velocity_xyz'>�������� ��������� ������� �� ���� ����.</param>
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

///<summary>�������� ������ ������� � �������� ���������.</summary>
///<param name = 'velocity_x'>�������� ��������� ������� �� X.</param>
///<param name = 'velocity_y'>�������� ��������� ������� �� Y.</param>
///<param name = 'velocity_z'>�������� ��������� ������� �� Z.</param>
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

///<summary>����� ������� ��������.</summary>
///<param name = 'position'>�������.</param>
void IEntity::setPosition(const glm::vec3 position)
{
	this->position_ = position;
	this->translationMatrix_ = glm::translate(this->position_);
}

///<summary>����� ������� ��������.</summary>
///<param name = 'angle'>���� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
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

///<summary>����� ������ �������� �� ���������.</summary>
///<param name = 'scale'>����������� �������.</param>
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

///<summary>���������� ��� ������.</summary>
std::string IEntity::getName() const
{
	return this->name_;
}

///<summary>���������� ������� ������.</summary>
glm::mat4 IEntity::getModelMatrix() const
{
	return this->translationMatrix_ * this->rotationMatrix_ * this->scaleMatrix_;
}

///<summary>���������� ������� ������.</summary>
glm::vec3 IEntity::getPosition() const
{
	return this->position_;
}

///<summary>���������� ������.</summary>
glm::vec3 IEntity::getScale() const
{
	return this->scaleCoeffs_;
}