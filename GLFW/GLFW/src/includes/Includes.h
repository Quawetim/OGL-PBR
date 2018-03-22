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

///<summary>���������, �������� ���������� � ��������.</summary>
struct QTexture
{
	///<summary>�������������.</summary>
    unsigned int id;

	///<summary>���: diffuse, specular, normal etc.</summary>
    QTextureType type;

    ///<summary>���� � ��������.</summary>
    std::string path;
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