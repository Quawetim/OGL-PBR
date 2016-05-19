#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

using namespace std;
using namespace glm;

GLFWwindow* window;

#include "TextureLoader.h"
#include "VboIndexer.h"
#include "Text.h"

/* WindowWidth, WindowHeight - ширина и высота окна */
/* FOV - field of view */
/* CameraMode - выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная */
/* GenTextureSize - размер генерируемой текстуры */
/* Wireframe - отображение сетки объектов, переключение по F1 */
/* StopRotations - переключение вращений по F2 */
/* ShowLights - переключение отображения источников света по F3 */
/* MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map */
int WindowWidth = 1280, WindowHeight = 800;
int CameraMode = 2;
int GenTextureSize = 512;
float FOV = 90.0f;
float SkyBoxSide = 500.0f;
bool Wireframe = false;
bool StopRotations = true;
bool ShowLights = false;
bool MirrorExample = false;
bool FullSceen = false;

/* Обработка ошибок */
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* Обработка клавиатуры */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static int PrevCamera;
	/* Клавиши для выхода из приложения */
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	/* Переключение камер №1 и №2 */
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		CameraMode++;
		if (CameraMode > 2) CameraMode = 1;
	}

	/* Включение камеры №3 */
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		if (CameraMode != 3)
		{
			PrevCamera = CameraMode;
			CameraMode = 3;
		}
		else CameraMode = PrevCamera;
	}

	/* Переключение отображения полигональной сетки */
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		if (Wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			Wireframe = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Wireframe = true;
		}
	}

	/* Переключение примеров работы */
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (MirrorExample)
		{
			MirrorExample = false;
		}
		else
		{
			MirrorExample = true;
		}
	}

	/* Включение/отключение вращений */
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		if (StopRotations)
		{
			StopRotations = false;
		}
		else
		{
			StopRotations = true;
		}
	}

	/* Включение/отключение отображения источников света */
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		if (ShowLights)
		{
			ShowLights = false;
		}
		else
		{
			ShowLights = true;
		}
	}
}

/* Обработка кнопок мышки */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetKey(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) FOV = 90.0f;
}

/* Обработка колёсика мышки */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
	{
		if (FOV > 10.0f) FOV -= (float)yoffset * 10.0f;
		else FOV = 10.0f;
	}
	else
	{
		if (FOV < 90.0f) FOV -= (float)yoffset * 10.0f;
		else FOV = 90.0f;
	}
}

/* Вывод ошибок в консоль */
void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	printf("OpenGL Debug Output message : ");

	if (source == GL_DEBUG_SOURCE_API_ARB) printf("Source : API; ");
	else
	{
		if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB) printf("Source : WINDOW_SYSTEM; ");
		else
		{
			if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB) printf("Source : SHADER_COMPILER; ");
			else
			{
				if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB) printf("Source : THIRD_PARTY; ");
				else
				{
					if (source == GL_DEBUG_SOURCE_APPLICATION_ARB) printf("Source : APPLICATION; ");
					else
					{
						if (source == GL_DEBUG_SOURCE_OTHER_ARB) printf("Source : OTHER; ");
					}
				}
			}
		}
	}

	if (type == GL_DEBUG_TYPE_ERROR_ARB) printf("Type : ERROR; ");
	else
	{
		if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB) printf("Type : DEPRECATED_BEHAVIOR; ");
		else
		{
			if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB) printf("Type : UNDEFINED_BEHAVIOR; ");
			else
			{
				if (type == GL_DEBUG_TYPE_PORTABILITY_ARB) printf("Type : PORTABILITY; ");
				else
				{
					if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB) printf("Type : PERFORMANCE; ");
					else
					{
						if (type == GL_DEBUG_TYPE_OTHER_ARB) printf("Type : OTHER; ");
					}
				}
			}
		}
	}

	if (severity == GL_DEBUG_SEVERITY_HIGH_ARB) printf("Severity : HIGH; ");
	else
	{
		if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB) printf("Severity : MEDIUM; ");
		else
		{
			if (severity == GL_DEBUG_SEVERITY_LOW_ARB) printf("Severity : LOW; ");
		}
	}

	printf("Message : %s\n", message);
}

class CAMERA
{
private:
	/* CameraPosition - позиция камеры */
	/* CameraLookTo - точка, на которую направлена камера */
	/* CameraUp - направление верха камеры */
	/* CameraStaticPosition - позиция статической камеры */
	/* Radius, RadiusMin, RadiusMax - радиус полёта камеры от начала координат для камеры №2 и его минимальное и максимальное значения */
	/* Speed, Speed2 - скорость движения камер №1 и №2  */
	/* MouseSpeed - скорость движения мышки */
	vec3 CameraPosition, CameraLookTo = vec3(0.0f, 0.0f, 0.0f), CameraUp = vec3(0.0f, 1.0f, 0.0f), CameraStaticPosition = vec3(8.0f, 3.0f, 6.0f);
	float Pi = pi<float>();
	float DeltaTime, Radius = 20.0f, RadiusMin = 2.0f, RadiusMax = 100.0f;
	float Speed = 15.0f, MouseSpeed = 0.003f;

	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	/* ViewMatrix - матрица вида для координатных осей */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* Обработка клавиатуры для движения камеры №2 */
	/* window - указатель на окно */
	void CheckMove(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { if (Radius > RadiusMin) Radius -= DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { if (Radius < RadiusMax) Radius += DeltaTime * Speed; }
	}

	/* Обработка клавиатуры для движения камеры №1 */
	/* window - указатель на окно */
	/* direction - направление камеры */
	/* right - вектор "вправо" для камеры */
	void CheckMove(GLFWwindow* window, vec3 &Position, vec3 Direction, vec3 Right)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { Position += normalize(Direction) * DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { Position -= normalize(Direction) * DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { Position += normalize(Right) * DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { Position -= normalize(Right) * DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { Position.y += DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { Position.y -= DeltaTime * Speed; }

		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) Speed += 1.0f;
		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) Speed -= 1.0f;

		if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) Speed = 15.0f;

		if (Position.x > SkyBoxSide) Position.x = SkyBoxSide - 0.1f;
		if (Position.x < -SkyBoxSide) Position.x = -SkyBoxSide + 0.1f;
		if (Position.y > SkyBoxSide) Position.y = SkyBoxSide - 0.1f;
		if (Position.y < -SkyBoxSide) Position.y = -SkyBoxSide + 0.1f;
		if (Position.z > SkyBoxSide) Position.z = SkyBoxSide - 0.1f;
		if (Position.z < -SkyBoxSide) Position.z = -SkyBoxSide + 0.1f;
	}

public:
	/* Возвращает матрицу проекции */
	mat4 getProjectionMatrix() { return ProjectionMatrix; }
	/* Возвращает матрицу вида */
	mat4 getViewMatrix() { return ViewMatrix; }
	/* Возвращает матрицу вида для координатных осей */
	mat4 getViewMatrixAxes() { return ViewMatrixAxes; }

	/* Вычисление матриц вида и проекции, возвращает позицию камеры */
	/* window - указатель окна */
	/* CameraMode - выбранная камера */
	vec3 ComputeViewMatrix(GLFWwindow* window, int CameraMode)
	{
		double CurrentTime, MouseX, MouseY;
		static double LastTime = glfwGetTime();

		CurrentTime = glfwGetTime();
		DeltaTime = float(CurrentTime - LastTime);

		glfwGetCursorPos(window, &MouseX, &MouseY);
		glfwSetCursorPos(window, WindowWidth / 2.0f, WindowHeight / 2.0f);

		switch (CameraMode)
		{
			/* От первого лица */
		case 1:
		{
			static vec3 Position = vec3(Radius, 0.0f, 0.0f), Direction, Right;
			static float HorizontalAngle = radians(180.0f), VerticalAngle = radians(0.0f);

			if (degrees(VerticalAngle) > 90.0f) VerticalAngle = radians(89.9f);
			if (degrees(VerticalAngle) < -90.0f) VerticalAngle = radians(-89.9f);

			Direction = normalize(vec3(cos(HorizontalAngle) * cos(VerticalAngle), sin(VerticalAngle), sin(-HorizontalAngle) * cos(VerticalAngle)));
			Right = cross(Direction, CameraUp);

			CheckMove(window, Position, Direction, Right);

			ViewMatrix = lookAt(Position, Position + Direction, CameraUp);

			ViewMatrixAxes = lookAt(vec3(-5.0f * cos(HorizontalAngle), Position.y, 5.0f * sin(HorizontalAngle)), CameraLookTo, CameraUp);

			CameraPosition = Position;

			HorizontalAngle += MouseSpeed * (float)(WindowWidth / 2.0f - MouseX);
			VerticalAngle += MouseSpeed * (float)(WindowHeight / 2.0f - MouseY);
			break;
		}
		/* От третьего лица */
		case 2:
		{
			static vec3 Position;
			static float HorizontalAngle = radians(0.0f), VerticalAngle = radians(-90.0f);

			if (degrees(VerticalAngle) > 0.0f) VerticalAngle = radians(-0.9f);
			if (degrees(VerticalAngle) < -180.0f) VerticalAngle = radians(-179.9f);

			/* Переход из сферической системы в декартову */
			Position = vec3(-Radius * sin(VerticalAngle) * cos(HorizontalAngle), Radius * cos(VerticalAngle), Radius * sin(VerticalAngle) * sin(-HorizontalAngle));

			CheckMove(window);

			ViewMatrix = lookAt(Position, CameraLookTo, CameraUp);

			ViewMatrixAxes = lookAt(vec3(5.0f * cos(HorizontalAngle), 5.0f * cos(VerticalAngle), 5.0 * sin(HorizontalAngle)), CameraLookTo, CameraUp);

			CameraPosition = Position;

			HorizontalAngle += MouseSpeed * (float)(WindowWidth / 2.0f - MouseX);
			VerticalAngle += MouseSpeed * (float)(WindowHeight / 2.0f - MouseY);
			break;
		}
		/* Фиксированная */
		case 3:
		{
			ViewMatrix = lookAt(CameraStaticPosition, CameraLookTo, CameraUp);

			ViewMatrixAxes = ViewMatrix;

			CameraPosition = CameraStaticPosition;
			break;
		}
		default:
			break;
		}

		ProjectionMatrix = perspective(radians(FOV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);

		LastTime = CurrentTime;

		return CameraPosition;
	}
};

class OBJECT
{
private:
	/* Material - материал*/
	/* ID - тип: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - "цилиндр"*/
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
	int LightsCount = 0;	

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
	mat3 ModelView3x3Matrix;

	/* Идентификаторы шейдера, источника света, матриц и текстур для шейдеров*/
	GLuint ShaderID, CameraPositionID, LightID, LightsCountID, RefractiveIndexID;
	GLuint ProjectionMatrixID, ViewMatrixID, ModelMatrixID, ModelView3x3MatrixID;
	GLuint DiffuseTextureID, NormalTextureID, SpecularTextureID, cubemapTextureID;
	/* Текстуры */
	GLuint DiffuseTexture, SpecularTexture, NormalTexture, CubeMapTexture;
	/* Буферы */
	GLuint vertexbuffer, colorbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer, elementbuffer;

	/* */
	vec3 LightPosition;
	vector<vec3> vertices, normals, tangents, bitangents;
	vector<vec2> uvs;
	vector<unsigned short> indices;

	/* Подготовка данных для объекта сплошного цвета */
	void PrepareSolidColor()
	{
		LoadShaders("shaders//SolidColor.vs", "shaders//SolidColor.fs");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	/* Рендеринг объекта сплошного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSolidColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glUniformMatrix4fv(glGetUniformLocation(ShaderID, "P"), 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(ShaderID, "V"), 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(ShaderID, "M"), 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniform3f(glGetUniformLocation(ShaderID, "Material.AmbientColor"), Material.AmbientColor.x, Material.AmbientColor.y, Material.AmbientColor.z);
		glUniform3f(glGetUniformLocation(ShaderID, "Material.DiffuseColor"), Material.DiffuseColor.x, Material.DiffuseColor.y, Material.DiffuseColor.z);
		glUniform3f(glGetUniformLocation(ShaderID, "Material.SpecularColor"), Material.SpecularColor.x, Material.SpecularColor.y, Material.SpecularColor.z);
		glUniform1f(glGetUniformLocation(ShaderID, "Material.Shine"), Material.Shine);
		glUniform1i(glGetUniformLocation(ShaderID, "LightsCount"), LightsCount);
		glUniform3f(glGetUniformLocation(ShaderID, "CameraPosition"), Camera.x, Camera.y, Camera.z);
		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			glUniform3f(glGetUniformLocation(ShaderID, buf), PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);
			sprintf(buf, "PointLight[%d].Color", i);
			glUniform3f(glGetUniformLocation(ShaderID, buf), PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
			sprintf(buf, "PointLight[%d].Power", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Power);
			sprintf(buf, "PointLight[%d].Constant", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Constant);
			sprintf(buf, "PointLight[%d].Linear", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Linear);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Quadratic);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для объекта градиентного цвета */
	void PrepareGradientColor()
	{
		LoadShaders("shaders//GradientColor.vs", "shaders//GradientColor.fs");

		float *colorbuffer_data = new float[vertices.size() * sizeof(vec3) * 3];

		for (unsigned int i = 0; i < vertices.size() * sizeof(vec3) * 3; i += 3)
		{
			if (i % 2 == 0)
			{
				colorbuffer_data[i] = 0.33f;
				colorbuffer_data[i + 1] = 0.66f;
				colorbuffer_data[i + 2] = 0.99f;
			}
			else
			{
				colorbuffer_data[i] = 0.24f;
				colorbuffer_data[i + 1] = 0.16f;
				colorbuffer_data[i + 2] = 0.71f;
			}
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * sizeof(vec3) * 3, colorbuffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	/* Рендеринг объекта градиентного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderGradientColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glUniformMatrix4fv(glGetUniformLocation(ShaderID, "P"), 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(ShaderID, "V"), 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(ShaderID, "M"), 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniform3f(glGetUniformLocation(ShaderID, "Material.AmbientColor"), Material.AmbientColor.x, Material.AmbientColor.y, Material.AmbientColor.z);
		//glUniform3f(glGetUniformLocation(ShaderID, "Material.DiffuseColor"), Material.DiffuseColor.x, Material.DiffuseColor.y, Material.DiffuseColor.z);
		glUniform3f(glGetUniformLocation(ShaderID, "Material.SpecularColor"), Material.SpecularColor.x, Material.SpecularColor.y, Material.SpecularColor.z);
		glUniform1f(glGetUniformLocation(ShaderID, "Material.Shine"), Material.Shine);
		glUniform1i(glGetUniformLocation(ShaderID, "LightsCount"), LightsCount);
		glUniform3f(glGetUniformLocation(ShaderID, "CameraPosition"), Camera.x, Camera.y, Camera.z);
		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			glUniform3f(glGetUniformLocation(ShaderID, buf), PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);
			sprintf(buf, "PointLight[%d].Color", i);
			glUniform3f(glGetUniformLocation(ShaderID, buf), PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
			sprintf(buf, "PointLight[%d].Power", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Power);
			sprintf(buf, "PointLight[%d].Constant", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Constant);
			sprintf(buf, "PointLight[%d].Linear", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Linear);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Quadratic);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для стекла/зеркала */
	void PrepareReflectionRefraction()
	{
		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");	
		CameraPositionID = glGetUniformLocation(ShaderID, "CameraPosition");
		RefractiveIndexID = glGetUniformLocation(ShaderID, "RefractiveIndex");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	/* Рендеринг стекла/зеркала */
	/* Camera - позиция камеры */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderReflectionRefraction(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));			
		glUniform3f(CameraPositionID, Camera.x, Camera.y, Camera.z);
		glUniform1f(RefractiveIndexID, Material.RefractiveIndex);

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для цилиндра с картой нормалей */
	void PrepareCylinder()
	{
		LoadShaders("shaders//Cylinder.vs", "shaders//Cylinder.fs");

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");
		ModelView3x3MatrixID = glGetUniformLocation(ShaderID, "MV3x3");
		DiffuseTextureID = glGetUniformLocation(ShaderID, "DiffuseTexture");
		NormalTextureID = glGetUniformLocation(ShaderID, "NormalTexture");
		SpecularTextureID = glGetUniformLocation(ShaderID, "SpecularTexture");
		LightID = glGetUniformLocation(ShaderID, "LightPosition_worldspace");

		DiffuseTexture = LoadDDS("textures//diffuse.DDS");
		NormalTexture = LoadBMP("textures//normal.bmp");
		SpecularTexture = LoadDDS("textures//specular.DDS");	

		ComputeTBT(vertices, uvs, normals, tangents, bitangents);

		vector<vec3> indexed_vertices;
		vector<vec2> indexed_uvs;
		vector<vec3> indexed_normals;
		vector<vec3> indexed_tangents;
		vector<vec3> indexed_bitangents;
		IndexVBO(vertices, uvs, normals, tangents, bitangents, indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(vec3), &indexed_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(vec2), &indexed_uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(vec3), &indexed_normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(vec3), &indexed_tangents[0], GL_STATIC_DRAW);

		glGenBuffers(1, &bitangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);
	}

	/* Рендеринг цилиндра с картой нормалей */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderCylinder(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		ModelView3x3Matrix = mat3(ViewMatrix * ModelMatrix);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));	
		glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, value_ptr(ModelView3x3Matrix));

		glUniform3f(LightID, LightPosition.x, LightPosition.y, LightPosition.z);

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
		glUniform1i(DiffuseTextureID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NormalTexture);
		glUniform1i(NormalTextureID, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, SpecularTexture);
		glUniform1i(SpecularTextureID, 2);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

		glBindVertexArray(0);
	}

	/* Загрузка модели из obj-файла, возвращает успех/ошибку */
	/* Path - путь к файлу */
	/* Vertices - список координат вершин, возвращаемое значение */
	/* UVs - список текстурных координат вершин, возвращаемое значение */
	/* Normals - список нормалей вершин, возвращаемое значение */
	bool LoadOBJ(const char *Path, vector<vec3> &Vertices, vector<vec2> &UVs, vector<vec3> &Normals)
	{
		char Buf[128];
		vector<int> VertexIndices, UVIndices, NormalIndices;
		vector<vec3> tmp_Vertices;
		vector<vec2> tmp_UVs;
		vector<vec3> tmp_Normals;

		printf("Loading OBJ file %s...\n", Path);		

		FILE *Fin = fopen(Path, "r");		

		if (Fin == NULL)
		{
			printf("OBJ file not found.\n");
			return false;
		}

		while (true)
		{
			if (fscanf(Fin, "%s", Buf) == EOF) break;

			if (strcmp(Buf, "v") == 0)
			{
				/* Координаты вершины */
				vec3 vertex;

				fscanf(Fin, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tmp_Vertices.push_back(vertex);
			}
			else
			{
				if (strcmp(Buf, "vt") == 0)
				{
					/* Текстурные координаты */
					vec2 uv;

					fscanf(Fin, "%f %f\n", &uv.x, &uv.y);
					uv.y = -uv.y; // ивертируем для DDS текстуры. Убрать для TGA или BMP.
					tmp_UVs.push_back(uv);
				}
				else
				{
					if (strcmp(Buf, "vn") == 0)
					{
						/* Нормаль */
						vec3 normal;

						fscanf(Fin, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
						tmp_Normals.push_back(normal);
					}
					else
					{
						/* Грань (полигон) */
						if (strcmp(Buf, "f") == 0)
						{
							int VertexIndex[3], UVIndex[3], NormalIndex[3];

							int count = fscanf(Fin, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &VertexIndex[0], &UVIndex[0], &NormalIndex[0], &VertexIndex[1], &UVIndex[1], &NormalIndex[1], &VertexIndex[2], &UVIndex[2], &NormalIndex[2]);

							if (count != 9)
							{
								printf("Can't read OBJ file.\n");
								return false;
							}

							VertexIndices.push_back(VertexIndex[0]);
							VertexIndices.push_back(VertexIndex[1]);
							VertexIndices.push_back(VertexIndex[2]);

							UVIndices.push_back(UVIndex[0]);
							UVIndices.push_back(UVIndex[1]);
							UVIndices.push_back(UVIndex[2]);

							NormalIndices.push_back(NormalIndex[0]);
							NormalIndices.push_back(NormalIndex[1]);
							NormalIndices.push_back(NormalIndex[2]);
						}
						else
						{
							/* Что-то ещё, не нужное */
							char Buf2[1000];
							fgets(Buf2, 1000, Fin);
						}
					}
				}
			}

		}

		/* Для каждой вершины в каждом треугольнике достаём данные на выход */
		for (unsigned int i = 0; i < VertexIndices.size(); i++)
		{
			int vertexIndex = VertexIndices[i];
			int uvIndex = UVIndices[i];
			int normalIndex = NormalIndices[i];

			vec3 vertex = tmp_Vertices[vertexIndex - 1];
			vec2 uv = tmp_UVs[uvIndex - 1];
			vec3 normal = tmp_Normals[normalIndex - 1];

			vertices.push_back(vertex);
			uvs.push_back(uv);
			normals.push_back(normal);
		}
		return true;
	}

	/* Загрузка вершинного и фрагментного шейдеров */
	/* VertexShader - путь к вершинному шейдеру */
	/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
	bool LoadShaders(const char *VertexShader, const char *FragmentShader)
	{
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		string VertexShaderCode;
		ifstream VertexShaderStream(VertexShader, ios::in);

		if (VertexShaderStream.is_open())
		{
			string Line = "";
			while (getline(VertexShaderStream, Line)) VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}
		else
		{
			printf("File %s not found.\n", VertexShader);
			return false;
		}

		string FragmentShaderCode;
		ifstream FragmentShaderStream(FragmentShader, ios::in);

		if (FragmentShaderStream.is_open())
		{
			string Line = "";
			while (getline(FragmentShaderStream, Line)) FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}
		else
		{
			printf("File %s not found.\n", FragmentShader);
			return false;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		printf("Compiling vertex shader: %s...\n", VertexShader);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("\nError: %s\n", &VertexShaderErrorMessage[0]);
		}

		printf("Compiling fragment shader: %s...\n", FragmentShader);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("\nError: %s\n", &FragmentShaderErrorMessage[0]);
		}

		printf("Linking program...\n");

		ShaderID = glCreateProgram();
		glAttachShader(ShaderID, VertexShaderID);
		glAttachShader(ShaderID, FragmentShaderID);
		glLinkProgram(ShaderID);

		glGetProgramiv(ShaderID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ShaderID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("\nError: %s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(ShaderID, VertexShaderID);
		glDetachShader(ShaderID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return true;
	}

	/* Вычисление касательных и бикасательных */
	/* Vertices - список координат вершин */
	/* UVs - список текстурных координат вершин */
	/* Normals - список нормалей вершин */
	/* Tangents - список касательных нормалей, возвращаемое значение */
	/* Bitangents - список бикасательных нормалей, возвращаемое значение */
	void ComputeTBT(vector<vec3> Vertices, vector<vec2> UVs, vector<vec3> Normals, vector<vec3> &Tangents, vector<vec3> &Bitangents)
	{
		float R;
		vec3 V0, V1, V2, DeltaPos1, DeltaPos2, Tangent, Bitangent, Normal;
		vec2 UV0, UV1, UV2, DeltaUV1, DeltaUV2;

		for (unsigned int i = 0; i < vertices.size(); i += 3)
		{
			V0 = Vertices[i + 0];
			V1 = Vertices[i + 1];
			V2 = Vertices[i + 2];

			UV0 = UVs[i + 0];
			UV1 = UVs[i + 1];
			UV2 = UVs[i + 2];

			DeltaPos1 = V1 - V0;
			DeltaPos2 = V2 - V0;

			DeltaUV1 = UV1 - UV0;
			DeltaUV2 = UV2 - UV0;

			R = 1.0f / (DeltaUV1.x * DeltaUV2.y - DeltaUV1.y * DeltaUV2.x);

			Tangent = R * (DeltaPos1 * DeltaUV2.y - DeltaPos2 * DeltaUV1.y);
			Bitangent = R * (DeltaPos2 * DeltaUV1.x - DeltaPos1 * DeltaUV2.x);

			Tangents.push_back(Tangent);
			Tangents.push_back(Tangent);
			Tangents.push_back(Tangent);

			Bitangents.push_back(Bitangent);
			Bitangents.push_back(Bitangent);
			Bitangents.push_back(Bitangent);
		}

		for (unsigned int i = 0; i < vertices.size(); i++)
		{
			Normal = Normals[i];
			Tangent = Tangents[i];
			Bitangent = Bitangents[i];

			/* Ортогонализация */
			Tangent = normalize(Tangent - Normal * dot(Normal, Tangent));

			/* Направление */
			if (dot(cross(Normal, Tangent), Bitangent) < 0.0f) Tangent = Tangent * -1.0f;
		}
	}

	/* Загрузка 6 CubeMap текстур, возвращает идентификатор загруженной текстуры */
	/* Порядок: +X (Право), -X (Лево), +Y (Верх), -Y (Низ), +Z (Перед), -Z (Зад) */
	/* faces - список изображений для каждой грани */
	GLuint LoadCubeMap(vector<const GLchar*> faces)
	{
		GLuint TextureID;
		glGenTextures(1, &TextureID);

		int Width, Height;
		unsigned char* Image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			Image = SOIL_load_image(faces[i], &Width, &Height, 0, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
			SOIL_free_image_data(Image);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return TextureID;
	}

public:
	/* Пустой конструктор для Скайбокса и координтных осей */
	OBJECT() {};

	/* Конструктор для объектов*/
	/* material - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - "цилиндр" */
	OBJECT(int materialID, const char *path)
	{
		Material.ID = materialID;
		LoadOBJ(path, vertices, uvs, normals);
	}

	/* Конструктор для объектов*/
	/* material - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - "цилиндр" */
	OBJECT(int materialID, int lightscount, const char *path)
	{
		Material.ID = materialID;
		LightsCount = lightscount;
		PointLight = new pointLight[LightsCount];
		LoadOBJ(path, vertices, uvs, normals);
	}

	/* Деструктор */
	~OBJECT()
	{
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &tangentbuffer);
		glDeleteBuffers(1, &bitangentbuffer);
		glDeleteBuffers(1, &elementbuffer);
		glDeleteTextures(1, &DiffuseTexture);
		glDeleteTextures(1, &NormalTexture);
		glDeleteTextures(1, &SpecularTexture);

		glDeleteVertexArrays(1, &VAO);
	}

	/* Возвращает указатель на шейдер */
	GLuint getShaderID() { return ShaderID; }

	/* Задаёт материал */
	/* 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - "цилиндр"*/
	void setMaterial(int value) { Material.ID = value; }
		
	/* Возвращает материал */
	int getMaterial() { return Material.ID; }

	/* Задаёт фоновый цвет */
	void setAmbientColor(float r, float g, float b) { Material.AmbientColor = vec3(r, g, b); }

	/* Задаёт фоновый цвет */
	void setAmbientColor(vec3 color) { Material.AmbientColor = color; }

	/* Возвращает фоновый */
	vec3 getAmbientColor() { return Material.AmbientColor; }

	/* Задаёт диффузный цвет */
	void setDiffuseColor(float r, float g, float b) { Material.DiffuseColor = vec3(r, g, b); }

	/* Задаёт диффузный цвет */
	void setDiffuseColor(vec3 color) { Material.DiffuseColor = color; }

	/* Возвращает диффузный цвет */
	vec3 getDiffuseColor() { return Material.DiffuseColor; }

	/* Задаёт отражённый цвет */
	void setSpecularColor(float r, float g, float b) { Material.SpecularColor = vec3(r, g, b); }

	/* Задаёт отражённый цвет */
	void setSpecularColor(vec3 color) { Material.SpecularColor = color; }

	/* Возвращает отражённый цвет */
	vec3 getSpecularColor() { return Material.SpecularColor; }

	/* Задаёт силу блеска */
	void setShinePower(float value) { Material.Shine = value; }

	/* Возвращает силу блеска */
	float getShinePower() { return Material.Shine; }

	/* Задаёт индекс преломления */
	/* 1.0 - Воздух, 1.309 - лёд, 1.33 - вода, 1.52 - стекло (по-умолчанию), 2.42 - алмаз */
	void setRefractiveIndex(float value) { if (value < 1.0f) value = 1.0f; Material.RefractiveIndex = value; }

	/* Возвращает индекс преломления */
	float getRefractiveIndex() { return Material.RefractiveIndex; }

	/* Задаёт CubeMap-текстуру */
	void setCubeMapTexture(GLuint value){ CubeMapTexture = value; }

	/* Возврщает указатель на CubeMap-текстуру */
	GLuint getCubeMapTexture() { return CubeMapTexture; }

	/* Задаёт размер объекта */
	void setScale(float value)
	{
		vec3 pos = getPosition();
		ModelMatrix = mat4(1.0);
		ModelMatrix *= scale(vec3(value, value, value));
		ModelMatrix[3].x = pos.x;
		ModelMatrix[3].y = pos.y;
		ModelMatrix[3].z = pos.z;
	}

	/* Возвращает размер объекта */
	float getScale() { return ModelMatrix[0].x; }

	/* Вращение объекта */
	/* angle - угол в градусах */
	/* axis - ось вращения: X, Y, Z, XY, XZ, YZ, XYZ */
	void setRotation(float angle, char axis[]) 
	{ 
		vec3 Axis;
		
		if (strcmp(axis, "X") == 0) Axis = vec3(1.0f, 0.0f, 0.0f);
		else
		{
			if (strcmp(axis, "Y") == 0) Axis = vec3(0.0f, 1.0f, 0.0f);
			else
			{
				if (strcmp(axis, "Z") == 0) Axis = vec3(0.0f, 0.0f, 1.0f);
				else
				{
					if ((strcmp(axis, "XY") == 0) || (strcmp(axis, "YX") == 0)) Axis = vec3(1.0f, 1.0f, 0.0f);
					else
					{
						if ((strcmp(axis, "XZ") == 0) || (strcmp(axis, "ZX") == 0)) Axis = vec3(1.0f, 0.0f, 1.0f);
						else
						{
							if ((strcmp(axis, "YZ") == 0) || (strcmp(axis, "ZY") == 0)) Axis = vec3(0.0f, 1.0f, 1.0f);
							else
							{
								if ((strcmp(axis, "XYZ") == 0) || (strcmp(axis, "XZY") == 0) || 
									(strcmp(axis, "YXZ") == 0) || (strcmp(axis, "YZX") == 0) || 
									(strcmp(axis, "ZXY") == 0) || (strcmp(axis, "ZYX") == 0)) Axis = vec3(1.0f, 1.0f, 1.0f);
								else return;
							}
						}
					}
				}
			}
		}
		ModelMatrix *= rotate(radians(angle), Axis); 
	}

	/* Задаёт позицию объекта */
	void setPosition(float x, float y, float z) 
	{ 
		float scale = getScale();
		ModelMatrix = mat4(1.0);
		ModelMatrix *= translate(vec3(x, y, z)); 
		ModelMatrix[0].x = scale;
		ModelMatrix[1].y = scale;
		ModelMatrix[2].z = scale;
	}	

	/* Задаёт позицию объекта */
	void setPosition(vec3 position)
	{
		float scale = getScale();
		ModelMatrix = mat4(1.0);
		ModelMatrix *= translate(vec3(position.x, position.y, position.z));
		ModelMatrix[0].x = scale;
		ModelMatrix[1].y = scale;
		ModelMatrix[2].z = scale;
	}

	/* Возвращает позицию объекта */
	vec3 getPosition() { return vec3(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z); }

	/* Задаёт позицию источника света */
	void setLightPosition(float x, float y, float z) { LightPosition = vec3(x, y, z); }

	/* Задаёт позицию источников света */
	void setLightsPositions(vec3 positions[]) { for (int i = 0; i < LightsCount; i++) PointLight[i].Position = positions[i]; }

	/* Возвращает позицию источника света по его ID */
	vec3 getLightPosition(int id) { return PointLight[id].Position; }

	/* Задаёт цвет источника света по его ID */
	void setLightColor(int id, float r, float g, float b) { PointLight[id].Color = vec3(r, g, b); }

	/* Задаёт цвет источника света по его ID */
	void setLightColor(int id, vec3 color) { PointLight[id].Color = color; }

	/* Задаёт цвета всех источников света */
	void setLightsColors(vec3 color[]) { for (int i = 0; i < LightsCount; i++) PointLight[i].Color = color[i]; }

	/* Возвращает цвет источника света по его ID */
	vec3 getLightColor(int id) { return PointLight[id].Color; }

	/* Задаёт свойства источника освещения по его ID */
	void setLightProperties(int id, float power, float constcoeff, float lincoeff, float quadcoeff) 
	{ 
		PointLight[id].Power = power; 
		PointLight[id].Constant = constcoeff;
		PointLight[id].Linear = lincoeff;
		PointLight[id].Quadratic = quadcoeff;
	}

	/* Задаёт свойства источника освещения по его ID */
	void setLightProperties(int id, vec4 properties)
	{
		PointLight[id].Power = properties.x;
		PointLight[id].Constant = properties.y;
		PointLight[id].Linear = properties.z;
		PointLight[id].Quadratic = properties.w;
	}

	/* Задаёт свойства всех источников освещения */
	void setLightsProperties(vec4 properties[])
	{
		for (int i = 0; i < LightsCount; i++)
		{
			PointLight[i].Power = properties[i].x;
			PointLight[i].Constant = properties[i].y;
			PointLight[i].Linear = properties[i].z;
			PointLight[i].Quadratic = properties[i].w;
		}
	}

	/* Возвращает свойства источника освещения по его ID */
	vec4 getlightProperties(int id) { return vec4(PointLight[id].Power, PointLight[id].Constant, PointLight[id].Linear, PointLight[id].Quadratic); }

	/* Выполняет инициализацию объекта */
	void Prepare()
	{
		switch (Material.ID)
		{
		case 0:
		{
			PrepareSolidColor();
			break;
		}
		case 1:
		{
			PrepareGradientColor();
			break;
		}
		case 2:
		{
			LoadShaders("shaders//Refraction.vs", "shaders//Refraction.fs");
			PrepareReflectionRefraction();
			break;
		}
		case 3:
		{
			LoadShaders("shaders//Reflection.vs", "shaders//Reflection.fs");
			PrepareReflectionRefraction();
			break;
		}
		case 4:
		{
			PrepareCylinder();
			break;
		}
		default:
			break;
		}
	}

	/* Рендеринг объекта */
	/* CameraPosition - позиция камеры */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void Render(vec3 CameraPosition, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		switch (Material.ID)
		{
		case 0:
		{
			RenderSolidColor(CameraPosition, ProjectionMatrix, ViewMatrix);
			break;
		}
		case 1:
		{
			RenderGradientColor(CameraPosition, ProjectionMatrix, ViewMatrix);
			break;
		}
		case 2:
		{
			RenderReflectionRefraction(CameraPosition, ProjectionMatrix, ViewMatrix);
			break;
		}
		case 3:
		{
			RenderReflectionRefraction(CameraPosition, ProjectionMatrix, ViewMatrix);
			break;
		}
		case 4:
		{
			RenderCylinder(ProjectionMatrix, ViewMatrix);
			break;
		}
		default:
			break;
		}
	}

	/* Инициализация координатных осей */
	void PrepareAxes()
	{
		LoadShaders("shaders//GradientColor.vs", "shaders//GradientColor.fs");

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");

		static const GLfloat vertexbuffer_data[] = {
			0.0f, 0.0f, 0.0f,
			3.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f,
		};

		static const GLfloat colorbuffer_data[] = {
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexbuffer_data), vertexbuffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorbuffer_data), colorbuffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		/* Вариант 2: координаты точек и их цвета хранятся в одном массиве
		static const GLfloat vertexcolorbuffer[] = {
		// XYZ, RGB для каждой точки
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
		};

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexcolorbuffer), vertexcolorbuffer, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);
		*/
	}

	/* Рендеринг координатных осей */
	/* ViewMatrix - матрица вида для осей */
	void RenderAxes(mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glViewport(5, 5, 60, 60);

		mat4 ProjectionMatrix = perspective(radians(45.0f), 1.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));

		glBindVertexArray(VAO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);

		glViewport(0, 0, WindowWidth, WindowHeight);
	}

	/* Инициализация Скайбокса */
	void PrepareSkyBox()
	{
		LoadShaders("shaders//SkyBox.vs", "shaders//SkyBox.fs");

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		cubemapTextureID = glGetUniformLocation(ShaderID, "cubemap");

		GLfloat skyboxVertices[] = {
			-SkyBoxSide,  SkyBoxSide, -SkyBoxSide,
			-SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			SkyBoxSide,  SkyBoxSide, -SkyBoxSide,
			-SkyBoxSide,  SkyBoxSide, -SkyBoxSide,

			-SkyBoxSide, -SkyBoxSide,  SkyBoxSide,
			-SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			-SkyBoxSide,  SkyBoxSide, -SkyBoxSide,
			-SkyBoxSide,  SkyBoxSide, -SkyBoxSide,
			-SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			-SkyBoxSide, -SkyBoxSide,  SkyBoxSide,

			SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			SkyBoxSide, -SkyBoxSide,  SkyBoxSide,
			SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			SkyBoxSide,  SkyBoxSide, -SkyBoxSide,
			SkyBoxSide, -SkyBoxSide, -SkyBoxSide,

			-SkyBoxSide, -SkyBoxSide,  SkyBoxSide,
			-SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			SkyBoxSide, -SkyBoxSide,  SkyBoxSide,
			-SkyBoxSide, -SkyBoxSide,  SkyBoxSide,

			-SkyBoxSide,  SkyBoxSide, -SkyBoxSide,
			SkyBoxSide,  SkyBoxSide, -SkyBoxSide,
			SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			-SkyBoxSide,  SkyBoxSide,  SkyBoxSide,
			-SkyBoxSide,  SkyBoxSide, -SkyBoxSide,

			-SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			-SkyBoxSide, -SkyBoxSide,  SkyBoxSide,
			SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			SkyBoxSide, -SkyBoxSide, -SkyBoxSide,
			-SkyBoxSide, -SkyBoxSide,  SkyBoxSide,
			SkyBoxSide, -SkyBoxSide,  SkyBoxSide
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindVertexArray(0);

		vector<const GLchar*> faces;
		faces.push_back("skybox//right.jpg");
		faces.push_back("skybox//left.jpg");
		faces.push_back("skybox//top.jpg");
		faces.push_back("skybox//bottom.jpg");
		faces.push_back("skybox//back.jpg");
		faces.push_back("skybox//front.jpg");
		CubeMapTexture = LoadCubeMap(faces);
	}

	/* Рендеринг Скайбокса */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSkyBox(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glDepthFunc(GL_LEQUAL);

		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
};

class SCENE
{
private:
	/* LightsCount - число источников света */
	/* ObjectsCount, ObjectsCountMirror - число объектов */
	int LightsCount = 0, ObjectsCount = 0, ObjectsCountMirror = 0;
	OBJECT *Lights, Axes, Skybox, *Objects, *ObjectsMirror;
	CAMERA Camera;

	bool SphereDecrease = true, SphereIncrease = false;
	float SpheresSize = 1.0f, SpheresSizeDelta = 0.004f, SpheresSizeMin = 0.8f, SpheresSizeMax = 1.0f;
	float CubeAngle = 3.0f, CylinderAngle = -1.0f;
	float Radius, Angle = 90.0f, Angle2 = 90.0f, AngleDelta = 2.0f, AngleDelta2 = 5.0f;
	vec3 Position, NewPosition;

	/* Позиция камеры */
	vec3 CameraPosition;
	/* Матрицы проекции и вида */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* Генерирует CubeMap-текстуру для зеркального объекта */
	/* id - идентификатор объекта, позиция в массиве всех объектов */
	/* camera - позиция камеры */
	/* ViewMatrix - матрица вида */
	GLuint MakeCubemap(int id, vec3 camera, mat4 ViewMatrix)
	{
		GLuint cubemap, framebuffer, depthbuffer;
		mat4 ProjectionMatrix = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);
		//glEnable(GL_CULL_FACE);
		//glDepthFunc(GL_LEQUAL);

		glGenTextures(1, &cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GenTextureSize, GenTextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		for (int i = 0; i < 6; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		}

		glGenRenderbuffers(1, &depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GenTextureSize, GenTextureSize);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("Framebuffer init problem.\n");

		for (int i = 0; i < 6; i++)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("Framebuffer face %d problem.\n", i);

			/*
			GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right	0
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left	1
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top		2
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom	3
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back	4
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front	5
			*/

			switch (i)
			{
			case 0:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 1:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 2:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
				break;
			}
			case 3:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
				break;
			}
			case 4:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 5:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			default:
				break;
			};

			glViewport(0, 0, GenTextureSize, GenTextureSize);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int j = 0; j < ObjectsCountMirror; j++)
			{
				if (j != id)
				{
					ObjectsMirror[j].Render(camera, ProjectionMatrix, ViewMatrix);
				}
			}
			Skybox.RenderSkyBox(ProjectionMatrix, ViewMatrix);
		}
		glViewport(0, 0, WindowWidth, WindowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteRenderbuffers(1, &depthbuffer);
		glDeleteFramebuffers(1, &framebuffer);

		return cubemap;
	}

public:
	/* Конструктор, инициализирует объекты сцены */
	SCENE()
	{
		Axes = OBJECT();
		Axes.PrepareAxes();

		Skybox = OBJECT();
		Skybox.PrepareSkyBox();

		LightsCount = 2;
		vec3 LightsPositions[] =
		{
			vec3(5.0f, 6.0f, 2.0f),
			vec3(-50.0f, 6.0f, 4.0f)
		};

		vec3 LightsColors[] =
		{
			vec3(1.0f, 1.0f, 1.0f),
			vec3(0.0f, 0.0f, 1.0f)
		};

		vec4 LightsProperties[] =
		{
			vec4(2.0f, 1.0f, 0.09f, 0.032f),
			vec4(8.0f, 1.0f, 0.01f, 0.008f)
		};

		if (LightsCount > 0)
		{
			Lights = new OBJECT[LightsCount];
			for (int i = 0; i < LightsCount; i++)
			{
				Lights[i] = OBJECT(0, "3dmodels//sphere.obj");
				Lights[i].Prepare();
				Lights[i].setDiffuseColor(LightsColors[i]);
				Lights[i].setPosition(LightsPositions[i]);
				Lights[i].setScale(0.5f);
			}
		}

		ObjectsCount = 13;
		Objects = new OBJECT[ObjectsCount];

		Objects[0] = OBJECT(0, LightsCount,"3dmodels//cube.obj");
		Objects[0].setLightsPositions(LightsPositions);
		Objects[0].setLightsColors(LightsColors);
		Objects[0].setLightsProperties(LightsProperties);
		Objects[0].Prepare();
		Objects[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
		Objects[0].setPosition(0.0f, 6.0f, 3.0f);	

		Objects[1] = OBJECT(1, LightsCount, "3dmodels//cube.obj");
		Objects[1].setLightsPositions(LightsPositions);
		Objects[1].setLightsColors(LightsColors);
		Objects[1].setLightsProperties(LightsProperties);
		Objects[1].Prepare();
		Objects[1].setPosition(0.0f, 3.0f, 3.0f);

		Objects[2] = OBJECT(2, "3dmodels//cube.obj");
		Objects[2].Prepare();
		Objects[2].setRefractiveIndex(1.1f);
		Objects[2].setPosition(0.0f, 0.0f, 3.0f);

		Objects[3] = OBJECT(3, "3dmodels//cube.obj");
		Objects[3].Prepare();
		Objects[3].setPosition(0.0f, -3.0f, 3.0f);

		Objects[4] = OBJECT(0, LightsCount, "3dmodels//sphere.obj");
		Objects[4].setLightsPositions(LightsPositions);
		Objects[4].setLightsColors(LightsColors);
		Objects[4].setLightsProperties(LightsProperties);
		Objects[4].Prepare();
		Objects[4].setDiffuseColor(0.6f, 0.3f, 0.9f);
		Objects[4].setPosition(0.0f, 6.0f, 0.0f);	

		Objects[5] = OBJECT(1, LightsCount, "3dmodels//sphere.obj");
		Objects[5].setLightsPositions(LightsPositions);
		Objects[5].setLightsColors(LightsColors);
		Objects[5].setLightsProperties(LightsProperties);
		Objects[5].Prepare();
		Objects[5].setPosition(0.0f, 3.0f, 0.0f);

		Objects[6] = OBJECT(2, "3dmodels//sphere.obj");
		Objects[6].Prepare();
		Objects[6].setRefractiveIndex(2.42f);

		Objects[7] = OBJECT(3, "3dmodels//sphere.obj");
		Objects[7].Prepare();
		Objects[7].setPosition(0.0f, -3.0f, 0.0f);

		Objects[8] = OBJECT(4, "3dmodels//cylinder.obj");
		Objects[8].Prepare();
		Objects[8].setPosition(0.0f, -7.0f, -3.0f);
		Objects[8].setLightPosition(0.0f, -7.0f, 0.0f);

		Objects[9] = OBJECT(0, LightsCount, "3dmodels//cylinder.obj");
		Objects[9].setLightsPositions(LightsPositions);
		Objects[9].setLightsColors(LightsColors);
		Objects[9].setLightsProperties(LightsProperties);
		Objects[9].Prepare();
		Objects[9].setDiffuseColor(0.1f, 0.9f, 0.8f);
		Objects[9].setPosition(0.0f, 5.0f, -3.0f);

		Objects[10] = OBJECT(1, LightsCount, "3dmodels//cylinder.obj");
		Objects[10].setLightsPositions(LightsPositions);
		Objects[10].setLightsColors(LightsColors);
		Objects[10].setLightsProperties(LightsProperties);
		Objects[10].Prepare();
		Objects[10].setPosition(0.0f, 2.0f, -3.0f);

		Objects[11] = OBJECT(2, "3dmodels//cylinder.obj");
		Objects[11].Prepare();
		Objects[11].setRefractiveIndex(1.33f);
		Objects[11].setPosition(0.0f, -1.0f, -3.0f);

		Objects[12] = OBJECT(3, "3dmodels//cylinder.obj");
		Objects[12].Prepare();
		Objects[12].setPosition(0.0f, -4.0f, -3.0f);

		Objects[2].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[3].setCubeMapTexture(Skybox.getCubeMapTexture());

		Objects[6].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[7].setCubeMapTexture(Skybox.getCubeMapTexture());

		Objects[11].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[12].setCubeMapTexture(Skybox.getCubeMapTexture());

		ObjectsCountMirror = 7;
		ObjectsMirror = new OBJECT[ObjectsCountMirror];

		ObjectsMirror[0] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[0].setLightsPositions(LightsPositions);
		ObjectsMirror[0].setLightsColors(LightsColors);
		ObjectsMirror[0].setLightsProperties(LightsProperties);
		ObjectsMirror[0].Prepare();
		ObjectsMirror[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
		ObjectsMirror[0].setPosition(0.0f, 0.0f, 10.0f);

		ObjectsMirror[1] = OBJECT(1, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[1].setLightsPositions(LightsPositions);
		ObjectsMirror[1].setLightsColors(LightsColors);
		ObjectsMirror[1].setLightsProperties(LightsProperties);
		ObjectsMirror[1].Prepare();
		ObjectsMirror[1].setPosition(0.0f, 0.0f, -10.0f);

		ObjectsMirror[2] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[2].setLightsPositions(LightsPositions);
		ObjectsMirror[2].setLightsColors(LightsColors);
		ObjectsMirror[2].setLightsProperties(LightsProperties);
		ObjectsMirror[2].Prepare();
		ObjectsMirror[2].setDiffuseColor(0.3f, 0.8f, 0.5f);
		ObjectsMirror[2].setPosition(5.0f, 0.0f, 0.0f);

		ObjectsMirror[3] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[3].setLightsPositions(LightsPositions);
		ObjectsMirror[3].setLightsColors(LightsColors);
		ObjectsMirror[3].setLightsProperties(LightsProperties);
		ObjectsMirror[3].Prepare();
		ObjectsMirror[3].setDiffuseColor(0.5f, 0.0f, 0.9f);
		ObjectsMirror[3].setPosition(-5.0f, 0.0f, 0.0f);

		ObjectsMirror[4] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[4].setLightsPositions(LightsPositions);
		ObjectsMirror[4].setLightsColors(LightsColors);
		ObjectsMirror[4].setLightsProperties(LightsProperties);
		ObjectsMirror[4].Prepare();
		ObjectsMirror[4].setDiffuseColor(0.1f, 0.3f, 0.5f);
		ObjectsMirror[4].setPosition(0.0f, 15.0f, 0.0f);

		ObjectsMirror[5] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[5].setLightsPositions(LightsPositions);
		ObjectsMirror[5].setLightsColors(LightsColors);
		ObjectsMirror[5].setLightsProperties(LightsProperties);
		ObjectsMirror[5].Prepare();
		ObjectsMirror[5].setDiffuseColor(0.5f, 0.8f, 0.9f);
		ObjectsMirror[5].setScale(2.0f);
		ObjectsMirror[5].setPosition(0.0f, -15.0f, 0.0f);

		ObjectsMirror[6] = OBJECT(3, "3dmodels//sphere.obj");
		ObjectsMirror[6].Prepare();
		ObjectsMirror[6].setRefractiveIndex(1.1f);
		ObjectsMirror[6].setScale(2.0f);
	};

	/* Деструктор, удаляет шейдеры */
	~SCENE()
	{
		for (int i = 0; i < ObjectsCount; i++)
		{
			glDeleteProgram(Objects[i].getShaderID());
		}

		for (int i = 0; i < ObjectsCountMirror; i++)
		{
			glDeleteProgram(ObjectsMirror[i].getShaderID());
		}

		glDeleteProgram(Skybox.getShaderID());
		glDeleteProgram(Axes.getShaderID());
	};
	
	/* Рендеринг сцены */
	void Render()
	{
		CameraPosition = Camera.ComputeViewMatrix(window, CameraMode);
		ProjectionMatrix = Camera.getProjectionMatrix();
		ViewMatrix = Camera.getViewMatrix();
		ViewMatrixAxes = Camera.getViewMatrixAxes();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (MirrorExample)
		{
			for (int i = 0; i < ObjectsCountMirror; i++)
			{
				int mat = ObjectsMirror[i].getMaterial();
				if ((mat == 2) || (mat == 3)) ObjectsMirror[i].setCubeMapTexture(MakeCubemap(i, CameraPosition, ViewMatrix));
			}

			for (int i = 0; i < ObjectsCountMirror; i++)
			{
				ObjectsMirror[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
				int mat = ObjectsMirror[i].getMaterial();
				if ((mat == 2) || (mat == 3))
				{
					GLuint tex = ObjectsMirror[i].getCubeMapTexture();
					glDeleteTextures(1, &tex);
				}
			}

			if ((LightsCount > 0) && (ShowLights))
			{
				for (int i = 0; i < LightsCount; i++) Lights[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			}

			Skybox.RenderSkyBox(ProjectionMatrix, ViewMatrix);
			Axes.RenderAxes(ViewMatrixAxes);
					
			if (!StopRotations)
			{
				Position = ObjectsMirror[0].getPosition();
				Radius = sqrt(Position.y * Position.y + (Position.x * Position.x + Position.z * Position.z));
				NewPosition = vec3(Radius * cos(radians(Angle)), 0.0f, Radius * sin(radians(Angle)));
				ObjectsMirror[0].setPosition(NewPosition.x, Position.y, NewPosition.z);

				Position = ObjectsMirror[1].getPosition();
				Radius = sqrt(Position.y * Position.y + (Position.x * Position.x + Position.z * Position.z));
				NewPosition = vec3(-Radius * cos(radians(Angle)), 0.0f, -Radius * sin(radians(Angle)));
				ObjectsMirror[1].setPosition(NewPosition.x, Position.y, NewPosition.z);

				ObjectsMirror[2].setRotation(CubeAngle, "YZ");
				ObjectsMirror[3].setRotation(-CubeAngle, "XZ");

				Position = ObjectsMirror[4].getPosition();
				Radius = sqrt(Position.y * Position.y + (Position.x * Position.x + Position.z * Position.z));
				NewPosition = vec3(Radius * cos(radians(Angle2)), Radius * sin(radians(Angle2)), 0.0f);
				ObjectsMirror[4].setPosition(NewPosition.x, NewPosition.y, Position.z);

				Position = ObjectsMirror[5].getPosition();
				Radius = sqrt(Position.y * Position.y + (Position.x * Position.x + Position.z * Position.z));
				NewPosition = vec3(-Radius * cos(radians(Angle2)), -Radius * sin(radians(Angle2)), 0.0f);
				ObjectsMirror[5].setPosition(NewPosition.x, NewPosition.y, Position.z);

				Angle += AngleDelta;
				Angle2 += AngleDelta2;
				if (Angle > 360.0f) Angle = 0.0f;
				if (Angle2 > 360.0f) Angle2 = 0.0f;
			}
		}
		else
		{
			for (int i = 0; i < ObjectsCount; i++)
			{
				Objects[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			}

			if ((LightsCount > 0) && (ShowLights))
			{
				for (int i = 0; i < LightsCount; i++) Lights[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			}

			Skybox.RenderSkyBox(ProjectionMatrix, ViewMatrix);
			Axes.RenderAxes(ViewMatrixAxes);

			if (!StopRotations)
			{
				Objects[0].setRotation(-CubeAngle, "XYZ");
				Objects[1].setRotation(CubeAngle, "XY");
				Objects[2].setRotation(-CubeAngle, "X");
				Objects[3].setRotation(CubeAngle, "X");

				Objects[4].setScale(SpheresSize);
				Objects[5].setScale(SpheresSize);
				Objects[6].setScale(SpheresSize);
				Objects[7].setScale(SpheresSize);

				Objects[8].setRotation(-CylinderAngle, "Y");
				Objects[9].setRotation(CylinderAngle, "Y");
				Objects[10].setRotation(-CylinderAngle, "Y");
				Objects[11].setRotation(CylinderAngle, "Y");
				Objects[12].setRotation(-CylinderAngle, "Y");

				if ((SpheresSize > SpheresSizeMin) && SphereDecrease) SpheresSize -= SpheresSizeDelta;
				else
				{
					SphereDecrease = false;
					SphereIncrease = true;
				}

				if ((SpheresSize < SpheresSizeMax) && SphereIncrease) SpheresSize += SpheresSizeDelta;
				else
				{
					SphereDecrease = true;
					SphereIncrease = false;
				}
			}
		}
	}
};

void main()
{
	int nbFrames = 0;
	double currentTime, lastTime;
	char text[30], text2[30];

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		printf("Can't init GLFW.\n");
		getchar();
	}

	/* 4x сглаживание, OpenGL 3.3 */
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);


	GLFWmonitor *Screen = glfwGetPrimaryMonitor();

	if (FullSceen)
	{
		WindowWidth = 1920; WindowHeight = 1080;

		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		window = glfwCreateWindow(WindowWidth, WindowHeight, "Diploma", Screen, NULL);
	}
	else
	{
		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		window = glfwCreateWindow(WindowWidth, WindowHeight, "Diploma", NULL, NULL);

		/* Информация об экране, разрешение */
		const GLFWvidmode *VidMode = glfwGetVideoMode(Screen);
		/* Окно в центр экрана */
		glfwSetWindowPos(window, VidMode->width / 2 - WindowWidth / 2, VidMode->height / 2 - WindowHeight / 2);
	}

	if (!window)
	{
		printf("Can't open GLFW window.\n");
		getchar();
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Can't init GLEW.\n");
		getchar();
		glfwTerminate();
	}

	if (GLEW_ARB_debug_output)
	{
		printf("OpenGL debug output Ok.\n");
		glDebugMessageCallbackARB(&DebugOutputCallback, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	}
	else
	{
		printf("OpenGL debug output doesn't work.\n");
	}

	glViewport(0, 0, WindowWidth, WindowHeight);

	/* Скрыть курсор */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		
	glfwSetCursorPos(window, WindowWidth / 2, WindowHeight / 2);
	/* Цвет фона, RGBA */
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
	/* Включаем буфер глубины */
	glEnable(GL_DEPTH_TEST);
	/* Выбираем фрагмент, ближайший к камере */
	glDepthFunc(GL_LESS);																		  
	//glEnable(GL_CULL_FACE);
	/*glEnable(GL_BLEND);												
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	SCENE Scene = SCENE();

	InitText("textures//Text.DDS");

	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 0.01)
		{
			//sprintf(text, "%.3f ms for frame. %d frames\n", 1000.0 / double(nbFrames), nbFrames);
			sprintf(text, "%d FPS", nbFrames);
			sprintf(text2, "Diploma at %d FPS", nbFrames);
			glfwSetWindowTitle(window, text2);
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwPollEvents();

		Scene.Render();
		PrintText(text, 0, 580, 12);

		glfwSwapBuffers(window);														  
	}

	DeleteText();
	glfwTerminate();
}