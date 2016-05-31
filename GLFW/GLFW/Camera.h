#ifndef CAMERA_H
#define CAMERA_H

#include "Includes.h"

class CAMERA
{
private:
	/* CameraPosition - позици€ камеры */
	/* CameraLookTo - точка, на которую направлена камера */
	/* CameraUp - направление вверх камеры */
	/* CameraStaticPosition - позици€ статичной камеры */
	/* Side, Radius, RadiusMin, RadiusMax - граница полЄта камеры, радиус полЄта камеры от начала координат дл€ камеры є2 и его минимальное и максимальное значени€ */
	/* Speed - скорость движени€ камеры */
	/* MouseSpeed - скорость движени€ мышки */
	vec3 CameraPosition, CameraLookTo = vec3(0.0f, 0.0f, 0.0f), CameraUp = vec3(0.0f, 1.0f, 0.0f), CameraStaticPosition = vec3(8.0f, 3.0f, 6.0f);
	float Pi = pi<float>(), DeltaTime;
	float Side = 100.0f, Radius = 20.0f, RadiusMin = 2.0f, RadiusMax = 100.0f;
	float Speed = 15.0f, MouseSpeed = 0.003f;

	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	/* ViewMatrix - матрица вида дл€ координатных осей */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* ќбработка клавиатуры дл€ движени€ камеры є2 */
	/* window - указатель на окно */
	void CheckMove(GLFWwindow *window);

	/* ќбработка клавиатуры дл€ движени€ камеры є1 */
	/* window - указатель на окно */
	/* Position - нова€ позици€ камеры */
	/* Direction - направление камеры */
	/* Right - вектор "вправо" дл€ камеры */
	void CheckMove(GLFWwindow* window, vec3 &Position, vec3 Direction, vec3 Right);

public:
	/*  онструктор по-умолчанию */
	CAMERA() {};

	/*  онструктор */
	/* value - половина длины стороны скайбокса */
	CAMERA(float value);

	/* ¬озвращает матрицу проекции */
	mat4 getProjectionMatrix();

	/* ¬озвращает матрицу вида */
	mat4 getViewMatrix();

	/* ¬озвращает матрицу вида дл€ координатных осей */
	mat4 getViewMatrixAxes();

	/* ¬ычисление матриц вида и проекции, возвращает позицию камеры */
	/* WindowInfo - »нформаци€ об окне */
	/* CameraMode - выбранна€ камера */
	/* FOV - field of view */
	vec3 ComputeViewMatrix(windowInfo WindowInfo, int CameraMode, float FOV);
};

#endif
