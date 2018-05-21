#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"
#include "../object/light/Light.h"

///<summary>�����-�������� ��� �������� aka �����, �������.</summary>
class IScene
{
protected:
	std::vector<std::shared_ptr<Object>> objects_;
	std::vector<std::shared_ptr<PointLight>> lights_;

	bool objectsMoving_ = false;
	bool lightsMoving_ = false;
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

	void moveObjects(const bool move) { this->objectsMoving_ = move; }

	void moveLights(const bool move) { this->lightsMoving_ = move; }

	void showLights(const bool show) { this->lightsVisible_ = show; }

	bool getMoveObjects() { return this->objectsMoving_; }

	bool getMoveLights() { return this->lightsMoving_; }

	bool getShowLights() { return this->lightsVisible_; }
};
