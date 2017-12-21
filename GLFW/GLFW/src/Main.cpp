#include "Includes.h"
#include "Camera.h"
#include "Object.h"
#include "Scene.h"
#include "TextureLoader.h"
#include "VboIndexer.h"
#include "Gui.h"

/* WindowInfo - ������ ���������� �� ���� */
windowInfo WindowInfo;

/* CameraMode - ��������� ������: 1 - �� ������� ����, 2 - �� �������� ����, 3 - ������������� */
/* GenTextureSize - ������ ������������ �������� */
/* FOV - field of view */
/* SkyBoxSize - ������ ��������� */
/* Rotations - ������������ �������� */
/* ShowLights - ������������ ����������� ���������� ����� */
/* Blinn - ������������ ������ ��������� */
/* MirrorExample - true = ������ ����������� ������ � Reflection Map, false = ��� ������� ��� Reflection Map */
/* ShowHelp - ������������ ����������� ������� */
int CameraMode = 2;
int GenTextureSize = 128;
float FOV = 90.0f;
float SkyBoxSize = 500.0f;
bool Rotations = false;
bool ShowLights = false;
bool Blinn = false;
bool MirrorExample = false;
bool ShowHelp = false;

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
	//if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	//if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

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

	/* ����������� ������� ��� ����������� CTRL */
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		WindowInfo.ShowCursor = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	/* ������� ������� ��� ���������� CTRL */
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		WindowInfo.ShowCursor = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, WindowInfo.Width / 2, WindowInfo.Height / 2);
	}

	/* ����������� ������� ��� ����������� F1 */
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		ShowHelp = true;
	}

	/* ������� ������� ��� ���������� F1 */
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
	{
		ShowHelp = false;
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
    system("pause");
}

/* ���������� �������� �� ����� ������������ */
/* Winfo - ��������� ����� �� ���� */
/* Reflection_res - ������ ����� ��������� */
void ReadConfig(windowInfo &Winfo, int &Reflection_res)
{
	FILE *Fin = fopen("config//config.ini", "r");

	/* ��������� ��-��������� */
	Winfo.FullScreen = false; Winfo.Vsync = true; Winfo.ShowCursor = false;
	Winfo.Width = 800; Winfo.Height = 600;
	Winfo.HalfWidth = Winfo.Width / 2.0f; Winfo.HalfHeight = Winfo.Height / 2.0f;

	if (Fin != NULL)
	{
		char Buf[128];

		while (true)
		{
			if (fscanf(Fin, "%s", Buf) == EOF) break;

			if (strcmp(Buf, "Fullscreen") == 0)
			{
				fscanf(Fin, "%s", Buf);
				if (strcmp(Buf, "=") == 0)
				{
					fscanf(Fin, "%s", Buf);
					if (strcmp(Buf, "true") == 0) Winfo.FullScreen = true;
					else Winfo.FullScreen = false;

					continue;
				}
			}

			if (strcmp(Buf, "Vsync") == 0)
			{
				fscanf(Fin, "%s", Buf);
				if (strcmp(Buf, "=") == 0)
				{
					fscanf(Fin, "%s", Buf);
					if (strcmp(Buf, "true") == 0) Winfo.Vsync = true;
					else Winfo.Vsync = false;

					continue;
				}
			}
				
			if (strcmp(Buf, "WindowWidth") == 0)
			{
				fscanf(Fin, "%s", Buf);
				if (strcmp(Buf, "=") == 0)
				{
					fscanf(Fin, "%d", &Winfo.Width);
					Winfo.HalfWidth = Winfo.Width / 2.0f;

					continue;
				}
			}
					
			if (strcmp(Buf, "WindowHeight") == 0)
			{
				fscanf(Fin, "%s", Buf);
				if (strcmp(Buf, "=") == 0)
				{
					fscanf(Fin, "%d", &Winfo.Height);
					Winfo.HalfHeight = Winfo.Height / 2.0f;

					continue;
				}
			}

			if (strcmp(Buf, "ReflectionResolution") == 0)
			{
				fscanf(Fin, "%s", Buf);
				if (strcmp(Buf, "=") == 0)
				{
					fscanf(Fin, "%d", &Reflection_res);

					continue;
				}
			}
		}
	}
}

void main()
{
	int nbFrames = 0;
	double currentTime, lastTime, MouseX, MouseY;
	char text[100], text2[30];

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) 
	{ 
		printf("Can't init GLFW.\n"); 
		getchar(); 
	}
	else
	{
		/* 4x �����������, OpenGL 3.3 */
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

		/* ��������� �� ����� �� */
		GLFWmonitor *Screen = glfwGetPrimaryMonitor();

		ReadConfig(WindowInfo, GenTextureSize);

		if (WindowInfo.FullScreen)
		{
			/* ���������� �� ������, ���������� */
			const GLFWvidmode *VidMode = glfwGetVideoMode(Screen);

			WindowInfo.Width = VidMode->width; WindowInfo.HalfWidth = WindowInfo.Width / 2.0f;
			WindowInfo.Height = VidMode->height; WindowInfo.HalfHeight = WindowInfo.Height / 2.0f;

			/* ������, ������, �������� ����, ������� (S�reen , NUll - �������), ����� ��������� � ����� (NULL - ��� ������) */
			WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", Screen, NULL);
		}
		else
		{
			/* ������, ������, �������� ����, ������� (S�reen , NUll - �������), ����� ��������� � ����� (NULL - ��� ������) */
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

		/* ��������� ������������ ������������� */
		if (WindowInfo.Vsync) glfwSwapInterval(1);

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
		//glfwSetInputMode(WindowInfo.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glfwSetCursorPos(WindowInfo.Window, WindowInfo.Width / 2, WindowInfo.Height / 2);

		/* ���� ����, RGBA */
		//glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

		/* �������� ����� ������� */
		glEnable(GL_DEPTH_TEST);

		/* �������� ��������, ��������� � ������ */
		glDepthFunc(GL_LESS);

		/* ��������� ��������� ������ */
		glEnable(GL_CULL_FACE);

		SCENE Scene = SCENE(WindowInfo, SkyBoxSize, Blinn);

		TEXT Text = TEXT("resources//textures//gui//Text.DDS");

        int fontSize = 32;
        int margin = 30;
        int spacing = 5;
        //vec3 textColor = vec3(0.43f, 0.15f, 0.91f);
        //vec3 textColor = vec3(0.43f, 0.87f, 0.51f);
        vec3 textColor = vec3(0.0f, 0.0f, 0.0f);
        TEXT FreeType = TEXT("resources//fonts//timesbd.ttf", fontSize);

		int ButtonsCount = 5;
		BUTTON *Buttons = new BUTTON[ButtonsCount];
		Buttons[0].Prepare(0, false, "resources//textures//gui//wireframe_inactive.bmp", "resources//textures//gui//wireframe_inactivehover.bmp", "resources//textures//gui//wireframe_active.bmp", "resources//textures//gui//wireframe_activehover.bmp");
		Buttons[1].Prepare(1, false, "resources//textures//gui//rotations_inactive.bmp", "resources//textures//gui//rotations_inactivehover.bmp", "resources//textures//gui//rotations_active.bmp", "resources//textures//gui//rotations_activehover.bmp");
		Buttons[2].Prepare(2, false, "resources//textures//gui//lights_inactive.bmp", "resources//textures//gui//lights_inactivehover.bmp", "resources//textures//gui//lights_active.bmp", "resources//textures//gui//lights_activehover.bmp");
		Buttons[3].Prepare(3, false, "resources//textures//gui//blinn_inactive.bmp", "resources//textures//gui//blinn_inactivehover.bmp", "resources//textures//gui//blinn_active.bmp", "resources//textures//gui//blinn_activehover.bmp");
		Buttons[4].Prepare(4, false, "resources//textures//gui//scene.bmp", "resources//textures//gui//scene_hover.bmp", "resources//textures//gui//scene.bmp", "resources//textures//gui//scene_hover.bmp");

		WINDOW HelpWindow = WINDOW();
		HelpWindow.Prepare("resources//textures//gui//help.bmp");

		lastTime = glfwGetTime();

		while (!glfwWindowShouldClose(WindowInfo.Window))
		{
			/* "���������" ������ */
			for (int i = 0; i < ButtonsCount; i++)
			{
				if (Buttons[i].frames > 30)
				{
					Buttons[i].flag = true;
				}
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/* ������� ������ � ������� � ������� �� ��������� 1 ����� */
			currentTime = glfwGetTime();
			nbFrames++;
		
			if (currentTime - lastTime >= 0.01f)
			{
				sprintf(text, "%d FPS, %.3f ms", nbFrames, 1000.0 / double(nbFrames));
				sprintf(text2, "Diploma at %d FPS", nbFrames);
				glfwSetWindowTitle(WindowInfo.Window, text2);
				nbFrames = 0;
				lastTime += 1.0;
			}

			/* �������� �� ������� ������ */
			glfwPollEvents();

			/* ��������� ����� */
			//Scene.Render(WindowInfo, CameraMode, GenTextureSize, FOV, MirrorExample, Rotations, ShowLights, Blinn);
			
			/* ��������� GUI */
			//Text.Render(text, textColor, 0, 580, 12);                 
            //Text.Render("ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 0, 580, 16);
            //Text.Render("abcdefghijklmnopqrstuvwxyz", textColor, 0, 560, 16);
            //Text.Render("1234567890,.!?/:;-=+<>{}()\"\\@#�$^&*`~[]|", textColor, 0, 540, 16);

            //int offsety = WindowInfo.Height - margin - fontSize - 80;
            int offsety = WindowInfo.Height - margin - fontSize;
            FreeType.RenderFreeType(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"abcdefghijklmnopqrstuvwxyz", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"1234567890,.!?/:;-=+<>{}()\"\\@#�$^&*`~[]|", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"�����Ũ��������������������������", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"��������������������������������", textColor, 1.0, 0, offsety);

			//glfwGetCursorPos(WindowInfo.Window, &MouseX, &MouseY);

			//Buttons[0].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.9f, 0.05f, 0.08f);
			//Rotations = Buttons[1].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.72f, 0.05f, 0.08f);
			//ShowLights = Buttons[2].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.54f, 0.05f, 0.08f);
			//Blinn = Buttons[3].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.36f, 0.05f, 0.08f);
			//MirrorExample = Buttons[4].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.18f, 0.05f, 0.08f);

			if (ShowHelp) HelpWindow.Render(WindowInfo, 0.0f, 0.0f, 0.8f, 0.8f);

			for (int i = 0; i < ButtonsCount; i++) Buttons[i].frames++;

			glfwSwapBuffers(WindowInfo.Window);
		}
		glfwTerminate();
	}
}