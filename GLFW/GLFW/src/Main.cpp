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

int abcdefg = 128;
void sleep(float seconds)
{
	float end = static_cast<float>(glfwGetTime()) + seconds;
	while (glfwGetTime() < end) {};
}

void testfunc()
{
	abcdefg = 256;
}

#if defined(_WIN64) && defined(NDEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
    logger.start(__FUNCTION__);

	renderer = new OpenGLRenderer();
	unsigned int frame = renderer->generateTexture2D_RGB16F(renderer->getWindowWidth(), renderer->getWindowHeight());
	unsigned int frameBuffer = renderer->generateFrameBuffer(frame);

	GLint ttt;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &ttt);

	InputHandler inputHandler;
	inputHandler.setEventHandling();

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

	Shader uiShader("ui");
	Shader materialShader("material");
	Shader pbrShader("PBR");
	Shader axesShader("axes");
	Shader skyboxShader("skybox");
	Shader postProcessingShader("postProcessing");

	////////////////////////////////////////////////////////////LoadingScreen////////////////////////////////////////////////////////////		

	unsigned int debugTexture = textureLoader::loadTexture("resources/textures/test.png", TextureType::albedo);

	renderer->clearScreen();
	renderer->swapBuffers();

	////////////////////////////////////////////////////////////LoadingData//////////////////////////////////////////////////////////////   		

	std::vector<Model*> models;

	Model *cube = new Model("resources/3dmodels/cube.obj");
	models.push_back(cube);

	Model *sphere = new Model("resources/3dmodels/sphere_highpoly.obj");
	models.push_back(sphere);

	Model *cylinder = new Model("resources/3dmodels/cylinder.obj");
	models.push_back(cylinder);

	CoordinateAxes coordinateAxes;

	Scene1 scene1;
	scene1.init(models);

	Scene2 scene2;
	//scene2.init(models);

	Scene3 scene3;
	//scene3.init(models);

	unsigned int environmentMap = textureLoader::loadCubeMap("env_map_01");
	//unsigned int environmentMap = textureLoader::loadCubeMapHDR("env_map_03", 1024);
	renderer->setEnvironmentMap(environmentMap);	
	
	std::shared_ptr<Skybox> skybox(new Skybox(10000.0f));		// ��� ���� ������ ��-�� ��������� ������� ������� (2048*2048*6*3 ���� = 72 ��)
	skybox->setRotation(-90, glm::vec3(0.0f, 1.0f, 0.0f));
	
	////////////////////////////////DEBUG////////////////////////////////

	std::shared_ptr<UiPanel> panel(new UiPanel(0, 0, 50, 300));
	panel->setBgColor(255, 0, 0);	
	
	std::shared_ptr<UiButton> button(new UiButton());
	button->setClickFunction(&testfunc);
	button->click();

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
	
	while (!renderer->quit())
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

		renderer->bindFrameBuffer(frameBuffer);
		renderer->useDepthTesting(true);

		// �������� �����
		renderer->clearScreen();

		////////////////////////////////DEBUG////////////////////////////////


		////////////////////////////////DEBUG////////////////////////////////

		scene1.render(deltaTime, pbrShader, camera->getViewMatrix(), camera->getPosition());
		//scene2.render(deltaTime, pbrShader, camera->getViewMatrix(), camera->getPosition());
		//scene3.render(deltaTime, pbrShader, camera->getViewMatrix(), camera->getPosition());

		renderer->drawSkybox(skybox, skyboxShader, camera->getViewMatrix(), camera->getPosition());

		// GUI

		coordinateAxes.draw(axesShader, camera->getViewMatrixAxes());

		//renderer->drawDebugQuad(renderer->brdfLutMap_, camera->getViewMatrix(), guiShader);

		renderer->drawUiElement(std::dynamic_pointer_cast<UiElement>(panel));

		// Frame

		renderer->bindFrameBuffer(0);
		renderer->useDepthTesting(false);
		
		renderer->clearScreen();
		renderer->drawFrame(postProcessingShader, frame);

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

		if (!scene3.move)
		{
			if (currentFrameTime - startTime >= 2.0f) scene3.move = true;
		}
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
	//		/* "���������" ������ */
	//		for (int i = 0; i < ButtonsCount; i++)
	//		{
	//			if (Buttons[i].frames > 30)
	//			{
	//				Buttons[i].flag = true;
	//			}
	//		}

	//		/* ��������� ����� */
	//		Scene.Render(WindowInfo, CameraMode, GenTextureSize, FOV, MirrorExample, Rotations, ShowLights, Blinn);
	//		
	//		/* ��������� GUI */
	//		Text.Render(text, textColor, 0, 580, 12);                 
 //           //Text.Render("ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 0, 580, 16);
 //           //Text.Render("abcdefghijklmnopqrstuvwxyz", textColor, 0, 560, 16);
 //           //Text.Render("1234567890,.!?/:;-=+<>{}()\"\\@#�$^&*`~[]|", textColor, 0, 540, 16);

 //           //int offsety = WindowInfo.Height - margin - fontSize - 80;
 //           /*int offsety = WindowInfo.Height - margin - fontSize;
 //           FreeType.RenderFreeType(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"abcdefghijklmnopqrstuvwxyz", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"1234567890,.!?/:;-=+<>{}()\"\\@#�$^&*`~[]|", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"�����Ũ��������������������������", textColor, 1.0, 0, offsety);
 //           offsety -= spacing + fontSize;
 //           FreeType.RenderFreeType(L"��������������������������������", textColor, 1.0, 0, offsety);*/

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