#include "includes\Includes.h"
#include "callbacks\Callbacks.h"
#include "config\Config.h"
#include "shader\Shader.h"
//#include "Camera.h"
//#include "Object.h"
//#include "Scene.h"
//#include "TextureLoader.h"
//#include "VboIndexer.h"
//#include "Gui.h"

extern Logger logger;

int main()
{    
    logger.start("MAIN");

    if (!glfwInit())
    {
        logger.log("MAIN", QErrorType::error, "Failed to initialize GLFW.");
        logger.stop("MAIN");
        return Q_ERROR_INIT_GLFW;
    }
    
    // OpenGL 4.3       
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        
    // MSAAx4
    glfwWindowHint(GLFW_SAMPLES, 4);
        
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    // Create window section
    ReadConfig(windowInfo, GenTextureSize);
    GLFWmonitor *screen = glfwGetPrimaryMonitor();

    // Параметры монитора
    const GLFWvidmode *vidMode = glfwGetVideoMode(screen);

    if (windowInfo.FullScreen)
    {        
        windowInfo.Width = vidMode->width; windowInfo.HalfWidth = windowInfo.Width / 2.0f;
        windowInfo.Height = vidMode->height; windowInfo.HalfHeight = windowInfo.Height / 2.0f;
        
        // Ширина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого)
        windowInfo.Window = glfwCreateWindow(windowInfo.Width, windowInfo.Height, "Diploma", screen, NULL);
    }
    else
    {
        // Ширина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого)
        windowInfo.Window = glfwCreateWindow(windowInfo.Width, windowInfo.Height, "Diploma", NULL, NULL);             
    }

    // Vsync
    if (windowInfo.Vsync) glfwSwapInterval(1);

    if (!windowInfo.Window)
    {
        logger.log("MAIN", QErrorType::error, "Failed to initialize WINDOW.");
        logger.stop("MAIN");
        glfwTerminate();
        return Q_ERROR_INIT_WINDOW;
    }   

    // Окно в центр экрана
    glfwSetWindowPos(windowInfo.Window, vidMode->width / 2 - windowInfo.Width / 2, vidMode->height / 2 - windowInfo.Height / 2);

    glfwMakeContextCurrent(windowInfo.Window); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger.log("MAIN", QErrorType::error, "Failed to initialize GLAD.");
        logger.stop("MAIN");
        return Q_ERROR_INIT_GLAD;
    }

    int GL_Current_Version_Major = GLVersion.major;
    int GL_Current_Version_Minor = GLVersion.minor;

    // Debug output
#ifdef _DEBUG
    GLint gl_context_flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &gl_context_flags);
    if (gl_context_flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);

        // Фильтр ошибок
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        logger.log("MAIN", QErrorType::error, "Failed to initialize GL DebugOutput.");
        logger.stop("MAIN");       
        glfwTerminate();
        return Q_ERROR_INIT_DEBUG_OUTPUT;
    }
#endif

    // Callbacks
    glfwSetErrorCallback(GLFWErrorCallback);
    glfwSetFramebufferSizeCallback(windowInfo.Window, FramebufferSizeCallback);
    glfwSetKeyCallback(windowInfo.Window, KeyCallback);
    glfwSetScrollCallback(windowInfo.Window, ScrollCallback);

    // Скрыть курсор, поместить в центр экрана
    //windowInfo.ShowCursor = false;
    //glfwSetInputMode(WindowInfo.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPos(WindowInfo.Window, WindowInfo.Width / 2, WindowInfo.Height / 2);

    glViewport(0, 0, windowInfo.Width, windowInfo.Height);

    // Цвет фона, RGBA
    glClearColor(0.0f, 0.6f, 0.8f, 0.0f);

    // Включаем буфер глубины
    glEnable(GL_DEPTH_TEST);

    //	Отсечение невидимых граней
    glEnable(GL_CULL_FACE);

    //	Выбираем фрагмент, ближайший к камере
    glDepthFunc(GL_LESS);    

    //***********************************************//

    Shader testShader("resources//shaders//testShader.vs", "resources//shaders//testShader.fs");
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //***********************************************//

    // Главный цикл
    double currentFrameTime = 0.0;

    logger.log("MAIN", QErrorType::info, "Initialization complete. Entering main loop.");

    while (!glfwWindowShouldClose(windowInfo.Window))
    {
        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Очистить экран
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        testShader.activate();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        // Меняем кадр
        glfwSwapBuffers(windowInfo.Window);
        glfwPollEvents();
    }    

    // Выход из программы.
    glfwTerminate();
    logger.stop("MAIN", false);
    return Q_GOOD_EXIT;

	//int nbFrames = 0;
	//double currentTime, lastTime, MouseX, MouseY;
	//char text[100], text2[30];


	//	SCENE Scene = SCENE(WindowInfo, SkyBoxSize, Blinn);

	//	TEXT Text = TEXT("resources//textures//gui//Text.DDS");

 //       int fontSize = 32;
 //       int margin = 30;
 //       int spacing = 5;
 //       //vec3 textColor = vec3(0.43f, 0.15f, 0.91f);
 //       //vec3 textColor = vec3(0.43f, 0.87f, 0.51f);
 //       vec3 textColor = vec3(1.0f, 1.0f, 1.0f);
 //       TEXT FreeType = TEXT("resources//fonts//timesbd.ttf", fontSize);

	//	int ButtonsCount = 5;
	//	BUTTON *Buttons = new BUTTON[ButtonsCount];
	//	Buttons[0].Prepare(0, false, "resources//textures//gui//wireframe_inactive.bmp", "resources//textures//gui//wireframe_inactivehover.bmp", "resources//textures//gui//wireframe_active.bmp", "resources//textures//gui//wireframe_activehover.bmp");
	//	Buttons[1].Prepare(1, false, "resources//textures//gui//rotations_inactive.bmp", "resources//textures//gui//rotations_inactivehover.bmp", "resources//textures//gui//rotations_active.bmp", "resources//textures//gui//rotations_activehover.bmp");
	//	Buttons[2].Prepare(2, false, "resources//textures//gui//lights_inactive.bmp", "resources//textures//gui//lights_inactivehover.bmp", "resources//textures//gui//lights_active.bmp", "resources//textures//gui//lights_activehover.bmp");
	//	Buttons[3].Prepare(3, false, "resources//textures//gui//blinn_inactive.bmp", "resources//textures//gui//blinn_inactivehover.bmp", "resources//textures//gui//blinn_active.bmp", "resources//textures//gui//blinn_activehover.bmp");
	//	Buttons[4].Prepare(4, false, "resources//textures//gui//scene.bmp", "resources//textures//gui//scene_hover.bmp", "resources//textures//gui//scene.bmp", "resources//textures//gui//scene_hover.bmp");

	//	WINDOW HelpWindow = WINDOW();
	//	HelpWindow.Prepare("resources//textures//gui//help.bmp");

	//	lastTime = glfwGetTime();

	//	while (!glfwWindowShouldClose(WindowInfo.Window))
	//	{
	//		/* "Заморозка" кнопок */
	//		for (int i = 0; i < ButtonsCount; i++)
	//		{
	//			if (Buttons[i].frames > 30)
	//			{
	//				Buttons[i].flag = true;
	//			}
	//		}

	//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//		/* Подсчёт кадров в секунду и времени на отрисовку 1 кадра */
	//		currentTime = glfwGetTime();
	//		nbFrames++;
	//	
	//		if (currentTime - lastTime >= 0.01f)
	//		{
	//			sprintf(text, "%d FPS, %.3f ms", nbFrames, 1000.0 / double(nbFrames));
	//			sprintf(text2, "Diploma at %d FPS", nbFrames);
	//			glfwSetWindowTitle(WindowInfo.Window, text2);
	//			nbFrames = 0;
	//			lastTime += 1.0;
	//		}

	//		/* Проверка на нажатие кнопок */
	//		glfwPollEvents();

	//		/* Отрисовка сцены */
	//		Scene.Render(WindowInfo, CameraMode, GenTextureSize, FOV, MirrorExample, Rotations, ShowLights, Blinn);
	//		
	//		/* Отрисовка GUI */
	//		Text.Render(text, textColor, 0, 580, 12);                 
 //           //Text.Render("ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 0, 580, 16);
 //           //Text.Render("abcdefghijklmnopqrstuvwxyz", textColor, 0, 560, 16);
 //           //Text.Render("1234567890,.!?/:;-=+<>{}()\"\\@#№$^&*`~[]|", textColor, 0, 540, 16);

 //           //int offsety = WindowInfo.Height - margin - fontSize - 80;
 //           /*int offsety = WindowInfo.Height - margin - fontSize;
 //           FreeType.RenderFreeType(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"abcdefghijklmnopqrstuvwxyz", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"1234567890,.!?/:;-=+<>{}()\"\\@#№$^&*`~[]|", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя", textColor, 1.0, 0, offsety);*/

	//		glfwGetCursorPos(WindowInfo.Window, &MouseX, &MouseY);

	//		Buttons[0].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.9f, 0.05f, 0.08f);
	//		Rotations = Buttons[1].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.72f, 0.05f, 0.08f);
	//		ShowLights = Buttons[2].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.54f, 0.05f, 0.08f);
	//		Blinn = Buttons[3].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.36f, 0.05f, 0.08f);
	//		MirrorExample = Buttons[4].Render(WindowInfo, MouseX, MouseY, 0.91f, 0.18f, 0.05f, 0.08f);

	//		if (ShowHelp) HelpWindow.Render(WindowInfo, 0.0f, 0.0f, 0.8f, 0.8f);

	//		for (int i = 0; i < ButtonsCount; i++) Buttons[i].frames++;

	//		glfwSwapBuffers(WindowInfo.Window);
	//	}
	//	glfwTerminate();
	//}
}