#pragma once
#include "..\includes\Includes.h"

class IEntity
{
protected:
	///<summary>��� ������.</summary>
	std::string name;

	///<summary>������� ������������.</summary>
	glm::mat4 translationMatrix;

	///<summary>������� ��������.</summary>
	glm::mat4 rotationMatrix;

	///<summary>������� �������.</summary>
	glm::mat4 scaleMatrix;

	///<summary>������� ������.</summary>
	glm::vec3 position;

	///<summary>���� ��������.</summary>
	double rotationAngle;

	///<summary>��� ��������.</summary>
	glm::vec3 rotationAxis;

	///<summary>����������� ������� �� ������ �� ����.</summary>
	glm::vec3 scaleCoefficients;

	///<summary>Ambient ����.</summary>
	glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);

	///<summary>Diffuse ����.</summary>
	glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);

	///<summary>Specular ����.</summary>
	glm::vec3 specularColor = glm::vec3(0.9f, 0.9f, 0.9f);

	///<summary>���� (�������) �����.</summary>
	float shinePower = 8.0f;

public:
	///<summary>��������� ��������.</summary>
	///<param name = 'shader'>������.</param>
	virtual void draw(const Shader shader) = 0;

	///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
	///<param name = 'velocityX'>�������� �� ��� x.</param>
	///<param name = 'velocityY'>�������� �� ��� y.</param>
	///<param name = 'velocityZ'>�������� �� ��� z.</param>
	void move(const float velocityX, const float velocityY, const float velocityZ);

	///<summary>������� ������ � �������� ���������.</summary>
	///<param name = 'angle'>�������� �������� � ��������.</param>
	///<param name = 'axis'>��� ��������.</param>
	void rotate(const double angle, const glm::vec3 axis);

	///<summary>�������� ������ ������� � �������� ���������.</summary>
	///<param name = 'scaleXYZ'>�������� ��������� ������� �� ���� ����.</param>
	void scale(const float scaleXYZ);

	///<summary>�������� ������ ������� � �������� ���������.</summary>
	///<param name = 'scaleX'>�������� ��������� ������� �� X.</param>
	///<param name = 'scaleY'>�������� ��������� ������� �� Y.</param>
	///<param name = 'scaleZ'>�������� ��������� ������� �� Z.</param>
	void scale(const float scaleX, const float scaleY, const float scaleZ);

	///<summary>����� ambient ���� ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	virtual void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue) = 0;

	///<summary>����� diffuse ���� ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	virtual void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue) = 0;

	///<summary>����� specular ���� ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	virtual void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue) = 0;

	///<summary>����� ���� (�������) �����.</summary>
	///<param name = 'value'>��������.</param>
	virtual void setShinePower(const float value) = 0;

	///<summary>����� ������� ��������.</summary>
	///<param name = 'position'>�������.</param>
	virtual void setPosition(const glm::vec3 position);

	///<summary>����� ������� ��������.</summary>
	///<param name = 'angle'>���� �������� � ��������.</param>
	///<param name = 'axis'>��� ��������.</param>
	virtual void setRotation(const double angle, const glm::vec3 axis);

	///<summary>����� ������ �������� �� ���������.</summary>
	///<param name = 'scale'>����������� �������.</param>
	virtual void setScale(const glm::vec3 scale);

	///<summary>���������� ��� ������.</summary>
	std::string getName() const;

	///<summary>���������� ������� ������.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>���������� ������� ������.</summary>
	glm::vec3 getPosition() const;
};
