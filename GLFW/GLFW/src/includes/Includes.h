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

#include <GLAD\glad.h>

#include <GLFW\glfw3.h>

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "..\errorcodes\ErrorCodes.h"
#include "..\logger\Logger.h"

#include "..\shader\Shader.h"

// Структура хранящая параметры вершин
struct QVertexData
{
    // Позиция вершины
    glm::vec3 position;

    // Вектор нормали
    glm::vec3 normal;

    // Текстурные координаты
    glm::vec2 textureCoords;

    // Касательная
    glm::vec3 tangent;

    // Бикасательная
    glm::vec3 bitangent;
};

// Тип текстуры
enum QTextureType { diffuse, specular, normal };

// Структура, хранящая информацию о текстуре
struct QTexture
{
    // Идентификатор
    unsigned int id;

    // Тип: diffuse, specular, normal etc.
    QTextureType type;

    // Путь к текстуре
    std::string path;
};

// Структура, которая хранит информацию об окне
struct QWindowInfo
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
extern QWindowInfo windowInfo;

// Тип ошибки для Logger
enum QErrorType { info, warning, error };

// Выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная
extern int CameraMode;

// Размер генерируемой текстуры
extern int GenTextureSize;

// Field of view
extern float FOV;

// Размер скайбокса
extern float SkyBoxSize;

// Переключение вращений
extern bool Rotations;

// Переключение отображения источников света
extern bool ShowLights;

// Переключение модели освещения
extern bool Blinn;

// Mtrue = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map
extern bool MirrorExample;

// Переключение отображения справки
extern bool ShowHelp;

// Время, прошедшее между текущим кадром и предыдущим
extern double deltaTime;

// Время, затраченное на отрисовку предыдущего кадра
extern double lastFrameTime;