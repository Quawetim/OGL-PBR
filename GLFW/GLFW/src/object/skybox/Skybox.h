#pragma once
#include "..\Object.h"

class Skybox : public Object
{
private:
	///<summary>������.</summary>
	Model model_;

	///<summary>������������� ��������.</summary>
	unsigned int cubeMapID;

public:
	///<summary>�����������.</summary>
	///<param name = 'size'>������ ���������.</param>
	Skybox(const float size);

	///<summary>��������� ���������.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'projection_matrix'>������� ��������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void draw(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};