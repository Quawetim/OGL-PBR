#include "Object.h"

///<summary>Конструктор.</summary>
///<param name = 'name'>Имя объекта.</param>
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

///<summary>Конструктор.</summary>
///<param name = 'name'>Имя объекта.</param>
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

///<summary>Деструктор.</summary>
Object::~Object()
{
	this->models.clear();
}

///<summary>Отрисовка объекта.
///<para>Если задан флаг "objectColors", то все модели рисуются с цветом объекта, </para>
///<para>иначе - с заданным цветом модели.</para>
///</summary>
///<param name = 'shader'>Шейдер.</param>
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

///<summary>Задаёт флаг использования цвета этого объекта для всех моделей, 
///<para>принадлежащих объекту.</para>
///<para>Приоритет выше флага модели, но ниже флага текстур.</para>
///</summary>
///<param name = 'use'>Использовать цвет объекта или нет.</param>
void Object::setObjectColorsFlag(const bool use)
{
	this->object_colors_flag = use;
}

///<summary>Задаёт ambient цвет всем мешам модели в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Object::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor = glm::vec3(red / 255.0f, green / 255.f, blue / 255.0f);
}

///<summary>Задаёт diffuse цвет всем мешам модели в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Object::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor = glm::vec3(red / 255.0f, green / 255.f, blue / 255.0f);
}

///<summary>Задаёт specular цвет всем мешам модели в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Object::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor = glm::vec3(red / 255.0f, green / 255.f, blue / 255.0f);
}

///<summary>Задаёт силу (яркость) блика всем мешам модели.</summary>
///<param name = 'value'>Значение.</param>
void Object::setShinePower(const float value)
{
	this->shinePower = value;
}

///<summary>Добавляет модель к объекту.</summary>
///<param name = 'model'>Модель.</param>
void Object::addModel(Model *model)
{
	this->models.push_back(model);
}

///<summary>Извлекает модель из объекта пл имени.</summary>
///<param name = 'name'>Имя извлекаемой модели.</param>
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

///<summary>Возвращает флаг использования цвета этого объекта для всех моделей, 
///<para>принадлежащих объекту.</para>
///</summary>
bool Object::getObjectColorsFlag() const
{
	return this->object_colors_flag;
}