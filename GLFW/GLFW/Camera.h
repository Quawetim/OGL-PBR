#ifndef CAMERA_H
#define CAMERA_H

#include "Includes.h"

class CAMERA
{
private:
	/* CameraPosition - ������� ������ */
	/* CameraLookTo - �����, �� ������� ���������� ������ */
	/* CameraUp - ����������� ����� ������ */
	/* CameraStaticPosition - ������� ��������� ������ */
	/* Side, Radius, RadiusMin, RadiusMax - ������� ����� ������, ������ ����� ������ �� ������ ��������� ��� ������ �2 � ��� ����������� � ������������ �������� */
	/* Speed - �������� �������� ������ */
	/* MouseSpeed - �������� �������� ����� */
	vec3 CameraPosition, CameraLookTo = vec3(0.0f, 0.0f, 0.0f), CameraUp = vec3(0.0f, 1.0f, 0.0f), CameraStaticPosition = vec3(8.0f, 3.0f, 6.0f);
	float Pi = pi<float>(), DeltaTime;
	float Side = 100.0f, Radius = 20.0f, RadiusMin = 2.0f, RadiusMax = 100.0f;
	float Speed = 15.0f, MouseSpeed = 0.003f;

	/* ProjectionMatrix - ������� �������� */
	/* ViewMatrix - ������� ���� */
	/* ViewMatrix - ������� ���� ��� ������������ ���� */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* ��������� ���������� ��� �������� ������ �2 */
	/* window - ��������� �� ���� */
	void CheckMove(GLFWwindow *window);

	/* ��������� ���������� ��� �������� ������ �1 */
	/* window - ��������� �� ���� */
	/* Position - ����� ������� ������ */
	/* Direction - ����������� ������ */
	/* Right - ������ "������" ��� ������ */
	void CheckMove(GLFWwindow* window, vec3 &Position, vec3 Direction, vec3 Right);

public:
	/* ����������� ��-��������� */
	CAMERA() {};

	/* ����������� */
	/* value - �������� ����� ������� ��������� */
	CAMERA(float value);

	/* ���������� ������� �������� */
	mat4 getProjectionMatrix();

	/* ���������� ������� ���� */
	mat4 getViewMatrix();

	/* ���������� ������� ���� ��� ������������ ���� */
	mat4 getViewMatrixAxes();

	/* ���������� ������ ���� � ��������, ���������� ������� ������ */
	/* WindowInfo - ���������� �� ���� */
	/* CameraMode - ��������� ������ */
	/* FOV - field of view */
	vec3 ComputeViewMatrix(windowInfo WindowInfo, int CameraMode, float FOV);
};

#endif
