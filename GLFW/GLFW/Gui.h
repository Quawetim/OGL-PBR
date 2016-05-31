#ifndef GUI_H
#define GUI_H

#include "Includes.h"
#include "TextureLoader.h"

class TEXT
{
private:
	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* ��������� ��� �������*/
	GLuint ShaderID, TextID, TextureID, VertexBufferID, UVBufferID;

	/* �������� �������� */
	/* VertexShader - ���� � ���������� ������� */
	/* FragmentShader - ���� � ������������ (�����������) ������� */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* ����������� ��-��������� */
	TEXT();

	/* ����������� */
	/* TexturePath - ���� � �������� */
	TEXT(const char *TexturePath);

	/* ���������� */
	~TEXT();

	/* ������� ����� �� ����� */
	/* Text - ����� */
	/* X, Y - ���������� ��������� �� ������ */
	/* Size - ������ */
	void Render(const char *Text, int X, int Y, int Size);
};

class BUTTON
{
private:
	/* ������� ������ */
	int Function;

	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* ������� ������ */
	mat4 ModelMatrix = mat4(1.0f);

	/* ������*/
	GLuint vertexbuffer, uvbuffer;

	/* �������� */
	GLuint InactiveTexture, InactiveHoverTexture, ActiveTexture, ActiveHoverTexture;

	/* ������/��� */
	bool Pressed = false;	

	/* ��������� ��� �������*/
	GLuint ShaderID, ModelMatrixID, TextureID;

	/* �������� �������� */
	/* VertexShader - ���� � ���������� ������� */
	/* FragmentShader - ���� � ������������ (�����������) ������� */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* "���������" ������ � ����� ������ � ���������� ������� */
	bool flag = true;
	int frames = 0;

	/* ����������� ��-��������� */
	BUTTON();

	/* ����������� */
	/* function - ������� ������: 0 - ����������� �����, 1 - ��������� ��������, 2 - ����������� ���������� �����, 3 - ������ ��������� ������, 4 - ������������ ����� */
	void Prepare(int function, bool defaultstate, const char* inactivetexturepath, const char* inactivehovertexturepath, const char* activetexturepath, const char* activehovertexturepath);

	/* ���������� */
	~BUTTON();

	/* ������ ������� ������ */
	/* position - ������� ������� */
	/* scale - ������ */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* ������� ����������� �� ����� */
	/* X, Y - ���������� ��������� �� ������ */
	/* Size - ������ */
	bool Render(windowInfo Winfo, double MouseX, double MouseY, float x, float y, float sizex, float sizey);
};

class WINDOW
{
private:
	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* ������� ������ */
	mat4 ModelMatrix = mat4(1.0f);

	/* ������*/
	GLuint vertexbuffer, uvbuffer;

	/* �������� */
	GLuint Texture;

	/* ��������� ��� �������*/
	GLuint ShaderID, ModelMatrixID, TextureID;

	/* �������� �������� */
	/* VertexShader - ���� � ���������� ������� */
	/* FragmentShader - ���� � ������������ (�����������) ������� */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* ����������� ��-��������� */
	WINDOW();

	/* ����������� */
	/* function - ������� ������: 0 - ����������� �����, 1 - ��������� ��������, 2 - ����������� ���������� �����, 3 - ������ ��������� ������, 4 - ������������ ����� */
	void Prepare(const char* texturepath);

	/* ���������� */
	~WINDOW();

	/* ������ ������� ������ */
	/* position - ������� ������� */
	/* scale - ������ */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* ������� ����������� �� ����� */
	/* X, Y - ���������� ��������� �� ������ */
	/* Size - ������ */
	void Render(windowInfo Winfo, float x, float y, float sizex, float sizey);
};


#endif
