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
	///<summary></summary>
	ICamera();

	///<summary>Обработка клавиатуры.</summary>
	///<param name = 'windowInfo'>Указатель на окно.</param>
	virtual void handleInput(QWindowInfo windowInfo) = 0;

	///<summary>Вычисляет и возвращает матрицу вида камеры.</summary>
    glm::mat4 getViewMatrix();

	///<summary>Вычисляет и возвращает матрицу вида осей координат.</summary>
	glm::mat4 getViewMatrixAxes();
};

///<summary>First person camera.</summary>
class FirstPersonCamera : public ICamera
{
private:
	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	void computeViewMatrixAxes();

public:
	///<summary>Обработка клавиатуры.</summary>
	///<param name = 'windowInfo'>Указатель на окно.</param>
	void handleInput(QWindowInfo windowInfo);
};
 
///<summary>Third person camera.</summary>
class ThirdPersonCamera : public ICamera
{
private:
	///<summary>Расстояние между камерой и объектом.</summary>
	float radius = 20.0f;

	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	void computeViewMatrixAxes();

public:
	///<summary>Обработка клавиатуры.</summary>
	///<param name = 'windowInfo'>Указатель на окно.</param>
	void handleInput(QWindowInfo windowInfo);
};

///<summary>Статичная камера.</summary>
class StaticCamera : public ICamera
{
private:
	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();
	void computeViewMatrixAxes();

public:
	///<summary>Обработка клавиатуры.</summary>
	///<param name = 'windowInfo'>Указатель на окно.</param>
	void handleInput(QWindowInfo windowInfo);
};

///<summary>Свободная камера.</summary>
class FreeCamera : public ICamera
{
private:
	///<summary>Вычисляет матрицу вида камеры.</summary>
	void computeViewMatrix();

	///<summary>Вычисляет матрицу вида осей координат.</summary>
	void computeViewMatrixAxes();

public:
	///<summary>Обработка клавиатуры.</summary>
	///<param name = 'windowInfo'>Указатель на окно.</param>
	void handleInput(QWindowInfo windowInfo);
};