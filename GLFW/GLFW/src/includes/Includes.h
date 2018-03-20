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

// Тип ошибки для Logger
enum QErrorType { info, warning, error };

// Тип текстуры
enum QTextureType { diffuse, specular, normal };

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

// Класс, которая хранит информацию об окне
class QWindowInfo
{
private:
    // Указатель на окно
    GLFWwindow* window = nullptr;

    // Ширина окна
    int width = 800;

    // Высота окна
    int height = 600;

    // Полный экран
    bool fullScreen = false;

    // Dертикальная синхронизация 
    bool vsync = false;

    // Отображение курсора
    bool showCursor = true;

    // FPS
    int fps = 0;

public:
    ///<summary>Задаёт указатель на окно.<summary>
    ///<param name = 'window'>Указатель.</param>
    void setWindowPointer(GLFWwindow* window);

    ///<summary>Задаёт ширину окна.<summary>
    ///<param name = 'width'>Ширина.</param>
    void setWidth(const int width);

    ///<summary>Задаёт высоту окна.<summary>
    ///<param name = 'height'>Высота.</param>
    void setHeight(const int height);

    ///<summary>Задаёт полноэкранный режим.<summary>
    ///<param name = 'fullScreen'>Полноэкранный режим.</param>
    void setFullScreen(const bool fullScreen);

    ///<summary>Включает/отключает вертикальную синхронизацию.<summary>
    ///<param name = 'vsync'>Вертикальная инхронизация.</param>
    void setVsync(const bool vsync);

    ///<summary>Задаёт отображение курсора.<summary>
    ///<param name = 'showCursor'>Отображать курсор.</param>
    void setShowCursor(const bool showCursor);

    ///<summary>Задаёт текущее число кадров в секунду.<summary>
    ///<param name = 'fps'>FPS.</param>
    void setFPS(const int fps);

    ///<summary>Возвращает указатель на окно.<summary>
    GLFWwindow* getWindowPointer() const;

    ///<summary>Возвращает ширину окна.<summary>
    int getWidth() const;

    ///<summary>Возвращает высоту окна.<summary>
    int getHeight() const;

    ///<summary>Возвращает полуширину окна.<summary>
    float getHalfWidth() const;

    ///<summary>Возвращает полувысоту окна.<summary>
    float getHalfHeight() const;

    ///<summary>Возвращает признак полноэкранности.<summary>
    bool getFullScreen() const;

    ///<summary>Возвращает включена вертикальная синхронизация или нет.<summary>
    bool getVsync() const;

    ///<summary>Возвращает признак отображаемости курсора.<summary>
    bool getShowCursor() const;

    ///<summary>Возвращает текущее число кадров в секунду.<summary>
    int getFPS() const;
};

// Структура, которая хранит информацию об окне
extern QWindowInfo windowInfo;

// Задаёт соответствие QTextureType и string
extern const std::map<QTextureType, std::string> mapTextureType;

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