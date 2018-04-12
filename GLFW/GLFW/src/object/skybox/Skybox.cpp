#include "Skybox.h"

///<summary>Конструктор.</summary>
///<param name = 'size'>Размер скайбокса.</param>
Skybox::Skybox(const float size)
{
	this->models_.push_back(new Model("resources/3dmodels/skybox.obj"));
	this->cubeMapID = textureLoader::loadCubeMap("resources/textures/skybox");

	this->scaleCoeffs_ = glm::vec3(size);
	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>Деструктор.</summary>
Skybox::~Skybox()
{
	textureLoader::deleteTexture(this->cubeMapID);
}

///<summary>Возвращает идентификатор CubeMap.</summary>
unsigned int Skybox::getCubeMapID() const
{
	return this->cubeMapID;
}