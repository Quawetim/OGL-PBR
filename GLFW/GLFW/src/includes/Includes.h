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

// ��� ������ ��� Logger
enum QErrorType { info, warning, error };

// ��� ��������
enum QTextureType { diffuse, specular, normal };

// ��������� �������� ��������� ������
struct QVertexData
{
    // ������� �������
    glm::vec3 position;

    // ������ �������
    glm::vec3 normal;

    // ���������� ����������
    glm::vec2 textureCoords;

    // �����������
    glm::vec3 tangent;

    // �������������
    glm::vec3 bitangent;
};

// ���������, �������� ���������� � ��������
struct QTexture
{
    // �������������
    unsigned int id;

    // ���: diffuse, specular, normal etc.
    QTextureType type;

    // ���� � ��������
    std::string path;
};

// �����, ������� ������ ���������� �� ����
class QWindowInfo
{
private:
    // ��������� �� ����
    GLFWwindow* window = nullptr;

    // ������ ����
    int width = 800;

    // ������ ����
    int height = 600;

    // ������ �����
    bool fullScreen = false;

    // D����������� ������������� 
    bool vsync = false;

    // ����������� �������
    bool showCursor = true;

    // FPS
    int fps = 0;

public:
    ///<summary>����� ��������� �� ����.<summary>
    ///<param name = 'window'>���������.</param>
    void setWindowPointer(GLFWwindow* window);

    ///<summary>����� ������ ����.<summary>
    ///<param name = 'width'>������.</param>
    void setWidth(const int width);

    ///<summary>����� ������ ����.<summary>
    ///<param name = 'height'>������.</param>
    void setHeight(const int height);

    ///<summary>����� ������������� �����.<summary>
    ///<param name = 'fullScreen'>������������� �����.</param>
    void setFullScreen(const bool fullScreen);

    ///<summary>��������/��������� ������������ �������������.<summary>
    ///<param name = 'vsync'>������������ ������������.</param>
    void setVsync(const bool vsync);

    ///<summary>����� ����������� �������.<summary>
    ///<param name = 'showCursor'>���������� ������.</param>
    void setShowCursor(const bool showCursor);

    ///<summary>����� ������� ����� ������ � �������.<summary>
    ///<param name = 'fps'>FPS.</param>
    void setFPS(const int fps);

    ///<summary>���������� ��������� �� ����.<summary>
    GLFWwindow* getWindowPointer() const;

    ///<summary>���������� ������ ����.<summary>
    int getWidth() const;

    ///<summary>���������� ������ ����.<summary>
    int getHeight() const;

    ///<summary>���������� ���������� ����.<summary>
    float getHalfWidth() const;

    ///<summary>���������� ���������� ����.<summary>
    float getHalfHeight() const;

    ///<summary>���������� ������� ���������������.<summary>
    bool getFullScreen() const;

    ///<summary>���������� �������� ������������ ������������� ��� ���.<summary>
    bool getVsync() const;

    ///<summary>���������� ������� �������������� �������.<summary>
    bool getShowCursor() const;

    ///<summary>���������� ������� ����� ������ � �������.<summary>
    int getFPS() const;
};

// ���������, ������� ������ ���������� �� ����
extern QWindowInfo windowInfo;

// ����� ������������ QTextureType � string
extern const std::map<QTextureType, std::string> mapTextureType;

// ��������� ������: 1 - �� ������� ����, 2 - �� �������� ����, 3 - �������������
extern int CameraMode;

// ������ ������������ ��������
extern int GenTextureSize;

// Field of view
extern float FOV;

// ������ ���������
extern float SkyBoxSize;

// ������������ ��������
extern bool Rotations;

// ������������ ����������� ���������� �����
extern bool ShowLights;

// ������������ ������ ���������
extern bool Blinn;

// Mtrue = ������ ����������� ������ � Reflection Map, false = ��� ������� ��� Reflection Map
extern bool MirrorExample;

// ������������ ����������� �������
extern bool ShowHelp;

// �����, ��������� ����� ������� ������ � ����������
extern double deltaTime;

// �����, ����������� �� ��������� ����������� �����
extern double lastFrameTime;