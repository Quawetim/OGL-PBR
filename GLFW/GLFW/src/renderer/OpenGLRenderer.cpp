#include "Renderer.h"
#include "..\callbacks\Callbacks.h"

///<summary>Конструктор.</summary>
OpenGLRenderer::OpenGLRenderer()
{
	this->ogl_ = true;

	if (!glfwInit())
	{
		logger.log(__FUNCTION__, ErrorType::error, "Failed to initialize GLFW.");
		logger.stop(__FUNCTION__);
		std::exit(ERROR_INIT_GLFW);
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

	GLFWmonitor *screen = glfwGetPrimaryMonitor();

	// Параметры монитора
	const GLFWvidmode *vidMode = glfwGetVideoMode(screen);

	if (this->isFullScreen_)
	{
		this->windowWidth_ = vidMode->width;
		this->windowHeight_ = vidMode->height;

		// Ширина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого)
		GLFWwindow* wnd = glfwCreateWindow(this->windowWidth_, this->windowHeight_, "Diploma", screen, NULL);
		this->window_.OGLwindow = wnd;
	}
	else
	{
		// Ширина, высота, название окна, монитор (Sсreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого)
		GLFWwindow* wnd = glfwCreateWindow(this->windowWidth_, this->windowHeight_, "Diploma", NULL, NULL);
		this->window_.OGLwindow = wnd;
	}

	if (!this->window_.OGLwindow)
	{
		logger.log(__FUNCTION__, ErrorType::error, "Failed to initialize WINDOW.");
		logger.stop(__FUNCTION__);
		glfwTerminate();
		std::exit(ERROR_INIT_WINDOW);
	}

	// Окно в центр экрана
	glfwSetWindowPos(this->window_.OGLwindow, vidMode->width / 2 - static_cast<int>(this->getWindowHalfWidth()), vidMode->height / 2 - static_cast<int>(this->getWindowHalfHeight()));

	glfwMakeContextCurrent(this->window_.OGLwindow);

	// Vsync
	if (this->isVSync_) glfwSwapInterval(1);
	else glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		logger.log(__FUNCTION__, ErrorType::error, "Failed to initialize GLAD.");
		logger.stop(__FUNCTION__);
		std::exit(ERROR_INIT_GLAD);
	}

	int GL_Current_Version_Major = GLVersion.major;
	int GL_Current_Version_Minor = GLVersion.minor;

	// Debug output
#ifdef _DEBUG
	GLint gl_context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &gl_context_flags);
	if (gl_context_flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		//glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(callbacks::glDebugOutput, nullptr);

		// Фильтр ошибок
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	else
	{
		logger.log(__FUNCTION__, ErrorType::error, "Failed to initialize GL DebugOutput.");
		logger.stop(__FUNCTION__);
		glfwTerminate();
		std::exit(ERROR_INIT_DEBUG_OUTPUT);
	}

	int max_texture_image_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);

	char* s = (char*)glGetString(GL_VENDOR);
	char* s1 = (char*)glGetString(GL_RENDERER);
#endif

	// Callbacks
	glfwSetErrorCallback(callbacks::glfwErrorCallback);
	glfwSetFramebufferSizeCallback(this->window_.OGLwindow, InputHandle::framebufferSizeDispatch);
	glfwSetKeyCallback(this->window_.OGLwindow, InputHandle::keyboardDispatch);
	glfwSetCursorPosCallback(this->window_.OGLwindow, InputHandle::cursorPosDispatch);
	glfwSetMouseButtonCallback(this->window_.OGLwindow, InputHandle::mouseButtonsDispatch);
	glfwSetScrollCallback(this->window_.OGLwindow, InputHandle::scrollDispatch);

	// Скрыть курсор, поместить в центр экрана
	if (this->isShowCursor_)
	{
		glfwSetInputMode(this->window_.OGLwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(this->window_.OGLwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(this->window_.OGLwindow, this->getWindowHalfWidth(), this->getWindowHalfHeight());
	}

	glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, this->windowWidth_, this->windowHeight_);

	// Цвет фона, RGBA
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);

	// Включаем буфер глубины
	glEnable(GL_DEPTH_TEST);

	// Выбираем фрагмент, ближайший к камере
	glDepthFunc(GL_LESS);

	// Отсечение граней, у которых не видно лицевую сторону
	glEnable(GL_CULL_FACE);

	// Блендинг
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Frame VAO
	float quadVertices[] =
	{
		// positions   // textureCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO_);
	glGenBuffers(1, &this->quadVBO_);
	
	glBindVertexArray(this->quadVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	float cubeVertices[] =
	{
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	glGenVertexArrays(1, &this->cubeVAO_);
	glGenBuffers(1, &this->cubeVBO_);

	glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO_);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->frame_ = this->generateTexture2D_RGB16F(this->windowWidth_, this->windowHeight_);
	this->frameRenderBuffer_ = this->generateRenderBuffer(this->windowWidth_, this->windowHeight_);
	this->frameFrameBuffer_ = this->generateFrameBuffer(this->frameRenderBuffer_, this->frame_);

	this->irradianceShader_ = Shader("irradiance");
	this->prefilteringShader_ = Shader("prefiltering");
	this->brdfLutShader_ = Shader("brdf");
	this->coordinateAxesShader_ = Shader("axes");

	this->tempRenderBuffer_ = this->generateRenderBuffer(256, 256);
	this->tempFrameBuffer_ = this->generateFrameBufferCube(this->tempRenderBuffer_);

	this->irradianceMap_ = this->generateCubeMap16F(256, false);
	this->prefilteredMap_ = this->generateCubeMap16F(512, true);
	this->brdfLutMap_ = this->generateTexture2D_RG16F(256, 256);

	this->generateBrdfLutMap();
}

///<summary>Деструктор.</summary>
OpenGLRenderer::~OpenGLRenderer()
{
	glfwTerminate();
}

////////////////////////////////////////////// private-функции //////////////////////////////////////////////

///<summary>Пересчитывает размер frame текстуры.</summary>
void OpenGLRenderer::updateFrameSize()
{
	unsigned int oldFrame = this->frame_;

	this->frame_ = this->generateTexture2D_RGB16F(this->windowWidth_, this->windowHeight_);

	this->bindFrameBuffer(this->frameFrameBuffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, this->frameRenderBuffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->windowWidth_, this->windowHeight_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->frame_, 0);

	this->deleteTexture2D(oldFrame);
}

///<summary>Генерирует irradiance map.</summary>
void OpenGLRenderer::generateIrradianceMap()
{
	glDisable(GL_CULL_FACE);

	int size = 256;

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 view[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	this->irradianceShader_.activate();
	this->irradianceShader_.setProjectionMatrix(projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->environmentMap_);
	this->irradianceShader_.setInt("envMap", 0);

	this->setViewport(0, 0, size, size);
	this->bindFrameBuffer(this->tempFrameBuffer_);

	for (int i = 0; i < 6; i++)
	{
		this->irradianceShader_.setMat4("viewMatrix", view[i]);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->irradianceMap_, 0);
		
		this->clearScreen();
		this->renderCube();
	}

	this->bindFrameBuffer(0);
	this->restoreViewPort();

	glEnable(GL_CULL_FACE);
}

///<summary>Генерирует pre-filtered map.</summary>
void OpenGLRenderer::generatePrefilteredMap()
{
	glDisable(GL_CULL_FACE);

	int size = 512;

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 view[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	this->prefilteringShader_.activate();
	this->prefilteringShader_.setProjectionMatrix(projection);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->environmentMap_);
	this->prefilteringShader_.setInt("envMap", 0);

	int levels = 5;
	this->bindFrameBuffer(this->tempFrameBuffer_);
	for (int mip = 0; mip < levels; mip++)
	{
		float roughness = static_cast<float>(mip) / static_cast<float>(levels - 1);
		this->prefilteringShader_.setFloat("roughness", roughness);

		int mipSize = static_cast<int>(size * std::pow(0.5f, mip));

		glBindRenderbuffer(GL_RENDERBUFFER, this->tempRenderBuffer_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipSize, mipSize);

		this->setViewport(0, 0, mipSize, mipSize);	

		for (int i = 0; i < 6; i++)
		{
			this->prefilteringShader_.setMat4("viewMatrix", view[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->prefilteredMap_, mip);
			
			this->clearScreen();
			this->renderCube();
		}
	}

	this->bindFrameBuffer(0);
	this->restoreViewPort();

	glEnable(GL_CULL_FACE);
}

///<summary>Генерирует BRDF LUT map.</summary>
void OpenGLRenderer::generateBrdfLutMap()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	int size = 256;

	this->brdfLutShader_.activate();

	this->bindFrameBuffer(this->tempFrameBuffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, this->tempRenderBuffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->brdfLutMap_, 0);

	this->setViewport(0, 0, size, size);
	
	this->clearScreen();
	this->renderQuad();

	this->bindFrameBuffer(0);
	this->restoreViewPort();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
}

///<summary>Отрисовка модели.</summary>
///<param name = 'model'>Модель.</param>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'material'>Материал.</param>
void OpenGLRenderer::drawModel(Model* model, Shader shader, Material material)
{
	for (size_t j = 0; j < model->getMeshes().size(); j++)
	{
		unsigned int albedoMapsCount = 0;
		unsigned int smoothnessMapsCount = 0;
		unsigned int metallicMapsCount = 0;		
		unsigned int ambientOcclusionMapsCount = 0;
		unsigned int normalMapsCount = 0;
		unsigned int heightMapsCount = 0;

		bool useAlbedoMaps = false;
		bool useSmoothnessMaps = false;
		bool useMetallicMaps = false;		
		bool useAmbientOcclusionMaps = false;
		bool useNormalMaps = false;
		bool useHeightMaps = false;

		TextureKeys mapKeys;
		TextureKeys albedoKeys = mapTextureType.find(TextureType::albedo)->second;
		TextureKeys	smoothnessKeys = mapTextureType.find(TextureType::smoothness)->second;
		TextureKeys	metallicKeys = mapTextureType.find(TextureType::metallic)->second;
		TextureKeys ambientOcclusionKeys = mapTextureType.find(TextureType::ambientOcclusion)->second;
		TextureKeys normalKeys = mapTextureType.find(TextureType::normal)->second;
		TextureKeys heightKeys = mapTextureType.find(TextureType::height)->second;

		std::string mapNumber;
		std::vector<Texture> pointer;

		// костылище
		// проверить на жор памяти копированием
		if (material.isTexturesEmpty()) pointer = model->getMeshes()[j].getTextures();
		else pointer = material.getTextures();

		// Push textures, i = texture unit
		for (size_t i = 0; i < pointer.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			switch (pointer[i].getType())
			{
				case TextureType::albedo:
					{
						if (albedoMapsCount < 5)
						{
							mapKeys = albedoKeys;

							mapNumber = std::to_string(albedoMapsCount);
							albedoMapsCount++;

							if (!useAlbedoMaps) useAlbedoMaps = true;
						}

						break;
					}
				case TextureType::smoothness:
					{
						if (smoothnessMapsCount < 5)
						{
							mapKeys = smoothnessKeys;

							mapNumber = std::to_string(smoothnessMapsCount);
							smoothnessMapsCount++;

							if (!useSmoothnessMaps) useSmoothnessMaps = true;
						}

						break;
					}
				case TextureType::metallic:
					{
						if (metallicMapsCount < 5)
						{
							mapKeys = metallicKeys;

							mapNumber = std::to_string(metallicMapsCount);
							metallicMapsCount++;

							if (!useMetallicMaps) useMetallicMaps = true;
						}

						break;
					}				
				case TextureType::ambientOcclusion:
					{
						if (ambientOcclusionMapsCount < 5)
						{
							mapKeys = ambientOcclusionKeys;

							mapNumber = std::to_string(ambientOcclusionMapsCount);
							ambientOcclusionMapsCount++;

							if (!useAmbientOcclusionMaps) useAmbientOcclusionMaps = true;
						}

						break;
					}
				case TextureType::normal:
					{
						if (normalMapsCount < 5)
						{
							mapKeys = normalKeys;

							mapNumber = std::to_string(normalMapsCount);
							normalMapsCount++;

							if (!useNormalMaps) useNormalMaps = true;
						}

						break;
					}
				case TextureType::height:
					{
						if (heightMapsCount < 5)
						{
							mapKeys = heightKeys;

							mapNumber = std::to_string(heightMapsCount);
							heightMapsCount++;

							if (!useHeightMaps) useHeightMaps = true;
						}

						break;
					}
				default:
					{
						logger.log(__FUNCTION__, ErrorType::error, "Unexpected texture type");
						logger.stop(__FUNCTION__, true, "Unexpected texture type");
						std::exit(ERROR_UNEXPECTED_TEXTURE_TYPE);
					}
			}

			shader.setInt(std::string(mapKeys.mapsName + "[" + mapNumber + "]"), i);
			glBindTexture(GL_TEXTURE_2D, pointer[i].getID());
		}

		// Push texture flags
		shader.setBool(albedoKeys.mapsUse, useAlbedoMaps);
		shader.setBool(smoothnessKeys.mapsUse, useSmoothnessMaps);
		shader.setBool(metallicKeys.mapsUse, useMetallicMaps);		
		shader.setBool(ambientOcclusionKeys.mapsUse, useAmbientOcclusionMaps);
		shader.setBool(normalKeys.mapsUse, useNormalMaps);
		shader.setBool(heightKeys.mapsUse, useHeightMaps);

		shader.setInt(albedoKeys.mapsCount, albedoMapsCount);
		shader.setInt(smoothnessKeys.mapsCount, smoothnessMapsCount);
		shader.setInt(metallicKeys.mapsCount, metallicMapsCount);		
		shader.setInt(ambientOcclusionKeys.mapsCount, ambientOcclusionMapsCount);
		shader.setInt(normalKeys.mapsCount, normalMapsCount);
		shader.setInt(heightKeys.mapsCount, heightMapsCount);

		int textureFreeNumber = pointer.size();

		glActiveTexture(GL_TEXTURE0 + textureFreeNumber);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->environmentMap_);
		shader.setInt("environmentMap", textureFreeNumber);
		textureFreeNumber++;

		glActiveTexture(GL_TEXTURE0 + textureFreeNumber);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->irradianceMap_);
		shader.setInt("irradianceMap", textureFreeNumber);
		textureFreeNumber++;

		glActiveTexture(GL_TEXTURE0 + textureFreeNumber);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->prefilteredMap_);
		shader.setInt("prefilteredMap", textureFreeNumber);
		textureFreeNumber++;

		glActiveTexture(GL_TEXTURE0 + textureFreeNumber);
		glBindTexture(GL_TEXTURE_2D, this->brdfLutMap_);
		shader.setInt("brdfLutMap", textureFreeNumber);
		textureFreeNumber++;

		glBindVertexArray(model->getMeshes()[j].getVAO());
		glDrawElements(GL_TRIANGLES, model->getMeshes()[j].getIndicesSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (size_t i = 0; i < pointer.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glActiveTexture(GL_TEXTURE0 + pointer.size());
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glActiveTexture(GL_TEXTURE0);
	}
}

void OpenGLRenderer::renderQuad()
{
	glBindVertexArray(this->quadVAO_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void OpenGLRenderer::renderCube()
{
	glBindVertexArray(this->cubeVAO_);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

////////////////////////////////////////////// public-функции //////////////////////////////////////////////

///<summary>Отрисовка кадра во весь экран.</summary>
///<param name = 'shader'>Шейдер.</param>
void OpenGLRenderer::drawFrame(Shader shader)
{
	shader.activate();
	shader.setFloat("gamma", gamma);
	shader.setVec2("resolution", glm::vec2(this->windowWidth_, this->windowHeight_));
	
	this->bindTexture2D(this->frame_);
	glBindVertexArray(this->quadVAO_);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	this->bindTexture2D(0);
}

///<summary>Отрисовка объекта.</summary>
///<param name = 'object'>Объект.</param>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void OpenGLRenderer::drawObject(Object* object, Shader shader, std::vector<std::shared_ptr<PointLight>> lights, glm::mat4 view_matrix, glm::vec3 camera_position)
{
	shader.activate();
	shader.setProjectionViewModelMatrices(this->perspectiveProjection_, view_matrix, object->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);	

	// Push material params
	glm::vec3 albedo = glm::pow(object->getMaterial().getAlbedo(), glm::vec3(gamma));

	shader.setVec3("material.albedo", albedo);
	shader.setFloat("material.metallic", object->getMaterial().getMetallic());	
	shader.setFloat("material.smoothness", object->getMaterial().getSmoothness());
	shader.setFloat("material.surfaceHeight", object->getMaterial().getSurfaceHeight());

	// Push lights
	shader.setInt("lightsCount", lights.size());

	for (size_t i = 0; i < lights.size(); i++)
	{
		std::string name;

		name = "light[" + std::to_string(i) + "].position";
		shader.setVec3(name, lights[i]->getPosition());

		name = "light[" + std::to_string(i) + "].radius";
		shader.setFloat(name, lights[i]->getRadius());

		name = "light[" + std::to_string(i) + "].diffuseColor";
		glm::vec3 diffuseColor = glm::pow(lights[i]->getDiffuseColor(), glm::vec3(gamma));
		shader.setVec3(name, diffuseColor);

		name = "light[" + std::to_string(i) + "].specularColor";
		glm::vec3 specularColor = glm::pow(lights[i]->getSpecularColor(), glm::vec3(gamma));
		shader.setVec3(name, specularColor);

		name = "light[" + std::to_string(i) + "].power";
		shader.setFloat(name, lights[i]->getPower());
	}	

	// Для каждой из моделей в объекте
	for (size_t i = 0; i < object->getModels().size(); i++)
	{
		drawModel(object->getModels()[i], shader, object->getMaterial());
	}	
}

///<summary>Отрисовка скайбокса.</summary>
///<param name = 'skybox'>Скайбокс.</param>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void OpenGLRenderer::drawSkybox(std::shared_ptr<Skybox> skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position)
{
	glDisable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);

	shader.activate();
	shader.setProjectionViewModelMatrices(this->perspectiveProjection_, view_matrix, skybox->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);

	shader.setInt("envMap", 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->environmentMap_);

	glBindVertexArray(skybox->getModels()[0]->getMeshByName("skybox").getVAO());
	glDrawElements(GL_TRIANGLES, skybox->getModels()[0]->getMeshByName("skybox").getIndicesSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

///<summary>Отрисовка точечного источника освещения.</summary>
///<param name = 'light'>Точечный источник освещения.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void OpenGLRenderer::drawPointLight(std::shared_ptr<PointLight> light, glm::mat4 view_Matrix, glm::vec3 camera_position)
{
	glm::vec3 color;

	light->getShader()->activate();
	light->getShader()->setProjectionViewModelMatrices(this->perspectiveProjection_, view_Matrix, light->getModelMatrix());

	color = glm::pow(light->getSpecularColor(), glm::vec3(gamma));
	light->getShader()->setVec3("color", color);

	glBindVertexArray(light->getModel()->getMeshByName("specular").getVAO());
	glDrawElements(GL_TRIANGLES, light->getModel()->getMeshByName("specular").getIndicesSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	color = glm::pow(light->getDiffuseColor(), glm::vec3(gamma));
	light->getShader()->setVec3("color", color);

	glBindVertexArray(light->getModel()->getMeshByName("diffuse").getVAO());
	glDrawElements(GL_TRIANGLES, light->getModel()->getMeshByName("diffuse").getIndicesSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

///<summary>Отрисовка UI элемента.</summary>
///<param name = 'ui_element'>Элемент.</param>
void OpenGLRenderer::drawUiElement(std::shared_ptr<UiElement> ui_element)
{
	glDisable(GL_DEPTH_TEST);

	std::shared_ptr<Shader> shader = ui_element->getShader();

	/*	
	*	   (0;0) ___________________
	*			|					|
	*			|	ui_element		|
	*			|					|
	*			|___________________|
	*								(1;1)			
	*/

	// Magic.

	//left = (ui_element->getX() * this->uiScaleX_ - this->getWindowHalfWidth()) / this->getWindowHalfWidth() * this->aspectRatio_;
	//top = (this->getWindowHalfHeight() - ui_element->getY() * this->uiScaleY_) / this->getWindowHalfHeight();
	
	//right = (ui_element->getX() * this->uiScaleX_ + ui_element->getWidth() * this->uiScaleX_ - this->getWindowHalfWidth()) / this->getWindowHalfWidth() * this->aspectRatio_;
	//bottom = (this->getWindowHalfHeight() - ui_element->getY() * this->uiScaleY_ - ui_element->getHeight() * this->uiScaleY_) / this->getWindowHalfHeight();

	float left = (ui_element->getX() * this->uiScaleX_ / this->getWindowHalfWidth() - 1.0f) * this->aspectRatio_;
	float right = left + (ui_element->getWidth() * this->uiScaleX_ * this->aspectRatio_) / this->getWindowHalfWidth();

	float top = 1.0f - (ui_element->getY() * this->uiScaleY_ / this->getWindowHalfHeight());
	float bottom = top - (ui_element->getHeight() * this->uiScaleY_ / this->getWindowHalfHeight());

	float vertices[] =
	{
		// positions   // textureCoords
		left,	top,	0.0f, 0.0f,
		left,	bottom,	0.0f, 1.0f,
		right,	top,	1.0f, 0.0f,
		right,	bottom,	1.0f, 1.0f
	};

	if (ui_element->VAO_ == 0)
	{
		glGenVertexArrays(1, &ui_element->VAO_);
		glGenBuffers(1, &ui_element->VBO_);
	}

	glBindVertexArray(ui_element->VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, ui_element->VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	shader->activate();

	shader->setProjectionMatrix(this->orthoProjection_);

	if (ui_element->useBgTexture())
	{
		shader->setBool("useTexture", true);

		std::shared_ptr<Texture> texture = ui_element->getBgTexture();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		shader->setInt("Texture", 0);		
	}
	else
	{
		shader->setBool("useTexture", false);	
	}

	shader->setVec3("color", ui_element->getColor());

	glBindVertexArray(ui_element->VAO_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	if (ui_element->useBgTexture())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glEnable(GL_DEPTH_TEST);
}

///<summary>Отрисовка осей координат.</summary>
///<param name = 'view_matrix'>Матрица вида.</param>
void OpenGLRenderer::drawCoordinateAxes(std::shared_ptr<CoordinateAxes> axes, glm::mat4 view_matrix)
{
	this->coordinateAxesShader_.activate();
	this->coordinateAxesShader_.setProjectionViewModelMatrices(axes->getProjectionMatrix(), view_matrix, axes->getModelMatrix());

	int margin = 5;
	int size = 60;

	glViewport(this->windowWidth_ - size - margin, margin, size, size);

	glBindVertexArray(axes->getVAO());
	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	glViewport(0, 0, this->windowWidth_, this->windowHeight_);
}

void OpenGLRenderer::drawDebugQuad(unsigned int textureID, Shader shader)
{
	glm::mat4 modelMatrix;
	modelMatrix = glm::scale(glm::vec3(0.3f));
	modelMatrix *= glm::translate(glm::vec3(5.0f, 2.0f, 0.0f));

	glDisable(GL_DEPTH_TEST);

	shader.activate();

	shader.setProjectionMatrix(this->orthoProjection_);
	shader.setModelMatrix(modelMatrix);
	
	shader.setBool("useBgTexture", true);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader.setInt("bgTexture", 0);

	glBindVertexArray(this->quadVAO_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
}

////////////////////////////////////////////// служебные функции //////////////////////////////////////////////

///<summary>Очистка экрана.</summary>
void OpenGLRenderer::clearScreen() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///<summary>Поменять передний и задний буфферы местами.</summary>
void OpenGLRenderer::swapBuffers() const
{
	glfwSwapBuffers(this->window_.OGLwindow);
}

///<summary>Обработка ивентов.</summary>
void OpenGLRenderer::pollEvents() const
{
	glfwPollEvents();
}

///<summary>Флаг выхода из главного цикла.</summary>
bool OpenGLRenderer::shouldExit() const
{
	return glfwWindowShouldClose(this->window_.OGLwindow);
}

///<summary>Завершение работы.</summary>
void OpenGLRenderer::exit() const
{
	glfwSetWindowShouldClose(this->window_.OGLwindow, GL_TRUE);
}

///<summary>Задаёт параметры вьюпорта.</summary>
///<param name = 'x'>Координата x левого нижнего угла.</param>
///<param name = 'y'>Координата y левого нижнего угла.</param>
///<param name = 'width'>Ширина.</param>
///<param name = 'height'>Высота.</param>
void OpenGLRenderer::setViewport(const int x, const int y, const int width, const int height)
{
	glViewport(x, y, width, height);
}

///<summary>Возвращает размер вьюпорта к дефолным настройкам.</summary>
void OpenGLRenderer::restoreViewPort()
{
	glViewport(0, 0, this->windowWidth_, this->windowHeight_);
}

///<summary>Создаёт текстуру RGB.</summary>
///<param name = 'width'>Ширина.</param>
///<param name = 'height'>Высота.</param>
unsigned int OpenGLRenderer::generateTexture2D_RGB(const int width, const int height)
{
	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return ID;
}

///<summary>Создаёт текстуру RGB16F.</summary>
///<param name = 'width'>Ширина.</param>
///<param name = 'height'>Высота.</param>
unsigned int OpenGLRenderer::generateTexture2D_RGB16F(const int width, const int height)
{
	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return ID;
}

///<summary>Создаёт текстуру RG16F.</summary>
///<param name = 'width'>Ширина.</param>
///<param name = 'height'>Высота.</param>
unsigned int OpenGLRenderer::generateTexture2D_RG16F(const int width, const int height)
{
	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return ID;
}

///<summary>Создаёт float CubeMap.</summary>
///<param name = 'size'>Размер.</param>
///<param name = 'generate_mipmap'>Нужно ли генерировать mipmap.</param>
unsigned int OpenGLRenderer::generateCubeMap16F(const int size, bool generate_mipmap)
{
	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	if (generate_mipmap)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return ID;
}

///<summary>Задаёт активную текстуру.</summary>
///<param name = 'ID'>Идентификатор текстуры.</param>
void OpenGLRenderer::bindTexture2D(const unsigned int ID)
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

///<summary>Удаляет текстуру.</summary>
///<param name = 'ID'>Идентификатор текстуры.</param>
void OpenGLRenderer::deleteTexture2D(const unsigned int ID)
{
	glDeleteTextures(1, &ID);
}

///<summary>Создаёт рендербуффер.</summary>
///<param name = 'width'>Ширина.</param>
///<param name = 'height'>Высота.</param>
unsigned int OpenGLRenderer::generateRenderBuffer(const int width, const int height)
{
	unsigned int renderBuffer;

	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	return renderBuffer;
}

///<summary>Создаёт фреймбуффер.</summary>
///<param name = 'width'>Ширина.</param>
///<param name = 'height'>Высота.</param>
///<param name = 'textureID'>Идентификатор текстуры, хранящей значения фреймбуффера.</param>
unsigned int OpenGLRenderer::generateFrameBuffer(const unsigned int renderBuffer, const unsigned int textureID)
{
	unsigned int frameBuffer;

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		std::string msg = "Can't generate framebuffer.";
		logger.log(__FUNCTION__, ErrorType::error, msg);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return frameBuffer;
}

///<summary>Создаёт кубический фреймбуффер.</summary>
///<param name = 'renderBuffer'>Рендербуффер.</param>
unsigned int OpenGLRenderer::generateFrameBufferCube(const unsigned int renderBuffer)
{
	unsigned int frameBuffer;

	glGenFramebuffers(1, &frameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	return frameBuffer;
}

///<summary>Задаёт активный фреймбуффер для кадра.</summary>
void OpenGLRenderer::bindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameFrameBuffer_);
}

///<summary>Задаёт активный фреймбуффер.</summary>
///<param name = 'ID'>Идентификатор фреймбуффера.</param>
void OpenGLRenderer::bindFrameBuffer(const unsigned int ID)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

///<summary>Удаляет фреймбуффер.</summary>
///<param name = 'ID'>Идентификатор фреймбуффера.</param>
void OpenGLRenderer::deleteFrameBuffer(const unsigned int ID)
{
	glDeleteFramebuffers(1, &ID);
}

///<summary>Включает или отключает буфер глубины.</summary>
///<param name = 'use'>Вкл/выкл.</param>
void OpenGLRenderer::useDepthTesting(const bool use)
{
	if (use) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}

///<summary>Возвращает указатель на окно.</summary>
QWindow OpenGLRenderer::getWindow() const
{
	return this->window_;
}

///<summary>Задаёт полноэкранный режим.</summary>
///<param name = 'fullScreen'>Полноэкранный режим.</param>
void OpenGLRenderer::setFullScreen(const bool fullScreen)
{
	//this->isFullScreen_ = fullScreen;
}

///<summary>Включает/отключает вертикальную синхронизацию.</summary>
///<param name = 'vsync'>Вертикальная инхронизация.</param>
void OpenGLRenderer::setVsync(const bool vsync)
{
	this->isVSync_ = vsync;
	
	if (this->isVSync_) glfwSwapInterval(1);
	else glfwSwapInterval(0);
}

///<summary>Задаёт отображение курсора.</summary>
///<param name = 'showCursor'>Отображать курсор.</param>
void OpenGLRenderer::setShowCursor(const bool showCursor)
{
	this->isShowCursor_ = showCursor;

	if (this->isShowCursor_)
	{
		glfwSetInputMode(this->window_.OGLwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);		
	}
	else
	{
		glfwSetInputMode(this->window_.OGLwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	glfwSetCursorPos(this->window_.OGLwindow, this->getWindowHalfWidth(), this->getWindowHalfHeight());
}

///<summary>Задаёт имя окна.</summary>
///<param name = 'title'>Имя.</param>
void OpenGLRenderer::setWindowTitle(const std::string title)
{
	glfwSetWindowTitle(this->window_.OGLwindow, title.c_str());
}

///<summary>Задаёт environment map.</summary>
///<param name = 'ID'>Идентификатор.</param>
void OpenGLRenderer::setEnvironmentMap(const unsigned int ID)
{
	this->environmentMap_ = ID;

	this->generateIrradianceMap();
	this->generatePrefilteredMap();
}