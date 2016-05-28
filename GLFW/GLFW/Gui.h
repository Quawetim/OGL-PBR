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
	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* ������� ������ */
	mat4 ModelMatrix = mat4(1.0f);

	/* ������*/
	GLuint vertexbuffer, uvbuffer;

	/* �������� */
	GLuint InactiveTexture, HoverTexture, ActiveTexture;

	/* ��������� ��� �������*/
	GLuint ShaderID, ModelMatrixID, TextureID;

	/* �������� �������� */
	/* VertexShader - ���� � ���������� ������� */
	/* FragmentShader - ���� � ������������ (�����������) ������� */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* ����������� ��-��������� */
	BUTTON();

	/* ����������� */
	BUTTON(const char* inactivetexturepath, const char* hovertexturepath, const char* activetexturepath);

	/* ���������� */
	~BUTTON();

	/* ������ ������� ������ */
	/* position - ������� ������� */
	/* scale - ������ */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* ������� ����������� �� ����� */
	/* X, Y - ���������� ��������� �� ������ */
	/* Size - ������ */
	void Render(float x, float y, float sizex, float sizey);
};

#endif
