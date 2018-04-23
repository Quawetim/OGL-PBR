#include "Light.h"
///<summary>Задаёт diffuse цвет света.</summary>
///<param name = 'diffuseColor'>Цвет.</param>
void Light::setDiffuseColor(const glm::vec3 diffuseColor)
{
	this->diffuseColor_ = diffuseColor;
}

///<summary>Задаёт diffuse цвет света.</summary>
///<param name = 'red'>Красная составляющая.</param>
///<param name = 'green'>Зелёная составляющая.</param>
///<param name = 'blue'>Синяя составляющая.</param>
void Light::setDiffuseColor(const unsigned char red, const unsigned char green, unsigned char blue)
{
	this->diffuseColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт specular цвет света.</summary>
///<param name = 'specularColor'>Цвет.</param>
void Light::setSpecularColor(const glm::vec3 specularColor)
{
	this->specularColor_ = specularColor;
}

///<summary>Задаёт specular цвет света.</summary>
///<param name = 'red'>Красная составляющая.</param>
///<param name = 'green'>Зелёная составляющая.</param>
///<param name = 'blue'>Синяя составляющая.</param>
void Light::setSpecularColor(const unsigned char red, const unsigned char green, unsigned char blue)
{
	this->specularColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт мощность источника света.</summary>
///<param name = 'power'>Мощность.</param>
void Light::setPower(float power)
{
	this->power_ = power;
}

///<summary>Возвращает diffuse цвет света.</summary>
glm::vec3 Light::getDiffuseColor() const
{
	return this->diffuseColor_;
}

///<summary>Возвращает specular цвет света.</summary>
glm::vec3 Light::getSpecularColor() const
{
	return this->specularColor_;
}

///<summary>Возвращает мощность источника света.</summary>
float Light::getPower() const
{
	return this->power_;
}

///<summary>Конструктор.</summary>
///<param name = 'shader'>Шейдер для отрисовки модели источника.</param>
///<param name = 'model'>3D модель источника.</param>
PointLight::PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model) : shader_(shader), model_(model)
{	
	this->position_ = glm::vec3(0.0f);
	this->radius_ = 1.0f;

	this->diffuseColor_ = glm::vec3(0.9f);
	this->specularColor_ = glm::vec3(0.95f);
	this->power_ = 360.0f;

	this->translationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
}

///<summary>Конструктор.</summary>
///<param name = 'shader'>Шейдер для отрисовки модели источника.</param>
///<param name = 'model'>3D модель источника.</param>
///<param name = 'position'>Позиция источника.</param>
///<param name = 'radius'>Радиус источника.</param>
///<param name = 'diffuseColor'>Diffuse цвет света источника.</param>
///<param name = 'specularColor'>Specular цвет света источника.</param>
///<param name = 'power'>Мощность источника.</param>
PointLight::PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model, glm::vec3 position, float radius, glm::vec3 diffuseColor, glm::vec3 specularColor, float power) :
	shader_(shader), model_(model), position_(position), radius_(radius)
{
	this->diffuseColor_ = diffuseColor;
	this->specularColor_ = specularColor;
	this->power_ = power;

	this->translationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
}

///<summary>Задаёт позицию источника.</summary>
///<param name = 'position'>Позиция.</param>
void PointLight::setPosition(glm::vec3 position)
{
	this->position_ = position;
	this->translationMatrix_ = glm::translate(position);
}

///<summary>Задаёт радиус источника.</summary>
///<param name = 'radius'>Радиус.</param>
void PointLight::setRadius(float radius)
{
	this->radius_ = radius;
	this->scaleMatrix_ = glm::scale(glm::vec3(radius));
}

///<summary>Возвращает позицию источника.</summary>
glm::vec3 PointLight::getPosition() const
{
	return this->position_;
}

///<summary>Возвращает радиус источника.</summary>
float PointLight::getRadius() const
{
	return this->radius_;
}

///<summary>Возвращает матрицу модели источника света для отрисовки позиции источника.</summary>
glm::mat4 PointLight::getModelMatrix()
{
	return this->translationMatrix_ * this->scaleMatrix_;
}

///<summary>Возвращает шейдер.</summary>
std::shared_ptr<Shader> PointLight::getShader() const
{
	return this->shader_;
}

///<summary>Возвращает 3D модель.</summary>
std::shared_ptr<Model> PointLight::getModel() const
{
	return this->model_;
}