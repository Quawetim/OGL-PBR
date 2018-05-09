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

///<summary>��� ������ ��� Logger.</summary>
enum ErrorType { info, warning, error };

///<summary>��� ��������.</summary>
enum TextureType { diffuse, specular, normal };

///<summary>��������� �������� ��������� ������.</summary>
struct VertexData
{
	///<summary>������� �������.</summary>
    glm::vec3 position;

	///<summary>������ �������.</summary>
    glm::vec3 normal;

	///<summary>���������� ����������.</summary>
    glm::vec2 textureCoords;

	///<summary>�����������.</summary>
    glm::vec3 tangent;

	///<summary>�������������.</summary>
    glm::vec3 bitangent;
};

class InputHandle
{
private:
	static InputHandle *inputHandle_;

protected:
	///<summary>��������� ������� �������.</summary>
	///<param name = 'window'>��������� �� ����.</param>
	///<param name = 'xpos'>���������� �� ��� x.</param>
	///<param name = 'ypos'>���������� �� ��� y.</param>
	virtual void handleCursorPosition(GLFWwindow* window, double xpos, double ypos) = 0;

	///<summary>��������� ������� �����.</summary>
	///<param name = 'window'>��������� �� ����.</param>
	///<param name = 'xoffset'>�������� �� ��� X.</param>
	///<param name = 'yoffset'>�������� �� ��� Y.</param>
	virtual void handleScroll(GLFWwindow* window, double xoffset, double yoffset) = 0;

	///<summary>��������� ����������.</summary>
	///<param name = 'window'>��������� �� ����.</param>
	///<param name = 'key'>�������.</param>
	///<param name = 'scancode'>Scancode.</param>
	///<param name = 'action'>��������.</param>
	///<param name = 'mods'>������������.</param>
	virtual void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;

	///<summary>��������� ��������� ������� ����.</summary>
	///<param name = 'window'>��������� �� ����.</param>
	///<param name = 'width'>����� ������.</param>
	///<param name = 'height'>����� ������.</param>
	virtual void handleFramebufferSize(GLFWwindow* window, int width, int height) = 0;

public:
	///<summary>�������� ��������� ����� ��� ����� �������.</summary>
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

///<summary>�����, �������� ���������� � ��������.</summary>
class Texture
{
private:
	///<summary>�������������.</summary>
	unsigned int id_;

	///<summary>���: diffuse, specular, normal etc.</summary>
	TextureType type_;

	///<summary>���� � ��������.</summary>
	std::string path_;

	///<summary>��� ��������.</summary>
	std::string name_;

public:
	///<summary>�����������.</summary>
	Texture();

	///<summary>�����������.</summary>
	///<param name = 'path'>���� � ��������.</param>
	///<param name = 'type'>��� ��������.</param>
	Texture(std::string path, TextureType type);

	///<summary>����� ��� ��������..</summary>
	///<param name = 'type'>��� ��������.</param>
	void setType(const TextureType type);

	///<summary>����� ��� ��������.</summary>
	///<param name = 'name'>��� ��������.</param>
	void setName(const std::string name);

	///<summary>���������� ������������� ��������.</summary>
	unsigned int getID() const;

	///<summary>���������� ��� ��������.</summary>
	TextureType getType() const;

	///<summary>���������� ��� ��������.</summary>
	std::string getName() const;
};

///<summary>�����, �������� �������� ���������.</summary>
class Material
{
private:
	///<summary>Albedo.</summary>
	glm::vec3 albedo_;

	///<summary>��������.</summary>
	std::vector<Texture> textures_;

	///<summary>������������� �����������.</summary>
	float metallic_;

	///<summary>��������� �����������.</summary>
	float smoothness_;

public:
	///<summary>�����������.</summary>
	Material();

	///<summary>����������.</summary>
	~Material();

	///<summary>����� � �������� ���������.</summary>
	void setDefault();

	///<summary>����� ������� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setAlbedo(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� ������� ����� ������.</summary>
	///<param name = 'color'>����.</param>
	void setAlbedo(const glm::vec3 color);

	///<summary>����� ������������� �����������.</summary>
	///<param name = 'metallic'>�������������.</param>
	void setMetallic(const float metallic);

	///<summary>����� ��������� �����������.</summary>
	///<param name = 'smoothness'>���������.</param>
	void setSmoothness(const float smoothness);

	///<summary>����� ��������.</summary>
	///<param name = 'texture'>��������.</param>
	void addTexture(Texture texture);

	///<summary>���������� ������� � RGB �������.</summary>
	glm::vec3 getAlbedo() const;

	///<summary>���������� ������������� �����������.</summary>
	float getMetallic() const;

	///<summary>���������� ��������� �����������.</summary>
	float getSmoothness() const;

	///<summary>���������� ������ �������.</summary>
	std::vector<Texture> getTextures() const;

	///<summary>��������� ������ ������� �� �������.</summary>
	bool isTexturesEmpty() const;
};

struct TextureKeys
{
	std::string mapsName;
	std::string mapsUse;
	std::string mapsCount;
};

///<summary>����� ������������ TextureType � � TextureKeys � �������.</summary>
extern const std::map<TextureType, TextureKeys> mapTextureType;

///<summary>�������� ����� ��� �����-���������.</summary>
extern float gamma;