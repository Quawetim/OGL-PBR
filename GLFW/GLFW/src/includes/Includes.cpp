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
int reflectionMapResolution = 128;

///<summary>Время, прошедшее между текущим кадром и предыдущим.</summary>
double deltaTime = 0.0;

///<summary>Время, затраченное на отрисовку предыдущего кадра.</summary>
double lastFrameTime = 0.0;

////////////////////////////////////////////////////////////QWindowInfo////////////////////////////////////////////////////////////

///<summary>Задаёт указатель на окно.</summary>
///<param name = 'window'>Указатель.</param>
void QWindowInfo::setWindowPointer(GLFWwindow* window)
{
    this->window = window;
}

///<summary>Задаёт ширину окна.</summary>
///<param name = 'width'>Ширина.</param>
void QWindowInfo::setWidth(const int width)
{
    this->width = width;
}

///<summary>Задаёт высоту окна.</summary>
///<param name = 'height'>Высота.</param>
void QWindowInfo::setHeight(const int height)
{
    this->height = height;
}

///<summary>Задаёт полноэкранный режим.</summary>
///<param name = 'fullScreen'>Полноэкранный режим.</param>
void QWindowInfo::setFullScreen(const bool fullScreen)
{
    this->fullScreen = fullScreen;
}

///<summary>Включает/отключает вертикальную синхронизацию.</summary>
///<param name = 'vsync'>Вертикальная инхронизация.</param>
void QWindowInfo::setVsync(const bool vsync)
{
    this->vsync = vsync;
}

///<summary>Задаёт отображение курсора.</summary>
///<param name = 'showCursor'>Отображать курсор.</param>
void QWindowInfo::setShowCursor(const bool showCursor)
{
    this->showCursor = showCursor;
}

///<summary>Задаёт текущее число кадров в секунду.</summary>
///<param name = 'fps'>FPS.</param>
void QWindowInfo::setFPS(const int fps)
{
    this->fps = fps;
}

///<summary>Возвращает указатель на окно.</summary>
GLFWwindow* QWindowInfo::getWindowPointer() const
{
    return this->window;
}

///<summary>Возвращает ширину окна.</summary>
int QWindowInfo::getWidth() const
{
    return this->width;
}

///<summary>Возвращает высоту окна.</summary>
int QWindowInfo::getHeight() const
{
    return this->height;
}

///<summary>Возвращает полуширину окна.</summary>
float QWindowInfo::getHalfWidth() const
{
    return this->width / 2.0f;
}

///<summary>Возвращает полувысоту окна.</summary>
float QWindowInfo::getHalfHeight() const
{
    return this->height / 2.0f;
}

///<summary>Возвращает признак полноэкранности.</summary>
bool QWindowInfo::getFullScreen() const
{
    return this->fullScreen;
}

///<summary>Возвращает включена вертикальная синхронизация или нет.</summary>
bool QWindowInfo::getVsync() const
{
    return this->vsync;
}

///<summary>Возвращает признак отображаемости курсора.</summary>
bool QWindowInfo::getShowCursor() const
{
    return this->showCursor;
}

///<summary>Возвращает текущее число кадров в секунду.</summary>
int QWindowInfo::getFPS() const
{
    return this->fps;
}

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>Конструктор.</summary>
///<param name = 'path'>Путь к текстуре.</param>
///<param name = 'type'>Тип текстуры.</param>
QTexture::QTexture(std::string path, QTextureType type)
{
	this->path = path;
	this->type = type;
	this->id = TextureLoader::loadTexture(this->path);
}

///<summary>Задаёт тип текстуры.</summary>
///<param name = 'type'>Тип текстуры.</param>
void QTexture::setType(const QTextureType type)
{
	this->type = type;
}

///<summary>Задаёт имя текстуры.</summary>
///<param name = 'name'>Имя текстуры.</param>
void QTexture::setName(const std::string name)
{
	this->name = name;
}

///<summary>Возвращает идентификатор текстуры..</summary>
unsigned int QTexture::getID() const
{
	return this->id;
}

///<summary>Возвращает тип текстуры.</summary>
QTextureType QTexture::getType() const
{
	return this->type;
}

///<summary>Возвращает имя текстуры.</summary>
std::string QTexture::getName() const
{
	return this->name;
}

////////////////////////////////////////////////////////////QMaterial////////////////////////////////////////////////////////////

///<summary>Сброс к дефолным значениям.</summary>
void QMaterial::reset()
{
	this->ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	this->specularColor = glm::vec3(0.7f, 0.7f, 0.7f);
	this->shinePower = 8.0f;
	
	this->textures.clear();
	std::vector<QTexture>(this->textures).swap(this->textures);	
}

///<summary>Задаёт ambient цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void QMaterial::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт diffuse цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void QMaterial::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт specular цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void QMaterial::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт силу (яркость) блика.</summary>
///<param name = 'value'>Значение.</param>
void QMaterial::setShinePower(const float value)
{
	this->shinePower = value;
}

///<summary>Задаёт диффузную текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void QMaterial::addTexture(QTexture texture)
{
	this->textures.push_back(texture);
}

///<summary>Возвращает ambient цвет в RGB формате.</summary>
glm::vec3 QMaterial::getAmbientColor() const
{
	return this->ambientColor;
}

///<summary>Возвращает diffuse цвет в RGB формате.</summary>
glm::vec3 QMaterial::getDiffuseColor() const
{
	return this->diffuseColor;
}

///<summary>Возвращает specular цвет в RGB формате.</summary>
glm::vec3 QMaterial::getSpecularColor() const
{
	return this->specularColor;
}

///<summary>Возвращает силу (яркость) блика.</summary>
float QMaterial::getShinePower() const
{
	return this->shinePower;
}

///<summary>Возвращает список текстур.</summary>
std::vector<QTexture> QMaterial::getTextures() const
{
	return this->textures;
}

///<summary>Проверяет список текстур на пустоту.</summary>
bool QMaterial::noTextures() const
{
	return this->textures.empty();
}