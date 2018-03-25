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

#include "../errorcodes/ErrorCodes.h"
#include "../logger/Logger.h"
#include "../shader/Shader.h"
#include "../texture_loader/TextureLoader.h"

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

///<summary>Класс, хранящий информацию о текстуре.</summary>
class QTexture
{
private:
	///<summary>Идентификатор.</summary>
	unsigned int id = 0;

	///<summary>Тип: diffuse, specular, normal etc.</summary>
	QTextureType type;

	///<summary>Путь к текстуре.</summary>
	std::string path;

	///<summary>Имя текстуры.</summary>
	std::string name;

public:
	///<summary>Конструктор.</summary>
	QTexture() {};

	///<summary>Конструктор.</summary>
	///<param name = 'path'>Путь к текстуре.</param>
	///<param name = 'type'>Тип текстуры.</param>
	QTexture(std::string path, QTextureType type);

	///<summary>Задаёт тип текстуры..</summary>
	///<param name = 'type'>Тип текстуры.</param>
	void setType(const QTextureType type);

	///<summary>Задаёт имя текстуры.</summary>
	///<param name = 'name'>Имя текстуры.</param>
	void setName(const std::string name);

	///<summary>Возвращает идентификатор текстуры.</summary>
	unsigned int getID() const;

	///<summary>Возвращает тип текстуры.</summary>
	QTextureType getType() const;

	///<summary>Возвращает имя текстуры.</summary>
	std::string getName() const;
};

///<summary>Класс, хранящий свойства материала.</summary>
class QMaterial
{
private:
	///<summary>Ambient цвет.</summary>
	glm::vec3 ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);

	///<summary>Diffuse цвет.</summary>
	glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);

	///<summary>Specular цвет.</summary>
	glm::vec3 specularColor = glm::vec3(0.7f, 0.7f, 0.7f);

	///<summary>Текстуры.</summary>
	std::vector<QTexture> textures;

	///<summary>Сила (яркость) блика.</summary>
	float shinePower = 8.0f;

public:
	///<summary>Сброс к дефолным значениям.</summary>
	void reset();

	///<summary>Задаёт ambient цвет в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>Задаёт diffuse цвет в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>Задаёт specular цвет в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>Задаёт силу (яркость) блика.</summary>
	///<param name = 'value'>Значение.</param>
	void setShinePower(const float value);

	///<summary>Задаёт текстуру.</summary>
	///<param name = 'texture'>Текстура.</param>
	void addTexture(QTexture texture);

	///<summary>Возвращает ambient цвет в RGB формате.</summary>
	glm::vec3 getAmbientColor() const;

	///<summary>Возвращает diffuse цвет в RGB формате.</summary>
	glm::vec3 getDiffuseColor() const;

	///<summary>Возвращает specular цвет в RGB формате.</summary>
	glm::vec3 getSpecularColor() const;

	///<summary>Возвращает силу (яркость) блика.</summary>
	float getShinePower() const;

	///<summary>Возвращает список текстур.</summary>
	std::vector<QTexture> getTextures() const;

	///<summary>Проверяет список текстур на пустоту.</summary>
	bool noTextures() const;
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