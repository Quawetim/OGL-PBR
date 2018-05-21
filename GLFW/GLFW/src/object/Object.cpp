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
Object::Object(std::string name, std::shared_ptr<Model> model)
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
Object::Object(std::string name, std::vector<std::shared_ptr<Model>> models)
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
void Object::addModel(std::shared_ptr<Model> model)
{
	this->models_.push_back(model);
}

///<summary>��������� ������ �� ������� �o �����.</summary>
///<param name = 'name'>��� ����������� ������.</param>
std::shared_ptr<Model> Object::removeModel(const std::string name)
{
	std::shared_ptr<Model> model = nullptr;

	for (size_t i = 0; i < this->models_.size(); i++)
	{
		if (this->models_[i]->getName() == name)
		{
			model = this->models_[i];
			this->models_.erase(this->models_.begin() + i);
			std::vector<std::shared_ptr<Model>>(models_).swap(this->models_);

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
const std::vector<std::shared_ptr<Model>> Object::getModels() const
{
	return this->models_;
}

const std::shared_ptr<Model> Object::getModelByName(const std::string name) const
{

}