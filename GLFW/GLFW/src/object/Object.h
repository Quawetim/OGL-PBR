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
	std::vector<std::shared_ptr<Model>> models_;

public:
	///<summary>����������� ��-���������.</summary>
	Object();

	///<summary>�����������.</summary>
	///<param name = 'name'>��� �������.</param>
	///<param name = 'model'>������, ������������ �����.</param>
	Object(std::string name, std::shared_ptr<Model> model);

	///<summary>�����������.</summary>
	///<param name = 'name'>��� �������.</param>
	///<param name = 'models'>������ �������, ������������ ������.</param>
	Object(std::string name, std::vector<std::shared_ptr<Model>> models);

	///<summary>����������.</summary>
	~Object();

	///<summary>��������� ������ � �������.</summary>
	///<param name = 'model'>������.</param>
	void addModel(std::shared_ptr<Model> model);

	///<summary>��������� ������ �� ������� �� �����.</summary>
	///<param name = 'name'>��� ����������� ������.</param>
	std::shared_ptr<Model> removeModel(const std::string name);

	///<summary>����� ��������.</summary>
	void setMaterial(Material material);

	///<summary>���������� ��������.</summary>
	Material getMaterial() const;

	///<summary>���������� ������, �� ������� ������� ������.</summary>
	const std::vector<std::shared_ptr<Model>> getModels() const;

	///<summary>���������� ������ �� �����. ��� ������, ���� �� �������.</summary>
	///<param name = 'name'>��� ������.</param>
	const std::shared_ptr<Model> getModelByName(const std::string name) const;
};