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

#include <ft2build.h>
#include FT_FREETYPE_H

#include "..\errorcodes\ErrorCodes.h"
#include "..\logger\Logger.h"

//using namespace std;
//using namespace glm;

// ���������, ������� ������ ���������� �� ����
struct QWindowInfo
{
    // ��������� �� ����
	GLFWwindow* Window;

    // ������ ����
    int Width;

    // ������ ����
    int Height;

    // ���������� ����
    float HalfWidth;

    // ���������� ����
    float HalfHeight;

    // ������ �����
    bool FullScreen;
    
    // D����������� ������������� 
    bool Vsync;

    // ����������� �������
    bool ShowCursor;
};

// ���������, ������� ������ ���������� �� ����
extern QWindowInfo windowInfo;

// ��� ������ ��� Logger
enum QErrorType { info, warning, error };

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