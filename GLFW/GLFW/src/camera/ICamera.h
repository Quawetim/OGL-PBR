#pragma once
#include "..\includes\Includes.h"

///<summary>Скорости для каждого направления.</summary>
struct QCameraVelocities
{
	///<summary>Скорость вперёд.</summary>
	const float forward = 20.0f;

	///<summary>Скорость назад.</summary>
	const float backward = 10.0f;

	///<summary>Скорость стрейфа.</summary>
	const float strafe = 15.0f;

	///<summary>Скорость тангажа (град/сек).</summary>
	const float pitch = 40.0f;

	///<summary>Скорость рысканья (град/сек).</summary>
	const float yaw = 40.0f;

	///<summary>Скорость крена (град/сек).</summary>
	const float roll = 20.0f;
};

///<summary>Абстрактный класс камера.</summary>
class ICamera
{
protected:
	///<summary>Позиция.
	///<para>direction = position + front</para>
	///</summary>
    glm::vec3 position_;

	///<summary>Направление.
	///<para>direction = position + front</para>
	///</summary>
	glm::vec3 front_;

	///<summary>Вектор, указывающий верх.</summary>
	glm::vec3 up_;

	///<summary>Вектор, указывающий право.</summary>
	glm::vec3 right_;
	
	///<summary>Тангаж.</summary>
	float pitch_;

	///<summary>Рысканье.</summary>
	float yaw_;

	///<summary>Крен.</summary>
	float roll_;

	///<summary>Радиус облёта камеры для осей координат.</summary>
	const float axesRadius_ = 2.0f;

	///<summary>Скорости для каждого направления.</summary>
	QCameraVelocities cameraVelocities_;

	///<summary>Матрица вида камеры.</summary>
    glm::mat4 viewMatrix_;

	///<summary>Матрица вида осей координат.</summary>
	glm::mat4 viewMatrixAxes_;

	///<summary>Вычисляет матрицу вида камеры.</summary>
	virtual void computeViewMatrix() = 0;

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	virtual void computeViewMatrixAxes() = 0;

public:
	///<summary>Конструктор.</summary>
	ICamera();

	///<summary>Обработка клавиатуры.</summary>
	virtual void handleInput() = 0;

	///<summary>Вычисляет и возвращает матрицу вида камеры.</summary>
    glm::mat4 getViewMatrix();

	///<summary>Вычисляет и возвращает матрицу вида осей координат.</summary>
	glm::mat4 getViewMatrixAxes();

	///<summary>Возвращает позицию камеры.</summary>
	glm::vec3 getPosition() const;
};

///<summary>First person camera.</summary>
class FirstPersonCamera : public ICamera
{
private:
	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>Конструктор.</summary>
	FirstPersonCamera();

	///<summary>Обработка клавиатуры.</summary>
	void handleInput() override;
};
 
///<summary>Third person camera.</summary>
class ThirdPersonCamera : public ICamera
{
private:
	///<summary>Радиус облёта камеры.</summary>
	const float radius_ = 20.0f;

	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>Конструктор.</summary>
	ThirdPersonCamera();

	///<summary>Обработка клавиатуры.</summary>
	void handleInput() override;
};

///<summary>Статичная камера.</summary>
class StaticCamera : public ICamera
{
private:
	///<summary>Направление камеры.</summary>
	glm::vec3 direction_;

	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>Конструктор.</summary>
	StaticCamera();

	///<summary>Конструктор.</summary>
	///<param name = 'position'>Позиция.</param>
	///<param name = 'direction'>направление.</param>
	///<param name = 'up'>Вектор вверх.</param>
	StaticCamera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	///<summary>Обработка клавиатуры.</summary>
	void handleInput() override;
};

///<summary>Свободная камера.</summary>
class FreeCamera : public ICamera
{
private:
	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	void computeViewMatrixAxes();

	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	///<summary>Конструктор.</summary>
	FreeCamera();

	///<summary>Обработка клавиатуры.</summary>
	void handleInput() override;
};