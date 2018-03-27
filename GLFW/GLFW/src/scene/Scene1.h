#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../object/Object.h"
#include "../texture_loader/TextureLoader.h"

///<summary>����� 1.</summary>
class Scene1 : public IScene
{
private:
	std::vector<Object> cubes_;
	std::vector<Object> spheres_;
	std::vector<Object> cylinders_;
	bool decrease_;

public:
	///<summary>����������.</summary>
	~Scene1();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<Model*> models);

	///<summary>��������� �����.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'projection_matrix'>������� ��������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	void render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix);
};
