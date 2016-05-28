#ifndef OBJECT_H
#define OBJECT_H

#include "Includes.h"

class OBJECT
{
private:
	/* WindowInfo - содержит информацию об окне */
	windowInfo WindowInfo;

	/* ModelLoadedFlag - флаг успешной/неуспешной загрузки модели */
	/* DiffuseTextureFlag - флаг наличия/отсутствия диффузной текстуры */
	/* SpecularTextureFlag - флаг наличия/отсутствия карты отражений */
	/* NormalTextureFlag - флаг наличия/отсутствия карты нормалей */
	bool ModelLoadedFlag = false;
	bool DiffuseTextureFlag = false;
	bool SpecularTextureFlag = false;
	bool NormalTextureFlag = false;

	char Name[50];
	/* Material - материал*/
	/* ID - тип: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping*/
	/* AmbientColor - фоновый цвет */
	/* DiffuseColor - диффузный цвет */
	/* SpecularColor - отражённый цвет */
	/* Shine - сила блеска */
	/* RefractionIndex - индекс преломления: 1.0 - Воздух, 1.309 - лёд, 1.33 - вода, 1.52 - стекло (по-умолчанию), 2.42 - алмаз */
	struct material
	{
		int ID;
		vec3 AmbientColor = vec3(0.05f, 0.05f, 0.05f);
		vec3 DiffuseColor = vec3(0.5f, 0.5f, 0.5f);
		vec3 SpecularColor = vec3(0.7f, 0.7f, 0.7f);
		float Shine = 8.0f;
		float RefractiveIndex = 1.52f;
	} Material;

	/* LightsCount - число источников освещения */
	/* Blinn - модель освещения Блинна-Фонга */
	int LightsCount = 0;
	bool Blinn = false;

	/* Position - позиция объекта */
	/* Axis - ось последнего вращения */
	/* Angle - угол поворота */
	/* Scale - размер */
	vec3 Position = vec3(0.0f, 0.0f, 0.0f), Axis = vec3(0.0f, 1.0f, 0.0f);
	float Angle = 0.0f, Scale = 1.0f;

	/* PointLight - точечный источник света */
	/* Position - позиция */
	/* Color - цвет */
	/* Power - сила света */
	/* Constant - постоянный коэффициент затухания */
	/* Linear - линейный коэффициент затухания */
	/* Quadratic - квадратичный коэффициент затухания */
	struct pointLight
	{
		vec3 Position = vec3(5.0f, 0.0f, 0.0f);
		vec3 Color = vec3(1.0f, 1.0f, 1.0f);
		float Power = 2.0f;
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;
	} *PointLight;

	/* Vertex Array Object */
	GLuint VAO;

	/* ModelMatrix - матрица модели */
	mat4 ModelMatrix = mat4(1.0f);

	/* Идентификаторы шейдера, источника света, матриц и текстур для шейдеров*/
	GLuint ProjectionMatrixID, ViewMatrixID, ModelMatrixID;
	GLuint BlinnID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MaterialShineID, RefractiveIndexID;
	GLuint *PointLightPositionsIDs, *PointLightColorsIDs, *PointLightPowersIDs, *PointLightConstantsIDs, *PointLightLinearsIDs, *PointLightQuadraticsIDs;
	GLuint ShaderID, CameraPositionID, LightsCountID;
	GLuint DiffuseTextureFlagID, NormalTextureFlagID, SpecularTextureFlagID;
	GLuint DiffuseTextureID, NormalTextureID, SpecularTextureID, cubemapTextureID;
	/* Текстуры */
	GLuint DiffuseTexture = 0, SpecularTexture = 0, NormalTexture = 0, CubeMapTexture = 0;
	/* Буферы */
	GLuint vertexbuffer, colorbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer, elementbuffer;

	vector<vec3> vertices, normals, tangents, bitangents;
	vector<vec2> uvs;
	vector<unsigned short> indices;

	/* Подготовка данных для объекта сплошного цвета */
	void PrepareSolidColor();

	/* Рендеринг объекта сплошного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSolidColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* Подготовка данных для объекта градиентного цвета */
	void PrepareGradientColor();

	/* Рендеринг объекта градиентного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderGradientColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* Подготовка данных для стекла/зеркала */
	void PrepareReflectionRefraction();

	/* Рендеринг стекла/зеркала */
	/* Camera - позиция камеры */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderReflectionRefraction(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* Подготовка данных для цилиндра с картой нормалей */
	void PrepareNormalMapping();

	/* Рендеринг цилиндра с картой нормалей */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderNormalMapping(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* Загрузка модели из obj-файла, возвращает успех/ошибку */
	/* Path - путь к файлу */
	/* Vertices - список координат вершин, возвращаемое значение */
	/* UVs - список текстурных координат вершин, возвращаемое значение */
	/* Normals - список нормалей вершин, возвращаемое значение */
	bool LoadOBJ(const char *Path, vector<vec3> &Vertices, vector<vec2> &UVs, vector<vec3> &Normals);

	/* Загрузка вершинного и фрагментного шейдеров */
	/* VertexShader - путь к вершинному шейдеру */
	/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
	bool LoadShaders(const char *VertexShader, const char* GeometryShader, const char *FragmentShader);

	/* Вычисление касательных и бикасательных */
	/* Vertices - список координат вершин */
	/* UVs - список текстурных координат вершин */
	/* Normals - список нормалей вершин */
	/* Tangents - список касательных нормалей, возвращаемое значение */
	/* Bitangents - список бикасательных нормалей, возвращаемое значение */
	void ComputeTBT(vector<vec3> Vertices, vector<vec2> UVs, vector<vec3> Normals, vector<vec3> &Tangents, vector<vec3> &Bitangents);

	/* Загрузка 6 CubeMap текстур, возвращает идентификатор загруженной текстуры */
	/* Порядок: +X (Право), -X (Лево), +Y (Верх), -Y (Низ), +Z (Перед), -Z (Зад) */
	/* faces - список изображений для каждой грани */
	GLuint LoadCubeMap(vector<const GLchar*> faces);

public:
	/* Конструктор по-умолчанию */
	OBJECT();

	/* Конструктор для Скайбокса и координтных осей */
	OBJECT(windowInfo Winfo);

	OBJECT(int lightscount);

	/* Конструктор для объектов*/
	/* materialID - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
	/* path - путь к модели */
	OBJECT(int materialID, const char *path);

	/* Конструктор для объектов*/
	/* materialID - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
	/* lightscount - число источников света */
	/* path - путь к модели */
	OBJECT(int materialID, int lightscount, bool blinn, const char *path);

	/* Деструктор */
	~OBJECT();

	/* Возвращает указатель на шейдер */
	GLuint getShaderID();

	/* Создаёт матрицу модели */
	/* position - позиция объекта */
	/* initialangle - начальный угол поворота */
	/* axis - ось поворота */
	/* size - исходный размер */
	void createModelMatrix(vec3 position, float initialangle, char axis[], float size);

	/* Создаёт матрицу модели */
	/* position - позиция объекта */
	/* initialangle - начальный угол поворота */
	/* axis - ось поворота */
	/* size - исходный размер */
	void createModelMatrix(vec3 position, float initialangle, vec3 axis, float size);

	/* Возвращает угол поворота */
	float getAngle();

	/* Возвращает ось поворота */
	vec3 getAxis();

	/* Задаёт размер объекта */
	void increaseScale(float value);

	/* Возвращает размер объекта */
	float getScale();

	/* Вращение объекта на заданный угол вокруг заданной оси */
	/* angle - угол в градусах */
	/* axis - ось вращения: X, Y, Z, XY, XZ, YZ, XYZ */
	void increaseRotation(float angle, char axis[]);

	/* Вращение объекта на заданный угол вокруг заданной оси */
	/* angle - угол в градусах */
	/* axis - ось вращения: X, Y, Z, XY, XZ, YZ, XYZ */
	void increaseRotation(float angle, vec3 axis);

	/* Задаёт позицию объекта */
	void setPosition(float x, float y, float z);

	/* Задаёт позицию объекта */
	void setPosition(vec3 position);

	/* Возвращает позицию объекта */
	vec3 getPosition();

	/* Задаёт материал */
	/* 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
	void setMaterial(int value);

	/* Возвращает материал */
	int getMaterial();

	/* Задаёт фоновый цвет */
	void setAmbientColor(float r, float g, float b);

	/* Задаёт фоновый цвет */
	void setAmbientColor(vec3 color);

	/* Возвращает фоновый */
	vec3 getAmbientColor();

	/* Задаёт диффузный цвет */
	void setDiffuseColor(float r, float g, float b);

	/* Задаёт диффузный цвет */
	void setDiffuseColor(vec3 color);

	/* Возвращает диффузный цвет */
	vec3 getDiffuseColor();

	/* Задаёт отражённый цвет */
	void setSpecularColor(float r, float g, float b);

	/* Задаёт отражённый цвет */
	void setSpecularColor(vec3 color);

	/* Возвращает отражённый цвет */
	vec3 getSpecularColor();

	/* Задаёт силу блеска */
	void setShinePower(float value);

	/* Возвращает силу блеска */
	float getShinePower();

	/* Задаёт индекс преломления >= 1 */
	/* 1.0 - Воздух, 1.309 - лёд, 1.33 - вода, 1.52 - стекло (по-умолчанию), 2.42 - алмаз */
	void setRefractiveIndex(float value);

	/* Возвращает индекс преломления */
	float getRefractiveIndex();

	/* Задаёт диффузную текстуру */
	void setDiffuseTexture(const char *path, bool DDS);

	/* Задаёт карту отражений */
	void setSpecularTexture(const char *path, bool DDS);

	/* задаёт карту нормалей */
	void setNormalTexture(const char *path);

	/* Задаёт позицию источников света по его ID */
	void setLightPosition(int id, float x, float y, float z);

	/* Задаёт позицию источников света по его ID */
	void setLightPosition(int id, vec3 position);

	/* Задаёт позицию источников света */
	void setLightsPositions(vec3 positions[]);

	/* Возвращает позицию источника света по его ID */
	vec3 getLightPosition(int id);

	/* Задаёт цвет источника света по его ID */
	void setLightColor(int id, float r, float g, float b);

	/* Задаёт цвет источника света по его ID */
	void setLightColor(int id, vec3 color);

	/* Задаёт цвета всех источников света */
	void setLightsColors(vec3 color[]);

	/* Возвращает цвет источника света по его ID */
	vec3 getLightColor(int id);

	/* Задаёт свойства источника света по его ID */
	void setLightProperties(int id, float power, float constcoeff, float lincoeff, float quadcoeff);

	/* Задаёт свойства источника света по его ID */
	/* Порядок: power, constant, linear, quadratic */
	void setLightProperties(int id, vec4 properties);

	/* Задаёт свойства всех источников света */
	void setLightsProperties(vec4 properties[]);

	/* Задаёт модель освещения */
	void setBlinn(bool value);

	/* Возвращает свойства источника света по его ID */
	vec4 getlightProperties(int id);

	/* Задаёт CubeMap-текстуру */
	void setCubeMapTexture(GLuint value);

	/* Возврщает указатель на CubeMap-текстуру */
	GLuint getCubeMapTexture();

	/* Выполняет инициализацию объекта */
	void Prepare();

	/* Рендеринг объекта */
	/* CameraPosition - позиция камеры */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void Render(vec3 CameraPosition, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* Инициализация координатных осей */
	void PrepareAxes();

	/* Рендеринг координатных осей */
	/* ViewMatrix - матрица вида для осей */
	void RenderAxes(mat4 ViewMatrix);

	/* Инициализация Скайбокса */
	void PrepareSkyBox(float SkyBoxHalfSide);

	/* Рендеринг Скайбокса */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSkyBox(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);
};

#endif
