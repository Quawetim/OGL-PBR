#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"
#include "../object/light/Light.h"

///<summary>�����-�������� ��� �������� aka �����, �������.</summary>
class IScene
{
protected:
	///<summary>������ ��������.</summary>
	std::vector<std::shared_ptr<Object>> objects_;

	///<summary>������ ���������� ���������.</summary>
	std::vector<std::shared_ptr<PointLight>> lights_;

	///<summary>���� �������� ��������.</summary>
	bool objectsMoving_ = false;

	///<summary>���� �������� ���������� ���������.</summary>
	bool lightsMoving_ = false;

	///<summary>���� ����������� ���������� ���������.</summary>
	bool lightsVisible_ = false;

	template<typename Type> int getSign(Type value) { return (value > 0) ? 1 : -1; }

public:
	///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
	///<param name = 'models'>������ �������.</param>
	virtual void init(std::vector<std::shared_ptr<Model>> models) = 0;

	///<summary>��������� �����.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void render(float deltaTime, std::shared_ptr<Shader> shader, const glm::mat4 view_matrix, const glm::vec3 camera_position) = 0;

	///<summary>������������� �������� ��������.</summary>
	///<param name = 'show'>���/����.</param>
	void moveObjects(const bool move) { this->objectsMoving_ = move; }

	///<summary>������������� �������� ���������� ���������.</summary>
	///<param name = 'show'>���/����.</param>
	void moveLights(const bool move) { this->lightsMoving_ = move; }

	///<summary>������������� ����������� ���������� ���������.</summary>
	///<param name = 'show'>���/����.</param>
	void showLights(const bool show) { this->lightsVisible_ = show; }

	///<summary>���������� ���� �������� ��������.</summary>
	bool getMoveObjects() { return this->objectsMoving_; }

	///<summary>���������� ���� �������� ���������� ���������.</summary>
	bool getMoveLights() { return this->lightsMoving_; }

	///<summary>���������� ���� ����������� ���������� ���������.</summary>
	bool getShowLights() { return this->lightsVisible_; }
};
