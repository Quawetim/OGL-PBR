#include "Includes.h"
#include "Camera.h"
#include "Object.h"
#include "Scene.h"
#include "TextureLoader.h"
#include "VboIndexer.h"
#include "Text.h"

/* WindowInfo - ������ ���������� �� ���� */
windowInfo WindowInfo;

/* CameraMode - ��������� ������: 1 - �� ������� ����, 2 - �� �������� ����, 3 - ������������� */
/* GenTextureSize - ������ ������������ �������� */
/* FOV - field of view */
/* Wireframe - ����������� ����� ��������, ������������ �� F1 */
/* StopRotations - ������������ �������� �� F2 */
/* ShowLights - ������������ ����������� ���������� ����� �� F3 */
/* Blinn - ������������ ������ ��������� �� F4 */
/* MirrorExample - true = ������ ����������� ������ � Reflection Map, false = ��� ������� ��� Reflection Map */
int CameraMode = 2;
int GenTextureSize = 512;
float FOV = 90.0f;
bool Wireframe = false;
bool StopRotations = true;
bool ShowLights = false;
bool Blinn = false;
bool MirrorExample = false;

/* ��������� ������ */
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* ��������� ���������� */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static int PrevCamera;
	/* ������� ��� ������ �� ���������� */
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	/* FOV ��-�������� */
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) { FOV = 90.0f; }

	/* ������������ ����� �1 � �2 */
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		CameraMode++;
		if (CameraMode > 2) CameraMode = 1;
	}

	/* ��������� ������ �3 */
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		if (CameraMode != 3)
		{
			PrevCamera = CameraMode;
			CameraMode = 3;
		}
		else CameraMode = PrevCamera;
	}

	/* ������������ ����������� ������������� ����� */
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

	/* ���������/���������� �������� */
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

	/* ���������/���������� ����������� ���������� ����� */
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

	/* ������������ ������ ��������� */
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

	/* ������������ ���� */
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

/* ��������� ������� ����� */
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

/* ����� ������ � ������� */
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

void main()
{
	int nbFrames = 0;
	double currentTime, lastTime;
	char text[100], text2[30];

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		printf("Can't init GLFW.\n");
		getchar();
	}

	/* 4x �����������, OpenGL 3.3 */
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

		/* ������, ������, �������� ����, ������� (FullSreen , NUll - �������), ����� ��������� � ����� (NULL - ��� ������) */
		WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", Screen, NULL);
	}
	else
	{
		/* ������, ������, �������� ����, ������� (FullSreen , NUll - �������), ����� ��������� � ����� (NULL - ��� ������) */
		WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", NULL, NULL);

		/* ���������� �� ������, ���������� */
		const GLFWvidmode *VidMode = glfwGetVideoMode(Screen);
		/* ���� � ����� ������ */
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

	/* ������ ������ */
	glfwSetInputMode(WindowInfo.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(WindowInfo.Window, WindowInfo.Width / 2, WindowInfo.Height / 2);
	/* ���� ����, RGBA */
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
	/* �������� ����� ������� */
	glEnable(GL_DEPTH_TEST);
	/* �������� ��������, ��������� � ������ */
	glDepthFunc(GL_LESS);																		  
	glEnable(GL_CULL_FACE);

	SCENE Scene = SCENE(Blinn, WindowInfo);

	InitText("textures//Text.DDS");

	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(WindowInfo.Window))
	{
		currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 0.01)
		{
			sprintf(text, "%.3f ms for frame. %d frames\n", 1000.0 / double(nbFrames), nbFrames);
			//sprintf(text, "%d FPS", nbFrames);
			sprintf(text2, "Diploma at %d FPS", nbFrames);
			glfwSetWindowTitle(WindowInfo.Window, text2);
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwPollEvents();

		Scene.Render(WindowInfo, CameraMode, GenTextureSize, FOV, MirrorExample, StopRotations, ShowLights);
		PrintText(text, 0, 580, 12);

		glfwSwapBuffers(WindowInfo.Window);
	}

	DeleteText();
	glfwTerminate();
}