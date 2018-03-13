#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include <glad\glad.h>
#include <GL\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
//#include <SOIL\SOIL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "..\errorcodes\ErrorCodes.h"
#include "..\logger\Logger.h"

//using namespace std;
//using namespace glm;

// Структура, которая хранит информацию об окне
struct windowInfo
{
    // Указатель на окно
	GLFWwindow* Window;

    // Ширина окна
    int Width;

    // Высота окна
    int Height;

    // Полуширина окна
    float HalfWidth;

    // Полувысота окна
    float HalfHeight;

    // Полный экран
    bool FullScreen;
    
    // Dертикальная синхронизация 
    bool Vsync;

    // Отображение курсора
    bool ShowCursor;
};

// Структура, которая хранит информацию об окне
extern windowInfo WindowInfo;

// Тип ошибки для Logger
enum errorType { info, warning, error };

// CameraMode - выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная
extern int CameraMode;

// GenTextureSize - размер генерируемой текстуры
extern int GenTextureSize;

// FOV - field of view
extern float FOV;

// SkyBoxSize - размер скайбокса
extern float SkyBoxSize;

// Rotations - переключение вращений
extern bool Rotations;

// ShowLights - переключение отображения источников света
extern bool ShowLights;

// Blinn - переключение модели освещения
extern bool Blinn;

// MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map
extern bool MirrorExample;

// ShowHelp - переключение отображения справки
extern bool ShowHelp;