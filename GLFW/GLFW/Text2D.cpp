#include <vector>
#include <string>
#include <fstream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

//#include "Shader.h"
#include "TextureLoader.h"

#include "Text2D.h"

unsigned int Text2DTextureID;
unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;
unsigned int Text2DShaderID;
unsigned int Text2DUniformID;

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Создание шейдеров
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);		// Вершинный
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);	// Фрагментный

																	// Читаем код вершинного шейдера из файла vertex_file_path
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);

	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line)) VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		printf("File %s not found.\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Читаем код фрагментного шейдера из файла fragment_file_path
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);

	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line)) FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Компилируем вершинный шейдер
	printf("Compiling vertex shader: %s...", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Проверка
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("\nError: %s\n", &VertexShaderErrorMessage[0]);
	}

	// Компилируем фрагментный шейдер
	printf("Compiling fragment shader: %s...", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Проверка
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("\nError: %s\n", &FragmentShaderErrorMessage[0]);
	}

	// Линкуем
	printf("Linking program...");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Проверка
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("\nError: %s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint VAO;

void initText2D(const char * texturePath)
{
	// загрузка текстуры
	Text2DTextureID = loadDDS(texturePath);

	// VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	// Загрузка шейдера
	Text2DShaderID = LoadShaders( "shaders//Text.vertexshader", "shaders//Text.fragmentshader" );

	// Связываем шейдер и данные
	Text2DUniformID = glGetUniformLocation( Text2DShaderID, "Texture" );
}

void printText2D(const char * text, int x, int y, int size)
{
	int i, length = strlen(text);
	float uv_x, uv_y;
	char character;

	vector<vec2> vertices, UVs;
	vec2 vertex_up_left, vertex_up_right, vertex_down_right, vertex_down_left, uv_up_left, uv_up_right, uv_down_right, uv_down_left;

	for (i = 0; i < length; i++)
	{		
		vertex_up_left = vec2(x + i*size, y + size);
		vertex_up_right = vec2(x + i*size + size, y + size);
		vertex_down_right = vec2(x + i*size + size, y);
		vertex_down_left = vec2(x + i*size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		character = text[i];
		uv_x = (character % 16) / 16.0f;
		uv_y = (character / 16) / 16.0f;

		uv_up_left = vec2(uv_x, uv_y);
		uv_up_right = vec2(uv_x + 1.0f / 16.0f, uv_y);
		uv_down_right = vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		uv_down_left = vec2(uv_x, (uv_y + 1.0f / 16.0f));

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(vec2), &UVs[0], GL_STATIC_DRAW);

	glUseProgram(Text2DShaderID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	glUniform1i(Text2DUniformID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glBindVertexArray(0);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
}

void cleanupText2D()
{
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);
	glDeleteTextures(1, &Text2DTextureID);
	glDeleteProgram(Text2DShaderID);
}
