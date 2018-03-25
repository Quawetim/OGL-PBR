#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"
#include "Model.h"

///<summary>�������� ��������. ��������� �������.</summary>
class Object : public IEntity
{
	///<summary>������, �� ������� ������� ������.</summary>
	std::vector<Model*> models;									// �� ����� ������? ������� ����� ������������� �� ���������� ���������...

	///<summary>������������ �������� ����� �������.<./summary>
	bool object_material = false;

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

	///<summary>����� �������� �������.</summary>
	///<param name = 'material'>��������.</param>
	void setMaterial(QMaterial material);

	///<summary>����� ���� ������������� ��������� ����� ������� ��� ���� �������, 
	///<para>������������� �������.</para>
	///<para>��������� ���� ����� ������, �� ���� ����� �������.</para>
	///</summary>
	void useObjectMaterial();

	///<summary>��������� ������ � �������.</summary>
	///<param name = 'model'>������.</param>
	void addModel(Model *model);

	///<summary>��������� ������ �� ������� �� �����.</summary>
	///<param name = 'name'>��� ����������� ������.</param>
	Model* deleteModel(const std::string name);
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