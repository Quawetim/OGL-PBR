#ifndef SCENE_H
#define SCENE_H

#include "Includes.h"
#include "Camera.h"
#include "Object.h"

class SCENE
{
private:
	/* WindowInfo - ������ ���������� �� ���� */
	windowInfo WindowInfo;

	/* LightsCount - ����� ���������� ����� */
	/* ObjectsCount, ObjectsCountMirror - ����� �������� */
	int LightsCount = 0, ObjectsCount = 0, ObjectsCountMirror = 0;
	OBJECT *Lights, Axes, Skybox, Plane, *Objects, *ObjectsMirror;
	CAMERA Camera;

	bool SphereDecrease = true, SphereIncrease = false;
	float SpheresSize = 1.0f, SpheresSizeDelta = 0.004f, SpheresSizeMin = 0.8f, SpheresSizeMax = 1.0f;
	float CubeAngle = 1.0f, CylinderAngle = -0.2f;
	float Radius, Angle = 0.0f, Angle2 = 0.0f, Angle3 = 0.0f, LightAngle = 0.0f;
	float AngleDelta = 2.0f, AngleDelta2 = 1.0f, AngleDelta3 = 1.0f, LightAngleDelta = 0.5f;
	vec3 Position, NewPosition;

	/* ������� ������ */
	vec3 CameraPosition;

	/* LightsPositions - ������� ���������� ����� */
	/* LightsColors - ����� ���������� ����� */
	vec3 *LightsPositions, *LightsColors;

	/* �������� ���������� ����� */
	vec4 *LightsProperties;

	/* ������� �������� � ���� */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* ���������� CubeMap-�������� ��� ����������� ������� */
	/* id - ������������� �������, ������� � ������� ���� �������� */
	/* camera - ������� ������ */
	/* ViewMatrix - ������� ���� */
	GLuint MakeCubemap(int texturesize, int id, vec3 camera, mat4 ViewMatrix);

public:
	/* ����������� ��-��������� */
	SCENE();

	/* �����������, �������������� ������� ����� */
	SCENE(bool blinn, windowInfo Winfo);

	/* ����������, ������� ������� */
	~SCENE();

	/* ��������� ����� */
	void Render(windowInfo Winfo, int cameramode, int texturesize, float fov, bool mirrorexample, bool stoprotations, bool showlights, bool blinn);
};
#endif
