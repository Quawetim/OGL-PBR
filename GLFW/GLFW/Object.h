#ifndef OBJECT_H
#define OBJECT_H

#include "Includes.h"

class OBJECT
{
private:
	/* WindowInfo - �������� ���������� �� ���� */
	windowInfo WindowInfo;

	/* ModelLoadedFlag - ���� ��������/���������� �������� ������ */
	/* DiffuseTextureFlag - ���� �������/���������� ��������� �������� */
	/* SpecularTextureFlag - ���� �������/���������� ����� ��������� */
	/* NormalTextureFlag - ���� �������/���������� ����� �������� */
	bool ModelLoadedFlag = false;
	bool DiffuseTextureFlag = false;
	bool SpecularTextureFlag = false;
	bool NormalTextureFlag = false;

	char Name[50];
	/* Material - ��������*/
	/* ID - ���: 0 - �������� ����, 1 - ����������� ����, 2 - ������, 3 - �������,  4 - Normal Mapping*/
	/* AmbientColor - ������� ���� */
	/* DiffuseColor - ��������� ���� */
	/* SpecularColor - ��������� ���� */
	/* Shine - ���� ������ */
	/* RefractionIndex - ������ �����������: 1.0 - ������, 1.309 - ��, 1.33 - ����, 1.52 - ������ (��-���������), 2.42 - ����� */
	struct material
	{
		int ID;
		vec3 AmbientColor = vec3(0.05f, 0.05f, 0.05f);
		vec3 DiffuseColor = vec3(0.5f, 0.5f, 0.5f);
		vec3 SpecularColor = vec3(0.7f, 0.7f, 0.7f);
		float Shine = 8.0f;
		float RefractiveIndex = 1.52f;
	} Material;

	/* LightsCount - ����� ���������� ��������� */
	/* Blinn - ������ ��������� ������-����� */
	int LightsCount = 0;
	bool Blinn = false;

	/* Position - ������� ������� */
	/* Axis - ��� ���������� �������� */
	/* Angle - ���� �������� */
	/* Scale - ������ */
	vec3 Position = vec3(0.0f, 0.0f, 0.0f), Axis = vec3(0.0f, 1.0f, 0.0f);
	float Angle = 0.0f, Scale = 1.0f;

	/* PointLight - �������� �������� ����� */
	/* Position - ������� */
	/* Color - ���� */
	/* Power - ���� ����� */
	/* Constant - ���������� ����������� ��������� */
	/* Linear - �������� ����������� ��������� */
	/* Quadratic - ������������ ����������� ��������� */
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

	/* ModelMatrix - ������� ������ */
	mat4 ModelMatrix = mat4(1.0f);

	/* �������������� �������, ��������� �����, ������ � ������� ��� ��������*/
	GLuint ProjectionMatrixID, ViewMatrixID, ModelMatrixID;
	GLuint BlinnID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MaterialShineID, RefractiveIndexID;
	GLuint *PointLightPositionsIDs, *PointLightColorsIDs, *PointLightPowersIDs, *PointLightConstantsIDs, *PointLightLinearsIDs, *PointLightQuadraticsIDs;
	GLuint ShaderID, CameraPositionID, LightsCountID;
	GLuint DiffuseTextureFlagID, NormalTextureFlagID, SpecularTextureFlagID;
	GLuint DiffuseTextureID, NormalTextureID, SpecularTextureID, cubemapTextureID;
	/* �������� */
	GLuint DiffuseTexture = 0, SpecularTexture = 0, NormalTexture = 0, CubeMapTexture = 0;
	/* ������ */
	GLuint vertexbuffer, colorbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer, elementbuffer;

	vector<vec3> vertices, normals, tangents, bitangents;
	vector<vec2> uvs;
	vector<unsigned short> indices;

	/* ���������� ������ ��� ������� ��������� ����� */
	void PrepareSolidColor();

	/* ��������� ������� ��������� ����� */
	/* ProjectionMatrix - ������� �������� */
	/* ViewMatrix - ������� ���� */
	void RenderSolidColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* ���������� ������ ��� ������� ������������ ����� */
	void PrepareGradientColor();

	/* ��������� ������� ������������ ����� */
	/* ProjectionMatrix - ������� �������� */
	/* ViewMatrix - ������� ���� */
	void RenderGradientColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* ���������� ������ ��� ������/������� */
	void PrepareReflectionRefraction();

	/* ��������� ������/������� */
	/* Camera - ������� ������ */
	/* ProjectionMatrix - ������� �������� */
	/* ViewMatrix - ������� ���� */
	void RenderReflectionRefraction(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* ���������� ������ ��� �������� � ������ �������� */
	void PrepareNormalMapping();

	/* ��������� �������� � ������ �������� */
	/* ProjectionMatrix - ������� �������� */
	/* ViewMatrix - ������� ���� */
	void RenderNormalMapping(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* �������� ������ �� obj-�����, ���������� �����/������ */
	/* Path - ���� � ����� */
	/* Vertices - ������ ��������� ������, ������������ �������� */
	/* UVs - ������ ���������� ��������� ������, ������������ �������� */
	/* Normals - ������ �������� ������, ������������ �������� */
	bool LoadOBJ(const char *Path, vector<vec3> &Vertices, vector<vec2> &UVs, vector<vec3> &Normals);

	/* �������� ���������� � ������������ �������� */
	/* VertexShader - ���� � ���������� ������� */
	/* FragmentShader - ���� � ������������ (�����������) ������� */
	bool LoadShaders(const char *VertexShader, const char* GeometryShader, const char *FragmentShader);

	/* ���������� ����������� � ������������� */
	/* Vertices - ������ ��������� ������ */
	/* UVs - ������ ���������� ��������� ������ */
	/* Normals - ������ �������� ������ */
	/* Tangents - ������ ����������� ��������, ������������ �������� */
	/* Bitangents - ������ ������������� ��������, ������������ �������� */
	void ComputeTBT(vector<vec3> Vertices, vector<vec2> UVs, vector<vec3> Normals, vector<vec3> &Tangents, vector<vec3> &Bitangents);

	/* �������� 6 CubeMap �������, ���������� ������������� ����������� �������� */
	/* �������: +X (�����), -X (����), +Y (����), -Y (���), +Z (�����), -Z (���) */
	/* faces - ������ ����������� ��� ������ ����� */
	GLuint LoadCubeMap(vector<const GLchar*> faces);

public:
	/* ����������� ��-��������� */
	OBJECT();

	/* ����������� ��� ��������� � ����������� ���� */
	OBJECT(windowInfo Winfo);

	OBJECT(int lightscount);

	/* ����������� ��� ��������*/
	/* materialID - ��������: 0 - �������� ����, 1 - ����������� ����, 2 - ������, 3 - �������,  4 - Normal Mapping */
	/* path - ���� � ������ */
	OBJECT(int materialID, const char *path);

	/* ����������� ��� ��������*/
	/* materialID - ��������: 0 - �������� ����, 1 - ����������� ����, 2 - ������, 3 - �������,  4 - Normal Mapping */
	/* lightscount - ����� ���������� ����� */
	/* path - ���� � ������ */
	OBJECT(int materialID, int lightscount, bool blinn, const char *path);

	/* ���������� */
	~OBJECT();

	/* ���������� ��������� �� ������ */
	GLuint getShaderID();

	/* ������ ������� ������ */
	/* position - ������� ������� */
	/* initialangle - ��������� ���� �������� */
	/* axis - ��� �������� */
	/* size - �������� ������ */
	void createModelMatrix(vec3 position, float initialangle, char axis[], float size);

	/* ������ ������� ������ */
	/* position - ������� ������� */
	/* initialangle - ��������� ���� �������� */
	/* axis - ��� �������� */
	/* size - �������� ������ */
	void createModelMatrix(vec3 position, float initialangle, vec3 axis, float size);

	/* ���������� ���� �������� */
	float getAngle();

	/* ���������� ��� �������� */
	vec3 getAxis();

	/* ����� ������ ������� */
	void increaseScale(float value);

	/* ���������� ������ ������� */
	float getScale();

	/* �������� ������� �� �������� ���� ������ �������� ��� */
	/* angle - ���� � �������� */
	/* axis - ��� ��������: X, Y, Z, XY, XZ, YZ, XYZ */
	void increaseRotation(float angle, char axis[]);

	/* �������� ������� �� �������� ���� ������ �������� ��� */
	/* angle - ���� � �������� */
	/* axis - ��� ��������: X, Y, Z, XY, XZ, YZ, XYZ */
	void increaseRotation(float angle, vec3 axis);

	/* ����� ������� ������� */
	void setPosition(float x, float y, float z);

	/* ����� ������� ������� */
	void setPosition(vec3 position);

	/* ���������� ������� ������� */
	vec3 getPosition();

	/* ����� �������� */
	/* 0 - �������� ����, 1 - ����������� ����, 2 - ������, 3 - �������,  4 - Normal Mapping */
	void setMaterial(int value);

	/* ���������� �������� */
	int getMaterial();

	/* ����� ������� ���� */
	void setAmbientColor(float r, float g, float b);

	/* ����� ������� ���� */
	void setAmbientColor(vec3 color);

	/* ���������� ������� */
	vec3 getAmbientColor();

	/* ����� ��������� ���� */
	void setDiffuseColor(float r, float g, float b);

	/* ����� ��������� ���� */
	void setDiffuseColor(vec3 color);

	/* ���������� ��������� ���� */
	vec3 getDiffuseColor();

	/* ����� ��������� ���� */
	void setSpecularColor(float r, float g, float b);

	/* ����� ��������� ���� */
	void setSpecularColor(vec3 color);

	/* ���������� ��������� ���� */
	vec3 getSpecularColor();

	/* ����� ���� ������ */
	void setShinePower(float value);

	/* ���������� ���� ������ */
	float getShinePower();

	/* ����� ������ ����������� >= 1 */
	/* 1.0 - ������, 1.309 - ��, 1.33 - ����, 1.52 - ������ (��-���������), 2.42 - ����� */
	void setRefractiveIndex(float value);

	/* ���������� ������ ����������� */
	float getRefractiveIndex();

	/* ����� ��������� �������� */
	void setDiffuseTexture(const char *path, bool DDS);

	/* ����� ����� ��������� */
	void setSpecularTexture(const char *path, bool DDS);

	/* ����� ����� �������� */
	void setNormalTexture(const char *path);

	/* ����� ������� ���������� ����� �� ��� ID */
	void setLightPosition(int id, float x, float y, float z);

	/* ����� ������� ���������� ����� �� ��� ID */
	void setLightPosition(int id, vec3 position);

	/* ����� ������� ���������� ����� */
	void setLightsPositions(vec3 positions[]);

	/* ���������� ������� ��������� ����� �� ��� ID */
	vec3 getLightPosition(int id);

	/* ����� ���� ��������� ����� �� ��� ID */
	void setLightColor(int id, float r, float g, float b);

	/* ����� ���� ��������� ����� �� ��� ID */
	void setLightColor(int id, vec3 color);

	/* ����� ����� ���� ���������� ����� */
	void setLightsColors(vec3 color[]);

	/* ���������� ���� ��������� ����� �� ��� ID */
	vec3 getLightColor(int id);

	/* ����� �������� ��������� ����� �� ��� ID */
	void setLightProperties(int id, float power, float constcoeff, float lincoeff, float quadcoeff);

	/* ����� �������� ��������� ����� �� ��� ID */
	/* �������: power, constant, linear, quadratic */
	void setLightProperties(int id, vec4 properties);

	/* ����� �������� ���� ���������� ����� */
	void setLightsProperties(vec4 properties[]);

	/* ����� ������ ��������� */
	void setBlinn(bool value);

	/* ���������� �������� ��������� ����� �� ��� ID */
	vec4 getlightProperties(int id);

	/* ����� CubeMap-�������� */
	void setCubeMapTexture(GLuint value);

	/* ��������� ��������� �� CubeMap-�������� */
	GLuint getCubeMapTexture();

	/* ��������� ������������� ������� */
	void Prepare();

	/* ��������� ������� */
	/* CameraPosition - ������� ������ */
	/* ProjectionMatrix - ������� �������� */
	/* ViewMatrix - ������� ���� */
	void Render(vec3 CameraPosition, mat4 ProjectionMatrix, mat4 ViewMatrix);

	/* ������������� ������������ ���� */
	void PrepareAxes();

	/* ��������� ������������ ���� */
	/* ViewMatrix - ������� ���� ��� ���� */
	void RenderAxes(mat4 ViewMatrix);

	/* ������������� ��������� */
	void PrepareSkyBox(float SkyBoxHalfSide);

	/* ��������� ��������� */
	/* ProjectionMatrix - ������� �������� */
	/* ViewMatrix - ������� ���� */
	void RenderSkyBox(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix);
};

#endif
