#pragma once
#include "..\includes\Includes.h"
#include "Model.h"

///<summary>�������� ��������. ��������� �������.</summary>
class Object : public IEntity
{
private:
	///<summary>��������.</summary>
	Material material_;

protected:
	///<summary>������, �� ������� ������� ������.</summary>
	std::vector<Model*> models_;									// �� ����� ������? ������� ����� ������������� �� ���������� ���������...

public:
	///<summary>�����������.</summary>
	Object();

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

	///<summary>��������� ������ � �������.</summary>
	///<param name = 'model'>������.</param>
	void addModel(Model *model);

	///<summary>��������� ������ �� ������� �� �����.</summary>
	///<param name = 'name'>��� ����������� ������.</param>
	Model* removeModel(const std::string name);

	///<summary>����� ��������.</summary>
	void setMaterial(Material material);

	///<summary>���������� ��������.</summary>
	Material getMaterial() const;

	///<summary>���������� ������, �� ������� ������� ������.</summary>
	const std::vector<Model*>& getModels() const;
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