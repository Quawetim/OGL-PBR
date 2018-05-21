#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/light/Light.h"

///<summary>����� 2.</summary>
class Scene2 : public IScene
{
private:
	bool right_ = true;

public:
	///<summary>����������.</summary>
	~Scene2();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<std::shared_ptr<Model>> models);

	///<summary>��������� �����.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void render(float deltaTime, std::shared_ptr<Shader> shader, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
