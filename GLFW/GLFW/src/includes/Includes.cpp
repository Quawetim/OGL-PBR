#include "Includes.h"

///<summary>������. �������� ������ �� ����� � � ���� "log.txt".</summary>
Logger logger;

///<summary>���������, ������� ������ ���������� �� ����.</summary>
QWindowInfo windowInfo;

///<summary>����� ������������ QTextureType � string.</summary>
const std::map<QTextureType, std::string> mapTextureType = 
{ 
    {QTextureType::diffuse, "diffuseMap"}, 
    {QTextureType::specular, "specularMap"}, 
    {QTextureType::normal, "normalMap"} 
};

///<summary>Field of view.</summary>
float FOV = 60.0f;

///<summary>������ ������������ ����� ���������.</summary>
int reflectionMapResolution = 128;

///<summary>�����, ��������� ����� ������� ������ � ����������.</summary>
double deltaTime = 0.0;

///<summary>�����, ����������� �� ��������� ����������� �����.</summary>
double lastFrameTime = 0.0;

////////////////////////////////////////////////////////////QWindowInfo////////////////////////////////////////////////////////////

///<summary>����� ��������� �� ����.</summary>
///<param name = 'window'>���������.</param>
void QWindowInfo::setWindowPointer(GLFWwindow* window)
{
    this->window = window;
}

///<summary>����� ������ ����.</summary>
///<param name = 'width'>������.</param>
void QWindowInfo::setWidth(const int width)
{
    this->width = width;
}

///<summary>����� ������ ����.</summary>
///<param name = 'height'>������.</param>
void QWindowInfo::setHeight(const int height)
{
    this->height = height;
}

///<summary>����� ������������� �����.</summary>
///<param name = 'fullScreen'>������������� �����.</param>
void QWindowInfo::setFullScreen(const bool fullScreen)
{
    this->fullScreen = fullScreen;
}

///<summary>��������/��������� ������������ �������������.</summary>
///<param name = 'vsync'>������������ ������������.</param>
void QWindowInfo::setVsync(const bool vsync)
{
    this->vsync = vsync;
}

///<summary>����� ����������� �������.</summary>
///<param name = 'showCursor'>���������� ������.</param>
void QWindowInfo::setShowCursor(const bool showCursor)
{
    this->showCursor = showCursor;
}

///<summary>����� ������� ����� ������ � �������.</summary>
///<param name = 'fps'>FPS.</param>
void QWindowInfo::setFPS(const int fps)
{
    this->fps = fps;
}

///<summary>���������� ��������� �� ����.</summary>
GLFWwindow* QWindowInfo::getWindowPointer() const
{
    return this->window;
}

///<summary>���������� ������ ����.</summary>
int QWindowInfo::getWidth() const
{
    return this->width;
}

///<summary>���������� ������ ����.</summary>
int QWindowInfo::getHeight() const
{
    return this->height;
}

///<summary>���������� ���������� ����.</summary>
float QWindowInfo::getHalfWidth() const
{
    return this->width / 2.0f;
}

///<summary>���������� ���������� ����.</summary>
float QWindowInfo::getHalfHeight() const
{
    return this->height / 2.0f;
}

///<summary>���������� ������� ���������������.</summary>
bool QWindowInfo::getFullScreen() const
{
    return this->fullScreen;
}

///<summary>���������� �������� ������������ ������������� ��� ���.</summary>
bool QWindowInfo::getVsync() const
{
    return this->vsync;
}

///<summary>���������� ������� �������������� �������.</summary>
bool QWindowInfo::getShowCursor() const
{
    return this->showCursor;
}

///<summary>���������� ������� ����� ������ � �������.</summary>
int QWindowInfo::getFPS() const
{
    return this->fps;
}

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
///<param name = 'path'>���� � ��������.</param>
///<param name = 'type'>��� ��������.</param>
QTexture::QTexture(std::string path, QTextureType type)
{
	this->path = path;
	this->type = type;
	this->id = TextureLoader::loadTexture(this->path);
}

///<summary>����� ��� ��������.</summary>
///<param name = 'type'>��� ��������.</param>
void QTexture::setType(const QTextureType type)
{
	this->type = type;
}

///<summary>����� ��� ��������.</summary>
///<param name = 'name'>��� ��������.</param>
void QTexture::setName(const std::string name)
{
	this->name = name;
}

///<summary>���������� ������������� ��������..</summary>
unsigned int QTexture::getID() const
{
	return this->id;
}

///<summary>���������� ��� ��������.</summary>
QTextureType QTexture::getType() const
{
	return this->type;
}

///<summary>���������� ��� ��������.</summary>
std::string QTexture::getName() const
{
	return this->name;
}

////////////////////////////////////////////////////////////QMaterial////////////////////////////////////////////////////////////

///<summary>����� � �������� ���������.</summary>
void QMaterial::reset()
{
	this->ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	this->specularColor = glm::vec3(0.7f, 0.7f, 0.7f);
	this->shinePower = 8.0f;
	
	this->textures.clear();
	std::vector<QTexture>(this->textures).swap(this->textures);	
}

///<summary>����� ambient ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void QMaterial::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� diffuse ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void QMaterial::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� specular ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void QMaterial::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� ���� (�������) �����.</summary>
///<param name = 'value'>��������.</param>
void QMaterial::setShinePower(const float value)
{
	this->shinePower = value;
}

///<summary>����� ��������� ��������.</summary>
///<param name = 'texture'>��������.</param>
void QMaterial::addTexture(QTexture texture)
{
	this->textures.push_back(texture);
}

///<summary>���������� ambient ���� � RGB �������.</summary>
glm::vec3 QMaterial::getAmbientColor() const
{
	return this->ambientColor;
}

///<summary>���������� diffuse ���� � RGB �������.</summary>
glm::vec3 QMaterial::getDiffuseColor() const
{
	return this->diffuseColor;
}

///<summary>���������� specular ���� � RGB �������.</summary>
glm::vec3 QMaterial::getSpecularColor() const
{
	return this->specularColor;
}

///<summary>���������� ���� (�������) �����.</summary>
float QMaterial::getShinePower() const
{
	return this->shinePower;
}

///<summary>���������� ������ �������.</summary>
std::vector<QTexture> QMaterial::getTextures() const
{
	return this->textures;
}

///<summary>��������� ������ ������� �� �������.</summary>
bool QMaterial::noTextures() const
{
	return this->textures.empty();
}