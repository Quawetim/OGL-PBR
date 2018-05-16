#include "includes\Includes.h"
#include "renderer\Renderer.h"
#include "callbacks\Callbacks.h"
#include "object\Object.h"
#include "scene\Scene1.h"
#include "scene\Scene2.h"
#include "scene\Scene3.h"
#include "object\coordinate_axes\CoordinateAxes.h"
#include "camera\ICamera.h"
#include "object\skybox\Skybox.h"
#include "texture_loader\TextureLoader.h"
#include "ui\UiElement.h"

// Force use Nvidia
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// Force use AMD
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int activeScene = 0;

void func1(std::shared_ptr<IScene> scene)
{
	activeScene++;

	if (activeScene > 1) activeScene = 0;
}

void func2(std::shared_ptr<IScene> scene)
{
	bool value = scene->getMoveObjects();
	scene->moveObjects(!value);
}

void func3(std::shared_ptr<IScene> scene)
{
	bool value = scene->getMoveLights();
	scene->moveLights(!value);
}

void func4(std::shared_ptr<IScene> scene)
{
	bool value = scene->getShowLights();
	scene->showLights(!value);
}

#if defined(_WIN64) && defined(NDEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
    logger.start(__FUNCTION__);

	renderer = new OpenGLRenderer();

	std::shared_ptr<InputHandler> inputHandler(new InputHandler);
	inputHandler->setEventHandling();

	std::shared_ptr<ICamera> camera_FPC(new FirstPersonCamera());
	std::shared_ptr<ICamera> camera_TPC(new ThirdPersonCamera());
	std::shared_ptr<ICamera> camera_static(new StaticCamera());
	std::shared_ptr<ICamera> camera_free(new FreeCamera());

	std::vector<std::shared_ptr<ICamera>> cameras;
	cameras.push_back(camera_FPC);
	cameras.push_back(camera_TPC);
	cameras.push_back(camera_static);
	//cameras.push_back(camera_free);

	std::shared_ptr<ICamera> camera = cameras[0];

	Shader materialShader("material");
	Shader pbrShader("PBR");
	Shader skyboxShader("skybox");
	Shader postProcessingShader("postProcessing");

	////////////////////////////////////////////////////////////LoadingScreen////////////////////////////////////////////////////////////		

	std::shared_ptr<Texture> debugTexture(new Texture("test.png", TextureType::albedo));

	renderer->clearScreen();
	renderer->swapBuffers();

	////////////////////////////////////////////////////////////LoadingData//////////////////////////////////////////////////////////////   		

	std::vector<Model*> models;

	Model *cube = new Model("cube.obj");
	models.push_back(cube);

	Model *sphere = new Model("sphere_highpoly.obj");
	models.push_back(sphere);

	Model *cylinder = new Model("cylinder.obj");
	models.push_back(cylinder);

	std::shared_ptr<CoordinateAxes> coordinateAxes(new CoordinateAxes());

	std::shared_ptr<Scene1> scene1(new Scene1);
	//scene1->init(models);

	std::shared_ptr<Scene2> scene2(new Scene2);
	scene2->init(models);

	std::shared_ptr<Scene3> scene3(new Scene3);
	scene3->init(models);

	std::vector<std::shared_ptr<IScene>> allScene;
	//allScene.push_back(std::dynamic_pointer_cast<IScene>(scene1));
	allScene.push_back(std::dynamic_pointer_cast<IScene>(scene2));
	allScene.push_back(std::dynamic_pointer_cast<IScene>(scene3));

	unsigned int environmentMap = textureLoader::loadCubeMap("env_maps/env_map_01");
	//unsigned int environmentMap = textureLoader::loadCubeMapHDR("env_maps/env_map_03", 1024);
	renderer->setEnvironmentMap(environmentMap);	
	
	std::shared_ptr<Skybox> skybox(new Skybox(10000.0f));		// жрёт кучу памяти из-за огромного размера текстур (2048*2048*6*3 байт = 72 Мб)
	skybox->setRotation(-90, glm::vec3(0.0f, 1.0f, 0.0f));
	
	////////////////////////////////DEBUG////////////////////////////////

	std::shared_ptr<UiPanel> panel(new UiPanel(0, 0, 40, 720));
	panel->setBgColor(47, 46, 48);	
	
	std::shared_ptr<UiButton> button1(new UiButton(0, 0, 40, 40));
	button1->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button1(new Texture("ui/next.png", TextureType::albedo));
	button1->setBgTexture(icon_button1);
	button1->setClickFunction(&func1);

	std::shared_ptr<UiButton> button2(new UiButton(0, 40, 40, 40));
	button2->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button2(new Texture("ui/rotate_objects.png", TextureType::albedo));
	button2->setBgTexture(icon_button2);
	button2->setClickFunction(&func2);

	std::shared_ptr<UiButton> button3(new UiButton(0, 80, 40, 40));
	button3->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button3(new Texture("ui/rotate_lights.png", TextureType::albedo));
	button3->setBgTexture(icon_button3);
	button3->setClickFunction(&func3);

	std::shared_ptr<UiButton> button4(new UiButton(0, 120, 40, 40));
	button4->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button4(new Texture("ui/light1.png", TextureType::albedo));
	button4->setBgTexture(icon_button4);
	button4->setClickFunction(&func4);

	////////////////////////////////DEBUG////////////////////////////////

	renderer->setVsync(false);

	////////////////////////////////////////////////////////////RenderLoop///////////////////////////////////////////////////////////////

	float startTime = static_cast<float>(glfwGetTime());
    float currentFrameTime = 0.0f;
	float fpsLastCheckTime = 0.0f;
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;
	
	int fps = 0;
	int frames = 0;

    logger.log(__FUNCTION__, ErrorType::info, "Initialization complete. Entering main loop.");
	
	while (!renderer->shouldExit())
	//while (frames < 1)
	{
		currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;		

		fps++;

		// Each second
		if (currentFrameTime - fpsLastCheckTime >= 1.0f)
		{
			fpsLastCheckTime = static_cast<float>(glfwGetTime());

			std::stringstream title;
			title << "Diploma at " << fps << " FPS. " << "Frame time: " << 1000.0f / fps << "ms.";
			renderer->setWindowTitle(title.str());

			fps = 0;
		}

		renderer->bindFrameBuffer();
		renderer->useDepthTesting(true);

		// Очистить экран
		renderer->clearScreen();

		////////////////////////////////DEBUG////////////////////////////////


		////////////////////////////////DEBUG////////////////////////////////

		allScene[activeScene]->render(deltaTime, pbrShader, camera->getViewMatrix(), camera->getPosition());

		renderer->drawSkybox(skybox, skyboxShader, camera->getViewMatrix(), camera->getPosition());

		// Frame

		renderer->bindFrameBuffer(0);
		renderer->useDepthTesting(false);
		
		renderer->clearScreen();
		renderer->drawFrame(postProcessingShader);

		// GUI
		renderer->drawCoordinateAxes(coordinateAxes, camera->getViewMatrixAxes());

		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(panel));
		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(button1));
		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(button2));
		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(button3));
		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(button4));

		button1->checkActions(inputHandler, allScene[activeScene], renderer->getUiScaleX(), renderer->getUiScaleY());
		button2->checkActions(inputHandler, allScene[activeScene], renderer->getUiScaleX(), renderer->getUiScaleY());
		button3->checkActions(inputHandler, allScene[activeScene], renderer->getUiScaleX(), renderer->getUiScaleY());
		button4->checkActions(inputHandler, allScene[activeScene], renderer->getUiScaleX(), renderer->getUiScaleY());

		// Обработка ввода
						
		if (!renderer->isShowCursor())
		{
			camera->handleInput(deltaTime);
			//skybox->setPosition(camera->getPosition());
		}

        // Меняем кадр

		renderer->swapBuffers();
		renderer->pollEvents();
		frames++;

		//if (!scene3->move)
		//{
		//	if (currentFrameTime - startTime >= 2.0f) scene3->move = true;
		//}
    }

	delete cylinder;
	delete sphere;
	delete cube;	

	cameras.clear();
	std::vector<std::shared_ptr<ICamera>>(cameras).swap(cameras);

	delete renderer;

    // Выход из программы.
    logger.stop(__FUNCTION__, false);
    return GOOD_EXIT;

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