#ifndef GUI_H
#define GUI_H

#include "Includes.h"
#include "TextureLoader.h"

class TEXT
{
private:
    FT_Library _ft;
    FT_Face _face;

    struct Character
    {
        GLuint TextureID;
        ivec2 Size;
        ivec2 Bearing;
        GLuint Advance;
    };

    map<wchar_t, Character> Characters;

	/* VAO - Vertex Array Object*/
	GLuint VAO, VBO;

	/* ��������� ��� �������*/
	GLuint ShaderID, TextID, TextureID, VertexBufferID, UVBufferID;

	/* �������� �������� */
	/* VertexShader - ���� � ���������� ������� */
	/* FragmentShader - ���� � ������������ (�����������) ������� */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* ����������� */
	/* TexturePath - ���� � �������� */
	TEXT(const char *TexturePath);

    TEXT(const char *FontPath, int FontSize);

	/* ���������� */
	~TEXT();

	/* ������� ����� �� ����� */
	/* Text - ����� */
	/* X, Y - ���������� ��������� �� ������ */
	/* Size - ������ */
	void Render(const char *Text, vec3 Color, int X, int Y, int Size);

    void RenderFreeType(const wchar_t *Text, vec3 Color, float Scale, float X, float Y);
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

	/* ���������� ������ */
	/* function - ������� ������: 0 - ����������� �����, 1 - ��������� ��������, 2 - ����������� ���������� �����, 3 - ������ ��������� ������, 4 - ������������ ����� */
	/* defaultstate - �������� ��-��������� */
	/* inactivetexturepath, inactivehovertexturepath, activetexturepath, activehovertexturepath - ���� � ��������� */
	void Prepare(int function, bool defaultstate, const char* inactivetexturepath, const char* inactivehovertexturepath, const char* activetexturepath, const char* activehovertexturepath);

	/* ���������� */
	~BUTTON();

	/* ������ ������� ������ */
	/* x, y - ������� ������ ([0;1], ��� [0:0] - ����� ������) */
	/* sizex, sizey - ������ ������ �� X, �� Y */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* ������� ������ �� ����� */
	/* Winfo - ���������� �� ���� */
	/* mousex, mousey - ���������� ������� */
	/* x, y - ������� ������ ([0;1], ��� [0:0] - ����� ������) */
	/* sizex, sizey - ������ ������ �� X, �� Y */
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

	/* ���������� ������ */
	/* texturepath - ���� � ������� */
	void Prepare(const char* texturepath);

	/* ���������� */
	~WINDOW();

	/* ������ ������� ������ */
	/* x, y - ������� ������ ([0;1], ��� [0:0] - ����� ������) */
	/* sizex, sizey - ������ ������ �� X, �� Y */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* ������� ���� �� ����� */
	/* Winfo - ���������� �� ���� */
	/* mousex, mousey - ���������� ������� */
	/* x, y - ������� ������ ([0;1], ��� [0:0] - ����� ������) */
	/* sizex, sizey - ������ ������ �� X, �� Y */
	void Render(windowInfo Winfo, float x, float y, float sizex, float sizey);
};


#endif
