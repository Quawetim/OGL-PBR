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
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float vertices[] =
	{
		// positions   // textureCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
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
}

///<summary>Деструктор.</summary>
OpenGLRenderer::~OpenGLRenderer()
{
	glfwTerminate();
}

////////////////////////////////////////////// draw-функции //////////////////////////////////////////////

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

		glActiveTexture(GL_TEXTURE0 + pointer.size());
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->envMap_);
		shader.setInt("envMap", pointer.size());

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

///<summary>Отрисовка кадра во весь экран.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'frame'>Кадр.</param>
void OpenGLRenderer::drawFrame(Shader shader, unsigned int frame)
{
	shader.activate();
	shader.setFloat("gamma", gamma);

	glBindVertexArray(this->frameVAO);
	bindTexture2D(frame);

	glDrawArrays(GL_TRIANGLES, 0, 6);
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

		name = "lights[" + std::to_string(i) + "].position";
		shader.setVec3(name, lights[i]->getPosition());

		name = "lights[" + std::to_string(i) + "].radius";
		shader.setFloat(name, lights[i]->getRadius());

		name = "lights[" + std::to_string(i) + "].diffuseColor";
		diffuseColor = glm::pow(lights[i]->getDiffuseColor(), glm::vec3(gamma));
		shader.setVec3(name, diffuseColor);

		name = "lights[" + std::to_string(i) + "].specularColor";
		specularColor = glm::pow(lights[i]->getSpecularColor(), glm::vec3(gamma));
		shader.setVec3(name, specularColor);

		name = "lights[" + std::to_string(i) + "].power";
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

	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getEnvMapID());

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

///<summary>Создаёт текстуру.</summary>
unsigned int OpenGLRenderer::generateTexture2D()
{
	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->windowWidth_, this->windowHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return ID;
}

///<summary>Создаёт float текстуру.</summary>
unsigned int OpenGLRenderer::generateTexture2D16F()
{
	unsigned int ID;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, this->windowWidth_, this->windowHeight_, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

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

///<summary>Задаёт имя окна.</summary>
///<param name = 'title'>Имя.</param>
void OpenGLRenderer::setWindowTitle(const std::string title)
{
	glfwSetWindowTitle(this->window_.OGLwindow, title.c_str());
}