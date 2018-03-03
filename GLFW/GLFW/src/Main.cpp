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
/* SkyBoxSize - размер скайбокса */
/* Rotations - переключение вращений */
/* ShowLights - переключение отображения источников света */
/* Blinn - переключение модели освещения */
/* MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map */
/* ShowHelp - переключение отображения справки */
int CameraMode = 2;
int GenTextureSize = 128;
float FOV = 90.0f;
float SkyBoxSize = 500.0f;
bool Rotations = false;
bool ShowLights = false;
bool Blinn = false;
bool MirrorExample = false;
bool ShowHelp = false;

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
	//if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	//if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

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

	/* Отображение справки при удерживании F1 */
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		ShowHelp = true;
	}

	/* Скрытие справки при отпускании F1 */
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
	{
		ShowHelp = false;
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
    system("pause");
}

/* Считывание настроек из файла конфигурации */
/* Winfo - считанные даные об окне */
/* Reflection_res - размер карты отражений */
void ReadConfig(windowInfo &Winfo, int &Reflection_res)
{
	FILE *Fin = fopen("config//config.ini", "r");

	/* Настройки по-умолчанию */
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


//*************************************************************//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader_t.h"

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3(1.0f, -1.0f, 0.0f);
        glm::vec3 pos4(1.0f, 1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent1 = glm::normalize(bitangent1);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent2 = glm::normalize(tangent2);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent2 = glm::normalize(bitangent2);


        float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

//*************************************************************//

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
		/* 4x сглаживание, OpenGL 3.3 */
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

		/* Указатель на экран ПК */
		GLFWmonitor *Screen = glfwGetPrimaryMonitor();

		ReadConfig(WindowInfo, GenTextureSize);

		if (WindowInfo.FullScreen)
		{
			/* Информация об экране, разрешение */
			const GLFWvidmode *VidMode = glfwGetVideoMode(Screen);

			WindowInfo.Width = VidMode->width; WindowInfo.HalfWidth = WindowInfo.Width / 2.0f;
			WindowInfo.Height = VidMode->height; WindowInfo.HalfHeight = WindowInfo.Height / 2.0f;

			/* Ширина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
			WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", Screen, NULL);
		}
		else
		{
			/* Ширина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
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

		/* Включение вертикальной синхронизации */
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

		/* Скрыть курсор */
		glfwSetInputMode(WindowInfo.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(WindowInfo.Window, WindowInfo.Width / 2, WindowInfo.Height / 2);

		/* Цвет фона, RGBA */
		//glClearColor(0.0f, 0.6f, 0.8f, 1.0f); //skyblue
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //white
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f); //grey

		/* Включаем буфер глубины */
		glEnable(GL_DEPTH_TEST);

		/* Выбираем фрагмент, ближайший к камере */
		glDepthFunc(GL_LESS);

		/* Отсечение невидимых граней */
		glEnable(GL_CULL_FACE);

		//SCENE Scene = SCENE(WindowInfo, SkyBoxSize, Blinn);

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

        //***********************************************************//

        Shader shader("resources//tmp//parallax_mapping.vs", "resources//tmp//parallax_mapping.fs");

        unsigned int diffuseMap = loadTexture("resources//tmp//rings_diffuse.jpg");
        unsigned int normalMap = loadTexture("resources//tmp//rings_normal2.jpg");
        unsigned int heightMap = loadTexture("resources//tmp//rings_disp.jpg");

        shader.use();
        shader.setInt("diffuseMap", 0);
        shader.setInt("normalMap", 1);
        shader.setInt("depthMap", 2);

        vec3 lightPos(0.5f, 1.0f, 0.3f);

        float angle = 25;
        bool up = true;
        bool stop = true;

        //***********************************************************//

		while (!glfwWindowShouldClose(WindowInfo.Window))
		{
			/* "Заморозка" кнопок */
			for (int i = 0; i < ButtonsCount; i++)
			{
				if (Buttons[i].frames > 30)
				{
					Buttons[i].flag = true;
				}
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/* Подсчёт кадров в секунду и времени на отрисовку 1 кадра */
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

			/* Проверка на нажатие кнопок */
			glfwPollEvents();

			/* Отрисовка сцены */
			//Scene.Render(WindowInfo, CameraMode, GenTextureSize, FOV, MirrorExample, Rotations, ShowLights, Blinn);
			
			/* Отрисовка GUI */
			//Text.Render(text, textColor, 0, 580, 12);                 
            //Text.Render("ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 0, 580, 16);
            //Text.Render("abcdefghijklmnopqrstuvwxyz", textColor, 0, 560, 16);
            //Text.Render("1234567890,.!?/:;-=+<>{}()\"\\@#№$^&*`~[]|", textColor, 0, 540, 16);

            //int offsety = WindowInfo.Height - margin - fontSize - 80;
            /*int offsety = WindowInfo.Height - margin - fontSize;
            FreeType.RenderFreeType(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"abcdefghijklmnopqrstuvwxyz", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"1234567890,.!?/:;-=+<>{}()\"\\@#№$^&*`~[]|", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", textColor, 1.0, 0, offsety);
            offsety -= spacing + fontSize;
            FreeType.RenderFreeType(L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя", textColor, 1.0, 0, offsety);*/

			glfwGetCursorPos(WindowInfo.Window, &MouseX, &MouseY);

			//Buttons[0].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.9f, 0.05f, 0.08f);
			//Rotations = Buttons[1].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.72f, 0.05f, 0.08f);
			//ShowLights = Buttons[2].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.54f, 0.05f, 0.08f);
			//Blinn = Buttons[3].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.36f, 0.05f, 0.08f);
			//MirrorExample = Buttons[4].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.18f, 0.05f, 0.08f);

			//if (ShowHelp) HelpWindow.Render(WindowInfo, 0.0f, 0.0f, 0.8f, 0.8f);

			//for (int i = 0; i < ButtonsCount; i++) Buttons[i].frames++;


            //***********************************************************//

            // configure view/projection matrices
            mat4 projection = perspective(radians(FOV), (float)WindowInfo.Width / (float)WindowInfo.Height, 0.1f, 100.0f);
            mat4 view = lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 3.0f) + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            // render parallax-mapped quad
            glm::mat4 model;
            //model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show parallax mapping from multiple directions            
            model = glm::rotate(model, glm::radians(angle), glm::normalize(vec3(0.0f, 1.0f, 0.0f)));

            if (!stop)
            {
                if (up)
                {
                    if (angle > -10) angle -= 0.1f;
                    else up = false;
                }
                else
                {
                    if (angle < 50) angle += 0.1f;
                    else up = true;
                }
            }

            shader.setMat4("model", model);
            shader.setVec3("viewPos", vec3(0.0f, 0.0f, 3.0f));
            shader.setVec3("lightPos", lightPos);
            shader.setFloat("heightScale", 0.005);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, heightMap);
            renderQuad();

            if (glfwGetKey(WindowInfo.Window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                if (stop) stop = false;
                else stop = true;
            }

            //***********************************************************//

			glfwSwapBuffers(WindowInfo.Window);
		}
		glfwTerminate();
	}
}