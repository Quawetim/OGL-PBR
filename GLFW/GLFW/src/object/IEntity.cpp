#include "IEntity.h"

///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
///<param name = 'velocityX'>�������� �� ��� x.</param>
///<param name = 'velocityY'>�������� �� ��� y.</param>
///<param name = 'velocityZ'>�������� �� ��� z.</param>
void IEntity::move(const float velocityX, const float velocityY, const float velocityZ)
{
	this->position += glm::vec3(velocityX * deltaTime, velocityY * deltaTime, velocityZ * deltaTime);
	this->translationMatrix = glm::translate(this->position);
}

///<summary>������� ������ � �������� ���������.</summary>
///<param name = 'angle'>�������� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
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

///<summary>�������� ������ ������� � �������� ���������.</summary>
///<param name = 'scaleXYZ'>�������� ��������� ������� �� ���� ����.</param>
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

///<summary>�������� ������ ������� � �������� ���������.</summary>
///<param name = 'scaleX'>�������� ��������� ������� �� X.</param>
///<param name = 'scaleY'>�������� ��������� ������� �� Y.</param>
///<param name = 'scaleZ'>�������� ��������� ������� �� Z.</param>
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

///<summary>����� ambient ���� ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void IEntity::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� diffuse ���� ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void IEntity::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� specular ���� ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void IEntity::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� ���� (�������) �����.</summary>
///<param name = 'value'>��������.</param>
void IEntity::setShinePower(const float value)
{
	this->shinePower = value;
}

///<summary>����� ������� ��������.</summary>
///<param name = 'position'>�������.</param>
void IEntity::setPosition(const glm::vec3 position)
{
	this->position = position;
	this->translationMatrix = glm::translate(this->position);
}

///<summary>����� ������� ��������.</summary>
///<param name = 'angle'>���� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
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

///<summary>����� ������ �������� �� ���������.</summary>
///<param name = 'scale'>����������� �������.</param>
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

///<summary>���������� ��� ������.</summary>
std::string IEntity::getName() const
{
	return this->name;
}

///<summary>���������� ������� ������.</summary>
glm::mat4 IEntity::getModelMatrix() const
{
	return this->translationMatrix * this->rotationMatrix * this->scaleMatrix;
}

///<summary>���������� ������� ������.</summary>
glm::vec3 IEntity::getPosition() const
{
	return this->position;
}