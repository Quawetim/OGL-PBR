#pragma once
#include "../includes/Includes.h"
#include "IScene.h"

///<summary>����� 1.</summary>
class Scene1 : public IScene
{
private:
	std::vector<Object*> cubes_;
	std::vector<Object*> spheres_;
	std::vector<Object*> cylinders_;
	bool decrease_;

public:
	///<summary>����������.</summary>
	~Scene1();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<Model*> models);

	///<summary>��������� �����.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void render(Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
