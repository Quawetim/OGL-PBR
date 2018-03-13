#include "Includes.h"

// ���������, ������� ������ ���������� �� ����
windowInfo WindowInfo = windowInfo();

// CameraMode - ��������� ������: 1 - �� ������� ����, 2 - �� �������� ����, 3 - �������������
int CameraMode = 2;

// GenTextureSize - ������ ������������ ��������
int GenTextureSize = 128;

// FOV - field of view
float FOV = 90.0f;

// SkyBoxSize - ������ ���������
float SkyBoxSize = 500.0f;

// Rotations - ������������ ��������
bool Rotations = false;

// ShowLights - ������������ ����������� ���������� �����
bool ShowLights = false;

// Blinn - ������������ ������ ���������
bool Blinn = false;

// MirrorExample - true = ������ ����������� ������ � Reflection Map, false = ��� ������� ��� Reflection Map
bool MirrorExample = false;

// ShowHelp - ������������ ����������� �������
bool ShowHelp = false;