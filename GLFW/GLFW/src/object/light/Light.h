#pragma once
#include "../../includes/Includes.h"
#include "../Model.h"

///<summary>Абстрактный источник освещения.</summary>
class Light
{
protected:
	///<summary>Diffuse цвет света.</summary>
	glm::vec3 diffuseColor_;

	///<summary>Specular цвет света.</summary>
	glm::vec3 specularColor_;

	///<summary>Мощность.</summary>
	float power_;

public:
	///<summary>Задаёт diffuse цвет света.</summary>
	///<param name = 'diffuseColor'>Цвет.</param>
	void setDiffuseColor(const glm::vec3 diffuseColor);

	///<summary>Задаёт diffuse цвет света.</summary>
	///<param name = 'red'>Красная составляющая.</param>
	///<param name = 'green'>Зелёная составляющая.</param>
	///<param name = 'blue'>Синяя составляющая.</param>
	void setDiffuseColor(const unsigned char red, const unsigned char green, unsigned char blue);

	///<summary>Задаёт specular цвет света.</summary>
	///<param name = 'specularColor'>Цвет.</param>
	void setSpecularColor(const glm::vec3 specularColor);

	///<summary>Задаёт specular цвет света.</summary>
	///<param name = 'red'>Красная составляющая.</param>
	///<param name = 'green'>Зелёная составляющая.</param>
	///<param name = 'blue'>Синяя составляющая.</param>
	void setSpecularColor(const unsigned char red, const unsigned char green, unsigned char blue);

	///<summary>Задаёт мощность источника света.</summary>
	///<param name = 'power'>Мощность.</param>
	void setPower(const float power);

	///<summary>Возвращает diffuse цвет света.</summary>
	glm::vec3 getDiffuseColor() const;

	///<summary>Возвращает specular цвет света.</summary>
	glm::vec3 getSpecularColor() const;
	
	///<summary>Возвращает мощность источника света.</summary>
	float getPower() const;

	///<summary>Возвращает матрицу модели источника света для отрисовки позиции источника.</summary>
	virtual glm::mat4 getModelMatrix() = 0;
};

///<summary>Точечный источник освещения.</summary>
class PointLight : public Light
{
private:
	///<summary>Шейдер для отрисовки модели источника.</summary>
	std::shared_ptr<Shader> shader_;

	///<summary>3D модель источника.</summary>
	std::shared_ptr<Model> model_;

	///<summary>Позиция источника.</summary>
	glm::vec3 position_;

	///<summary>Радиус источника.</summary>
	float radius_;

	///<summary>Матрица перемещения.</summary>
	glm::mat4 translationMatrix_;

	///<summary>Матрица размера.</summary>
	glm::mat4 scaleMatrix_;

public:
	///<summary>Конструктор.</summary>
	///<param name = 'shader'>Шейдер для отрисовки модели источника.</param>
	///<param name = 'model'>3D модель источника.</param>
	PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model);

	///<summary>Конструктор.</summary>
	///<param name = 'shader'>Шейдер для отрисовки модели источника.</param>
	///<param name = 'model'>3D модель источника.</param>
	///<param name = 'position'>Позиция источника.</param>
	///<param name = 'radius'>Радиус источника.</param>
	///<param name = 'diffuseColor'>Diffuse цвет света источника.</param>
	///<param name = 'specularColor'>Specular цвет света источника.</param>
	///<param name = 'power'>Мощность источника.</param>
	PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model, glm::vec3 position, float radius, glm::vec3 diffuseColor, glm::vec3 specularColor, float power);

	///<summary>Задаёт позицию источника.</summary>
	///<param name = 'position'>Позиция.</param>
	void setPosition(glm::vec3 position);

	///<summary>Задаёт радиус источника.</summary>
	///<param name = 'radius'>Радиус.</param>
	void setRadius(float radius);	

	///<summary>Возвращает позицию источника.</summary>
	glm::vec3 getPosition() const;

	///<summary>Возвращает радиус источника.</summary>
	float getRadius() const;

	///<summary>Возвращает матрицу модели источника света для отрисовки позиции источника.</summary>
	glm::mat4 getModelMatrix();

	///<summary>Возвращает шейдер.</summary>
	std::shared_ptr<Shader> getShader() const;

	///<summary>Возвращает 3D модель.</summary>
	std::shared_ptr<Model> getModel() const;
};