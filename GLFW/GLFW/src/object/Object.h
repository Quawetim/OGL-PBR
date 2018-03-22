#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"
#include "Model.h"

///<summary>�������� ��������. ��������� �������.</summary>
class Object : public IEntity
{
	///<summary>������, �� ������� ������� ������.</summary>
	std::vector<Model*> models;									// �� ����� ������? ������� ����� ������������� �� ���������� ���������...

	///<summary>������������ ���� ����� �������.<./summary>
	bool object_colors_flag = false;

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

	///<summary>��������� �������.
	///<para>���� ����� ���� "object_colors_flag", �� ��� ������ �������� � ������ �������, </para>
	///<para>����� - � �������� ������ ������.</para>
	///</summary>
	///<param name = 'shader'>������.</param>
	void draw(Shader shader);

	///<summary>����� ���� ������������� ����� ����� ������� ��� ���� �������, 
	///<para>������������� �������.</para>
	///<para>��������� ���� ����� ������, �� ���� ����� �������.</para>
	///</summary>
	///<param name = 'use'>������������ ���� ������� ��� ���.</param>
	void setObjectColorsFlag(const bool use);

	///<summary>����� ambient ���� ���� ����� ������ � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� diffuse ���� ���� ����� ������ � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� specular ���� ���� ����� ������ � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� ���� (�������) ����� ���� ����� ������.</summary>
	///<param name = 'value'>��������.</param>
	void setShinePower(const float value);

	///<summary>��������� ������ � �������.</summary>
	///<param name = 'model'>������.</param>
	void addModel(Model *model);

	///<summary>��������� ������ �� ������� �� �����.</summary>
	///<param name = 'name'>��� ����������� ������.</param>
	Model* deleteModel(const std::string name);

	///<summary>���������� ���� ������������� ����� ����� ������� ��� ���� �������, 
	///<para>������������� �������.</para>
	///</summary>
	bool getObjectColorsFlag() const;
};

class StarDestroyer : public Object
{

};

class StarFighter : public Object
{

};

class ImperialStarDestroyer : public StarDestroyer
{

};

class Venator : public StarDestroyer
{

};

class TieFighter : public StarFighter
{

};

class ARC170 : public StarFighter
{

};