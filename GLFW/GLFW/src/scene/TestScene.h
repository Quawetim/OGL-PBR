#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../texture_loader/TextureLoader.h"
#include "../object/Object.h"

///<summary>�������� �����.</summary>
class TestScene : public IScene
{
private:
	Mesh rectangle;
	Object Nanosuit;

public:
	///<summary>����������.</summary>
	~TestScene();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<Model*> models);

	///<summary>��������� �����.</summary>
	///<param name = 'shader'></param>
	void render(const Shader shader, const glm::mat4 P, const glm::mat4 V);
};
