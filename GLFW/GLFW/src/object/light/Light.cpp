#include "Light.h"

void Light::setDiffuseColor(const glm::vec3 diffuseColor)
{
	this->diffuseColor_ = diffuseColor;
}

void Light::setDiffuseColor(const unsigned char red, const unsigned char green, unsigned char blue)
{
	this->diffuseColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

void Light::setSpecularColor(const glm::vec3 specularColor)
{
	this->specularColor_ = specularColor;
}

void Light::setSpecularColor(const unsigned char red, const unsigned char green, unsigned char blue)
{
	this->specularColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

void Light::setPower(float power)
{
	this->power_ = power;
}

glm::vec3 Light::getDiffuseColor() const
{
	return this->diffuseColor_;
}

glm::vec3 Light::getSpecularColor() const
{
	return this->specularColor_;
}

float Light::getPower() const
{
	return this->power_;
}

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

PointLight::PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model, glm::vec3 position, float radius, glm::vec3 diffuseColor, glm::vec3 specularColor, float power) :
	shader_(shader), model_(model), position_(position), radius_(radius)
{
	this->diffuseColor_ = diffuseColor;
	this->specularColor_ = specularColor;
	this->power_ = power;

	this->translationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
}

void PointLight::setPosition(glm::vec3 position)
{
	this->position_ = position;
	this->translationMatrix_ = glm::translate(position);
}

void PointLight::setRadius(float radius)
{
	this->radius_ = radius;
	this->scaleMatrix_ = glm::scale(glm::vec3(radius));
}

glm::vec3 PointLight::getPosition() const
{
	return this->position_;
}

float PointLight::getRadius() const
{
	return this->radius_;
}

glm::mat4 PointLight::getModelMatrix()
{
	return this->translationMatrix_ * this->scaleMatrix_;
}

std::shared_ptr<Shader> PointLight::getShader() const
{
	return this->shader_;
}

std::shared_ptr<Model> PointLight::getModel() const
{
	return this->model_;
}