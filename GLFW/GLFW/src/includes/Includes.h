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
#include <memory>

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
enum ErrorType { info, warning, error };

///<summary>Тип текстуры.</summary>
enum TextureType { diffuse, specular, normal };

///<summary>Структура хранящая параметры вершин.</summary>
struct VertexData
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

class InputHandle
{
private:
	static InputHandle *inputHandle_;

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
class Texture
{
private:
	///<summary>Идентификатор.</summary>
	unsigned int id_;

	///<summary>Тип: diffuse, specular, normal etc.</summary>
	TextureType type_;

	///<summary>Путь к текстуре.</summary>
	std::string path_;

	///<summary>Имя текстуры.</summary>
	std::string name_;

public:
	///<summary>Конструктор.</summary>
	Texture();

	///<summary>Конструктор.</summary>
	///<param name = 'path'>Путь к текстуре.</param>
	///<param name = 'type'>Тип текстуры.</param>
	Texture(std::string path, TextureType type);

	///<summary>Задаёт тип текстуры..</summary>
	///<param name = 'type'>Тип текстуры.</param>
	void setType(const TextureType type);

	///<summary>Задаёт имя текстуры.</summary>
	///<param name = 'name'>Имя текстуры.</param>
	void setName(const std::string name);

	///<summary>Возвращает идентификатор текстуры.</summary>
	unsigned int getID() const;

	///<summary>Возвращает тип текстуры.</summary>
	TextureType getType() const;

	///<summary>Возвращает имя текстуры.</summary>
	std::string getName() const;
};

///<summary>Класс, хранящий свойства материала.</summary>
class Material
{
private:
	///<summary>Albedo.</summary>
	glm::vec3 albedo_;

	///<summary>Текстуры.</summary>
	std::vector<Texture> textures_;

	///<summary>Металличность поверхности.</summary>
	float metallic_;

	///<summary>Гладкость поверхности.</summary>
	float smoothness_;

public:
	///<summary>Конструктор.</summary>
	Material();

	///<summary>Деструктор.</summary>
	~Material();

	///<summary>Сброс к дефолным значениям.</summary>
	void setDefault();

	///<summary>Задаёт альбедо в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setAlbedo(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>Задаёт альбедо через вектор.</summary>
	///<param name = 'color'>Цвет.</param>
	void setAlbedo(const glm::vec3 color);

	///<summary>Задаёт металличность поверхности.</summary>
	///<param name = 'metallic'>Металличность.</param>
	void setMetallic(const float metallic);

	///<summary>Задаёт гладкость поверхности.</summary>
	///<param name = 'smoothness'>Гладкость.</param>
	void setSmoothness(const float smoothness);

	///<summary>Задаёт текстуру.</summary>
	///<param name = 'texture'>Текстура.</param>
	void addTexture(Texture texture);

	///<summary>Возвращает альбедо в RGB формате.</summary>
	glm::vec3 getAlbedo() const;

	///<summary>Возвращает металличность поверхности.</summary>
	float getMetallic() const;

	///<summary>Возвращает гладкость поверхности.</summary>
	float getSmoothness() const;

	///<summary>Возвращает список текстур.</summary>
	std::vector<Texture> getTextures() const;

	///<summary>Проверяет список текстур на пустоту.</summary>
	bool isTexturesEmpty() const;
};

struct TextureKeys
{
	std::string mapsName;
	std::string mapsUse;
	std::string mapsCount;
};

///<summary>Задаёт соответствие TextureType и её TextureKeys в шейдере.</summary>
extern const std::map<TextureType, TextureKeys> mapTextureType;

///<summary>Значение гаммы для гамма-коррекции.</summary>
extern float gamma;