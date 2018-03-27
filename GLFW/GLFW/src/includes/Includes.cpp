#include "Includes.h"

///<summary>Логгер. Логирует ошибки на экран и в файл "log.txt".</summary>
Logger logger;

///<summary>Структура, которая хранит информацию об окне.</summary>
QWindowInfo windowInfo;

///<summary>Задаёт соответствие QTextureType и string.</summary>
const std::map<QTextureType, std::string> mapTextureType = 
{ 
    {QTextureType::diffuse, "diffuseMap"}, 
    {QTextureType::specular, "specularMap"}, 
    {QTextureType::normal, "normalMap"} 
};

///<summary>Field of view.</summary>
float FOV = 60.0f;

///<summary>Размер генерируемой карты отражений.</summary>
int reflectionsResolution = 128;

///<summary>Время, прошедшее между текущим кадром и предыдущим.</summary>
float deltaTime = 0.0;

///<summary>Время, затраченное на отрисовку предыдущего кадра.</summary>
float lastFrameTime = 0.0;

////////////////////////////////////////////////////////////QWindowInfo////////////////////////////////////////////////////////////

///<summary>Конструктор.</summary>
QWindowInfo::QWindowInfo()
{
	this->window_ = nullptr;

	this->width_ = 800;
	this->height_ = 600;

	this->fullScreen_ = false;

	this->vsync_ = false;

	this->showCursor_ = true;

	this->fps_ = 0;
}

///<summary>Задаёт указатель на окно.</summary>
///<param name = 'window'>Указатель.</param>
void QWindowInfo::setWindowPointer(GLFWwindow* window)
{
    this->window_ = window;
}

///<summary>Задаёт ширину окна.</summary>
///<param name = 'width'>Ширина.</param>
void QWindowInfo::setWidth(const int width)
{
    this->width_ = width;
}

///<summary>Задаёт высоту окна.</summary>
///<param name = 'height'>Высота.</param>
void QWindowInfo::setHeight(const int height)
{
    this->height_ = height;
}

///<summary>Задаёт полноэкранный режим.</summary>
///<param name = 'fullScreen'>Полноэкранный режим.</param>
void QWindowInfo::setFullScreen(const bool fullScreen)
{
    this->fullScreen_ = fullScreen;
}

///<summary>Включает/отключает вертикальную синхронизацию.</summary>
///<param name = 'vsync'>Вертикальная инхронизация.</param>
void QWindowInfo::setVsync(const bool vsync)
{
    this->vsync_ = vsync;
}

///<summary>Задаёт отображение курсора.</summary>
///<param name = 'showCursor'>Отображать курсор.</param>
void QWindowInfo::setShowCursor(const bool showCursor)
{
    this->showCursor_ = showCursor;
}

///<summary>Задаёт текущее число кадров в секунду.</summary>
///<param name = 'fps'>FPS.</param>
void QWindowInfo::setFPS(const int fps)
{
    this->fps_ = fps;
}

///<summary>Возвращает указатель на окно.</summary>
GLFWwindow* QWindowInfo::getWindowPointer() const
{
    return this->window_;
}

///<summary>Возвращает ширину окна.</summary>
int QWindowInfo::getWidth() const
{
    return this->width_;
}

///<summary>Возвращает высоту окна.</summary>
int QWindowInfo::getHeight() const
{
    return this->height_;
}

///<summary>Возвращает полуширину окна.</summary>
float QWindowInfo::getHalfWidth() const
{
    return this->width_ / 2.0f;
}

///<summary>Возвращает полувысоту окна.</summary>
float QWindowInfo::getHalfHeight() const
{
    return this->height_ / 2.0f;
}

///<summary>Возвращает признак полноэкранности.</summary>
bool QWindowInfo::isFullScreen() const
{
    return this->fullScreen_;
}

///<summary>Возвращает включена вертикальная синхронизация или нет.</summary>
bool QWindowInfo::isVsyncEnabled() const
{
    return this->vsync_;
}

///<summary>Возвращает признак отображаемости курсора.</summary>
bool QWindowInfo::isShowCursor() const
{
    return this->showCursor_;
}

///<summary>Возвращает текущее число кадров в секунду.</summary>
int QWindowInfo::getFPS() const
{
    return this->fps_;
}

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>Конструктор.</summary>
QTexture::QTexture()
{
	this->id_ = 0;
	this->path_ = "resources/textures/test.png";
	this->type_ = QTextureType::diffuse;
}

///<summary>Конструктор.</summary>
///<param name = 'path'>Путь к текстуре.</param>
///<param name = 'type'>Тип текстуры.</param>
QTexture::QTexture(std::string path, QTextureType type)
{
	this->path_ = path;
	this->type_ = type;
	this->id_ = textureLoader::loadTexture(this->path_);
}

///<summary>Задаёт тип текстуры.</summary>
///<param name = 'type'>Тип текстуры.</param>
void QTexture::setType(const QTextureType type)
{
	this->type_ = type;
}

///<summary>Задаёт имя текстуры.</summary>
///<param name = 'name'>Имя текстуры.</param>
void QTexture::setName(const std::string name)
{
	this->name_ = name;
}

///<summary>Возвращает идентификатор текстуры..</summary>
unsigned int QTexture::getID() const
{
	return this->id_;
}

///<summary>Возвращает тип текстуры.</summary>
QTextureType QTexture::getType() const
{
	return this->type_;
}

///<summary>Возвращает имя текстуры.</summary>
std::string QTexture::getName() const
{
	return this->name_;
}

////////////////////////////////////////////////////////////QMaterial////////////////////////////////////////////////////////////

///<summary>Конструктор.</summary>
QMaterial::QMaterial()
{
	this->ambientColor_ = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor_ = glm::vec3(0.5f, 0.5f, 0.5f);
	this->specularColor_ = glm::vec3(0.7f, 0.7f, 0.7f);

	this->shininess_ = 8.0f;
}

///<summary>Сброс к дефолным значениям.</summary>
void QMaterial::reset()
{
	this->ambientColor_ = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor_ = glm::vec3(0.5f, 0.5f, 0.5f);
	this->specularColor_ = glm::vec3(0.7f, 0.7f, 0.7f);
	this->shininess_ = 8.0f;
	
	this->textures_.clear();
	std::vector<QTexture>(this->textures_).swap(this->textures_);	
}

///<summary>Задаёт ambient цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void QMaterial::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт diffuse цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void QMaterial::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт specular цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void QMaterial::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт силу (яркость) блика.</summary>
///<param name = 'shininess'>Значение.</param>
void QMaterial::setShininess(const float shininess)
{
	this->shininess_ = shininess;
}

///<summary>Задаёт диффузную текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void QMaterial::addTexture(QTexture texture)
{
	this->textures_.push_back(texture);
}

///<summary>Возвращает ambient цвет в RGB формате.</summary>
glm::vec3 QMaterial::getAmbientColor() const
{
	return this->ambientColor_;
}

///<summary>Возвращает diffuse цвет в RGB формате.</summary>
glm::vec3 QMaterial::getDiffuseColor() const
{
	return this->diffuseColor_;
}

///<summary>Возвращает specular цвет в RGB формате.</summary>
glm::vec3 QMaterial::getSpecularColor() const
{
	return this->specularColor_;
}

///<summary>Возвращает силу (яркость) блика.</summary>
float QMaterial::getShininess() const
{
	return this->shininess_;
}

///<summary>Возвращает список текстур.</summary>
std::vector<QTexture> QMaterial::getTextures() const
{
	return this->textures_;
}

///<summary>Проверяет список текстур на пустоту.</summary>
bool QMaterial::isTexturesEmpty() const
{
	return this->textures_.empty();
}