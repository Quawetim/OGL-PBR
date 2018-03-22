#include "includes\Includes.h"
#include "callbacks\Callbacks.h"
#include "config\Config.h"
#include "object\Mesh.h"
#include "object\Model.h"
#include "texture_loader\TextureLoader.h"
#include "object\Object.h"

#if defined(_WIN64) && defined(NDEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
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
    Config::readConfig(windowInfo, reflectionMapResolution);
    GLFWmonitor *screen = glfwGetPrimaryMonitor();

    // ѕараметры монитора
    const GLFWvidmode *vidMode = glfwGetVideoMode(screen);

    if (windowInfo.getFullScreen())
    {        
        windowInfo.setWidth(vidMode->width);
        windowInfo.setHeight(vidMode->height);
        
        // Ўирина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого)
        GLFWwindow* wnd = glfwCreateWindow(windowInfo.getWidth(), windowInfo.getHeight(), "Diploma", screen, NULL);
        windowInfo.setWindowPointer(wnd);
    }
    else
    {
        // Ўирина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого)
        GLFWwindow* wnd = glfwCreateWindow(windowInfo.getWidth(), windowInfo.getHeight(), "Diploma", NULL, NULL);
        windowInfo.setWindowPointer(wnd);
    }

    if (!windowInfo.getWindowPointer())
    {
        logger.log("MAIN", QErrorType::error, "Failed to initialize WINDOW.");
        logger.stop("MAIN");
        glfwTerminate();
        return Q_ERROR_INIT_WINDOW;
    }   

    // ќкно в центр экрана
    glfwSetWindowPos(windowInfo.getWindowPointer(), vidMode->width / 2 - (int)windowInfo.getHalfWidth(), vidMode->height / 2 - (int)windowInfo.getHalfHeight());

    glfwMakeContextCurrent(windowInfo.getWindowPointer()); 

    // Vsync
    if (windowInfo.getVsync()) glfwSwapInterval(1);

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
        glDebugMessageCallback(Callbacks::glDebugOutput, nullptr);

        // ‘ильтр ошибок
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
    glfwSetErrorCallback(Callbacks::GLFWErrorCallback);
    glfwSetFramebufferSizeCallback(windowInfo.getWindowPointer(), Callbacks::FramebufferSizeCallback);
    glfwSetKeyCallback(windowInfo.getWindowPointer(), Callbacks::KeyCallback);
    glfwSetScrollCallback(windowInfo.getWindowPointer(), Callbacks::ScrollCallback);

    // —крыть курсор, поместить в центр экрана
    //windowInfo.setShowCursor(false);
    //glfwSetInputMode(windowInfo.getWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPos(windowInfo.getWindowPointer(), windowInfo.getHalfWidth(), windowInfo.getHalfHeight());

    glViewport(0, 0, windowInfo.getWidth(), windowInfo.getHeight());

    // ÷вет фона, RGBA
    glClearColor(0.0f, 0.6f, 0.8f, 0.0f);

    // ¬ключаем буфер глубины
    glEnable(GL_DEPTH_TEST);

    //	ќтсечение невидимых граней
    //glEnable(GL_CULL_FACE);

    //	¬ыбираем фрагмент, ближайший к камере
    glDepthFunc(GL_LESS);    

	Shader materialShader("resources/shaders/material.vs", "resources/shaders/material.fs");

	Model *cube = new Model("resources/3dmodels/cube.obj");
	Model *sphere = new Model("resources/3dmodels/sphere_lowpoly.obj");
	Model *cylinder = new Model("resources/3dmodels/cylinder.obj");

	std::vector<Object> cubes;
	std::vector<Object> spheres;
	std::vector<Object> cylinders;

	glm::vec3 initialSpheresPos = glm::vec3(0.0f, 6.0f, 0.0f);
	glm::vec3 initialCylindersPos = glm::vec3(3.0f, 6.0f, 0.0f);

	for (size_t i = 0; i < 5; i++)
	{
		Object obj;
		std::string name;
		
		name = "cube" + std::to_string(i + 1);
		obj = Object(name, cube);
		obj.setPosition(glm::vec3(-3.0f, 6.0f - 3 * i, 0.0f));
		cubes.push_back(obj);

		name = "sphere" + std::to_string(i + 1);
		obj = Object(name, sphere);
		obj.setPosition(glm::vec3(0.0f, 6.0f - 3 * i, 0.0f));
		spheres.push_back(obj);

		name = "cylinder" + std::to_string(i + 1);
		obj = Object(name, cylinder);
		obj.setPosition(glm::vec3(3.0f, 6.0f - 3 * i, 0.0f));
		cylinders.push_back(obj);
	}

	cubes[0].setScale(glm::vec3(0.5f));

    //*************DEBUG*****************************//
    
    std::vector<QVertexData> vertices;
    std::vector<unsigned int> indices;
    std::vector<QTexture> textures;

    QVertexData v;
    v.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertices.push_back(v);
    v.position = glm::vec3(1.0f, -1.0f, 0.0f);
    vertices.push_back(v);
    v.position = glm::vec3(1.0f, 1.0f, 0.0f);
    vertices.push_back(v);
    v.position = glm::vec3(-1.0f, 1.0f, 0.0f);
    vertices.push_back(v);

    unsigned int ind;
    ind = 0;
    indices.push_back(ind);
    ind = 1;
    indices.push_back(ind);
    ind = 2;
    indices.push_back(ind);
    ind = 0;
    indices.push_back(ind);
    ind = 2;
    indices.push_back(ind);
    ind = 3;
    indices.push_back(ind);

    Mesh rectangle("rectangle", vertices, indices, textures); 
	rectangle.setPosition(glm::vec3(0.0f, -8.0f, 0.0f));
	rectangle.setRotation(45.0, glm::vec3(0.0f, 0.0f, 1.0f));
	rectangle.setScale(glm::vec3(0.5f));

    QTexture testTexture;
    testTexture.type = QTextureType::diffuse;
    testTexture.path = "resources/textures/test.bmp";
    testTexture.id = TextureLoader::loadTexture(testTexture.path);
    
    cube->setTestTexture(testTexture);
    sphere->setTestTexture(testTexture);
    cylinder->setTestTexture(testTexture);

	Model *nanosuit = new Model("_old/resources/3dmodels/nanosuit/nanosuit.obj");
	Object Nanosuit("nanosuit", nanosuit);
	Nanosuit.addModel(cube);
	Nanosuit.setPosition(glm::vec3(0.0f, -8.0f, 0.0f));
	Nanosuit.setRotation(30.0, glm::vec3(0.0f, 1.0f, 0.0f));
	Nanosuit.setScale(glm::vec3(0.5f));

	glfwSwapInterval(0);

	glm::mat4 P = glm::perspective(glm::radians(FOV), (float)windowInfo.getWidth() / (float)windowInfo.getHeight(), 0.05f, 500.0f);
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //***********************************************//

    // √лавный цикл
    double currentFrameTime = 0.0;

    logger.log("MAIN", QErrorType::info, "Initialization complete. Entering main loop.");
       
    double fpsInitTime = glfwGetTime();   

    while (!glfwWindowShouldClose(windowInfo.getWindowPointer()))
    {
        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        windowInfo.setFPS(windowInfo.getFPS() + 1);
        	
        if (currentFrameTime - fpsInitTime >= 0.01f)
        {
        	//sprintf(text, "%d FPS, %.3f ms", nbFrames, 1000.0 / double(nbFrames));
        	//sprintf(text2, "Diploma at %d FPS", nbFrames);
            std::stringstream title;
            title << "Diploma at " << windowInfo.getFPS() << " FPS. Frame time: " << 1000.0 / (double)windowInfo.getFPS() << "ms.";

        	glfwSetWindowTitle(windowInfo.getWindowPointer(), title.str().c_str());
            windowInfo.setFPS(0);
            fpsInitTime += 1.0;
        }

        // ќчистить экран
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//*************DEBUG*****************************//
            
        /*materialShader.setProjectionViewModelMatrices(P, V, rectangle.getModelMatrix());
        rectangle.drawMesh(materialShader);
        rectangle.moveMesh(0.0f, 1.0f, 0.0f);
        rectangle.rotateMesh(45.0, glm::vec3(0.0f, 0.0f, 1.0f));
        rectangle.scaleMesh(0.1f);*/

		/*materialShader.setProjectionViewModelMatrices(P, V, Nanosuit.getModelMatrix());
		Nanosuit.draw(materialShader);
		Nanosuit.moveObject(0.0f, 0.2f, 0.0f);
		Nanosuit.rotateObject(90.0, glm::vec3(0.0f, 1.0f, 0.0f));
		Nanosuit.scaleObject(0.01f);*/

		//***********************************************//

        //  убы
		for (size_t i = 0; i < cubes.size(); i++)
		{
			materialShader.setProjectionViewModelMatrices(P, V, cubes[i].getModelMatrix());
			cubes[i].draw(materialShader);
		}

		cubes[0].rotateObject(30.0, glm::vec3(0.0f, 1.0f, 0.0f));

        // —феры
		for (size_t i = 0; i < cubes.size(); i++)
		{
			materialShader.setProjectionViewModelMatrices(P, V, spheres[i].getModelMatrix());
			spheres[i].draw(materialShader);
		}

        // ÷илиндры
		for (size_t i = 0; i < cubes.size(); i++)
		{
			materialShader.setProjectionViewModelMatrices(P, V, cylinders[i].getModelMatrix());
			cylinders[i].draw(materialShader);
		}

        // ћен€ем кадр
        glfwSwapBuffers(windowInfo.getWindowPointer());
        glfwPollEvents();
    }    

	cubes.clear();
	spheres.clear();
	cylinders.clear();

	delete cube;
	delete sphere;
	delete cylinder;

    // ¬ыход из программы.
    glfwTerminate();
    logger.stop("MAIN", false);
    return Q_GOOD_EXIT;

	//double MouseX, MouseY;

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
	//		/* "«аморозка" кнопок */
	//		for (int i = 0; i < ButtonsCount; i++)
	//		{
	//			if (Buttons[i].frames > 30)
	//			{
	//				Buttons[i].flag = true;
	//			}
	//		}

	//		/* ќтрисовка сцены */
	//		Scene.Render(WindowInfo, CameraMode, GenTextureSize, FOV, MirrorExample, Rotations, ShowLights, Blinn);
	//		
	//		/* ќтрисовка GUI */
	//		Text.Render(text, textColor, 0, 580, 12);                 
 //           //Text.Render("ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 0, 580, 16);
 //           //Text.Render("abcdefghijklmnopqrstuvwxyz", textColor, 0, 560, 16);
 //           //Text.Render("1234567890,.!?/:;-=+<>{}()\"\\@#є$^&*`~[]|", textColor, 0, 540, 16);

 //           //int offsety = WindowInfo.Height - margin - fontSize - 80;
 //           /*int offsety = WindowInfo.Height - margin - fontSize;
 //           FreeType.RenderFreeType(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"abcdefghijklmnopqrstuvwxyz", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"1234567890,.!?/:;-=+<>{}()\"\\@#є$^&*`~[]|", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"јЅ¬√ƒ≈®∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЏџ№Ёёя", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"абвгдеЄжзийклмнопрстуфхцчшщъыьэю€", textColor, 1.0, 0, offsety);*/

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