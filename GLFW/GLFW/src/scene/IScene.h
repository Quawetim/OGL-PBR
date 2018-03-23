#pragma once
#include "../includes/Includes.h"
#include "../object/Model.h"

///<summary>�����-�������� ��� �������� aka �����, �������.</summary>
class IScene
{
public:
	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	virtual void init(std::vector<Model*> models) = 0;

	///<summary>��������� �����.</summary>
	///<param name = 'shader'></param>
	virtual void render(const Shader shader, const glm::mat4 P, const glm::mat4 V) = 0;
};
