#include "Includes.h"

// ���������, ������� ������ ���������� �� ����
QWindowInfo windowInfo = QWindowInfo();

const std::map<QTextureType, std::string> mapTextureType = 
{ 
    {QTextureType::diffuse, "diffuseMap"}, 
    {QTextureType::specular, "specularMap"}, 
    {QTextureType::normal, "normalMap"} 
};

// ��������� ������: 1 - �� ������� ����, 2 - �� �������� ����, 3 - �������������
int CameraMode = 2;

// ������ ������������ ��������
int GenTextureSize = 128;

// Field of view
float FOV = 90.0f;

// ������ ���������
float SkyBoxSize = 500.0f;

// ������������ ��������
bool Rotations = false;

// ������������ ����������� ���������� �����
bool ShowLights = false;

// ������������ ������ ���������
bool Blinn = false;

// true = ������ ����������� ������ � Reflection Map, false = ��� ������� ��� Reflection Map
bool MirrorExample = false;

// ������������ ����������� �������
bool ShowHelp = false;

// �����, ��������� ����� ������� ������ � ����������
double deltaTime = 0.0;

// �����, ����������� �� ��������� ����������� �����
double lastFrameTime = 0.0;