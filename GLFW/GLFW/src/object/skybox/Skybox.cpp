#include "Skybox.h"
#include "..\..\texture_loader\TextureLoader.h"

///<summary>�����������.</summary>
///<param name = 'size'>������ ���������.</param>
Skybox::Skybox(const float size)
{
	this->models_.push_back(new Model("skybox.obj"));

	this->scaleCoeffs_ = glm::vec3(size);
	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>����������.</summary>
Skybox::~Skybox() {}