#include "Renderer.h"
#include "..\callbacks\Callbacks.h"

///<summary>Конструктор.</summary>
OpenGLRenderer::OpenGLRenderer()
{
	this->ogl_ = true;

	if (!glfwInit())
	{
		logger.log("MAIN", QErrorType::error, "Failed to initialize GLFW.");
		logger.stop("MAIN");
		exit(Q_ERROR_INIT_GLFW);
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
		logger.log("MAIN", QErrorType::error, "Failed to initialize WINDOW.");
		logger.stop("MAIN");
		glfwTerminate();
		exit(Q_ERROR_INIT_WINDOW);
	}

	// Окно в центр экрана
	glfwSetWindowPos(this->window_.OGLwindow, vidMode->width / 2 - static_cast<int>(this->getWindowHalfWidth()), vidMode->height / 2 - static_cast<int>(this->getWindowHalfHeight()));

	glfwMakeContextCurrent(this->window_.OGLwindow);

	// Vsync
	if (this->isVSync_) glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		logger.log("MAIN", QErrorType::error, "Failed to initialize GLAD.");
		logger.stop("MAIN");
		exit(Q_ERROR_INIT_GLAD);
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
		logger.log("MAIN", QErrorType::error, "Failed to initialize GL DebugOutput.");
		logger.stop("MAIN");
		glfwTerminate();
		exit(Q_ERROR_INIT_DEBUG_OUTPUT);
	}
#endif

	// Callbacks
	glfwSetErrorCallback(callbacks::glfwErrorCallback);
	glfwSetFramebufferSizeCallback(this->window_.OGLwindow, QInputHandle::framebufferSizeDispatch);
	glfwSetKeyCallback(this->window_.OGLwindow, QInputHandle::keyboardDispatch);
	glfwSetCursorPosCallback(this->window_.OGLwindow, QInputHandle::cursorPosDispatch);
	glfwSetScrollCallback(this->window_.OGLwindow, QInputHandle::scrollDispatch);

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
}

///<summary>Деструктор.</summary>
OpenGLRenderer::~OpenGLRenderer()
{
	glfwTerminate();
}

void OpenGLRenderer::drawModel(Model* model, Shader shader, QMaterial material)
{
	for (size_t j = 0; j < model->getMeshes().size(); j++)
	{		
		unsigned int diffuseNumber = 1;
		unsigned int specularNumber = 1;
		unsigned int normalNumber = 1;

		std::string number, name;

		std::vector<QTexture> pointer;

		// костылище
		// проверить на жор памяти копированием
		if (material.isTexturesEmpty()) pointer = model->getMeshes()[j].getTextures();
		else pointer = material.getTextures();

		// Push textures, i = texture unit
		for (size_t i = 0; i < pointer.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			name = mapTextureType.find(pointer[i].getType())->second;

			switch (pointer[i].getType())
			{
				case QTextureType::diffuse:     number = std::to_string(diffuseNumber++); break;
				case QTextureType::specular:    number = std::to_string(specularNumber++); break;
				case QTextureType::normal:      number = std::to_string(normalNumber++); break;
				default:
					{
						logger.log("Mesh::drawMesh", QErrorType::error, "Unexpected texture type");
						logger.stop("Mesh::drawMesh", true, "Unexpected texture type");
						exit(Q_ERROR_UNEXPECTED_TEXTURE_TYPE);
					}
			}

			shader.setBool(std::string(name + number + "_flag"), true);
			shader.setInt(std::string(name + number), i);
			glBindTexture(GL_TEXTURE_2D, pointer[i].getID());
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(model->getMeshes()[j].getVAO());
		glDrawElements(GL_TRIANGLES, model->getMeshes()[j].getIndicesSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

///<summary>Отрисовка объекта.</summary>
///<param name = 'object'>Объект.</param>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void OpenGLRenderer::drawObject(Object* object, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position)
{
	shader.activate();
	shader.setProjectionViewModelMatrices(this->projectionMatrix_, view_matrix, object->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);

	// Push material params
	shader.setVec3("material.ambientColor", object->getMaterial().getAmbientColor());
	shader.setVec3("material.diffuseColor", object->getMaterial().getDiffuseColor());
	shader.setVec3("material.specularColor", object->getMaterial().getSpecularColor());
	shader.setFloat("material.shininess", object->getMaterial().getShininess());

	// Push texture flags
	shader.setBool("diffuseMap1_flag", false);
	shader.setBool("specularMap1_flag", false);
	shader.setBool("normalMap1_flag", false);

	// Для каждой из моделей в объекте
	for (size_t i = 0; i < object->getModels().size(); i++)
	{
		drawModel(object->getModels()[i], shader, object->getMaterial());
	}
}

///<summary>Отрисовка скайбокса.</summary>
///<param name = 'skybox'>Объект.</param>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void OpenGLRenderer::drawSkybox(Skybox* skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position)
{
	glDepthFunc(GL_EQUAL);

	shader.activate();
	shader.setProjectionViewModelMatrices(this->projectionMatrix_, view_matrix, skybox->getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);

	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getCubeMapID());

	for (size_t i = 0; i < skybox->getModels().size(); i++)
	{
		drawModel(skybox->getModels()[i], shader, skybox->getMaterial());
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDepthFunc(GL_LESS);
}

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
///<param name = 'width'>Ширина.</param>
///<param name = 'height'>Высота.</param>
void OpenGLRenderer::setViewport(const int width, const int height)
{
	glViewport(0, 0, width, height);
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