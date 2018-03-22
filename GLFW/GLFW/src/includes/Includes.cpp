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
float FOV = 90.0f;

///<summary>Размер генерируемой карты отражений.</summary>
int reflectionMapResolution = 128;

///<summary>Время, прошедшее между текущим кадром и предыдущим.</summary>
double deltaTime = 0.0;

///<summary>Время, затраченное на отрисовку предыдущего кадра.</summary>
double lastFrameTime = 0.0;

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