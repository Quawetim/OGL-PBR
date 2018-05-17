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

	if (activeScene > 2) activeScene = 0;
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

	////////////////////////////////////////////////////////////Loading Camera////////////////////////////////////////////////////////////

	std::shared_ptr<ICamera> camera_FPC(new FirstPersonCamera());
	//std::shared_ptr<ICamera> camera_TPC(new ThirdPersonCamera());
	//std::shared_ptr<ICamera> camera_static(new StaticCamera());
	//std::shared_ptr<ICamera> camera_free(new FreeCamera());

	std::vector<std::shared_ptr<ICamera>> cameras;
	cameras.push_back(camera_FPC);
	//cameras.push_back(camera_TPC);
	//cameras.push_back(camera_static);
	//cameras.push_back(camera_free);

	std::shared_ptr<ICamera> camera = cameras[0];

	////////////////////////////////////////////////////////////Loading Shader////////////////////////////////////////////////////////////

	Shader materialShader("material");
	Shader pbrShader("PBR");
	Shader skyboxShader("skybox");
	Shader postProcessingShader("postProcessing");

	////////////////////////////////////////////////////////////Loading Screen////////////////////////////////////////////////////////////		

	std::shared_ptr<Texture> debugTexture(new Texture("test.png", TextureType::albedo));

	renderer->clearScreen();
	renderer->swapBuffers();
	renderer->pollEvents();

	////////////////////////////////////////////////////////////Loading Model//////////////////////////////////////////////////////////////   		

	std::vector<Model*> models;

	Model *cube = new Model("cube.obj");
	models.push_back(cube);

	Model *sphere = new Model("sphere_highpoly.obj");
	models.push_back(sphere);

	Model *cylinder = new Model("cylinder.obj");
	models.push_back(cylinder);

	std::shared_ptr<CoordinateAxes> coordinateAxes(new CoordinateAxes());

	////////////////////////////////////////////////////////////Loading Scene//////////////////////////////////////////////////////////////

	std::shared_ptr<Scene1> scene1(new Scene1);
	scene1->init(models);

	renderer->pollEvents();

	std::shared_ptr<Scene2> scene2(new Scene2);
	scene2->init(models);

	renderer->pollEvents();

	std::shared_ptr<Scene3> scene3(new Scene3);
	scene3->init(models);

	renderer->pollEvents();

	std::vector<std::shared_ptr<IScene>> allScene;
	allScene.push_back(std::dynamic_pointer_cast<IScene>(scene1));
	allScene.push_back(std::dynamic_pointer_cast<IScene>(scene2));
	allScene.push_back(std::dynamic_pointer_cast<IScene>(scene3));

	unsigned int environmentMap = textureLoader::loadCubeMap("env_maps/env_map_01");
	//unsigned int environmentMap = textureLoader::loadCubeMapHDR("env_maps/env_map_03", 1024);
	renderer->setEnvironmentMap(environmentMap);	
	
	std::shared_ptr<Skybox> skybox(new Skybox(10000.0f));		// ��� ���� ������ ��-�� ��������� ������� ������� (2048*2048*6*3 ���� = 72 ��)
	skybox->setRotation(-90, glm::vec3(0.0f, 1.0f, 0.0f));
	
	////////////////////////////////////////////////////////////Loading User Interface///////////////////////////////////////////////////////////////

	std::shared_ptr<UiPanel> panel(new UiPanel(0, 0, 40, renderer->getWindowHeight()));
	panel->setBgColor(47, 46, 48);	
	
	std::shared_ptr<UiButton> button1(new UiButton(0, renderer->getWindowHeight() - 40, 40, 40));
	button1->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button1(new Texture("ui/next.png", TextureType::albedo));
	button1->setBgTexture(icon_button1);
	button1->setClickFunction(&func1);

	std::shared_ptr<UiButton> button2(new UiButton(0, renderer->getWindowHeight() - 80, 40, 40));
	button2->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button2(new Texture("ui/rotate_objects.png", TextureType::albedo));
	button2->setBgTexture(icon_button2);
	button2->setClickFunction(&func2);

	std::shared_ptr<UiButton> button3(new UiButton(0, renderer->getWindowHeight() - 120, 40, 40));
	button3->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button3(new Texture("ui/rotate_lights.png", TextureType::albedo));
	button3->setBgTexture(icon_button3);
	button3->setClickFunction(&func3);

	std::shared_ptr<UiButton> button4(new UiButton(0, renderer->getWindowHeight() - 160, 40, 40));
	button4->setBgColor(43, 141, 217);
	std::shared_ptr<Texture> icon_button4(new Texture("ui/light1.png", TextureType::albedo));
	button4->setBgTexture(icon_button4);
	button4->setClickFunction(&func4);

	std::shared_ptr<UiLabel> label1(new UiLabel("gothic", 14, renderer->getWindowWidth() - 200, renderer->getWindowHeight() - 20));
	label1->setBgColor(255, 255, 255);

	std::shared_ptr<UiLabel> label2(new UiLabel("gothic", 14, renderer->getWindowWidth() - 130, renderer->getWindowHeight() - 20));
	label2->setBgColor(255, 255, 255);

	////////////////////////////////DEBUG////////////////////////////////

#ifdef DEBUG
	renderer->setVsync(false);
#endif

	////////////////////////////////DEBUG////////////////////////////////

	////////////////////////////////////////////////////////////Render Loop///////////////////////////////////////////////////////////////

	float startTime = static_cast<float>(glfwGetTime());
    float currentFrameTime = 0.0f;
	float fpsLastCheckTime = 0.0f;
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;
	
	int fps = 0;
	int frames = 0;

    logger.log(__FUNCTION__, ErrorType::info, "Initialization complete. Entering main loop.");
	
	while (!renderer->shouldExit())
	{
		currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;		

		fps++;

		// Each second
		if (currentFrameTime - fpsLastCheckTime >= 1.0f)
		{
			fpsLastCheckTime = static_cast<float>(glfwGetTime());

			float ms = 1000.0f / fps;

			std::stringstream title;
			title << "Diploma at " << fps << " FPS. " << "Frame time: " << ms << "ms.";
			renderer->setWindowTitle(title.str());

			std::stringstream l1;
			l1 << "FPS: " << fps;
			label1->setText(l1.str());

			if (fps < 31) label1->setBgColor(255, 0, 0);
			else
			{
				if (fps < 61) label1->setBgColor(255, 255, 0);
				else label1->setBgColor(0, 255, 0);
			}

			std::stringstream l2;
			l2 << "Time: " << ms << "ms.";
			label2->setText(l2.str());

			if (ms < 16.6f) label2->setBgColor(0, 255, 0);
			else
			{
				if (fps < 33.3f) label1->setBgColor(255, 255, 0);
				else label1->setBgColor(255, 0, 0);
			}

			fps = 0;
		}

		renderer->bindFrameBuffer();
		renderer->useDepthTesting(true);

		// �������� �����
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

		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(label1));
		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(label2));

		button1->checkActions(inputHandler, allScene[activeScene]);
		button2->checkActions(inputHandler, allScene[activeScene]);
		button3->checkActions(inputHandler, allScene[activeScene]);
		button4->checkActions(inputHandler, allScene[activeScene]);

		// ��������� �����
						
		if (!renderer->isShowCursor())
		{
			camera->handleInput(deltaTime);
			//skybox->setPosition(camera->getPosition());
		}

        // ������ ����

		renderer->swapBuffers();
		renderer->pollEvents();
		frames++;
    }

	delete cylinder;
	delete sphere;
	delete cube;	

	cameras.clear();
	std::vector<std::shared_ptr<ICamera>>(cameras).swap(cameras);

	delete renderer;

    // ����� �� ���������.
    logger.stop(__FUNCTION__, false);
    return GOOD_EXIT;
}