#include "Skybox.h"
#include "..\..\texture_loader\TextureLoader.h"

///<summary>Конструктор.</summary>
///<param name = 'size'>Размер скайбокса.</param>
Skybox::Skybox(const float size)
{
	this->models_.push_back(std::shared_ptr<Model>(new Model("skybox.obj")));

	this->scaleCoeffs_ = glm::vec3(size);
	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>Деструктор.</summary>
Skybox::~Skybox() {}