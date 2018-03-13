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

#include <glad\glad.h>
#include <GL\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
//#include <SOIL\SOIL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "..\errorcodes\ErrorCodes.h"
#include "..\logger\Logger.h"

//using namespace std;
//using namespace glm;

// ���������, ������� ������ ���������� �� ����
struct windowInfo
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
extern windowInfo WindowInfo;

// ��� ������ ��� Logger
enum errorType { info, warning, error };

// CameraMode - ��������� ������: 1 - �� ������� ����, 2 - �� �������� ����, 3 - �������������
extern int CameraMode;

// GenTextureSize - ������ ������������ ��������
extern int GenTextureSize;

// FOV - field of view
extern float FOV;

// SkyBoxSize - ������ ���������
extern float SkyBoxSize;

// Rotations - ������������ ��������
extern bool Rotations;

// ShowLights - ������������ ����������� ���������� �����
extern bool ShowLights;

// Blinn - ������������ ������ ���������
extern bool Blinn;

// MirrorExample - true = ������ ����������� ������ � Reflection Map, false = ��� ������� ��� Reflection Map
extern bool MirrorExample;

// ShowHelp - ������������ ����������� �������
extern bool ShowHelp;