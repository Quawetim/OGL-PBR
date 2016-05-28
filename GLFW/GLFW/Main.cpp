#include "Includes.h"
#include "Camera.h"
#include "Object.h"
#include "TextureLoader.h"
#include "VboIndexer.h"
#include "Text.h"

/* WindowInfo - хранит информацию об окне */
windowInfo WindowInfo;

/* FOV - field of view */
/* CameraMode - выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная */
/* GenTextureSize - размер генерируемой текстуры */
/* Wireframe - отображение сетки объектов, переключение по F1 */
/* StopRotations - переключение вращений по F2 */
/* ShowLights - переключение отображения источников света по F3 */
/* Blinn - переключение модели освещения по F4 */
/* MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map */
int CameraMode = 2;
int GenTextureSize = 512;
float FOV = 90.0f;
//float SkyBoxHalfSide = 100.0f;
bool Wireframe = false;
bool StopRotations = true;
bool ShowLights = false;
bool Blinn = false;
bool MirrorExample = false;

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

	/* FOV по-молчанию */
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) { FOV = 90.0f; }

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

	/* Переключение модели освещения */
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		if (Blinn)
		{
			Blinn = false;
		}
		else
		{
			Blinn = true;
		}
	}

	/* Переключение сцен */
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

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		WindowInfo.ShowCursor = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		WindowInfo.ShowCursor = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, WindowInfo.Width / 2, WindowInfo.Height / 2);
	}
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

class SCENE
{
private:
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
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 1:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 2:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
				break;
			}
			case 3:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
				break;
			}
			case 4:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 5:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f));
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
			Skybox.RenderSkyBox(camera, ProjectionMatrix, ViewMatrix);
		}
		glViewport(0, 0, WindowInfo.Width, WindowInfo.Height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteRenderbuffers(1, &depthbuffer);
		glDeleteFramebuffers(1, &framebuffer);

		return cubemap;
	}

public:
	/* Конструктор, инициализирует объекты сцены */
	SCENE()
	{
		Axes = OBJECT(WindowInfo);
		Axes.PrepareAxes();

		LightsCount = 3;

		Skybox = OBJECT(0, "3dmodels//skybox.obj");
		Skybox.setLightsPositions(LightsPositions);
		Skybox.setLightsColors(LightsColors);
		Skybox.setLightsProperties(LightsProperties);
		Skybox.PrepareSkyBox(100.0f);
		Skybox.createModelMatrix(vec3(0.0f, 0.0f, 0.0f), NULL, NULL, 100.0f * 2.0);

		if (LightsCount > 0)
		{
			LightsPositions = new vec3[LightsCount];
			LightsPositions[0] = vec3(-60.0f, 50.0f, -80.0f);
			LightsPositions[1] = vec3(-60.0f, -60.0f, -80.0f);
			LightsPositions[2] = vec3(60.0f, 6.0f, 60.0f);

			LightsColors = new vec3[LightsCount];
			LightsColors[0] = vec3(1.0f, 1.0f, 1.0f);
			LightsColors[1] = vec3(1.0f, 1.0f, 1.0f);
			LightsColors[2] = vec3(1.0f, 1.0f, 1.0f);

			LightsProperties = new vec4[LightsCount];
			LightsProperties[0] = vec4(100.0f, 1.0f, 0.8f, 0.002f);
			LightsProperties[1] = vec4(100.0f, 1.0f, 0.7f, 0.008f);
			LightsProperties[2] = vec4(50.0f, 1.0f, 0.2f, 0.03f);

			Lights = new OBJECT[LightsCount];
			for (int i = 0; i < LightsCount; i++)
			{
				Lights[i] = OBJECT(0, "3dmodels//sphere_lowpoly.obj");
				Lights[i].Prepare();
				Lights[i].setDiffuseColor(LightsColors[i]);
				Lights[i].setPosition(LightsPositions[i]);
				Lights[i].createModelMatrix(LightsPositions[i], NULL, NULL, 0.5f);
			}
		}

		ObjectsCount = 15;
		Objects = new OBJECT[ObjectsCount];

		Objects[0] = OBJECT(0, LightsCount, Blinn, "3dmodels//cube.obj");
		Objects[0].Prepare();
		//Objects[0].setDiffuseTexture("textures//batman_diffuse.bmp", false);
		Objects[0].setSpecularTexture("textures//batman_specular.bmp", false);		
		//Objects[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
		Objects[0].createModelMatrix(vec3(0.0f, 6.0f, 3.0f), NULL, NULL, 0.5f);

		Objects[1] = OBJECT(0, LightsCount, Blinn, "3dmodels//sphere_lowpoly.obj");
		Objects[1].Prepare();
		Objects[1].setDiffuseColor(0.6f, 0.3f, 0.9f);
		Objects[1].setPosition(0.0f, 6.0f, 0.0f);

		Objects[2] = OBJECT(0, LightsCount, Blinn, "3dmodels//cylinder.obj");
		Objects[2].Prepare();
		Objects[2].setDiffuseColor(0.1f, 0.9f, 0.8f);
		Objects[2].setPosition(0.0f, 6.0f, -3.0f);

		Objects[3] = OBJECT(1, LightsCount, Blinn, "3dmodels//cube.obj");
		Objects[3].Prepare();
		Objects[3].setPosition(0.0f, 3.0f, 3.0f);

		Objects[4] = OBJECT(1, LightsCount, Blinn, "3dmodels//sphere_lowpoly.obj");
		Objects[4].Prepare();
		Objects[4].setPosition(0.0f, 3.0f, 0.0f);

		Objects[5] = OBJECT(1, LightsCount, Blinn, "3dmodels//cylinder.obj");
		Objects[5].Prepare();
		Objects[5].setPosition(0.0f, 3.0f, -3.0f);

		Objects[6] = OBJECT(2, "3dmodels//cube.obj");
		Objects[6].Prepare();
		Objects[6].setRefractiveIndex(1.1f);
		Objects[6].setPosition(0.0f, 0.0f, 3.0f);

		Objects[7] = OBJECT(2, "3dmodels//sphere_highpoly.obj");
		Objects[7].Prepare();
		Objects[7].setRefractiveIndex(2.42f);

		Objects[8] = OBJECT(2, "3dmodels//cylinder.obj");
		Objects[8].Prepare();
		Objects[8].setRefractiveIndex(1.33f);
		Objects[8].setPosition(0.0f, 0.0f, -3.0f);

		Objects[9] = OBJECT(3, "3dmodels//cube.obj");
		Objects[9].Prepare();
		Objects[9].setPosition(0.0f, -3.0f, 3.0f);		

		Objects[10] = OBJECT(3, "3dmodels//sphere_lowpoly.obj");
		Objects[10].Prepare();
		Objects[10].setPosition(0.0f, -3.0f, 0.0f);	

		Objects[11] = OBJECT(3, "3dmodels//cylinder.obj");
		Objects[11].Prepare();
		Objects[11].setPosition(0.0f, -3.0f, -3.0f);

		Objects[12] = OBJECT(4, "3dmodels//cube.obj");
		Objects[12].setLightPosition(0, 5.0f, -6.0f, 3.0f);
		Objects[12].Prepare();	
		Objects[12].setDiffuseTexture("textures//brick_diffuse.bmp", false);
		Objects[12].setSpecularTexture("textures//brick_specular.bmp", false);
		Objects[12].setNormalTexture("textures//brick_normal.bmp");		
		Objects[12].setShinePower(32.0f);
		Objects[12].setPosition(0.0f, -6.0f, 3.0f);	

		Objects[13] = OBJECT(4, "3dmodels//sphere_lowpoly.obj");
		Objects[13].setLightPosition(0, 5.0f, -6.0f, 0.0f);
		Objects[13].Prepare();		
		Objects[13].setDiffuseTexture("textures//brick2_diffuse.bmp", false);
		Objects[13].setSpecularTexture("textures//brick2_specular.bmp", false);
		Objects[13].setNormalTexture("textures//brick2_normal.bmp");		
		Objects[13].setPosition(0.0f, -6.0f, 0.0f);		

		Objects[14] = OBJECT(4, "3dmodels//cylinder.obj");
		Objects[14].setLightPosition(0, 5.0f, -6.0f, -3.0f);
		Objects[14].Prepare();		
		Objects[14].setDiffuseTexture("textures//diffuse.dds", true);
		Objects[14].setSpecularTexture("textures//specular.dds", true);
		Objects[14].setNormalTexture("textures//normal.bmp");
		Objects[14].setPosition(0.0f, -6.0f, -3.0f);		

		Objects[6].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[7].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[8].setCubeMapTexture(Skybox.getCubeMapTexture());

		Objects[9].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[10].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[11].setCubeMapTexture(Skybox.getCubeMapTexture());

		ObjectsCountMirror = 7;
		ObjectsMirror = new OBJECT[ObjectsCountMirror];

		ObjectsMirror[0] = OBJECT(0, LightsCount, Blinn, "3dmodels//cube.obj");
		ObjectsMirror[0].Prepare();
		ObjectsMirror[0].setLightsPositions(LightsPositions);
		ObjectsMirror[0].setLightsColors(LightsColors);
		ObjectsMirror[0].setLightsProperties(LightsProperties);		
		ObjectsMirror[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
		ObjectsMirror[0].setPosition(0.0f, 0.0f, 10.0f);

		ObjectsMirror[1] = OBJECT(0, LightsCount, Blinn, "3dmodels//cube.obj");
		ObjectsMirror[1].Prepare();
		ObjectsMirror[1].setLightsPositions(LightsPositions);
		ObjectsMirror[1].setLightsColors(LightsColors);
		ObjectsMirror[1].setLightsProperties(LightsProperties);		
		ObjectsMirror[1].setDiffuseColor(0.0f, 0.5f, 0.9f);
		ObjectsMirror[1].setPosition(0.0f, 0.0f, -10.0f);

		ObjectsMirror[2] = OBJECT(0, LightsCount, Blinn, "3dmodels//cube.obj");
		ObjectsMirror[2].Prepare();
		ObjectsMirror[2].setLightsPositions(LightsPositions);
		ObjectsMirror[2].setLightsColors(LightsColors);
		ObjectsMirror[2].setLightsProperties(LightsProperties);	
		ObjectsMirror[2].setDiffuseColor(0.3f, 0.8f, 0.5f);
		ObjectsMirror[2].setPosition(5.0f, 0.0f, 0.0f);

		ObjectsMirror[3] = OBJECT(0, LightsCount, Blinn, "3dmodels//cube.obj");
		ObjectsMirror[3].Prepare();
		ObjectsMirror[3].setLightsPositions(LightsPositions);
		ObjectsMirror[3].setLightsColors(LightsColors);
		ObjectsMirror[3].setLightsProperties(LightsProperties);		
		ObjectsMirror[3].setDiffuseColor(0.5f, 0.0f, 0.9f);
		ObjectsMirror[3].setDiffuseTexture("textures//deadpool.bmp", false);
		//ObjectsMirror[3].setPosition(-5.0f, 0.0f, 0.0f);
		ObjectsMirror[3].createModelMatrix(vec3(-5.0f, 0.0f, 0.0f), NULL, NULL, 1.0f);

		ObjectsMirror[4] = OBJECT(1, LightsCount, Blinn, "3dmodels//cylinder.obj");
		ObjectsMirror[4].Prepare();
		ObjectsMirror[4].setLightsPositions(LightsPositions);
		ObjectsMirror[4].setLightsColors(LightsColors);
		ObjectsMirror[4].setLightsProperties(LightsProperties);	
		ObjectsMirror[4].setDiffuseColor(0.1f, 0.3f, 0.5f);
		//ObjectsMirror[4].setPosition(0.0f, 15.0f, 0.0f);
		ObjectsMirror[4].createModelMatrix(vec3(0.0f, 15.0f, 0.0f), NULL, NULL, 3.0f);

		ObjectsMirror[5] = OBJECT(0, LightsCount, Blinn, "3dmodels//cylinder.obj");
		ObjectsMirror[5].setLightsPositions(LightsPositions);
		ObjectsMirror[5].setLightsColors(LightsColors);
		ObjectsMirror[5].setLightsProperties(LightsProperties);
		ObjectsMirror[5].Prepare();
		ObjectsMirror[5].setDiffuseColor(0.0f, 1.0f, 1.0f);
		ObjectsMirror[5].setPosition(0.0f, -15.0f, 0.0f);
		//ObjectsMirror[5].createModelMatrix(vec3(0.0f, -15.0f, 0.0f), NULL, NULL, 3.0f);

		ObjectsMirror[6] = OBJECT(3, "3dmodels//teapot.obj"); 
		//ObjectsMirror[6].createModelMatrix(vec3(0.0f, 0.0f, 0.0f), NULL, NULL, 0.5f);
		ObjectsMirror[6].Prepare();
		//ObjectsMirror[6].setRefractiveIndex(1.1f);
		//ObjectsMirror[6].setScale(2.0f);
		//ObjectsMirror[6].setRotation(90.0f, "Y");
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
		CameraPosition = Camera.ComputeViewMatrix(WindowInfo, CameraMode, FOV);
		ProjectionMatrix = Camera.getProjectionMatrix();
		ViewMatrix = Camera.getViewMatrix();
		ViewMatrixAxes = Camera.getViewMatrixAxes();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Skybox.RenderSkyBox(CameraPosition, ProjectionMatrix, ViewMatrix);
		
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
					
			if (!StopRotations)
			{
				Angle += AngleDelta;
				Angle2 += AngleDelta2;
				Angle3 += AngleDelta3;
				if (Angle > 360.0f) Angle = 0.0f;
				if (Angle2 > 360.0f) Angle2 = 0.0f;
				if (Angle3 > 360.0f) Angle3 = 0.0f;

				Position = ObjectsMirror[0].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(Radius * sin(radians(Angle)), 0.0f, Radius * cos(radians(Angle)));
				ObjectsMirror[0].createModelMatrix(NewPosition, -Angle, "Y", ObjectsMirror[0].getScale());

				Position = ObjectsMirror[1].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(-Radius * sin(radians(Angle)), 0.0f, -Radius * cos(radians(Angle)));
				ObjectsMirror[1].createModelMatrix(NewPosition, -Angle, "Y", ObjectsMirror[1].getScale());

				ObjectsMirror[2].increaseRotation(CubeAngle, "YZ");
				ObjectsMirror[3].increaseRotation(-CubeAngle, "XZ");

				Position = ObjectsMirror[4].getPosition();
				Radius = sqrt(Position.y * Position.y + Position.x * Position.x);
				NewPosition = vec3(Radius * sin(radians(Angle2)), Radius * cos(radians(Angle2)), 0.0f);
				ObjectsMirror[4].createModelMatrix(NewPosition, Angle3, NewPosition, ObjectsMirror[4].getScale());
				ObjectsMirror[4].increaseRotation(-Angle2, "Z");

				Position = ObjectsMirror[5].getPosition();
				Radius = sqrt(Position.y * Position.y + Position.x * Position.x);
				NewPosition = vec3(-Radius * sin(radians(Angle2)), -Radius * cos(radians(Angle2)), 0.0f);
				ObjectsMirror[5].createModelMatrix(NewPosition, Angle3, NewPosition, ObjectsMirror[5].getScale());
				ObjectsMirror[5].increaseRotation(-Angle2, "Z");

				ObjectsMirror[6].increaseRotation(1.0f, "Y");				
			}	
		}
		else
		{
			static vec3 color = vec3(1.0f, 0.0f, 0.0f);
			static bool flag = false;

			if ((color.r > 0.0f) && (!flag))
			{
				color.r -= 0.001f;
				color.g += 0.001f;
			}
			else
			{
				if (color.g > 0.0f)
				{
					color.g -= 0.001f;
					color.b += 0.001f;
				}
				else
				{
					if (color.b > 0.0f)
					{
						color.b -= 0.001f;
						color.r += 0.001f;
						if (!flag) flag = true;
					}
					else flag = false;
				}
			}

			LightsColors[0] = color;
			Lights[0].setDiffuseColor(color);

			for (int i = 0; i < ObjectsCount; i++)
			{
				if (i < 6)
				{
					Objects[i].setLightsPositions(LightsPositions);
					Objects[i].setLightsColors(LightsColors);
					Objects[i].setLightsProperties(LightsProperties);				
				}
				Objects[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			}

			if (LightsCount > 0)
			{
				LightAngle += LightAngleDelta;
				if (LightAngle > 360.0f) LightAngle = 0;

				Position = Lights[0].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(-Radius * sin(radians(LightAngle)), Position.y, -Radius * cos(radians(LightAngle)));
				LightsPositions[0] = NewPosition;
				Lights[0].createModelMatrix(NewPosition, -LightAngle, "Y", Lights[0].getScale());		

				Position = Lights[1].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(-Radius * sin(radians(LightAngle)), Position.y, -Radius * cos(radians(LightAngle)));
				LightsPositions[1] = NewPosition;
				Lights[1].createModelMatrix(NewPosition, -LightAngle, "Y", Lights[1].getScale());

				Position = Lights[2].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(Radius * sin(radians(LightAngle)), Position.y, Radius * cos(radians(LightAngle)));
				LightsPositions[2] = NewPosition;
				Lights[2].createModelMatrix(NewPosition, -LightAngle, "Y", Lights[2].getScale());

				if (ShowLights) for (int i = 0; i < LightsCount; i++) Lights[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			}

			if (!StopRotations)
			{
				Objects[0].increaseRotation(-CubeAngle, "Y");
				Objects[3].increaseRotation(CubeAngle, "XY");
				Objects[6].increaseRotation(-CubeAngle, "X");
				Objects[9].increaseRotation(CubeAngle, "X");
				Objects[12].increaseRotation(CylinderAngle * 3.0f, "Y");

				Objects[1].createModelMatrix(Objects[1].getPosition(), Objects[1].getAngle(), Objects[1].getAxis(), SpheresSize);
				Objects[4].createModelMatrix(Objects[4].getPosition(), Objects[4].getAngle(), Objects[4].getAxis(), SpheresSize);
				Objects[7].createModelMatrix(Objects[7].getPosition(), Objects[7].getAngle(), Objects[7].getAxis(), SpheresSize);
				Objects[10].createModelMatrix(Objects[10].getPosition(), Objects[10].getAngle(), Objects[10].getAxis(), SpheresSize);
				
				Objects[13].createModelMatrix(Objects[13].getPosition(), Objects[13].getAngle(), Objects[13].getAxis(), SpheresSize);
				Objects[13].increaseRotation(-0.6f, "XYZ");			

				Objects[2].increaseRotation(-CylinderAngle, "Y");
				Objects[5].increaseRotation(CylinderAngle, "Y");
				Objects[8].increaseRotation(-CylinderAngle, "Y");
				Objects[11].increaseRotation(CylinderAngle, "Y");
				Objects[14].increaseRotation(-CylinderAngle, "Y");

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
		
		if ((LightsCount > 0) && (ShowLights))
		{
			for (int i = 0; i < LightsCount; i++) Lights[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);		
		}

		//Skybox.RenderSkyBox(CameraPosition, ProjectionMatrix, ViewMatrix);
		Axes.RenderAxes(ViewMatrixAxes);
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

	WindowInfo.Width = 1280;
	WindowInfo.Height = 800;
	WindowInfo.ShowCursor = false;
	WindowInfo.FullScreen = false;

	if (WindowInfo.FullScreen)
	{
		WindowInfo.Width = 1920; WindowInfo.Height = 1080;

		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", Screen, NULL);
	}
	else
	{
		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", NULL, NULL);

		/* Информация об экране, разрешение */
		const GLFWvidmode *VidMode = glfwGetVideoMode(Screen);
		/* Окно в центр экрана */
		glfwSetWindowPos(WindowInfo.Window, VidMode->width / 2 - WindowInfo.Width / 2, VidMode->height / 2 - WindowInfo.Height / 2);
	}

	if (!WindowInfo.Window)
	{
		printf("Can't open GLFW window.\n");
		getchar();
		glfwTerminate();
	}

	glfwMakeContextCurrent(WindowInfo.Window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(WindowInfo.Window, key_callback);
	glfwSetScrollCallback(WindowInfo.Window, scroll_callback);

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

	glViewport(0, 0, WindowInfo.Width, WindowInfo.Height);

	/* Скрыть курсор */
	glfwSetInputMode(WindowInfo.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(WindowInfo.Window, WindowInfo.Width / 2, WindowInfo.Height / 2);
	/* Цвет фона, RGBA */
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
	/* Включаем буфер глубины */
	glEnable(GL_DEPTH_TEST);
	/* Выбираем фрагмент, ближайший к камере */
	glDepthFunc(GL_LESS);																		  
	glEnable(GL_CULL_FACE);

	SCENE Scene = SCENE();

	InitText("textures//Text.DDS");

	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(WindowInfo.Window))
	{
		currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 0.01)
		{
			//sprintf(text, "%.3f ms for frame. %d frames\n", 1000.0 / double(nbFrames), nbFrames);
			sprintf(text, "%d FPS", nbFrames);
			sprintf(text2, "Diploma at %d FPS", nbFrames);
			glfwSetWindowTitle(WindowInfo.Window, text2);
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwPollEvents();

		Scene.Render();
		PrintText(text, 0, 580, 12);

		glfwSwapBuffers(WindowInfo.Window);
	}

	DeleteText();
	glfwTerminate();
}