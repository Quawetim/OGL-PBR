#ifndef GUI_H
#define GUI_H

#include "Includes.h"
#include "TextureLoader.h"

class TEXT
{
private:
	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* Указатели для шейдера*/
	GLuint ShaderID, TextID, TextureID, VertexBufferID, UVBufferID;

	/* Загрузка шейдеров */
	/* VertexShader - путь к вершинному шейдеру */
	/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* Конструктор по-умолчанию */
	TEXT();

	/* Конструктор */
	/* TexturePath - путь к текстуре */
	TEXT(const char *TexturePath);

	/* Деструктор */
	~TEXT();

	/* Выводит текст на экран */
	/* Text - буфер */
	/* X, Y - координаты положения на экране */
	/* Size - размер */
	void Render(const char *Text, int X, int Y, int Size);
};

class BUTTON
{
private:
	/**/
	int ID;

	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* Матрица модели */
	mat4 ModelMatrix = mat4(1.0f);

	/* Буферы*/
	GLuint vertexbuffer, uvbuffer;

	/* Текстуры */
	GLuint InactiveTexture, HoverTexture, ActiveTexture;

	/**/
	bool Pressed = false;	

	/* Указатели для шейдера*/
	GLuint ShaderID, ModelMatrixID, TextureID;

	/* Загрузка шейдеров */
	/* VertexShader - путь к вершинному шейдеру */
	/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:

	bool flag = true;

	/* Конструктор по-умолчанию */
	BUTTON();

	/* Конструктор */
	BUTTON(int id, bool defaultstate, const char* inactivetexturepath, const char* hovertexturepath, const char* activetexturepath);

	/* Деструктор */
	~BUTTON();

	/* Создаёт матрицу модели */
	/* position - позиция объекта */
	/* scale - размер */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* Выводит изображение на экран */
	/* X, Y - координаты положения на экране */
	/* Size - размер */
	bool Render(int id, windowInfo Winfo, float x, float y, float sizex, float sizey);
};

#endif
