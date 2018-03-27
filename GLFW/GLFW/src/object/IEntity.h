#pragma once
#include "..\includes\Includes.h"

class IEntity
{
protected:
	///<summary>��� ������.</summary>
	std::string name_;

	///<summary>������� ������������.</summary>
	glm::mat4 translationMatrix_;

	///<summary>������� ��������.</summary>
	glm::mat4 rotationMatrix_;

	///<summary>������� �������.</summary>
	glm::mat4 scaleMatrix_;

	///<summary>������� ������.</summary>
	glm::vec3 position_;

	///<summary>���� ��������.</summary>
	double rotationAngle_;

	///<summary>��� ��������.</summary>
	glm::vec3 rotationAxis_;

	///<summary>����������� ������� �� ������ �� ����.</summary>
	glm::vec3 scaleCoeffs_;

	///<summary>��������.</summary>
	QMaterial material_;

public:
	///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
	///<param name = 'velocity_x'>�������� �� ��� x.</param>
	///<param name = 'velocity_y'>�������� �� ��� y.</param>
	///<param name = 'velocity_z'>�������� �� ��� z.</param>
	void move(const float velocity_x, const float velocity_y, const float velocity_z);

	///<summary>������� ������ � �������� ���������.</summary>
	///<param name = 'angle'>�������� �������� � ��������.</param>
	///<param name = 'axis'>��� ��������.</param>
	void rotate(const double angle, const glm::vec3 axis);

	///<summary>�������� ������ ������� � �������� ���������.</summary>
	///<param name = 'velocity_xyz'>�������� ��������� ������� �� ���� ����.</param>
	void scale(const float velocity_xyz);

	///<summary>�������� ������ ������� � �������� ���������.</summary>
	///<param name = 'velocity_x'>�������� ��������� ������� �� X.</param>
	///<param name = 'velocity_y'>�������� ��������� ������� �� Y.</param>
	///<param name = 'velocity_z'>�������� ��������� ������� �� Z.</param>
	void scale(const float velocity_x, const float velocity_y, const float velocity_z);

	///<summary>����� ambient ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	virtual void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� diffuse ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	virtual void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� specular ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	virtual void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� ���� (�������) �����.</summary>
	///<param name = 'shininess'>��������.</param>
	virtual void setShininess(const float shininess);

	///<summary>����� �������.</summary>
	///<param name = 'position'>�������.</param>
	virtual void setPosition(const glm::vec3 position);

	///<summary>����� �������.</summary>
	///<param name = 'angle'>���� �������� � ��������.</param>
	///<param name = 'axis'>��� ��������.</param>
	virtual void setRotation(const double angle, const glm::vec3 axis);

	///<summary>����� ������ �� ���������.</summary>
	///<param name = 'scale'>����������� �������.</param>
	virtual void setScale(const glm::vec3 scale);

	///<summary>���������� ���.</summary>
	std::string getName() const;

	///<summary>���������� ������� ������.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>���������� �������.</summary>
	glm::vec3 getPosition() const;

	///<summary>���������� ������.</summary>
	glm::vec3 getScale() const;
};
