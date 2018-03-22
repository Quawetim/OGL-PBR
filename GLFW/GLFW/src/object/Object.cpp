#include "Object.h"

///<summary>�����������.</summary>
///<param name = 'name'>��� �������.</param>
Object::Object(std::string name, Model *model)
{
	this->objectName = name;
	this->models.push_back(model);

	this->translationMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);
	this->scaleMatrix = glm::mat4(1.0f);
	this->position = glm::vec3(0.0f);
	this->rotationAngle = 0.0f;
	this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scale = glm::vec3(1.0f);
}

///<summary>�����������.</summary>
///<param name = 'name'>��� �������.</param>
Object::Object(std::string name, std::vector<Model*> models)
{
	this->objectName = name;
	this->models = models;

	this->translationMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);
	this->scaleMatrix = glm::mat4(1.0f);
	this->position = glm::vec3(0.0f);
	this->rotationAngle = 0.0f;
	this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scale = glm::vec3(1.0f);	
}

///<summary>����������.</summary>
Object::~Object()
{
	this->models.clear();
}

///<summary>��������� �������.</summary>
///<param name ='shader'>������.</param>
void Object::draw(Shader shader)
{
	if (this->models.empty()) logger.log("Object::draw", QErrorType::error, "Models list is empty. Nothing to render.");
	else
	{
		for (size_t i = 0; i < this->models.size(); i++)
		{
			this->models[i]->drawModel(shader);
		}
	}
}

///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
///<param name = 'velocityX'>�������� �� ��� x.</param>
///<param name = 'velocityY'>�������� �� ��� y.</param>
///<param name = 'velocityZ'>�������� �� ��� z.</param>
void Object::moveObject(const float velocityX, const float velocityY, const float velocityZ)
{
	this->position += glm::vec3(velocityX * deltaTime, velocityY * deltaTime, velocityZ * deltaTime);
	this->translationMatrix = glm::translate(this->position);
}

///<summary>������� ������ � �������� ���������.</summary>
///<param name = 'angle'>�������� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
void Object::rotateObject(const double angle, const glm::vec3 axis)
{
	this->rotationAngle += angle * deltaTime;
	this->rotationAxis = axis;

	if (this->rotationAngle == 360) this->rotationAngle = 0;
	else
	{
		if (this->rotationAngle > 360) this->rotationAngle -= 360;
	}

	this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>�������� ������ ������� � �������� ���������.</summary>
///<param name = 'scaleXYZ'>�������� ��������� ������� �� ���� ����.</param>
void Object::scaleObject(const float scaleXYZ)
{
	this->scale += glm::vec3(scaleXYZ * (float)deltaTime);

	if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Object::scaleObject", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Object::scaleObject", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix = glm::scale(this->scale);
}

///<summary>�������� ������ ������� � �������� ���������.</summary>
///<param name = 'scaleX'>�������� ��������� ������� �� X.</param>
///<param name = 'scaleY'>�������� ��������� ������� �� Y.</param>
///<param name = 'scaleZ'>�������� ��������� ������� �� Z.</param>
void Object::scaleObject(const float scaleX, const float scaleY, const float scaleZ)
{
	this->scale += glm::vec3(scaleX * deltaTime, scaleY * deltaTime, scaleZ * deltaTime);

	if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Object::scaleModel", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Object::scaleModel", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix = glm::scale(this->scale);
}

///<summary>����� ������� �������.</summary>
///<param name = 'position'>�������.</param>
void Object::setPosition(const glm::vec3 position)
{
	this->position = position;
	this->translationMatrix = glm::translate(this->position);
}

///<summary>����� ������� �������.</summary>
///<param name = 'angle'>���� �������� � ��������.</param>
///<param name = 'axis'>��� ��������.</param>
void Object::setRotation(const double angle, const glm::vec3 axis)
{
	this->rotationAngle = angle;
	this->rotationAxis = axis;

	if (this->rotationAngle == 360) this->rotationAngle = 0;
	else
	{
		if (this->rotationAngle > 360) this->rotationAngle -= 360;
	}

	this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>����� ������ ������� �� ���������.</summary>
///<param name = 'scale'>����������� �������.</param>
void Object::setScale(const glm::vec3 scale)
{
	this->scale = scale;

	if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Object::setScale", QErrorType::error, "Scale = 0");
	else
	{
		if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Object::setScale", QErrorType::warning, "Scale < 0");
	}

	this->scaleMatrix = glm::scale(this->scale);
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

///<summary>���������� ��� ������.</summary>
std::string Object::getName() const
{
	return this->objectName;
}

///<summary>���������� ������� ������.</summary>
glm::mat4 Object::getModelMatrix() const
{
	return this->translationMatrix * this->rotationMatrix * this->scaleMatrix;
}

///<summary>���������� ������� ������.</summary>
glm::vec3 Object::getPosition() const
{
	return this->position;
}