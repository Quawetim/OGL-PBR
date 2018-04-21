#pragma once
#include "../../includes/Includes.h"
#include "../Model.h"

class Light
{
protected:
	glm::vec3 diffuseColor_;
	glm::vec3 specularColor_;

	float power_;

public:
	void setDiffuseColor(const glm::vec3 diffuseColor);

	void setDiffuseColor(const unsigned char red, const unsigned char green, unsigned char blue);

	void setSpecularColor(const glm::vec3 specularColor);

	void setSpecularColor(const unsigned char red, const unsigned char green, unsigned char blue);

	void setPower(const float power);

	glm::vec3 getDiffuseColor() const;

	glm::vec3 getSpecularColor() const;

	float getPower() const;

	virtual glm::mat4 getModelMatrix() = 0;
};

class PointLight : public Light
{
private:
	std::shared_ptr<Shader> shader_;

	std::shared_ptr<Model> model_;

	glm::vec3 position_;
	float radius_;

	glm::mat4 translationMatrix_;
	glm::mat4 scaleMatrix_;

public:
	PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model);

	PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model, glm::vec3 position, float radius, glm::vec3 diffuseColor, glm::vec3 specularColor, float power);

	void setPosition(glm::vec3 position);

	void setRadius(float radius);	

	glm::vec3 getPosition() const;

	float getRadius() const;

	glm::mat4 getModelMatrix();

	std::shared_ptr<Shader> getShader() const;

	std::shared_ptr<Model> getModel() const;
};