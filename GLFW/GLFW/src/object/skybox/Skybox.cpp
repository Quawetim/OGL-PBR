#include "Skybox.h"

///<summary>�����������.</summary>
///<param name = 'size'>������ ���������.</param>
Skybox::Skybox(const float size)
{
	this->models_.push_back(new Model("resources/3dmodels/skybox.obj"));
	this->cubeMapID = textureLoader::loadCubeMap("resources/textures/skybox");

	this->scaleCoeffs_ = glm::vec3(size);
	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>����������.</summary>
Skybox::~Skybox()
{
	textureLoader::deleteTexture(this->cubeMapID);
}

///<summary>���������� ������������� CubeMap.</summary>
unsigned int Skybox::getCubeMapID() const
{
	return this->cubeMapID;
}