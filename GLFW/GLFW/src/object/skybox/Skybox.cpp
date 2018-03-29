#include "Skybox.h"

///<summary>Конструктор.</summary>
///<param name = 'size'>Размер скайбокса.</param>
Skybox::Skybox(const float size)
{
	this->model_ = Model("resources/3dmodels/skybox.obj");

	this->scaleCoeffs_ = glm::vec3(size);
	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>Отрисовка скайбокса.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'projection_matrix'>Матрица проекции.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void Skybox::draw(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	glDepthFunc(GL_EQUAL);

	shader.activate();
	shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);

	model_.draw(shader);

	glDepthFunc(GL_LESS);
}