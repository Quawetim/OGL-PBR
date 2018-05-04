#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"

///<summary>�����-�������� ��� �������� aka �����, �������.</summary>
class IScene
{
protected:
	template<typename Type> int getSign(Type value) { return (value > 0) ? 1 : -1; }

public:
	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	virtual void init(std::vector<Model*> models) = 0;

	///<summary>��������� �����.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void render(float deltaTime, Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position) = 0;
};
