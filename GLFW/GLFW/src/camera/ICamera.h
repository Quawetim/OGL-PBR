#pragma once
#include "..\includes\Includes.h"
#include "..\callbacks\Callbacks.h"

///<summary>�������� ��� ������� �����������.</summary>
struct QCameraVelocities
{
	///<summary>�������� �����.</summary>
	const float forward = 20.0f;

	///<summary>�������� �����.</summary>
	const float backward = 10.0f;

	///<summary>�������� �������.</summary>
	const float strafe = 15.0f;

	///<summary>�������� ������� (����/���).</summary>
	const float pitch = 40.0f;

	///<summary>�������� �������� (����/���).</summary>
	const float yaw = 40.0f;

	///<summary>�������� ����� (����/���).</summary>
	const float roll = 20.0f;
};

///<summary>����������� ����� ������.</summary>
class ICamera
{
protected:
	///<summary>�������.
	///<para>direction = position + front</para>
	///</summary>
    glm::vec3 position_;

	///<summary>�����������.
	///<para>direction = position + front</para>
	///</summary>
	glm::vec3 front_;

	///<summary>������, ����������� ����.</summary>
	glm::vec3 up_;

	///<summary>������, ����������� �����.</summary>
	glm::vec3 right_;
	
	///<summary>������.</summary>
	float pitch_;

	///<summary>��������.</summary>
	float yaw_;

	///<summary>����.</summary>
	float roll_;

	///<summary>������ ����� ������ ��� ���� ���������.</summary>
	const float axesRadius_ = 2.0f;

	///<summary>�������� ��� ������� �����������.</summary>
	QCameraVelocities cameraVelocities_;

	///<summary>������� ���� ������.</summary>
    glm::mat4 viewMatrix_;

	///<summary>������� ���� ���� ���������.</summary>
	glm::mat4 viewMatrixAxes_;

	///<summary>��������� ������� ���� ������.</summary>
	virtual void computeViewMatrix() = 0;

	///<summary>��������� ������� ���� ���� ���������.</summary>
	virtual void computeViewMatrixAxes() = 0;

public:
	///<summary>�����������.</summary>
	ICamera();

	///<summary>��������� ����������.</summary>
	///<param name = 'windowInfo'>��������� �� ����.</param>
	virtual void handleInput(QWindowInfo windowInfo) = 0;

	///<summary>��������� � ���������� ������� ���� ������.</summary>
    glm::mat4 getViewMatrix();

	///<summary>��������� � ���������� ������� ���� ���� ���������.</summary>
	glm::mat4 getViewMatrixAxes();
};

///<summary>First person camera.</summary>
class FirstPersonCamera : public ICamera
{
private:
	///<summary>��������� ������� ���� ������.</summary>
	void computeViewMatrix();

	///<summary>��������� ������� ���� ���� ���������.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>�����������.</summary>
	FirstPersonCamera();

	///<summary>��������� ����������.</summary>
	///<param name = 'windowInfo'>��������� �� ����.</param>
	void handleInput(QWindowInfo windowInfo);
};
 
///<summary>Third person camera.</summary>
class ThirdPersonCamera : public ICamera
{
private:
	///<summary>������ ����� ������.</summary>
	const float radius_ = 20.0f;

	///<summary>��������� ������� ���� ������.</summary>
	void computeViewMatrix();

	///<summary>��������� ������� ���� ���� ���������.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>�����������.</summary>
	ThirdPersonCamera();

	///<summary>��������� ����������.</summary>
	///<param name = 'windowInfo'>��������� �� ����.</param>
	void handleInput(QWindowInfo windowInfo);
};

///<summary>��������� ������.</summary>
class StaticCamera : public ICamera
{
private:
	///<summary>����������� ������.</summary>
	glm::vec3 direction_;

	///<summary>��������� ������� ���� ������.</summary>
	void computeViewMatrix();

	///<summary>��������� ������� ���� ���� ���������.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>�����������.</summary>
	StaticCamera();

	///<summary>�����������.</summary>
	///<param name = 'position'>�������.</param>
	///<param name = 'direction'>�����������.</param>
	///<param name = 'up'>������ �����.</param>
	StaticCamera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	///<summary>��������� ����������.</summary>
	///<param name = 'windowInfo'>��������� �� ����.</param>
	void handleInput(QWindowInfo windowInfo);
};

///<summary>��������� ������.</summary>
class FreeCamera : public ICamera
{
private:
	///<summary>��������� ������� ���� ������.</summary>
	void computeViewMatrix();

	///<summary>��������� ������� ���� ���� ���������.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>�����������.</summary>
	FreeCamera();

	///<summary>��������� ����������.</summary>
	///<param name = 'windowInfo'>��������� �� ����.</param>
	void handleInput(QWindowInfo windowInfo);
};