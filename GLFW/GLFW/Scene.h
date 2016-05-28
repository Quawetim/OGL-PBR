#ifndef SCENE_H
#define SCENE_H

#include "Includes.h"
#include "Camera.h"
#include "Object.h"

class SCENE
{
private:
	/* WindowInfo - хранит информацию об окне */
	windowInfo WindowInfo;

	/* LightsCount - число источников света */
	/* ObjectsCount, ObjectsCountMirror - число объектов */
	int LightsCount = 0, ObjectsCount = 0, ObjectsCountMirror = 0;
	OBJECT *Lights, Axes, Skybox, Plane, *Objects, *ObjectsMirror;
	CAMERA Camera;

	bool SphereDecrease = true, SphereIncrease = false;
	float SpheresSize = 1.0f, SpheresSizeDelta = 0.004f, SpheresSizeMin = 0.8f, SpheresSizeMax = 1.0f;
	float CubeAngle = 1.0f, CylinderAngle = -0.2f;
	float Radius, Angle = 0.0f, Angle2 = 0.0f, Angle3 = 0.0f, LightAngle = 0.0f;
	float AngleDelta = 2.0f, AngleDelta2 = 1.0f, AngleDelta3 = 1.0f, LightAngleDelta = 0.5f;
	vec3 Position, NewPosition;

	/* Позиция камеры */
	vec3 CameraPosition;

	/* LightsPositions - позиции источников света */
	/* LightsColors - цвета источников света */
	vec3 *LightsPositions, *LightsColors;

	/* Свойства источников света */
	vec4 *LightsProperties;

	/* Матрицы проекции и вида */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* Генерирует CubeMap-текстуру для зеркального объекта */
	/* id - идентификатор объекта, позиция в массиве всех объектов */
	/* camera - позиция камеры */
	/* ViewMatrix - матрица вида */
	GLuint MakeCubemap(int texturesize, int id, vec3 camera, mat4 ViewMatrix);

public:
	/* Конструктор по-умолчанию */
	SCENE();

	/* Конструктор, инициализирует объекты сцены */
	SCENE(bool blinn, windowInfo Winfo);

	/* Деструктор, удаляет шейдеры */
	~SCENE();

	/* Рендеринг сцены */
	void Render(windowInfo Winfo, int cameramode, int texturesize, float fov, bool mirrorexample, bool stoprotations, bool showlights, bool blinn);
};
#endif
