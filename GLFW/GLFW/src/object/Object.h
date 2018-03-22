#pragma once
#include "..\includes\Includes.h"
#include "Model.h"

///<summary>�������� ��������. ��������� �������.</summary>
class Object
{
	///<summary>��� ������.</summary>
	std::string objectName;
 
	///<summary>������, �� ������� ������� ������.</summary>
	std::vector<Model*> models;									// �� ����� ������? ������� ����� ������������� �� ���������� ���������...

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
	glm::vec3 scale;

public:
	///<summary>�����������.</summary>
	Object() {};

	///<summary>�����������.</summary>
	///<param name = 'name'>��� �������.</param>
	///<param name = 'model'>������, ������������ �����.</param>
	Object(std::string name, Model *model);

	///<summary>�����������.</summary>
	///<param name = 'name'>��� �������.</param>
	///<param name = 'models'>������ �������, ������������ ������.</param>
	Object(std::string name, std::vector<Model*> models);

	///<summary>����������.</summary>
	~Object();

	///<summary>��������� �������.</summary>
	///<param name ='shader'>������.</param>
	virtual void draw(Shader shader);

	///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
	///<param name = 'velocityX'>�������� �� ��� x.</param>
	///<param name = 'velocityY'>�������� �� ��� y.</param>
	///<param name = 'velocityZ'>�������� �� ��� z.</param>
	void moveObject(const float velocityX, const float velocityY, const float velocityZ);

	///<summary>������� ������ � �������� ���������.</summary>
	///<param name = 'angle'>�������� �������� � ��������.</param>
	///<param name = 'axis'>��� ��������.</param>
	void rotateObject(const double angle, const glm::vec3 axis);

	///<summary>�������� ������ ������� � �������� ���������.</summary>
	///<param name = 'scaleXYZ'>�������� ��������� ������� �� ���� ����.</param>
	void scaleObject(const float scaleXYZ);

	///<summary>�������� ������ ������� � �������� ���������.</summary>
	///<param name = 'scaleX'>�������� ��������� ������� �� X.</param>
	///<param name = 'scaleY'>�������� ��������� ������� �� Y.</param>
	///<param name = 'scaleZ'>�������� ��������� ������� �� Z.</param>
	void scaleObject(const float scaleX, const float scaleY, const float scaleZ);

	///<summary>����� ������� �������.</summary>
	///<param name = 'position'>�������.</param>
	void setPosition(const glm::vec3 position);

	///<summary>����� ������� �������.</summary>
	///<param name = 'angle'>���� �������� � ��������.</param>
	///<param name = 'axis'>��� ��������.</param>
	void setRotation(const double angle, const glm::vec3 axis);

	///<summary>����� ������ ������� �� ���������.</summary>
	///<param name = 'scale'>����������� �������.</param>
	void setScale(const glm::vec3 scale);

	///<summary>��������� ������ � �������.</summary>
	///<param name = 'model'>������.</param>
	void addModel(Model *model);

	///<summary>��������� ������ �� ������� �� �����.</summary>
	///<param name = 'name'>��� ����������� ������.</param>
	Model* deleteModel(const std::string name);

	///<summary>���������� ��� ������.</summary>
	std::string getName() const;

	///<summary>���������� ������� ������.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>���������� ������� ������.</summary>
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