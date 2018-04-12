#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <vector>
#include <list>
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

class QInputHandle
{
private:
	static QInputHandle *inputHandle_;

protected:
	///<summary>Обработка позиции курсора.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'xpos'>Координата по оси x.</param>
	///<param name = 'ypos'>Координата по оси y.</param>
	virtual void handleCursorPosition(GLFWwindow* window, double xpos, double ypos) = 0;

	///<summary>Обработка колёсика мышки.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'xoffset'>Смещение по оси X.</param>
	///<param name = 'yoffset'>Смещение по оси Y.</param>
	virtual void handleScroll(GLFWwindow* window, double xoffset, double yoffset) = 0;

	///<summary>Обработка клавиатуры.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'key'>Клавиша.</param>
	///<param name = 'scancode'>Scancode.</param>
	///<param name = 'action'>Действие.</param>
	///<param name = 'mods'>Модификаторы.</param>
	virtual void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;

	///<summary>Обработка изменения размера окна.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'width'>Новая ширина.</param>
	///<param name = 'height'>Новая высота.</param>
	virtual void handleFramebufferSize(GLFWwindow* window, int width, int height) = 0;

public:
	///<summary>Включить обработку ввода для этого объекта.</summary>
	virtual void setEventHandling() { inputHandle_ = this; }

	static void cursorPosDispatch(GLFWwindow* window, double xpos, double ypos)
	{
		if (inputHandle_) inputHandle_->handleCursorPosition(window, xpos, ypos);
	}

	static void scrollDispatch(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (inputHandle_) inputHandle_->handleScroll(window, xoffset, yoffset);
	}

	static void keyboardDispatch(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (inputHandle_) inputHandle_->handleKeyboard(window, key, scancode, action, mods);
	}	

	static void framebufferSizeDispatch(GLFWwindow* window, int width, int height)
	{
		if (inputHandle_) inputHandle_->handleFramebufferSize(window, width, height);
	}
};

///<summary>Класс, хранящий информацию о текстуре.</summary>
class QTexture
{
private:
	///<summary>Идентификатор.</summary>
	unsigned int id_;

	///<summary>Тип: diffuse, specular, normal etc.</summary>
	QTextureType type_;

	///<summary>Путь к текстуре.</summary>
	std::string path_;

	///<summary>Имя текстуры.</summary>
	std::string name_;

public:
	///<summary>Конструктор.</summary>
	QTexture();

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
	glm::vec3 ambientColor_;

	///<summary>Diffuse цвет.</summary>
	glm::vec3 diffuseColor_;

	///<summary>Specular цвет.</summary>
	glm::vec3 specularColor_;

	///<summary>Текстуры.</summary>
	std::vector<QTexture> textures_;

	///<summary>Сила (яркость) блика.</summary>
	float shininess_;

public:
	///<summary>Конструктор.</summary>
	QMaterial();

	///<summary>Деструктор.</summary>
	~QMaterial();

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
	///<param name = 'shininess'>Значение.</param>
	void setShininess(const float shininess);

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
	float getShininess() const;

	///<summary>Возвращает список текстур.</summary>
	std::vector<QTexture> getTextures() const;

	///<summary>Проверяет список текстур на пустоту.</summary>
	bool isTexturesEmpty() const;
};

///<summary>Задаёт соответствие QTextureType и string.</summary>
extern const std::map<QTextureType, std::string> mapTextureType;
 
///<summary>Field of view.</summary>
extern float FOV;
 
///<summary>Время, прошедшее между текущим кадром и предыдущим.</summary>
extern float deltaTime;
 
///<summary>Время, затраченное на отрисовку предыдущего кадра.</summary>
extern float lastFrameTime;