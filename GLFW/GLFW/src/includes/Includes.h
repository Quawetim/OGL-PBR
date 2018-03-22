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

///<summary>Тип ошибки для Logger.</summary>
enum QErrorType { info, warning, error };

///<summary>Тип текстуры.</summary>
enum QTextureType { diffuse, specular, normal };

///<summary>Структура хранящая параметры вершин.</summary>
struct QVertexData
{
	///<summary>Позиция вершины.</summary>
    glm::vec3 position;

	///<summary>Вектор нормали.</summary>
    glm::vec3 normal;

	///<summary>Текстурные координаты.</summary>
    glm::vec2 textureCoords;

	///<summary>Касательная.</summary>
    glm::vec3 tangent;

	///<summary>Бикасательная.</summary>
    glm::vec3 bitangent;
};

///<summary>Структура, хранящая информацию о текстуре.</summary>
struct QTexture
{
	///<summary>Идентификатор.</summary>
    unsigned int id;

	///<summary>Тип: diffuse, specular, normal etc.</summary>
    QTextureType type;

    ///<summary>Путь к текстуре.</summary>
    std::string path;
};

///<summary>Класс, который хранит информацию об окне.</summary>
class QWindowInfo
{
private: 
	///<summary>Указатель на окно.</summary>
    GLFWwindow* window = nullptr;
 
	///<summary>Ширина окна.</summary>
    int width = 800;
 
	///<summary>Высота окна.</summary>
    int height = 600;

	///<summary>Полный экран.</summary>
    bool fullScreen = false;
  
	///<summary>Вертикальная синхронизация.</summary>
    bool vsync = false;
 
	///<summary>Отображение курсора.</summary>
    bool showCursor = true;

	///<summary>FPS.</summary>
    int fps = 0;

public:
    ///<summary>Задаёт указатель на окно.</summary>
    ///<param name = 'window'>Указатель.</param>
    void setWindowPointer(GLFWwindow* window);

    ///<summary>Задаёт ширину окна.</summary>
    ///<param name = 'width'>Ширина.</param>
    void setWidth(const int width);

    ///<summary>Задаёт высоту окна.</summary>
    ///<param name = 'height'>Высота.</param>
    void setHeight(const int height);

    ///<summary>Задаёт полноэкранный режим.</summary>
    ///<param name = 'fullScreen'>Полноэкранный режим.</param>
    void setFullScreen(const bool fullScreen);

    ///<summary>Включает/отключает вертикальную синхронизацию.</summary>
    ///<param name = 'vsync'>Вертикальная инхронизация.</param>
    void setVsync(const bool vsync);

    ///<summary>Задаёт отображение курсора.</summary>
    ///<param name = 'showCursor'>Отображать курсор.</param>
    void setShowCursor(const bool showCursor);

    ///<summary>Задаёт текущее число кадров в секунду.</summary>
    ///<param name = 'fps'>FPS.</param>
    void setFPS(const int fps);

    ///<summary>Возвращает указатель на окно.</summary>
    GLFWwindow* getWindowPointer() const;

    ///<summary>Возвращает ширину окна.</summary>
    int getWidth() const;

    ///<summary>Возвращает высоту окна.</summary>
    int getHeight() const;

    ///<summary>Возвращает полуширину окна.</summary>
    float getHalfWidth() const;

    ///<summary>Возвращает полувысоту окна.</summary>
    float getHalfHeight() const;

    ///<summary>Возвращает признак полноэкранности.</summary>
    bool getFullScreen() const;

    ///<summary>Возвращает включена вертикальная синхронизация или нет.</summary>
    bool getVsync() const;

    ///<summary>Возвращает признак отображаемости курсора.</summary>
    bool getShowCursor() const;

    ///<summary>Возвращает текущее число кадров в секунду.</summary>
    int getFPS() const;
};

///<summary>Структура, которая хранит информацию об окне.</summary>
extern QWindowInfo windowInfo;

///<summary>Задаёт соответствие QTextureType и string.</summary>
extern const std::map<QTextureType, std::string> mapTextureType;
 
///<summary>Field of view.</summary>
extern float FOV;

///<summary>Размер генерируемой карты отражений.</summary>
extern int reflectionMapResolution;
 
///<summary>Время, прошедшее между текущим кадром и предыдущим.</summary>
extern double deltaTime;
 
///<summary>Время, затраченное на отрисовку предыдущего кадра.</summary>
extern double lastFrameTime;