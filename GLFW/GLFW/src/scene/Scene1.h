#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/light/Light.h"

///<summary>����� 1.</summary>
class Scene1 : public IScene
{
private:
	std::vector<std::shared_ptr<Object>> cubes_;
	std::vector<std::shared_ptr<Object>> spheres_;
	std::vector<std::shared_ptr<Object>> cylinders_;
	std::vector<std::shared_ptr<Object>> objects_;
	bool decrease_;

	std::vector<std::shared_ptr<PointLight>> lights_;

public:
	///<summary>����������.</summary>
	~Scene1();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<std::shared_ptr<Model>> models);

	///<summary>��������� �����.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void render(float deltaTime, std::shared_ptr<Shader> shader, const glm::mat4 view_matrix, const glm::vec3 camera_position);
};
