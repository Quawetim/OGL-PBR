#include "Object.h"

///<summary>Конструктор.</summary>
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

///<summary>Конструктор.</summary>
///<param name = 'name'>Имя объекта.</param>
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

///<summary>Конструктор.</summary>
///<param name = 'name'>Имя объекта.</param>
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

///<summary>Деструктор.</summary>
Object::~Object()
{
	this->models_.clear();
}

///<summary>Добавляет модель к объекту.</summary>
///<param name = 'model'>Модель.</param>
void Object::addModel(Model *model)
{
	this->models_.push_back(model);
}

///<summary>Извлекает модель из объекта пл имени.</summary>
///<param name = 'name'>Имя извлекаемой модели.</param>
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

///<summary>Задаёт материал.</summary>
void Object::setMaterial(Material material)
{
	this->material_ = material;
}

///<summary>Возвращает материал.</summary>
Material Object::getMaterial() const
{
	return this->material_;
}

///<summary>Возвращает модели, из которых состоит объект.</summary>
const std::vector<Model*>& Object::getModels() const
{
	return this->models_;
}