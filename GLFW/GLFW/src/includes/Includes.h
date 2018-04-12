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

///<summary>��� ������ ��� Logger.</summary>
enum QErrorType { info, warning, error };

///<summary>��� ��������.</summary>
enum QTextureType { diffuse, specular, normal };

///<summary>��������� �������� ��������� ������.</summary>
struct QVertexData
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

class QInputHandle
{
private:
	static QInputHandle *inputHandle_;

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
class QTexture
{
private:
	///<summary>�������������.</summary>
	unsigned int id_;

	///<summary>���: diffuse, specular, normal etc.</summary>
	QTextureType type_;

	///<summary>���� � ��������.</summary>
	std::string path_;

	///<summary>��� ��������.</summary>
	std::string name_;

public:
	///<summary>�����������.</summary>
	QTexture();

	///<summary>�����������.</summary>
	///<param name = 'path'>���� � ��������.</param>
	///<param name = 'type'>��� ��������.</param>
	QTexture(std::string path, QTextureType type);

	///<summary>����� ��� ��������..</summary>
	///<param name = 'type'>��� ��������.</param>
	void setType(const QTextureType type);

	///<summary>����� ��� ��������.</summary>
	///<param name = 'name'>��� ��������.</param>
	void setName(const std::string name);

	///<summary>���������� ������������� ��������.</summary>
	unsigned int getID() const;

	///<summary>���������� ��� ��������.</summary>
	QTextureType getType() const;

	///<summary>���������� ��� ��������.</summary>
	std::string getName() const;
};

///<summary>�����, �������� �������� ���������.</summary>
class QMaterial
{
private:
	///<summary>Ambient ����.</summary>
	glm::vec3 ambientColor_;

	///<summary>Diffuse ����.</summary>
	glm::vec3 diffuseColor_;

	///<summary>Specular ����.</summary>
	glm::vec3 specularColor_;

	///<summary>��������.</summary>
	std::vector<QTexture> textures_;

	///<summary>���� (�������) �����.</summary>
	float shininess_;

public:
	///<summary>�����������.</summary>
	QMaterial();

	///<summary>����������.</summary>
	~QMaterial();

	///<summary>����� � �������� ���������.</summary>
	void reset();

	///<summary>����� ambient ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� diffuse ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� specular ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>����� ���� (�������) �����.</summary>
	///<param name = 'shininess'>��������.</param>
	void setShininess(const float shininess);

	///<summary>����� ��������.</summary>
	///<param name = 'texture'>��������.</param>
	void addTexture(QTexture texture);

	///<summary>���������� ambient ���� � RGB �������.</summary>
	glm::vec3 getAmbientColor() const;

	///<summary>���������� diffuse ���� � RGB �������.</summary>
	glm::vec3 getDiffuseColor() const;

	///<summary>���������� specular ���� � RGB �������.</summary>
	glm::vec3 getSpecularColor() const;

	///<summary>���������� ���� (�������) �����.</summary>
	float getShininess() const;

	///<summary>���������� ������ �������.</summary>
	std::vector<QTexture> getTextures() const;

	///<summary>��������� ������ ������� �� �������.</summary>
	bool isTexturesEmpty() const;
};

///<summary>����� ������������ QTextureType � string.</summary>
extern const std::map<QTextureType, std::string> mapTextureType;
 
///<summary>Field of view.</summary>
extern float FOV;
 
///<summary>�����, ��������� ����� ������� ������ � ����������.</summary>
extern float deltaTime;
 
///<summary>�����, ����������� �� ��������� ����������� �����.</summary>
extern float lastFrameTime;