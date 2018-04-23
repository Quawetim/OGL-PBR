#pragma once
#include "../../includes/Includes.h"
#include "../Model.h"

///<summary>����������� �������� ���������.</summary>
class Light
{
protected:
	///<summary>Diffuse ���� �����.</summary>
	glm::vec3 diffuseColor_;

	///<summary>Specular ���� �����.</summary>
	glm::vec3 specularColor_;

	///<summary>��������.</summary>
	float power_;

public:
	///<summary>����� diffuse ���� �����.</summary>
	///<param name = 'diffuseColor'>����.</param>
	void setDiffuseColor(const glm::vec3 diffuseColor);

	///<summary>����� diffuse ���� �����.</summary>
	///<param name = 'red'>������� ������������.</param>
	///<param name = 'green'>������ ������������.</param>
	///<param name = 'blue'>����� ������������.</param>
	void setDiffuseColor(const unsigned char red, const unsigned char green, unsigned char blue);

	///<summary>����� specular ���� �����.</summary>
	///<param name = 'specularColor'>����.</param>
	void setSpecularColor(const glm::vec3 specularColor);

	///<summary>����� specular ���� �����.</summary>
	///<param name = 'red'>������� ������������.</param>
	///<param name = 'green'>������ ������������.</param>
	///<param name = 'blue'>����� ������������.</param>
	void setSpecularColor(const unsigned char red, const unsigned char green, unsigned char blue);

	///<summary>����� �������� ��������� �����.</summary>
	///<param name = 'power'>��������.</param>
	void setPower(const float power);

	///<summary>���������� diffuse ���� �����.</summary>
	glm::vec3 getDiffuseColor() const;

	///<summary>���������� specular ���� �����.</summary>
	glm::vec3 getSpecularColor() const;
	
	///<summary>���������� �������� ��������� �����.</summary>
	float getPower() const;

	///<summary>���������� ������� ������ ��������� ����� ��� ��������� ������� ���������.</summary>
	virtual glm::mat4 getModelMatrix() = 0;
};

///<summary>�������� �������� ���������.</summary>
class PointLight : public Light
{
private:
	///<summary>������ ��� ��������� ������ ���������.</summary>
	std::shared_ptr<Shader> shader_;

	///<summary>3D ������ ���������.</summary>
	std::shared_ptr<Model> model_;

	///<summary>������� ���������.</summary>
	glm::vec3 position_;

	///<summary>������ ���������.</summary>
	float radius_;

	///<summary>������� �����������.</summary>
	glm::mat4 translationMatrix_;

	///<summary>������� �������.</summary>
	glm::mat4 scaleMatrix_;

public:
	///<summary>�����������.</summary>
	///<param name = 'shader'>������ ��� ��������� ������ ���������.</param>
	///<param name = 'model'>3D ������ ���������.</param>
	PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model);

	///<summary>�����������.</summary>
	///<param name = 'shader'>������ ��� ��������� ������ ���������.</param>
	///<param name = 'model'>3D ������ ���������.</param>
	///<param name = 'position'>������� ���������.</param>
	///<param name = 'radius'>������ ���������.</param>
	///<param name = 'diffuseColor'>Diffuse ���� ����� ���������.</param>
	///<param name = 'specularColor'>Specular ���� ����� ���������.</param>
	///<param name = 'power'>�������� ���������.</param>
	PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Model> model, glm::vec3 position, float radius, glm::vec3 diffuseColor, glm::vec3 specularColor, float power);

	///<summary>����� ������� ���������.</summary>
	///<param name = 'position'>�������.</param>
	void setPosition(glm::vec3 position);

	///<summary>����� ������ ���������.</summary>
	///<param name = 'radius'>������.</param>
	void setRadius(float radius);	

	///<summary>���������� ������� ���������.</summary>
	glm::vec3 getPosition() const;

	///<summary>���������� ������ ���������.</summary>
	float getRadius() const;

	///<summary>���������� ������� ������ ��������� ����� ��� ��������� ������� ���������.</summary>
	glm::mat4 getModelMatrix();

	///<summary>���������� ������.</summary>
	std::shared_ptr<Shader> getShader() const;

	///<summary>���������� 3D ������.</summary>
	std::shared_ptr<Model> getModel() const;
};