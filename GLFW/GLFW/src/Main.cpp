#include "includes\Includes.h"
#include "renderer\Renderer.h"
#include "callbacks\Callbacks.h"
#include "object\Object.h"
#include "scene\TestScene.h"
#include "scene\Scene1.h"
#include "object\coordinate_axes\CoordinateAxes.h"
#include "camera\ICamera.h"
#include "object\skybox\Skybox.h"

#if defined(_WIN64) && defined(NDEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
    logger.start("MAIN");

	renderer = new OpenGLRenderer();

	InputHandler inputHandler;
	inputHandler.setEventHandling();

	////////////////////////////////////////////////////////////LoadingScreen////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////LoadingData//////////////////////////////////////////////////////////////   

	ICamera *camera_FPC = new FirstPersonCamera();
	ICamera *camera_TPC = new ThirdPersonCamera();
	ICamera *camera_static = new StaticCamera();
	ICamera *camera_free = new FreeCamera();

	std::vector<ICamera*> cameras;
	cameras.push_back(camera_FPC);
	cameras.push_back(camera_TPC);
	cameras.push_back(camera_static);
	//cameras.push_back(camera_free);

	ICamera *camera = cameras[0];

	Shader materialShader("resources/shaders/materialShader.vs", "resources/shaders/materialShader.fs");
	Shader simpleShader("resources/shaders/simpleShader.vs", "resources/shaders/simpleShader.fs");
	Shader skyboxShader("resources/shaders/skyboxShader.vs", "resources/shaders/skyboxShader.fs");

	std::vector<Model*> models;

	Model *cube = new Model("resources/3dmodels/cube.obj");
	models.push_back(cube);

	Model *sphere = new Model("resources/3dmodels/sphere_lowpoly.obj");
	models.push_back(sphere);

	Model *cylinder = new Model("resources/3dmodels/cylinder.obj");
	models.push_back(cylinder);

	CoordinateAxes coordinateAxes;

	Scene1 scene1;
	scene1.init(models);

	Skybox skybox(100.0f);

	////////////////////////////////DEBUG////////////////////////////////

	bool testSceneEnabled = false;

	TestScene testScene;

	if (testSceneEnabled)
	{
		std::vector<Model*> testModels;
		Model *nanosuit = new Model("_old/resources/3dmodels/nanosuit/nanosuit.obj");
		testModels.push_back(nanosuit);

		Model *deadpool = new Model("_old/resources/3dmodels/deadpool.obj");
		testModels.push_back(deadpool);
		
		testScene.init(testModels);
	}

	glfwSwapInterval(0);

	////////////////////////////////DEBUG////////////////////////////////

	////////////////////////////////////////////////////////////RenderLoop///////////////////////////////////////////////////////////////

    float currentFrameTime = 0.0;

    logger.log("MAIN", QErrorType::info, "Initialization complete. Entering main loop.");
       
    float fpsInitTime = static_cast<float>(glfwGetTime());

	while (!renderer->quit())
	{
		currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		renderer->setFPS(renderer->getFPS() + 1);

		if (currentFrameTime - fpsInitTime >= 0.01f)
		{
			//sprintf(text, "%d FPS, %.3f ms", nbFrames, 1000.0 / double(nbFrames));
			//sprintf(text2, "Diploma at %d FPS", nbFrames);
			std::stringstream title;
			title << "Diploma at " << renderer->getFPS() << " FPS. Frame time: " << 1000.0 / static_cast<double>(renderer->getFPS()) << "ms.";

			renderer->setWindowTitle(title.str());
			renderer->setFPS(0);
			fpsInitTime += 1.0;
		}

		// Очистить экран
		renderer->clearScreen();

		////////////////////////////////DEBUG////////////////////////////////

		if (testSceneEnabled) testScene.render(materialShader, renderer->getProjectionMatrix(), camera->getViewMatrix(), camera->getPosition());
		else
		{
		
			////////////////////////////////DEBUG////////////////////////////////

			scene1.render(materialShader, renderer->getProjectionMatrix(), camera->getViewMatrix(), camera->getPosition());
			skybox.draw(skyboxShader, renderer->getProjectionMatrix(), camera->getViewMatrix(), camera->getPosition());

			// GUI

			coordinateAxes.draw(simpleShader, camera->getViewMatrixAxes());

		}

		// Обработка ввода
				
		camera->handleInput();

        // Меняем кадр

		renderer->swapBuffers();
		renderer->pollEvents();
    }

	delete renderer;

	cameras.clear();
	std::vector<ICamera*>(cameras).swap(cameras);

	delete camera_FPC;
	delete camera_TPC;
	delete camera_static;
	delete camera_free;

	delete cube;
	delete sphere;
	delete cylinder;

    // Выход из программы.
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
	//		/* "Заморозка" кнопок */
	//		for (int i = 0; i < ButtonsCount; i++)
	//		{
	//			if (Buttons[i].frames > 30)
	//			{
	//				Buttons[i].flag = true;
	//			}
	//		}

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