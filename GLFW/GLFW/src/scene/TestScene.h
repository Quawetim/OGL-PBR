#pragma once
#include "../includes/Includes.h"
#include "IScene.h"

///<summary>�������� �����.</summary>
class TestScene : public IScene
{
private:
	std::vector<Object*> objects_;

public:
	///<summary>����������.</summary>
	TestScene() {};

	///<summary>����������.</summary>
	~TestScene();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<Model*> models);

	///<summary>��������� �����.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void render(Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
