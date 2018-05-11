#include "Renderer.h"

///<summary>�����������.</summary>
Renderer::Renderer()
{
	this->window_.OGLwindow = nullptr;

	this->windowWidth_ = 800;
	this->windowHeight_ = 600;

	this->isFullScreen_ = false;
	this->isVSync_ = false;
	this->isShowCursor_ = true;

	this->fov_ = 60.0f;

	readConfig();

	this->aspectRatio_ = static_cast<float>(this->windowWidth_) / static_cast<float>(this->windowHeight_);

	this->perspectiveProjection_ = glm::perspective(glm::radians(this->fov_), this->aspectRatio_, 0.05f, 500.0f);

	if (this->windowWidth_ >= this->windowHeight_) this->orthoProjection_ = glm::ortho(-1.0f * this->aspectRatio_, 1.0f * this->aspectRatio_, -1.0f, 1.0f, -1.0f, 1.0f);
	else this->orthoProjection_ = glm::ortho(-1.0f, 1.0f, -1.0f / this->aspectRatio_, 1.0f / this->aspectRatio_, -1.0f, 1.0f);

	this->environmentMap_ = 0;
	this->irradianceMap_ = 0;
	this->prefilteredMap_ = 0;
	this->brdfLutMap_ = 0;
}

///<summary>����������.</summary>
Renderer::~Renderer()
{
	
}

///<summary>������ ��������� �� ����� config.</summary>
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

///<summary>���������� ��� ���������.</summary>
bool Renderer::isOgl() const
{
	return this->ogl_;
}

///<summary>����� ������� �������� FOV.</summary>
///<param name = 'fov'>FOV.</param>
void Renderer::setFOV(const int fov)
{
	this->fov_ = static_cast<float>(fov);
	this->perspectiveProjection_ = glm::perspective(glm::radians(this->fov_), static_cast<float>(this->windowWidth_) / static_cast<float>(this->windowHeight_), 0.05f, 500.0f);
}

///<summary>���������� ��������� �� ����.</summary>
QWindow Renderer::getWindow() const
{
	return this->window_;
}

///<summary>���������� ������ ����.</summary>
int Renderer::getWindowWidth() const
{
	return this->windowWidth_;
}

///<summary>���������� ������ ����.</summary>
int Renderer::getWindowHeight() const
{
	return this->windowHeight_;
}

///<summary>���������� ���������� ����.</summary>
float Renderer::getWindowHalfWidth() const
{
	return this->windowWidth_ / 2.0f;
}

///<summary>���������� ���������� ����.</summary>
float Renderer::getWindowHalfHeight() const
{
	return this->windowHeight_ / 2.0f;
}

///<summary>���������� ������� ���������������.</summary>
bool Renderer::isFullScreen() const
{
	return this->isFullScreen_;
}

///<summary>���������� �������� ������������ ������������� ��� ���.</summary>
bool Renderer::isVsyncEnabled() const
{
	return this->isVSync_;
}

///<summary>���������� ������� �������������� �������.</summary>
bool Renderer::isShowCursor() const
{
	return this->isShowCursor_;
}

///<summary>���������� ������� �������� FOV.</summary>
int Renderer::getFOV() const
{
	return static_cast<int>(this->fov_);
}

///<summary>���������� ������������� ������� ��������.</summary>
glm::mat4 Renderer::getPerspectiveProjectionMatrix() const
{
	return this->perspectiveProjection_;
}

///<summary>���������� ��������������� ������� ��������.</summary>
glm::mat4 Renderer::getOrthoProjectionMatrix() const
{
	return this->orthoProjection_;
}

///<summary>���������� ������������� irradiance map.</summary>
unsigned int Renderer::getIrradianceMap() const
{
	return this->irradianceMap_;
}