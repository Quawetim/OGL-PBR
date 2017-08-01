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
	/* Функция кнопки */
	int Function;

	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* Матрица модели */
	mat4 ModelMatrix = mat4(1.0f);

	/* Буферы*/
	GLuint vertexbuffer, uvbuffer;

	/* Текстуры */
	GLuint InactiveTexture, InactiveHoverTexture, ActiveTexture, ActiveHoverTexture;

	/* Нажата/нет */
	bool Pressed = false;	

	/* Указатели для шейдера*/
	GLuint ShaderID, ModelMatrixID, TextureID;

	/* Загрузка шейдеров */
	/* VertexShader - путь к вершинному шейдеру */
	/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* "Заморозка" кнопки и число кадров с последнего нажатия */
	bool flag = true;
	int frames = 0;

	/* Конструктор по-умолчанию */
	BUTTON();

	/* Подготовка данных */
	/* function - функция кнопки: 0 - отображение сетки, 1 - остановка вращений, 2 - отображение источников света, 3 - модель освещения Блинна, 4 - переключение сцены */
	/* defaultstate - значение по-умолчанию */
	/* inactivetexturepath, inactivehovertexturepath, activetexturepath, activehovertexturepath - путь к текстурам */
	void Prepare(int function, bool defaultstate, const char* inactivetexturepath, const char* inactivehovertexturepath, const char* activetexturepath, const char* activehovertexturepath);

	/* Деструктор */
	~BUTTON();

	/* Создаёт матрицу модели */
	/* x, y - позиция кнопки ([0;1], где [0:0] - центр экрана) */
	/* sizex, sizey - размер кнопки по X, по Y */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* Выводит кнопку на экран */
	/* Winfo - информация об окне */
	/* mousex, mousey - координаты курсора */
	/* x, y - позиция кнопки ([0;1], где [0:0] - центр экрана) */
	/* sizex, sizey - размер кнопки по X, по Y */
	bool Render(windowInfo Winfo, double MouseX, double MouseY, float x, float y, float sizex, float sizey);
};

class WINDOW
{
private:
	/* VAO - Vertex Array Object*/
	GLuint VAO;

	/* Матрица модели */
	mat4 ModelMatrix = mat4(1.0f);

	/* Буферы*/
	GLuint vertexbuffer, uvbuffer;

	/* Текстура */
	GLuint Texture;

	/* Указатели для шейдера*/
	GLuint ShaderID, ModelMatrixID, TextureID;

	/* Загрузка шейдеров */
	/* VertexShader - путь к вершинному шейдеру */
	/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
	GLuint LoadShaders(const char *VertexShader, const char *FragmentShader);

public:
	/* Конструктор по-умолчанию */
	WINDOW();

	/* Подготовка данных */
	/* texturepath - путь к тестуре */
	void Prepare(const char* texturepath);

	/* Деструктор */
	~WINDOW();

	/* Создаёт матрицу модели */
	/* x, y - позиция кнопки ([0;1], где [0:0] - центр экрана) */
	/* sizex, sizey - размер кнопки по X, по Y */
	void createModelMatrix(float x, float y, float sizex, float sizey);

	/* Выводит окно на экран */
	/* Winfo - информация об окне */
	/* mousex, mousey - координаты курсора */
	/* x, y - позиция кнопки ([0;1], где [0:0] - центр экрана) */
	/* sizex, sizey - размер кнопки по X, по Y */
	void Render(windowInfo Winfo, float x, float y, float sizex, float sizey);
};


#endif
