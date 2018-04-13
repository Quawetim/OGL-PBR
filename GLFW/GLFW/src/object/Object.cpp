#include "Object.h"

///<summary>�����������.</summary>
Object::Object()
{
	this->name_ = "Name does not set.";

	this->translationMatrix_ = glm::mat4(1.0f);
	this->rotationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
	this->position_ = glm::vec3(0.0f);
	this->rotationAngle_ = 0.0f;
	this->rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scaleCoeffs_ = glm::vec3(1.0f);
}

///<summary>�����������.</summary>
///<param name = 'name'>��� �������.</param>
Object::Object(std::string name, Model *model)
{
	this->name_ = name;
	this->models_.push_back(model);

	this->translationMatrix_ = glm::mat4(1.0f);
	this->rotationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
	this->position_ = glm::vec3(0.0f);
	this->rotationAngle_ = 0.0f;
	this->rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scaleCoeffs_ = glm::vec3(1.0f);
}

///<summary>�����������.</summary>
///<param name = 'name'>��� �������.</param>
Object::Object(std::string name, std::vector<Model*> models)
{
	this->name_ = name;
	this->models_ = models;

	this->translationMatrix_ = glm::mat4(1.0f);
	this->rotationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
	this->position_ = glm::vec3(0.0f);
	this->rotationAngle_ = 0.0f;
	this->rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scaleCoeffs_ = glm::vec3(1.0f);
}

///<summary>����������.</summary>
Object::~Object()
{
	this->models_.clear();
}

///<summary>��������� ������ � �������.</summary>
///<param name = 'model'>������.</param>
void Object::addModel(Model *model)
{
	this->models_.push_back(model);
}

///<summary>��������� ������ �� ������� �� �����.</summary>
///<param name = 'name'>��� ����������� ������.</param>
Model* Object::removeModel(const std::string name)
{
	Model *model = nullptr;

	for (size_t i = 0; i < this->models_.size(); i++)
	{
		if (this->models_[i]->getName() == name)
		{
			model = models_[i];
			this->models_.erase(models_.begin() + i);
			std::vector<Model*>(models_).swap(models_);

			return model;
		}
	}

	std::string msg = "Model \"" + name + "\" not found.";
	logger.log(__FUNCTION__, ErrorType::error, msg);

	return model;
}

///<summary>����� ��������.</summary>
void Object::setMaterial(Material material)
{
	this->material_ = material;
}

///<summary>���������� ��������.</summary>
Material Object::getMaterial() const
{
	return this->material_;
}

///<summary>���������� ������, �� ������� ������� ������.</summary>
const std::vector<Model*>& Object::getModels() const
{
	return this->models_;
}