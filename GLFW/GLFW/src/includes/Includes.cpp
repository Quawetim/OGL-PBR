#include "Includes.h"

// ������. �������� ������ �� ����� � � ���� log.txt
Logger logger;

// ���������, ������� ������ ���������� �� ����
QWindowInfo windowInfo;

const std::map<QTextureType, std::string> mapTextureType = 
{ 
    {QTextureType::diffuse, "diffuseMap"}, 
    {QTextureType::specular, "specularMap"}, 
    {QTextureType::normal, "normalMap"} 
};

// ��������� ������: 1 - �� ������� ����, 2 - �� �������� ����, 3 - �������������
int CameraMode = 2;

// ������ ������������ ��������
int GenTextureSize = 128;

// Field of view
float FOV = 90.0f;

// ������ ���������
float SkyBoxSize = 500.0f;

// ������������ ��������
bool Rotations = false;

// ������������ ����������� ���������� �����
bool ShowLights = false;

// ������������ ������ ���������
bool Blinn = false;

// true = ������ ����������� ������ � Reflection Map, false = ��� ������� ��� Reflection Map
bool MirrorExample = false;

// ������������ ����������� �������
bool ShowHelp = false;

// �����, ��������� ����� ������� ������ � ����������
double deltaTime = 0.0;

// �����, ����������� �� ��������� ����������� �����
double lastFrameTime = 0.0;

///<summary>����� ��������� �� ����.<summary>
///<param name = 'window'>���������.</param>
void QWindowInfo::setWindowPointer(GLFWwindow* window)
{
    this->window = window;
}

///<summary>����� ������ ����.<summary>
///<param name = 'width'>������.</param>
void QWindowInfo::setWidth(const int width)
{
    this->width = width;
}

///<summary>����� ������ ����.<summary>
///<param name = 'height'>������.</param>
void QWindowInfo::setHeight(const int height)
{
    this->height = height;
}

///<summary>����� ������������� �����.<summary>
///<param name = 'fullScreen'>������������� �����.</param>
void QWindowInfo::setFullScreen(const bool fullScreen)
{
    this->fullScreen = fullScreen;
}

///<summary>��������/��������� ������������ �������������.<summary>
///<param name = 'vsync'>������������ ������������.</param>
void QWindowInfo::setVsync(const bool vsync)
{
    this->vsync = vsync;
}

///<summary>����� ����������� �������.<summary>
///<param name = 'showCursor'>���������� ������.</param>
void QWindowInfo::setShowCursor(const bool showCursor)
{
    this->showCursor = showCursor;
}

///<summary>����� ������� ����� ������ � �������.<summary>
///<param name = 'fps'>FPS.</param>
void QWindowInfo::setFPS(const int fps)
{
    this->fps = fps;
}

///<summary>���������� ��������� �� ����.<summary>
GLFWwindow* QWindowInfo::getWindowPointer() const
{
    return this->window;
}

///<summary>���������� ������ ����.<summary>
int QWindowInfo::getWidth() const
{
    return this->width;
}

///<summary>���������� ������ ����.<summary>
int QWindowInfo::getHeight() const
{
    return this->height;
}

///<summary>���������� ���������� ����.<summary>
float QWindowInfo::getHalfWidth() const
{
    return this->width / 2.0f;
}

///<summary>���������� ���������� ����.<summary>
float QWindowInfo::getHalfHeight() const
{
    return this->height / 2.0f;
}

///<summary>���������� ������� ���������������.<summary>
bool QWindowInfo::getFullScreen() const
{
    return this->fullScreen;
}

///<summary>���������� �������� ������������ ������������� ��� ���.<summary>
bool QWindowInfo::getVsync() const
{
    return this->vsync;
}

///<summary>���������� ������� �������������� �������.<summary>
bool QWindowInfo::getShowCursor() const
{
    return this->showCursor;
}

///<summary>���������� ������� ����� ������ � �������.<summary>
int QWindowInfo::getFPS() const
{
    return this->fps;
}