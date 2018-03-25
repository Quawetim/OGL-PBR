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

///<summary>�����, ������� ������ ���������� �� ����.</summary>
class QWindowInfo
{
private: 
	///<summary>��������� �� ����.</summary>
    GLFWwindow* window = nullptr;
 
	///<summary>������ ����.</summary>
    int width = 800;
 
	///<summary>������ ����.</summary>
    int height = 600;

	///<summary>������ �����.</summary>
    bool fullScreen = false;
  
	///<summary>������������ �������������.</summary>
    bool vsync = false;
 
	///<summary>����������� �������.</summary>
    bool showCursor = true;

	///<summary>FPS.</summary>
    int fps = 0;

public:
    ///<summary>����� ��������� �� ����.</summary>
    ///<param name = 'window'>���������.</param>
    void setWindowPointer(GLFWwindow* window);

    ///<summary>����� ������ ����.</summary>
    ///<param name = 'width'>������.</param>
    void setWidth(const int width);

    ///<summary>����� ������ ����.</summary>
    ///<param name = 'height'>������.</param>
    void setHeight(const int height);

    ///<summary>����� ������������� �����.</summary>
    ///<param name = 'fullScreen'>������������� �����.</param>
    void setFullScreen(const bool fullScreen);

    ///<summary>��������/��������� ������������ �������������.</summary>
    ///<param name = 'vsync'>������������ ������������.</param>
    void setVsync(const bool vsync);

    ///<summary>����� ����������� �������.</summary>
    ///<param name = 'showCursor'>���������� ������.</param>
    void setShowCursor(const bool showCursor);

    ///<summary>����� ������� ����� ������ � �������.</summary>
    ///<param name = 'fps'>FPS.</param>
    void setFPS(const int fps);

    ///<summary>���������� ��������� �� ����.</summary>
    GLFWwindow* getWindowPointer() const;

    ///<summary>���������� ������ ����.</summary>
    int getWidth() const;

    ///<summary>���������� ������ ����.</summary>
    int getHeight() const;

    ///<summary>���������� ���������� ����.</summary>
    float getHalfWidth() const;

    ///<summary>���������� ���������� ����.</summary>
    float getHalfHeight() const;

    ///<summary>���������� ������� ���������������.</summary>
    bool getFullScreen() const;

    ///<summary>���������� �������� ������������ ������������� ��� ���.</summary>
    bool getVsync() const;

    ///<summary>���������� ������� �������������� �������.</summary>
    bool getShowCursor() const;

    ///<summary>���������� ������� ����� ������ � �������.</summary>
    int getFPS() const;
};

///<summary>�����, �������� ���������� � ��������.</summary>
class QTexture
{
private:
	///<summary>�������������.</summary>
	unsigned int id = 0;

	///<summary>���: diffuse, specular, normal etc.</summary>
	QTextureType type;

	///<summary>���� � ��������.</summary>
	std::string path;

	///<summary>��� ��������.</summary>
	std::string name;

public:
	///<summary>�����������.</summary>
	QTexture() {};

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
	glm::vec3 ambientColor = glm::vec3(0.05f, 0.05f, 0.05f);

	///<summary>Diffuse ����.</summary>
	glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);

	///<summary>Specular ����.</summary>
	glm::vec3 specularColor = glm::vec3(0.7f, 0.7f, 0.7f);

	///<summary>��������.</summary>
	std::vector<QTexture> textures;

	///<summary>���� (�������) �����.</summary>
	float shinePower = 8.0f;

public:
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
	///<param name = 'value'>��������.</param>
	void setShinePower(const float value);

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
	float getShinePower() const;

	///<summary>���������� ������ �������.</summary>
	std::vector<QTexture> getTextures() const;

	///<summary>��������� ������ ������� �� �������.</summary>
	bool noTextures() const;
};

///<summary>���������, ������� ������ ���������� �� ����.</summary>
extern QWindowInfo windowInfo;

///<summary>����� ������������ QTextureType � string.</summary>
extern const std::map<QTextureType, std::string> mapTextureType;
 
///<summary>Field of view.</summary>
extern float FOV;

///<summary>������ ������������ ����� ���������.</summary>
extern int reflectionMapResolution;
 
///<summary>�����, ��������� ����� ������� ������ � ����������.</summary>
extern double deltaTime;
 
///<summary>�����, ����������� �� ��������� ����������� �����.</summary>
extern double lastFrameTime;