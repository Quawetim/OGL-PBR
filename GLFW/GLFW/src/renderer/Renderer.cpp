#include "Renderer.h"

///<summary>����������� ��-���������.</summary>
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

	this->initialWidth_ = this->windowWidth_;
	this->initialHeight_ = this->windowHeight_;

	this->updateAspectRatio();	
	
	this->uiScaleX_ = 1.0f;
	this->uiScaleY_ = 1.0f;
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
				if (s == "1")
					this->isFullScreen_ = true;
				else
					this->isFullScreen_ = false;

				continue;
			}

			if (s == "Vsync")
			{
				fin >> s;
				if (s == "1")
					this->isVSync_ = true;
				else
					this->isVSync_ = false;

				continue;
			}

			if (s == "WindowWidth")
			{
				fin >> this->windowWidth_;

				if (this->windowWidth_ <= 0) this->windowWidth_ = 800;

				continue;
			}

			if (s == "WindowHeight")
			{
				fin >> this->windowHeight_;

				if (this->windowHeight_ <= 0) this->windowHeight_ = 600;

				continue;
			}
		}
	}

	fin.close();
}

///<summary>��������� Aspect Ratio 
///<para>� ��������� projectionPerspective � orthoPerspective.</para>
///</summary>
void Renderer::updateAspectRatio()
{
	this->aspectRatio_ = static_cast<float>(this->windowWidth_) / static_cast<float>(this->windowHeight_);

	this->perspectiveProjection_ = glm::perspective(glm::radians(this->fov_), this->aspectRatio_, 0.05f, 500.0f);

	this->orthoProjection_ = glm::ortho(0.0f, static_cast<float>(this->windowWidth_), 0.0f, static_cast<float>(this->windowHeight_));

	this->uiScaleX_ = static_cast<float>(this->windowWidth_) / static_cast<float>(this->initialWidth_);
	this->uiScaleY_ = static_cast<float>(this->windowHeight_) / static_cast<float>(this->initialHeight_);
}

///<summary>����� ������ ����.</summary>
///<param name = 'width'>������.</param>
void Renderer::setWindowWidth(const int width)
{
	this->windowWidth_ = width;
	this->updateAspectRatio();
	this->updateFrameSize();
}

///<summary>����� ������ ����.</summary>
///<param name = 'height'>������.</param>
void Renderer::setWindowHeight(const int height)
{
	this->windowHeight_ = height;
	this->updateAspectRatio();
	this->updateFrameSize();
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

///<summary>���������� ����������� ������� ui �� ��� X.</summary>
float Renderer::getUiScaleX() const
{
	return this->uiScaleX_;
}

///<summary>���������� ����������� ������� ui �� ��� Y.</summary>
float Renderer::getUiScaleY() const
{
	return this->uiScaleY_;
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