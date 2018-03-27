#pragma once
#include "../includes/Includes.h"
#include "IScene.h"
#include "../texture_loader/TextureLoader.h"
#include "../object/Object.h"

///<summary>�������� �����.</summary>
class TestScene : public IScene
{
private:
	Mesh rectangle_;
	Object nanosuit_;

public:
	///<summary>����������.</summary>
	~TestScene();

	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	void init(std::vector<Model*> models);

	///<summary>��������� �����.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'projection_matrix'>������� ��������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	void render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix);
};
