#include "Renderer.h"

///<summary>Конструктор.</summary>
Renderer::Renderer()
{
	this->window_.OGLwindow = nullptr;

	this->windowWidth_ = 800;
	this->windowHeight_ = 600;

	this->isFullScreen_ = false;
	this->isVSync_ = false;
	this->isShowCursor_ = true;

	this->fps_ = 0;
	this->fov_ = 60.0f;

	readConfig();

	this->projectionMatrix_ = glm::perspective(glm::radians(this->fov_), static_cast<float>(this->windowWidth_) / static_cast<float>(this->windowHeight_), 0.05f, 500.0f);	
}

///<summary>Деструктор.</summary>
Renderer::~Renderer()
{
	
}

///<summary>Читает параметры из файла config.</summary>
void Renderer::readConfig()
{
	std::ifstream fin;
	fin.open("config\\config.ini");

	if (!fin)
	{
		logger.log(__FUNCTION__, ErrorType::warning, "Config file not found.");
	}
	else
	{
		std::string s;

		while (!fin.eof())
		{
			fin >> s;

			if (s == "Fullscreen")
			{
				fin >> s;
				if (s == "=")
				{
					fin >> s;
					if (s == "true")
						this->isFullScreen_ = true;
					else
						this->isFullScreen_ = false;

					continue;
				}
			}

			if (s == "Vsync")
			{
				fin >> s;
				if (s == "=")
				{
					fin >> s;
					if (s == "true")
						this->isVSync_ = true;
					else
						this->isVSync_ = false;

					continue;
				}
			}

			if (s == "WindowWidth")
			{
				fin >> s;
				if (s == "=")
				{
					fin >> this->windowWidth_;

					continue;
				}
			}

			if (s == "WindowHeight")
			{
				fin >> s;
				if (s == "=")
				{
					fin >> this->windowHeight_;

					continue;
				}
			}

			if (s == "ReflectionResolution")
			{
				fin >> s;
				if (s == "=")
				{
					fin >> this->reflectionsResolution_;

					continue;
				}
			}
		}
	}

	fin.close();
}

///<summary>Возвращает тип рендерера.</summary>
bool Renderer::isOgl() const
{
	return this->ogl_;
}

///<summary>Задаёт ширину окна.</summary>
///<param name = 'width'>Ширина.</param>
void Renderer::setWindowWidth(const int width)
{
	this->windowWidth_ = width;
}

///<summary>Задаёт высоту окна.</summary>
///<param name = 'height'>Высота.</param>
void Renderer::setWindowHeight(const int height)
{
	this->windowHeight_ = height;
}

///<summary>Задаёт полноэкранный режим.</summary>
///<param name = 'fullScreen'>Полноэкранный режим.</param>
void Renderer::setFullScreen(const bool fullScreen)
{
	this->isFullScreen_ = fullScreen;
}

///<summary>Включает/отключает вертикальную синхронизацию.</summary>
///<param name = 'vsync'>Вертикальная инхронизация.</param>
void Renderer::setVsync(const bool vsync)
{
	this->isVSync_ = vsync;
}

///<summary>Задаёт отображение курсора.</summary>
///<param name = 'showCursor'>Отображать курсор.</param>
void Renderer::setShowCursor(const bool showCursor)
{
	this->isShowCursor_ = showCursor;
}

///<summary>Задаёт текущее число кадров в секунду.</summary>
///<param name = 'fps'>FPS.</param>
void Renderer::setFPS(const int fps)
{
	this->fps_ = fps;
}

///<summary>Задаёт текущее значение FOV.</summary>
///<param name = 'fov'>FOV.</param>
void Renderer::setFOV(const int fov)
{
	this->fov_ = static_cast<float>(fov);
	this->projectionMatrix_ = glm::perspective(glm::radians(this->fov_), static_cast<float>(this->windowWidth_) / static_cast<float>(this->windowHeight_), 0.05f, 500.0f);
}

///<summary>Возвращает указатель на окно.</summary>
QWindow Renderer::getWindow() const
{
	return this->window_;
}

///<summary>Возвращает ширину окна.</summary>
int Renderer::getWindowWidth() const
{
	return this->windowWidth_;
}

///<summary>Возвращает высоту окна.</summary>
int Renderer::getWindowHeight() const
{
	return this->windowHeight_;
}

///<summary>Возвращает полуширину окна.</summary>
float Renderer::getWindowHalfWidth() const
{
	return this->windowWidth_ / 2.0f;
}

///<summary>Возвращает полувысоту окна.</summary>
float Renderer::getWindowHalfHeight() const
{
	return this->windowHeight_ / 2.0f;
}

///<summary>Возвращает признак полноэкранности.</summary>
bool Renderer::isFullScreen() const
{
	return this->isFullScreen_;
}

///<summary>Возвращает включена вертикальная синхронизация или нет.</summary>
bool Renderer::isVsyncEnabled() const
{
	return this->isVSync_;
}

///<summary>Возвращает признак отображаемости курсора.</summary>
bool Renderer::isShowCursor() const
{
	return this->isShowCursor_;
}

///<summary>Возвращает текущее число кадров в секунду.</summary>
int Renderer::getFPS() const
{
	return this->fps_;
}

///<summary>Возвращает текущее значение FOV.</summary>
int Renderer::getFOV() const
{
	return static_cast<int>(this->fov_);
}

///<summary>Возвращает матрицу проекции.</summary>
glm::mat4 Renderer::getProjectionMatrix() const
{
	return this->projectionMatrix_;
}