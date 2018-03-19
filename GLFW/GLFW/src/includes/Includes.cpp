#include "Includes.h"

// Структура, которая хранит информацию об окне
QWindowInfo windowInfo = QWindowInfo();

const std::map<QTextureType, std::string> mapTextureType = 
{ 
    {QTextureType::diffuse, "diffuseMap"}, 
    {QTextureType::specular, "specularMap"}, 
    {QTextureType::normal, "normalMap"} 
};

// Выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная
int CameraMode = 2;

// Размер генерируемой текстуры
int GenTextureSize = 128;

// Field of view
float FOV = 90.0f;

// Размер скайбокса
float SkyBoxSize = 500.0f;

// Переключение вращений
bool Rotations = false;

// Переключение отображения источников света
bool ShowLights = false;

// Переключение модели освещения
bool Blinn = false;

// true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map
bool MirrorExample = false;

// Переключение отображения справки
bool ShowHelp = false;

// Время, прошедшее между текущим кадром и предыдущим
double deltaTime = 0.0;

// Время, затраченное на отрисовку предыдущего кадра
double lastFrameTime = 0.0;