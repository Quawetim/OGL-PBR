#include "Renderer.h"
#include "..\callbacks\Callbacks.h"

///<summary>�����������.</summary>
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

	// ��������� ��������
	const GLFWvidmode *vidMode = glfwGetVideoMode(screen);

	if (this->isFullScreen_)
	{
		this->windowWidth_ = vidMode->width;
		this->windowHeight_ = vidMode->height;

		// ������, ������, �������� ����, ������� (S�reen , NUll - �������), ����� ��������� � ����� (NULL - ��� ������)
		GLFWwindow* wnd = glfwCreateWindow(this->windowWidth_, this->windowHeight_, "Diploma", screen, NULL);
		this->window_.OGLwindow = wnd;
	}
	else
	{
		// ������, ������, �������� ����, ������� (S�reen , NUll - �������), ����� ��������� � ����� (NULL - ��� ������)
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

	// ���� � ����� ������
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

		// ������ ������
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

	// ������ ������, ��������� � ����� ������
	//this->windowInfo_.setShowCursor(false);
	glfwSetInputMode(this->window_.OGLwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(this->window_.OGLwindow, this->getWindowHalfWidth(), this->getWindowHalfHeight());

	glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, this->windowWidth_, this->windowHeight_);

	// ���� ����, RGBA
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);

	// �������� ����� �������
	glEnable(GL_DEPTH_TEST);

	// �������� ��������, ��������� � ������
	glDepthFunc(GL_LESS);

	// ��������� ������, � ������� �� ����� ������� �������
	glEnable(GL_CULL_FACE);
}

///<summary>����������.</summary>
OpenGLRenderer::~OpenGLRenderer()
{
	glfwTerminate();
}

///<summary>������� ������.</summary>
void OpenGLRenderer::clearScreen() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///<summary>�������� �������� � ������ ������� �������.</summary>
void OpenGLRenderer::swapBuffers() const
{
	glfwSwapBuffers(this->window_.OGLwindow);
}

///<summary>��������� �������.</summary>
void OpenGLRenderer::pollEvents() const
{
	glfwPollEvents();
}

///<summary>���� ������ �� �������� �����.</summary>
bool OpenGLRenderer::quit() const
{
	return glfwWindowShouldClose(this->window_.OGLwindow);
}

///<summary>���������� ��������� �� ����.</summary>
QWindow OpenGLRenderer::getWindow() const
{
	return this->window_;
}

///<summary>����� ��� ����.</summary>
///<param name = 'title'>���.</param>
void OpenGLRenderer::setWindowTitle(const std::string title)
{
	glfwSetWindowTitle(this->window_.OGLwindow, title.c_str());
}