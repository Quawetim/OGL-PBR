#include "Gui.h"
#include "TextureLoader.h"

/* Загрузка вершинного и фрагментного шейдеров */
/* VertexShader - путь к вершинному шейдеру */
/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
GLuint TEXT::LoadShaders(const char *VertexShader, const char *FragmentShader)
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

/* Конструктор по-умолчанию */
TEXT::TEXT() {};

/* Конструктор */
/* TexturePath - путь к текстуре */
TEXT::TEXT(const char *TexturePath)
{
	TextureID = LoadDDS(TexturePath);

	glGenBuffers(1, &VertexBufferID);
	glGenBuffers(1, &UVBufferID);

	ShaderID = LoadShaders("shaders//Text.vs", "shaders//Text.fs");

	TextID = glGetUniformLocation(ShaderID, "Texture");
}

/* Деструктор */
TEXT::~TEXT()
{
	glDeleteBuffers(1, &VertexBufferID);
	glDeleteBuffers(1, &UVBufferID);
	glDeleteTextures(1, &TextureID);
	glDeleteProgram(ShaderID);
}

/* Выводит текст на экран */
/* Text - буфер */
/* X, Y - координаты положения на экране */
/* Size - размер */
void TEXT::Render(const char *Text, int X, int Y, int Size)
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

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(vec2), &UVs[0], GL_STATIC_DRAW);

	glUseProgram(ShaderID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glUniform1i(TextID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glBindVertexArray(0);
}

/* Загрузка вершинного и фрагментного шейдеров */
/* VertexShader - путь к вершинному шейдеру */
/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
GLuint BUTTON::LoadShaders(const char *VertexShader, const char *FragmentShader)
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

/* Конструктор по-умолчанию */
BUTTON::BUTTON() {};

/* Конструктор */
BUTTON::BUTTON(int id, bool defaultstate, const char* inactivetexturepath, const char* hovertexturepath, const char* activetexturepath)
{
	ID = id;
	Pressed = defaultstate;

	ShaderID = LoadShaders("shaders//Gui.vs", "shaders//Gui.fs");

	InactiveTexture = LoadBMP(inactivetexturepath);
	HoverTexture = LoadBMP(hovertexturepath);
	ActiveTexture = LoadBMP(activetexturepath);

	ModelMatrixID = glGetUniformLocation(ShaderID, "M");
	TextureID = glGetUniformLocation(ShaderID, "Texture");

	GLfloat verticiesUVs[]
	{
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesUVs), verticiesUVs, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); 
	glBindVertexArray(0);
}

/* Деструктор */
BUTTON::~BUTTON()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteTextures(1, &InactiveTexture);
	glDeleteTextures(1, &HoverTexture);
	glDeleteTextures(1, &ActiveTexture);
	glDeleteProgram(ShaderID);
}

/* Создаёт матрицу модели */
/* position - позиция объекта */
/* size - размер */
void BUTTON::createModelMatrix(float x, float y, float sizex, float sizey)
{
	ModelMatrix = mat4(1.0f);
	ModelMatrix *= translate(vec3(x, y, 0.0f));
	ModelMatrix *= scale(vec3(sizex, sizey, 1.0f));
}

/* Выводит текст на экран */
/* Text - буфер */
/* X, Y - координаты положения на экране */
/* Size - размер */
bool BUTTON::Render(int id, windowInfo Winfo, float x, float y, float sizex, float sizey)
{
	bool Answer = true;
	double MouseX, MouseY;
	float Left, Right, Top, Bottom;

	createModelMatrix(x, y, sizex, sizey);

	glUseProgram(ShaderID);

	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));

	glfwGetCursorPos(Winfo.Window, &MouseX, &MouseY);

	/*
		(MouseX > Winfo.HalfWidth * (1.0f + x - sizex)) && 
		(MouseX < Winfo.HalfWidth * (1.0f + x + sizex)) && 
		(MouseY > Winfo.HalfHeight * (1.0f - y - sizey)) && 
		(MouseY < Winfo.HalfHeight * (1.0f - y + sizey)))
	*/


	Left = Winfo.HalfWidth * (1.0f + x - sizex);
	Right = Winfo.HalfWidth * (1.0f + x + sizex);
	Top = Winfo.HalfHeight * (1.0f - y - sizey);
	Bottom = Winfo.HalfHeight * (1.0f - y + sizey);

	if ((glfwGetMouseButton(Winfo.Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && flag)
	{
		if ((MouseX > Left) && (MouseX < Right) && (MouseY > Top) && (MouseY < Bottom))
		{
			if (Pressed)
			{
				Pressed = false;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, InactiveTexture);
				glUniform1i(TextureID, 0);

				if (id == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				Pressed = true;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ActiveTexture);
				glUniform1i(TextureID, 0);

				if (id == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				if (id == 1);
			}

			flag = false;
		}
		else
		{
			if (Pressed)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ActiveTexture);
				glUniform1i(TextureID, 0);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, InactiveTexture);
				glUniform1i(TextureID, 0);
			}
		}
	}
	else
	{
		if ((MouseX > Left) && (MouseX < Right) && (MouseY > Top) && (MouseY < Bottom))
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, HoverTexture);
			glUniform1i(TextureID, 0);
		}
		else
		{
			if (Pressed)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, ActiveTexture);
				glUniform1i(TextureID, 0);

				if (id == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, InactiveTexture);
				glUniform1i(TextureID, 0);

				if (id == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
	}

	/*if ((MouseX > Left) && (MouseX < Right) && (MouseY > Top) && (MouseY < Bottom))
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HoverTexture);
		glUniform1i(TextureID, 0);

		if (id == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, InactiveTexture);
		glUniform1i(TextureID, 0);

		if (id == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}*/

	glBindVertexArray(VAO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return Answer;
}
