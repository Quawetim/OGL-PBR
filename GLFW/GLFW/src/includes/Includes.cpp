#include "Includes.h"

// Структура, которая хранит информацию об окне
windowInfo WindowInfo = windowInfo();

// CameraMode - выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная
int CameraMode = 2;

// GenTextureSize - размер генерируемой текстуры
int GenTextureSize = 128;

// FOV - field of view
float FOV = 90.0f;

// SkyBoxSize - размер скайбокса
float SkyBoxSize = 500.0f;

// Rotations - переключение вращений
bool Rotations = false;

// ShowLights - переключение отображения источников света
bool ShowLights = false;

// Blinn - переключение модели освещения
bool Blinn = false;

// MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map
bool MirrorExample = false;

// ShowHelp - переключение отображения справки
bool ShowHelp = false;