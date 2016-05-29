#include "Includes.h"
#include "Camera.h"
#include "Object.h"
#include "Scene.h"
#include "TextureLoader.h"
#include "VboIndexer.h"
#include "Gui.h"

/* WindowInfo - хранит информацию об окне */
windowInfo WindowInfo;

/* CameraMode - выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная */
/* GenTextureSize - размер генерируемой текстуры */
/* FOV - field of view */
/* Wireframe - отображение сетки объектов, переключение по F1 */
/* Rotations - переключение вращений по F2 */
/* ShowLights - переключение отображения источников света по F3 */
/* Blinn - переключение модели освещения по F4 */
/* MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map */
int CameraMode = 2;
int GenTextureSize = 128;
float FOV = 90.0f;
bool Vsync = false;
bool Rotations = false;
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

	/* Отображение курсора при удерживании CTRL */
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		WindowInfo.ShowCursor = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	/* Скрытие курсора при отпускании CTRL */
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

	/* 4x сглаживание, OpenGL 3.3 */
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	GLFWmonitor *Screen = glfwGetPrimaryMonitor();

	WindowInfo.Width = 1280; WindowInfo.Height = 800;
	WindowInfo.HalfWidth = 1280 / 2.0f; WindowInfo.HalfHeight = 800 / 2.0f;

	WindowInfo.ShowCursor = false;
	WindowInfo.FullScreen = false;

	if (WindowInfo.FullScreen)
	{
		WindowInfo.Width = 1920; WindowInfo.Height = 1080;
		WindowInfo.HalfWidth = 1920 / 2.0f; WindowInfo.HalfHeight = 1080 / 2.0f;

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
	if (Vsync) glfwSwapInterval(1);

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

	SCENE Scene = SCENE(Blinn, WindowInfo);

	TEXT Text = TEXT("textures//Text.DDS");

	int ButtonsCount = 5;
	BUTTON *Buttons = new BUTTON[ButtonsCount];
	Buttons[0].Prepare(0, false, "textures//gui//wireframe_inactive.bmp", "textures//gui//wireframe_inactivehover.bmp", "textures//gui//wireframe_active.bmp", "textures//gui//wireframe_activehover.bmp");
	Buttons[1].Prepare(1, false, "textures//gui//rotations_inactive.bmp", "textures//gui//rotations_inactivehover.bmp", "textures//gui//rotations_active.bmp", "textures//gui//rotations_activehover.bmp");
	Buttons[2].Prepare(2, false, "textures//gui//lights_inactive.bmp", "textures//gui//lights_inactivehover.bmp", "textures//gui//lights_active.bmp", "textures//gui//lights_activehover.bmp");
	Buttons[3].Prepare(3, false, "textures//gui//blinn_inactive.bmp", "textures//gui//blinn_inactivehover.bmp", "textures//gui//blinn_active.bmp", "textures//gui//blinn_activehover.bmp");
	Buttons[4].Prepare(4, false, "textures//gui//scene.bmp", "textures//gui//scene_hover.bmp", "textures//gui//scene.bmp", "textures//gui//scene_hover.bmp");

	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(WindowInfo.Window))
	{
		for (int i = 0; i < ButtonsCount; i++)
		{
			if (Buttons[i].frames > 30)
			{
				Buttons[i].flag = true;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 0.01f)
		{
			sprintf(text, "%d FPS, %.3f ms", nbFrames, 1000.0 / double(nbFrames));
			//sprintf(text, "%d FPS", nbFrames);
			sprintf(text2, "Diploma at %d FPS", nbFrames);
			glfwSetWindowTitle(WindowInfo.Window, text2);
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwPollEvents();

		Scene.Render(WindowInfo, CameraMode, GenTextureSize, FOV, MirrorExample, Rotations, ShowLights, Blinn);
		Text.Render(text, 0, 580, 12);

		double MouseX, MouseY;
		glfwGetCursorPos(WindowInfo.Window, &MouseX, &MouseY);

		Buttons[0].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.9f, 0.05f, 0.08f);
		Rotations = Buttons[1].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.72f, 0.05f, 0.08f);
		ShowLights = Buttons[2].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.54f, 0.05f, 0.08f);
		Blinn = Buttons[3].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.36f, 0.05f, 0.08f);
		MirrorExample = Buttons[4].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.18f, 0.05f, 0.08f);

		glfwSwapBuffers(WindowInfo.Window);

		for (int i = 0; i < ButtonsCount; i++) Buttons[i].frames++;
	}

	glfwTerminate();
}