#include "Object.h"

///<summary>�����������.</summary>
///<param name = 'name'>��� �������.</param>
Object::Object(std::string name, Model *model)
{
	this->name = name;
	this->models.push_back(model);

	this->translationMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);
	this->scaleMatrix = glm::mat4(1.0f);
	this->position = glm::vec3(0.0f);
	this->rotationAngle = 0.0f;
	this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scaleCoefficients = glm::vec3(1.0f);
}

///<summary>�����������.</summary>
///<param name = 'name'>��� �������.</param>
Object::Object(std::string name, std::vector<Model*> models)
{
	this->name = name;
	this->models = models;

	this->translationMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);
	this->scaleMatrix = glm::mat4(1.0f);
	this->position = glm::vec3(0.0f);
	this->rotationAngle = 0.0f;
	this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scaleCoefficients = glm::vec3(1.0f);
}

///<summary>����������.</summary>
Object::~Object()
{
	this->models.clear();
}

///<summary>��������� �������.
///<para>���� ����� ���� "objectColors", �� ��� ������ �������� � ������ �������, </para>
///<para>����� - � �������� ������ ������.</para>
///</summary>
///<param name = 'shader'>������.</param>
void Object::draw(Shader shader)
{
	if (this->models.empty()) logger.log("Object::draw", QErrorType::error, "Models list is empty. Nothing to render.");
	else
	{
		for (size_t i = 0; i < this->models.size(); i++)
		{
			if (object_colors_flag) this->models[i]->draw(shader, this->ambientColor, this->diffuseColor, this->specularColor, this->shinePower);
			else this->models[i]->draw(shader);
		}
	}
}

///<summary>����� ���� ������������� ����� ����� ������� ��� ���� �������, 
///<para>������������� �������.</para>
///<para>��������� ���� ����� ������, �� ���� ����� �������.</para>
///</summary>
///<param name = 'use'>������������ ���� ������� ��� ���.</param>
void Object::setObjectColorsFlag(const bool use)
{
	this->object_colors_flag = use;
}

///<summary>����� ambient ���� ���� ����� ������ � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Object::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor = glm::vec3(red / 255.0f, green / 255.f, blue / 255.0f);
}

///<summary>����� diffuse ���� ���� ����� ������ � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Object::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor = glm::vec3(red / 255.0f, green / 255.f, blue / 255.0f);
}

///<summary>����� specular ���� ���� ����� ������ � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Object::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor = glm::vec3(red / 255.0f, green / 255.f, blue / 255.0f);
}

///<summary>����� ���� (�������) ����� ���� ����� ������.</summary>
///<param name = 'value'>��������.</param>
void Object::setShinePower(const float value)
{
	this->shinePower = value;
}

///<summary>��������� ������ � �������.</summary>
///<param name = 'model'>������.</param>
void Object::addModel(Model *model)
{
	this->models.push_back(model);
}

///<summary>��������� ������ �� ������� �� �����.</summary>
///<param name = 'name'>��� ����������� ������.</param>
Model* Object::deleteModel(const std::string name)
{
	Model *model = nullptr;

	for (size_t i = 0; i < this->models.size(); i++)
	{
		if (this->models[i]->getName() == name)
		{
			model = models[i];
			this->models.erase(models.begin() + i);
			std::vector<Model*>(models).swap(models);

			return model;
		}
	}

	std::string msg = "Model \"" + name + "\" not found.";
	logger.log("Object::popModel", QErrorType::error, msg);

	return model;
}

///<summary>���������� ���� ������������� ����� ����� ������� ��� ���� �������, 
///<para>������������� �������.</para>
///</summary>
bool Object::getObjectColorsFlag() const
{
	return this->object_colors_flag;
}