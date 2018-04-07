#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"

///<summary>�����-�������� ��� �������� aka �����, �������.</summary>
class IScene
{
public:
	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	virtual void init(std::vector<Model*> models) = 0;

	///<summary>��������� �����.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'projection_matrix'>������� ��������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position) = 0;
};
