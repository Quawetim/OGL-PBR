#include "Skybox.h"
#include "..\..\texture_loader\TextureLoader.h"

///<summary>Конструктор.</summary>
///<param name = 'size'>Размер скайбокса.</param>
///<param name = 'envMap'>Идентификатор environment map.</param>
Skybox::Skybox(const float size, const unsigned int envMap)
{
	this->models_.push_back(new Model("resources/3dmodels/skybox.obj"));
	this->envMapID_ = envMap;

	this->scaleCoeffs_ = glm::vec3(size);
	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>Деструктор.</summary>
Skybox::~Skybox() {}

///<summary>Задаёт environment map.</summary>
///<param name = 'envMap'>Идентификатор environment map.</param>
void Skybox::setEnvMap(const unsigned int envMap)
{
	this->envMapID_ = envMap;
}

///<summary>Возвращает идентификатор CubeMap.</summary>
unsigned int Skybox::getEnvMapID() const
{
	return this->envMapID_;
}