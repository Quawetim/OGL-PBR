#include <vector>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

#include "TextureLoader.h"
#include "Text.h"

GLuint VAO;
unsigned int TextShaderID, TextID, TextTextureID, TextVertexBufferID, TextUVBufferID;

/* Загрузка вершинного и фрагментного шейдеров */
/* VertexShader - путь к вершинному шейдеру */
/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
GLuint LoadShaders(const char *VertexShader, const char *FragmentShader)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);		
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);	

																	
	string VertexShaderCode;
	ifstream VertexShaderStream(VertexShader, ios::in);

	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line)) VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		printf("File %s not found.\n", VertexShader);
		return 0;
	}

	string FragmentShaderCode;
	ifstream FragmentShaderStream(FragmentShader, ios::in);

	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line)) FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else
	{
		printf("File %s not found.\n", FragmentShader);
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling vertex shader: %s...", VertexShader);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("\nError: %s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling fragment shader: %s...", FragmentShader);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("\nError: %s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program...");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

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

/* Инициализация */
/* TexturePath - путь к текстуре */
void InitText(const char *TexturePath)
{
	TextTextureID = LoadDDS(TexturePath);

	glGenBuffers(1, &TextVertexBufferID);
	glGenBuffers(1, &TextUVBufferID);

	TextShaderID = LoadShaders("shaders//Text.vs", "shaders//Text.fs");

	TextID = glGetUniformLocation(TextShaderID, "Texture");
}

/* Выводит текст на экран */
/* Text - буфер */
/* X, Y - координаты положения на экране */
/* Size - размер */
void PrintText(const char *Text, int X, int Y, int Size)
{
	int length = strlen(Text);
	float uv_x, uv_y;
	char character;

	vector<vec2> vertices, UVs;
	vec2 vertex_up_left, vertex_up_right, vertex_down_right, vertex_down_left, uv_up_left, uv_up_right, uv_down_right, uv_down_left;

	for (int i = 0; i < length; i++)
	{		
		vertex_up_left = vec2(X + i*Size, Y + Size);
		vertex_up_right = vec2(X + i*Size + Size, Y + Size);
		vertex_down_right = vec2(X + i*Size + Size, Y);
		vertex_down_left = vec2(X + i*Size, Y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		character = Text[i];
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

	glBindBuffer(GL_ARRAY_BUFFER, TextVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, TextUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(vec2), &UVs[0], GL_STATIC_DRAW);

	glUseProgram(TextShaderID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextTextureID);
	glUniform1i(TextID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, TextVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, TextUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glBindVertexArray(0);
}

/* Чистка */
void DeleteText()
{
	glDeleteBuffers(1, &TextVertexBufferID);
	glDeleteBuffers(1, &TextUVBufferID);
	glDeleteTextures(1, &TextTextureID);
	glDeleteProgram(TextShaderID);
}
