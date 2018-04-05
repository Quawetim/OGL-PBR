#include "Skybox.h"

///<summary>�����������.</summary>
///<param name = 'size'>������ ���������.</param>
Skybox::Skybox(const float size)
{
	this->model_ = Model("resources/3dmodels/skybox.obj");
	this->cubeMapID = textureLoader::loadCubeMap("resources/textures/skybox");

	this->scaleCoeffs_ = glm::vec3(size);
	this->scaleMatrix_ = glm::scale(this->scaleCoeffs_);
}

///<summary>��������� ���������.</summary>
///<param name = 'shader'>������.</param>
///<param name = 'projection_matrix'>������� ��������.</param>
///<param name = 'camera_position'>������� ������.</param>
void Skybox::draw(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	glDepthFunc(GL_EQUAL);

	shader.activate();
	shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeMapID);

	model_.draw(shader);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDepthFunc(GL_LESS);
}