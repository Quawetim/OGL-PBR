#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/Object.h"

///<summary>����� 1.</summary>
class Scene1 : public IScene
{
private:
	std::vector<Object> cubes;
	std::vector<Object> spheres;
	std::vector<Object> cylinders;

public:
	///<summary>����������.</summary>
	~Scene1();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<Model*> models);

	///<summary>��������� �����.</summary>
	///<param name = 'shader'></param>
	void render(const Shader shader, const glm::mat4 P, const glm::mat4 V);
};
