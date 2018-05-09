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
		exit(ERROR_INIT_GLFW);
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
		exit(ERROR_INIT_WINDOW);
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
		exit(ERROR_INIT_GLAD);
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
		glDebugMessageCallback(callbacks::glDebugOutput, nullptr);

		// Фильтр ошибок
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	else
	{
		logger.log(__FUNCTION__, ErrorType::error, "Failed to initialize GL DebugOutput.");
		logger.stop(__FUNCTION__);
		glfwTerminate();
		exit(ERROR_INIT_DEBUG_OUTPUT);
	}
#endif

	// Callbacks
	glfwSetErrorCallback(callbacks::glfwErrorCallback);
	glfwSetFramebufferSizeCallback(this->window_.OGLwindow, InputHandle::framebufferSizeDispatch);
	glfwSetKeyCallback(this->window_.OGLwindow, InputHandle::keyboardDispatch);
	glfwSetCursorPosCallback(this->window_.OGLwindow, InputHandle::cursorPosDispatch);
	glfwSetScrollCallback(this->window_.OGLwindow, InputHandle::scrollDispatch);

	// Скрыть курсор, поместить в центр экрана
	//this->windowInfo_.setShowCursor(false);
	glfwSetInputMode(this->window_.OGLwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(this->window_.OGLwindow, this->getWindowHalfWidth(), this->getWindowHalfHeight());

	glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, this->windowWidth_, this->windowHeight_);

	// Цвет фона, RGBA
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);

	// Включаем буфер глубины
	glEnable(GL_DEPTH_TEST);

	// Выбираем фрагмент, ближайший к камере
	glDepthFunc(GL_LESS);

	// Отсечение граней, у которых не видно лицевую сторону
	//glEnable(GL_CULL_FACE);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	float vertices[] =
	{
		// positions   // textureCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->frameVAO);
	glGenBuffers(1, &this->frameVBO);
	
	glBindVertexArray(this->frameVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->frameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	this->irradianceShader_ = Shader("irradiance");
	this->prefilteringShader_ = Shader("prefiltering");
	this->brdfLutShader_ = Shader("brdf");

	this->cubeVAO = 0;
	this->cubeVBO = 0;

	this->debugQuadVAO = 0;
	this->debugQuadVBO = 0;

	this->tempRenderBuffer_ = this->generateRenderBuffer(256, 256);
	this->tempFrameBuffer_ = this->generateFrameBufferCube(this->tempRenderBuffer_);

	this->irradianceMap_ = this->generateCubeMap16F(256, false);
	this->prefilteringMap_ = this->generateCubeMap16F(256, true);
	this->brdfLutMap_ = this->generateTexture2D_RG16F(256, 256);

	this->generateBrdfLutMap();
}

///<summary>Деструктор.</summary>
OpenGLRenderer::~OpenGLRenderer()
{
	glfwTerminate();
}

////////////////////////////////////////////// private-функции //////////////////////////////////////////////

///<summary>Отрисовка модели.</summary>
///<param name = 'model'>Модель.</param>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'material'>Материал.</param>
void OpenGLRenderer::drawModel(Model* model, Shader shader, Material material)
{
	for (size_t j = 0; j < model->getMeshes().size(); j++)
	{		
		unsigned int diffuseMapsCount = 0;
		unsigned int specularMapsCount = 0;
		unsigned int normalMapsCount = 0;

		bool useDiffuseMaps = false;
		bool useSpecularMaps = false;
		bool useNormalMaps = false;

		TextureKeys mapKeys;
		TextureKeys diffuseKeys = mapTextureType.find(TextureType::diffuse)->second;
		TextureKeys	specularKeys = mapTextureType.find(TextureType::specular)->second;;
		TextureKeys normalKeys = mapTextureType.find(TextureType::normal)->second;;
		
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
				case TextureType::diffuse:     
					{
						mapKeys = diffuseKeys;

						mapNumber = std::to_string(diffuseMapsCount);
						diffuseMapsCount++;

						if (!useDiffuseMaps)
						{
							useDiffuseMaps = true;
							shader.setBool(mapKeys.mapsUse, useDiffuseMaps);
						}
						
						break;
					}
				case TextureType::specular:    
					{
						mapKeys = specularKeys;

						mapNumber = std::to_string(specularMapsCount);
						specularMapsCount++;
						
						if (!useSpecularMaps)
						{
							useSpecularMaps = true;
							shader.setBool(mapKeys.mapsUse, useSpecularMaps);
						}

						break;
					}
				case TextureType::normal:      
					{
						mapKeys = normalKeys;

						mapNumber = std::to_string(normalMapsCount);
						normalMapsCount++;
						
						if (!useNormalMaps)
						{
							useNormalMaps = true;
							shader.setBool(mapKeys.mapsUse, useNormalMaps);
						}
						
						break;
					}
				default:
					{
						logger.log(__FUNCTION__, ErrorType::error, "Unexpected texture type");
						logger.stop(__FUNCTION__, true, "Unexpected texture type");
						exit(ERROR_UNEXPECTED_TEXTURE_TYPE);
					}
			}

			shader.setInt(std::string(mapKeys.mapsName + "[" + mapNumber + "]"), i);
			glBindTexture(GL_TEXTURE_2D, pointer[i].getID());
		}

		shader.setInt(diffuseKeys.mapsCount, diffuseMapsCount);
		shader.setInt(specularKeys.mapsCount, specularMapsCount);
		shader.setInt(normalKeys.mapsCount, normalMapsCount);

		int textureFreeNumber = pointer.size();

		
		glActiveTexture(GL_TEXTURE0 + textureFreeNumber);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->environmentMap_);
		shader.setInt("environmentMap", textureFreeNumber);
		textureFreeNumber++;
		
		glActiveTexture(GL_TEXTURE0 + textureFreeNumber);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->irradianceMap_);
		shader.setInt("irradianceMap", textureFreeNumber);
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

void OpenGLRenderer::renderCube()
{
    if (this->cubeVAO == 0)
    {
        float vertices[] = {
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

        glGenVertexArrays(1, &this->cubeVAO);
        glGenBuffers(1, &this->cubeVBO);

        glBindBuffer(GL_ARRAY_BUFFER, this->cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        
		glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        
		glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        
		glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(this->cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void OpenGLRenderer::renderQuad()
{
	if (this->quadVAO == 0)
	{
		float vertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &this->quadVAO);
		glGenBuffers(1, &this->quadVBO);
		
		glBindVertexArray(this->quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

///<summary>Генерирует irradiance map.</summary>
void OpenGLRenderer::generateIrradianceMap()
{
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

	this->setViewport(0, 0, 256, 256);
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
}

///<summary>Генерирует pre-filtering map.</summary>
void OpenGLRenderer::generatePrefilteringMap()
{
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

		int mipSize = 256 * std::pow(0.5f, mip);

		glBindRenderbuffer(GL_RENDERBUFFER, this->tempRenderBuffer_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipSize, mipSize);

		this->setViewport(0, 0, mipSize, mipSize);	

		for (int i = 0; i < 6; i++)
		{
			this->prefilteringShader_.setMat4("viewMatrix", view[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->prefilteringMap_, mip);
			
			this->clearScreen();
			this->renderCube();
		}
	}

	this->bindFrameBuffer(0);
	this->restoreViewPort();
}

void OpenGLRenderer::generateBrdfLutMap()
{
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
}

////////////////////////////////////////////// public-функции //////////////////////////////////////////////

///<summary>Отрисовка кадра во весь экран.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'frame'>Кадр.</param>
void OpenGLRenderer::drawFrame(Shader shader, unsigned int frame)
{
	shader.activate();
	shader.setFloat("gamma", gamma);
	
	this->bindTexture2D(frame);
	glBindVertexArray(this->frameVAO);

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
	shader.setProjectionViewModelMatrices(this->projectionMatrix_, view_matrix, object->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);	

	// Push material params
	glm::vec3 ambientColor = glm::pow(object->getMaterial().getAmbientColor(), glm::vec3(gamma));
	glm::vec3 diffuseColor = glm::pow(object->getMaterial().getDiffuseColor(), glm::vec3(gamma));
	glm::vec3 specularColor = glm::pow(object->getMaterial().getSpecularColor(), glm::vec3(gamma));

	shader.setVec3("material.ambientColor", ambientColor);
	shader.setVec3("material.diffuseColor", diffuseColor);
	shader.setVec3("material.specularColor", specularColor);
	shader.setFloat("material.shininess", object->getMaterial().getShininess());	
	shader.setFloat("material.reflectiveIndex", object->getMaterial().getReflectiveIndex());
	shader.setFloat("material.refractiveIndex", object->getMaterial().getRefractiveIndex());

	// Push texture flags
	shader.setBool("useDiffuseMaps", false);
	shader.setBool("useSpecularMaps", false);
	shader.setBool("useNormalMaps", false);

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
		diffuseColor = glm::pow(lights[i]->getDiffuseColor(), glm::vec3(gamma));
		shader.setVec3(name, diffuseColor);

		name = "light[" + std::to_string(i) + "].specularColor";
		specularColor = glm::pow(lights[i]->getSpecularColor(), glm::vec3(gamma));
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
	glDepthFunc(GL_LEQUAL);

	shader.activate();
	shader.setProjectionViewModelMatrices(this->projectionMatrix_, view_matrix, skybox->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);

	shader.setInt("envMap", 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->environmentMap_);

	glBindVertexArray(skybox->getModels()[0]->getMeshByName("skybox").getVAO());
	glDrawElements(GL_TRIANGLES, skybox->getModels()[0]->getMeshByName("skybox").getIndicesSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDepthFunc(GL_LESS);
}

///<summary>Отрисовка точечного источника освещения.</summary>
///<param name = 'light'>Точечный источник освещения.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void OpenGLRenderer::drawPointLight(std::shared_ptr<PointLight> light, glm::mat4 view_Matrix, glm::vec3 camera_position)
{
	glm::vec3 color;

	light->getShader()->activate();
	light->getShader()->setProjectionViewModelMatrices(this->projectionMatrix_, view_Matrix, light->getModelMatrix());

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

void OpenGLRenderer::drawDebugQuad(unsigned int textureID, glm::mat4 view_Matrix, Shader shader)
{
	if (this->debugQuadVAO == 0)
	{
		float vertices[] =
		{
			// positions   // textureCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,			
			1.0f,  1.0f,  1.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f
		};

		glGenVertexArrays(1, &this->debugQuadVAO);
		glGenBuffers(1, &this->debugQuadVBO);

		glBindVertexArray(this->debugQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->debugQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	glm::mat4 modelMatrix;
	modelMatrix = glm::scale(glm::vec3(0.3f));
	modelMatrix *= glm::translate(glm::vec3(5.0f, 2.0f, 0.0f));

	float ratio = static_cast<float>(this->windowHeight_) / static_cast<float>(this->windowWidth_);
	glm::mat4 projectionMatrix;

	if (this->windowWidth_ >= this->windowHeight_) projectionMatrix = glm::ortho(-1.0f / ratio, 1.0f / ratio, -1.0f, 1.0f, -1.0f, 1.0f);
	else projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);

	shader.activate();

	shader.setProjectionMatrix(projectionMatrix);
	shader.setModelMatrix(modelMatrix);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader.setInt("bgTexture", 0);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_BLEND);

	glBindVertexArray(this->debugQuadVAO);
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
bool OpenGLRenderer::quit() const
{
	return glfwWindowShouldClose(this->window_.OGLwindow);
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

unsigned int OpenGLRenderer::generateRenderBuffer(const int width, const int height)
{
	unsigned int renderBuffer;

	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

	return renderBuffer;
}

///<summary>Создаёт фреймбуффер.</summary>
///<param name = 'textureID'>Идентификатор текстуры, хранящей значения фреймбуффера.</param>
unsigned int OpenGLRenderer::generateFrameBuffer(const unsigned int textureID)
{
	unsigned int frameBuffer;

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	unsigned int renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->windowWidth_, this->windowHeight_);
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
unsigned int OpenGLRenderer::generateFrameBufferCube(const unsigned int renderBuffer)
{
	unsigned int frameBuffer;

	glGenFramebuffers(1, &frameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	return frameBuffer;
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

///<summary>Задаёт ширину окна.</summary>
///<param name = 'width'>Ширина.</param>
void OpenGLRenderer::setWindowWidth(const int width)
{
	//this->windowWidth_ = width;
}

///<summary>Задаёт высоту окна.</summary>
///<param name = 'height'>Высота.</param>
void OpenGLRenderer::setWindowHeight(const int height)
{
	//this->windowHeight_ = height;
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
	//this->isShowCursor_ = showCursor;
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
}