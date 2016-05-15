#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

using namespace std;
using namespace glm;

GLFWwindow* window;

#include "TextureLoader.h"
#include "VboIndexer.h"
#include "Text2D.h"

int WindowWidth = 1280, WindowHeight = 800;
float FOV = 90.0;
int cameramode = 2;
int textureSize = 128;
bool wireframe = false;														// F1

class CAMERA
{
private:
	vec3 position;																							// Позиция камеры
	float Pi = pi<float>();
	float deltaTime, radius = 20.0, radiusMin = 2.0, radiusMax = 100.0, cameraheight = 1.0;					// Время, радиус и высота полёта второй камеры
	float horizontalAngle = 0.0, verticalAngle = 0.0;														// Горизонтальный и вертикальный углы
	float FoV = 90.0, speed = 6.0, speed2 = 6.0, mouseSpeed = 0.005;										// FOV, скорость движения камеры, скорость мышки

	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

public:
	mat4 getProjectionMatrix() { return ProjectionMatrix; }
	mat4 getViewMatrix() { return ViewMatrix; }
	mat4 getViewMatrixAxes() { return ViewMatrixAxes; }

	/* Обработка клавиатуры для движения камеры 2 */
	void checkmove(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { if (radius >= radiusMin) radius -= deltaTime * speed; }
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { if (radius <= radiusMax) radius += deltaTime * speed; }
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { if (cameraheight <= 10.0) cameraheight += deltaTime * speed; }
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { if (cameraheight >= -10.0) cameraheight -= deltaTime * speed; }
	}

	/* Обработка клавиатуры для движения камеры 1 */
	void checkmove(GLFWwindow* window, vec3 direction, vec3 right)
	{
		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) { if (speed2 < 20.0) speed2 += 0.2; }
		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) { if (speed2 > 1.0) speed2 -= 0.2; }
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { position += direction * deltaTime * speed2; }
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { position -= direction * deltaTime * speed2; }
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { position += right * deltaTime * speed2; }
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { position -= right * deltaTime * speed2; }
	}

	/* Вычисление матриц вида и проекции */
	/* Возвращает позицию камеры */
	vec3 ComputeViewMatrix(GLFWwindow* window, int cameramode)
	{
		double currentTime, mouseX, mouseY;

		static double lastTime = glfwGetTime();

		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

		glfwGetCursorPos(window, &mouseX, &mouseY);

		//glfwGetWindowSize(window, &width, &height);
		glfwSetCursorPos(window, WindowWidth / 2, WindowHeight / 2);

		horizontalAngle += mouseSpeed * float(WindowWidth / 2 - mouseX);
		verticalAngle += mouseSpeed * float(WindowHeight / 2 - mouseY);

		if (cameramode == 1)
		{
			vec3 direction, right, up;

			// Direction : Spherical coordinates to Cartesian coordinates conversion
			direction = vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
			right = vec3(sin(horizontalAngle - Pi / 2.0), 0, cos(horizontalAngle - Pi / 2.0));
			up = vec3(0, 1, 0);

			checkmove(window, direction, right);

			ViewMatrix = lookAt(position, position + direction, up);

			ViewMatrixAxes = ViewMatrix;
		}

		if (cameramode == 2)
		{
			vec3 direction, up;

			position = vec3(radius*cos(horizontalAngle), cameraheight, radius*sin(horizontalAngle));
			direction = vec3(0.0, -1.5, 0.0);
			up = vec3(0.0, 1.0, 0.0);

			checkmove(window);

			ViewMatrix = lookAt(position, direction, up);

			ViewMatrixAxes = lookAt(vec3(5 * cos(horizontalAngle), cameraheight, 5 * sin(horizontalAngle)), direction, up);
		}

		if (cameramode == 3)
		{
			vec3 direction, up;

			position = vec3(5.0, 3.0, 4.0);
			direction = vec3(0.0, 0.0, 0.0);
			up = vec3(0.0, 1.0, 0.0);

			ViewMatrix = lookAt(position, direction, up);

			ViewMatrixAxes = ViewMatrix;
		}

		//ProjectionMatrix = perspective(radians(FoV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f); // FOV, ratio, range : 0.1 unit <-> 100 units	 	
		lastTime = currentTime;

		return position;
	}
};


class OBJECT
{
public:
	int ID;
	int Material;
	GLuint ShaderID;														//Перетащить в Private, добавить метод для изменения
	GLuint cubemapTexture;

	OBJECT() {};

	/* id - уникальный идентификатор */
	/* material - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - зеркало, 3 - "цилиндр"*/
	OBJECT(int id, int material, const char *path)
	{
		ID = id;
		Material = material;
		loadOBJ(path, vertices, uvs, normals);
		/*switch (material)
		{
		case 0:
			PrepareSolidColor();
			break;
		case 1:
			PrepareGradientColor();
			break;
		case 2:
		{
			LoadShaders("shaders//Reflection.vertexshader", "shaders//Reflection.fragmentshader");
			PrepareReflectionRefraction();
		}
			break;
		case 3:
			PrepareCylinder();
			break;
		default:
			break;
		}*/
	}

	~OBJECT()
	{
		
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &tangentbuffer);
		glDeleteBuffers(1, &bitangentbuffer);
		glDeleteBuffers(1, &elementbuffer);
		glDeleteTextures(1, &DiffuseTexture);
		glDeleteTextures(1, &NormalTexture);
		glDeleteTextures(1, &SpecularTexture);	
	
		glDeleteVertexArrays(1, &VAO);
	}

	/* Загрузка вершинного и фрагментного шейдеров */
	bool LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
	{
		// Создание шейдеров
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER); // Вершинный
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); // Фрагментный

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
		printf("Compiling vertex shader: %s...\n", vertex_file_path);
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
		printf("Compiling fragment shader: %s...\n", fragment_file_path);
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
		printf("Linking program...\n");

		ShaderID = glCreateProgram();
		glAttachShader(ShaderID, VertexShaderID);
		glAttachShader(ShaderID, FragmentShaderID);
		glLinkProgram(ShaderID);

		// Проверка
		glGetProgramiv(ShaderID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ShaderID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("\nError: %s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(ShaderID, VertexShaderID);
		glDetachShader(ShaderID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return false;
	}

	/* Изменение размера объекта */
	void setScale(float size)
	{
		/* Если умножить, то радиус сферы -> 0 */
		ModelMatrix = scale(ModelMatrix, vec3(size, size, size));
	}

	/* Вращение объекта */
	void setRotation(float angle, vec3 axis)
	{
		ModelMatrix *= rotate(angle, axis);
	}

	/* Задаёт позицию объекта */
	void setPosition(float X, float Y, float Z)
	{
		ModelMatrix *= translate(vec3(X, Y, Z));
	}	

	/* возвращает позицию объекта */
	vec3 getPosition()
	{
		return vec3(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);
	}

	/* Задаёт цвет сплошной цвет объекта */
	void setSolidColor(float R, float G, float B)
	{
		solidcolor.x = R;
		solidcolor.y = G;
		solidcolor.z = B;
	}

	/* Material - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - зеркало, 3 - "цилиндр"*/
	void Prepare()
	{
		switch (Material)
		{
			case 0:
			{
				PrepareSolidColor();
				break;
			}
			case 1:
			{
				PrepareGradientColor();
				break;
			}
			case 2:
			{
				LoadShaders("shaders//Reflection.vertexshader", "shaders//Reflection.fragmentshader");
				PrepareReflectionRefraction();
				break;
			}
			case 3:
			{
				LoadShaders("shaders//Cylinder.vertexshader", "shaders//Cylinder.fragmentshader");
				PrepareCylinder();
				break;
			}
			default:
				break;
		}
	}

	void Render(vec3 campos, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		switch (Material)
		{
			case 0:
			{
				RenderSolidColor(ProjectionMatrix, ViewMatrix);
				break;
			}
			case 1:
			{
				RenderGradientColor(ProjectionMatrix, ViewMatrix);
				break;
			}
			case 2:
			{
				RenderReflectionRefraction(campos, ProjectionMatrix, ViewMatrix);
				break;
			}
			case 3:
			{
				RenderCylinder(ProjectionMatrix, ViewMatrix);
				break;
			}
			default:
				break;
		}
	}

	void PrepareAxes()
	{
		LoadShaders("shaders//GradientColor.vertexshader", "shaders//GradientColor.fragmentshader");

		MatrixID = glGetUniformLocation(ShaderID, "MVP");

		static const GLfloat vertexbuffer_data[] = {
			0.0f, 0.0f, 0.0f,
			3.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 3.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 3.0f,
		};

		static const GLfloat colorbuffer_data[] = {
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexbuffer_data), vertexbuffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorbuffer_data), colorbuffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		/* Вариант 2: координаты точек и их цвета хранятся в одном массиве
		static const GLfloat vertexcolorbuffer[] = {
		// XYZ, RGB для каждой точки
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
		};

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexcolorbuffer), vertexcolorbuffer, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);
		*/
	}

	void RenderAxes(mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glViewport(5, 5, 60, 60);

		mat4 ProjectionMatrix = perspective(radians(45.0f), 1.0f, 0.1f, 100.0f);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));

		glBindVertexArray(VAO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);

		glViewport(0, 0, WindowWidth, WindowHeight);
	}

	void PrepareSkyBox()
	{				
		LoadShaders("shaders//SkyBox.vertexshader", "shaders//SkyBox.fragmentshader");

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");			
		cubemapTextureID = glGetUniformLocation(ShaderID, "cubemap");

		GLfloat skyboxSide = 100.0;
		GLfloat skyboxVertices[] = {
			-skyboxSide,  skyboxSide, -skyboxSide,
			-skyboxSide, -skyboxSide, -skyboxSide,
			 skyboxSide, -skyboxSide, -skyboxSide,
			 skyboxSide, -skyboxSide, -skyboxSide,
			 skyboxSide,  skyboxSide, -skyboxSide,
			-skyboxSide,  skyboxSide, -skyboxSide,

			-skyboxSide, -skyboxSide,  skyboxSide,
			-skyboxSide, -skyboxSide, -skyboxSide,
			-skyboxSide,  skyboxSide, -skyboxSide,
			-skyboxSide,  skyboxSide, -skyboxSide,
			-skyboxSide,  skyboxSide,  skyboxSide,
			-skyboxSide, -skyboxSide,  skyboxSide,

			skyboxSide, -skyboxSide, -skyboxSide,
			skyboxSide, -skyboxSide,  skyboxSide,
			skyboxSide,  skyboxSide,  skyboxSide,
			skyboxSide,  skyboxSide,  skyboxSide,
			skyboxSide,  skyboxSide, -skyboxSide,
			skyboxSide, -skyboxSide, -skyboxSide,

			-skyboxSide, -skyboxSide,  skyboxSide,
			-skyboxSide,  skyboxSide,  skyboxSide,
			 skyboxSide,  skyboxSide,  skyboxSide,
			 skyboxSide,  skyboxSide,  skyboxSide,
			 skyboxSide, -skyboxSide,  skyboxSide,
			-skyboxSide, -skyboxSide,  skyboxSide,

			-skyboxSide,  skyboxSide, -skyboxSide,
			 skyboxSide,  skyboxSide, -skyboxSide,
			 skyboxSide,  skyboxSide,  skyboxSide,
			 skyboxSide,  skyboxSide,  skyboxSide,
			-skyboxSide,  skyboxSide,  skyboxSide,
			-skyboxSide,  skyboxSide, -skyboxSide,

			-skyboxSide, -skyboxSide, -skyboxSide,
			-skyboxSide, -skyboxSide,  skyboxSide,
			 skyboxSide, -skyboxSide, -skyboxSide,
			 skyboxSide, -skyboxSide, -skyboxSide,
			-skyboxSide, -skyboxSide,  skyboxSide,
			 skyboxSide, -skyboxSide,  skyboxSide
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindVertexArray(0);

		vector<const GLchar*> faces;
		faces.push_back("skybox//right.jpg");
		faces.push_back("skybox//left.jpg");
		faces.push_back("skybox//top.jpg");
		faces.push_back("skybox//bottom.jpg");
		faces.push_back("skybox//back.jpg");
		faces.push_back("skybox//front.jpg");
		cubemapTexture = loadCubemap(faces);
	}

	void RenderSkyBox(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glDepthFunc(GL_LEQUAL);

		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}	

private:
	/* Vertex Array Object */
	GLuint VAO;

	/* Матрицы */
	//mat4 ModelMatrix = mat4(1.0), ProjectionMatrix, ViewMatrix, ModelViewMatrix, MVP;
	mat4 ModelMatrix = mat4(1.0), ModelViewMatrix, MVP;
	mat3 ModelView3x3Matrix;

	/* Идентификаторы источника света, матриц и текстур для шейдеров*/
	GLuint CameraPosID, LightID, SolidColorID;
	GLuint MatrixID, ProjectionMatrixID, ViewMatrixID, ModelMatrixID, ModelView3x3MatrixID;
	GLuint DiffuseTextureID, NormalTextureID, SpecularTextureID, cubemapTextureID;
	/* Текстуры */
	GLuint DiffuseTexture, NormalTexture, SpecularTexture;
	/* Буферы */
	GLuint vertexbuffer, colorbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer, elementbuffer;

	vec3 solidcolor;
	vector<vec3> vertices, normals, tangents, bitangents;
	vector<vec2> uvs;
	vector<unsigned short> indices;

	/* Загрузка модели из obj-файла */
	bool loadOBJ(const char * path, vector<vec3> &vertices, vector<vec2> &uvs, vector<vec3> &normals)
	{
		printf("Loading OBJ file %s...\n", path);

		vector<int> vertexIndices, uvIndices, normalIndices;
		vector<vec3> tmp_vertices;
		vector<vec2> tmp_uvs;
		vector<vec3> tmp_normals;

		FILE *fin = fopen(path, "r");

		char buf[128];
		int i, res;

		if (fin == NULL)
		{
			printf("OBJ file not found.\n");
			getchar();
			return false;
		}

		while (true)
		{
			res = fscanf(fin, "%s", buf);

			if (res == EOF) break;

			if (strcmp(buf, "v") == 0)
			{
				vec3 vertex;
				fscanf(fin, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tmp_vertices.push_back(vertex);
			}
			else
			{
				if (strcmp(buf, "vt") == 0)
				{
					vec2 uv;
					fscanf(fin, "%f %f\n", &uv.x, &uv.y);
					uv.y = -uv.y; // Ивертируем для DDS текстуры.Убрать для TGA или BMP.
					tmp_uvs.push_back(uv);
				}
				else
				{
					if (strcmp(buf, "vn") == 0)
					{
						vec3 normal;
						fscanf(fin, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
						tmp_normals.push_back(normal);
					}
					else
					{
						if (strcmp(buf, "f") == 0)
						{
							int vertexIndex[3], uvIndex[3], normalIndex[3];

							int count = fscanf(fin, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

							if (count != 9)
							{
								printf("Can't read OBJ file.\n");
								return false;
							}

							vertexIndices.push_back(vertexIndex[0]);
							vertexIndices.push_back(vertexIndex[1]);
							vertexIndices.push_back(vertexIndex[2]);

							uvIndices.push_back(uvIndex[0]);
							uvIndices.push_back(uvIndex[1]);
							uvIndices.push_back(uvIndex[2]);

							normalIndices.push_back(normalIndex[0]);
							normalIndices.push_back(normalIndex[1]);
							normalIndices.push_back(normalIndex[2]);
						}
						else
						{
							char buf2[1000];
							fgets(buf2, 1000, fin);
						}
					}
				}
			}

		}

		//Для каждой вершины в каждом треугольнике
		for (i = 0; i < vertexIndices.size(); i++)
		{
			int vertexIndex = vertexIndices[i];
			int uvIndex = uvIndices[i];
			int normalIndex = normalIndices[i];

			vec3 vertex = tmp_vertices[vertexIndex - 1];
			vec2 uv = tmp_uvs[uvIndex - 1];
			vec3 normal = tmp_normals[normalIndex - 1];

			vertices.push_back(vertex);
			uvs.push_back(uv);
			normals.push_back(normal);
		}
		return true;
	}

	/* Вычисление касательных и бикасательных */
	void computeTangentBasis(vector<vec3> &vertices, vector<vec2> &uvs, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents)
	{
		int i;
		float r;

		vec3 v0, v1, v2, deltaPos1, deltaPos2, tangent, bitangent, normal;
		vec2 uv0, uv1, uv2, deltaUV1, deltaUV2;

		for (i = 0; i < vertices.size(); i += 3)
		{
			v0 = vertices[i + 0];
			v1 = vertices[i + 1];
			v2 = vertices[i + 2];

			uv0 = uvs[i + 0];
			uv1 = uvs[i + 1];
			uv2 = uvs[i + 2];

			// Postion delta
			deltaPos1 = v1 - v0;
			deltaPos2 = v2 - v0;

			// UV delta
			deltaUV1 = uv1 - uv0;
			deltaUV2 = uv2 - uv0;

			r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

			tangent = r*(deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
			bitangent = r*(deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);

			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);

			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);
		}

		for (i = 0; i < vertices.size(); i++)
		{
			normal = normals[i];
			tangent = tangents[i];
			bitangent = bitangents[i];

			// Ортогонализация
			tangent = normalize(tangent - normal * dot(normal, tangent));

			// Направление
			if (dot(cross(normal, tangent), bitangent) < 0.0f) tangent = tangent * -1.0f;
		}
	}

	/* Загрузка 6 cubemap текстур */
	/* Порядок: +X (право), -X (лево), +Y (верх), -Y (низ), +Z (перед), -Z (зад) */
	GLuint loadCubemap(vector<const GLchar*> faces)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);

		int i, width, height;
		unsigned char* image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (i = 0; i < faces.size(); i++)
		{
			image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return textureID;
	}

	void PrepareReflectionRefraction()
	{
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		CameraPosID = glGetUniformLocation(ShaderID, "cameraPos");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void RenderReflectionRefraction(vec3 camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniform3f(CameraPosID, camera.x, camera.y, camera.z);

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindVertexArray(0);
	}

	void PrepareSolidColor()
	{
		LoadShaders("shaders//SolidColor.vertexshader", "shaders//SolidColor.fragmentshader");

		MatrixID = glGetUniformLocation(ShaderID, "MVP");
		SolidColorID = glGetUniformLocation(ShaderID, "userColor");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void RenderSolidColor(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));
		glUniform3f(SolidColorID, solidcolor.x, solidcolor.y, solidcolor.z);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	void PrepareGradientColor()
	{
		LoadShaders("shaders//GradientColor.vertexshader", "shaders//GradientColor.fragmentshader");

		MatrixID = glGetUniformLocation(ShaderID, "MVP");
		SolidColorID = glGetUniformLocation(ShaderID, "userColor");

		GLfloat *colorbuffer_data = new GLfloat[vertices.size() * sizeof(vec3) * 3];

		for (int i = 0; i < vertices.size() * sizeof(vec3) * 3; i += 3)
		{
			if (i % 2 == 0)
			{
				colorbuffer_data[i] = 0.33;
				colorbuffer_data[i + 1] = 0.66;
				colorbuffer_data[i + 2] = 0.99;
			}
			else
			{
				colorbuffer_data[i] = 0.24;
				colorbuffer_data[i + 1] = 0.16;
				colorbuffer_data[i + 2] = 0.71;
			}
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * sizeof(vec3) * 3, colorbuffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void RenderGradientColor(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	void PrepareCylinder()
	{
		MatrixID = glGetUniformLocation(ShaderID, "MVP");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");
		ModelView3x3MatrixID = glGetUniformLocation(ShaderID, "MV3x3");

		DiffuseTexture = loadDDS("diffuse.DDS");
		NormalTexture = loadBMP_custom("normal.bmp");
		SpecularTexture = loadDDS("specular.DDS");

		DiffuseTextureID = glGetUniformLocation(ShaderID, "DiffuseTexture");
		NormalTextureID = glGetUniformLocation(ShaderID, "NormalTexture");
		SpecularTextureID = glGetUniformLocation(ShaderID, "SpecularTexture");
		LightID = glGetUniformLocation(ShaderID, "LightPosition_worldspace");

		computeTangentBasis(vertices, uvs, normals, tangents, bitangents);

		vector<vec3> indexed_vertices;
		vector<vec2> indexed_uvs;
		vector<vec3> indexed_normals;
		vector<vec3> indexed_tangents;
		vector<vec3> indexed_bitangents;
		indexVBO(vertices, uvs, normals, tangents, bitangents, indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(vec3), &indexed_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(vec2), &indexed_uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(vec3), &indexed_normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(vec3), &indexed_tangents[0], GL_STATIC_DRAW);

		glGenBuffers(1, &bitangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);
	}

	void RenderCylinder(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		ModelViewMatrix = ViewMatrix * ModelMatrix;
		ModelView3x3Matrix = mat3(ModelViewMatrix);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, value_ptr(ModelView3x3Matrix));

		vec3 lightPos = vec3(0.0, -7.0, 0.0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
		glUniform1i(DiffuseTextureID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NormalTexture);
		glUniform1i(NormalTextureID, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, SpecularTexture);
		glUniform1i(SpecularTextureID, 2);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

		glBindVertexArray(0);
	}
};

class SCENE
{
public:
	OBJECT Axes, Skybox;
	CAMERA camera;

	int ObjectsCount = 7;

	OBJECT *Objects = new OBJECT[ObjectsCount];

	SCENE() 
	{
		Axes = OBJECT();
		Axes.PrepareAxes();

		Skybox = OBJECT();
		Skybox.PrepareSkyBox();
		
		/*CUBE_Solid = OBJECT("3dmodels//cube.obj");
		CUBE_Solid.PrepareSolidColor();
		CUBE_Solid.SolidColor(0.9, 0.0, 0.5);
		CUBE_Solid.Move(0.0, 6.0, 3.0);

		CUBE_Gradient = OBJECT("3dmodels//cube.obj");
		CUBE_Gradient.PrepareGradientColor();
		CUBE_Gradient.Move(0.0, 3.0, 3.0);

		CUBE_Glass = OBJECT("3dmodels//cube.obj");
		CUBE_Glass.LoadShaders("shaders//Refraction.vertexshader", "shaders//Refraction.fragmentshader");
		CUBE_Glass.PrepareReflectionRefraction();
		CUBE_Glass.Move(0.0, 0.0, 3.0);

		CUBE_Mirror = OBJECT("3dmodels//cube.obj");
		CUBE_Mirror.LoadShaders("shaders//Reflection.vertexshader", "shaders//Reflection.fragmentshader");
		CUBE_Mirror.PrepareReflectionRefraction();
		CUBE_Mirror.Move(0.0, -3.0, 3.0);

		SPHERE_Solid = OBJECT("3dmodels//sphere.obj");
		SPHERE_Solid.PrepareSolidColor();
		SPHERE_Solid.SolidColor(0.6, 0.3, 0.9);
		SPHERE_Solid.Move(0.0, 6.0, 0.0);

		SPHERE_Gradient = OBJECT("3dmodels//sphere.obj");
		SPHERE_Gradient.PrepareGradientColor();
		SPHERE_Gradient.Move(0.0, 3.0, 0.0);

		SPHERE_Glass = OBJECT("3dmodels//sphere.obj");
		SPHERE_Glass.LoadShaders("shaders//Refraction.vertexshader", "shaders//Refraction.fragmentshader");
		SPHERE_Glass.PrepareReflectionRefraction();

		SPHERE_Mirror = OBJECT("3dmodels//sphere.obj");
		SPHERE_Mirror.LoadShaders("shaders//Reflection.vertexshader", "shaders//Reflection.fragmentshader");
		SPHERE_Mirror.PrepareReflectionRefraction();
		SPHERE_Mirror.Move(0.0, -3.0, 0.0);

		CYLINDER = OBJECT("3dmodels//cylinder.obj");
		CYLINDER.LoadShaders("shaders//Cylinder.vertexshader", "shaders//Cylinder.fragmentshader");
		CYLINDER.PrepareCylinder();
		CYLINDER.Move(0.0, -7.0, -3.0);

		CYLINDER_Solid = OBJECT("3dmodels//cylinder.obj");
		CYLINDER_Solid.PrepareSolidColor();
		CYLINDER_Solid.SolidColor(0.1, 0.9, 0.8);
		CYLINDER_Solid.Move(0.0, 5.0, -3.0);

		CYLINDER_Gradient = OBJECT("3dmodels//cylinder.obj");
		CYLINDER_Gradient.PrepareGradientColor();
		CYLINDER_Gradient.Move(0.0, 2.0, -3.0);

		CYLINDER_Glass = OBJECT("3dmodels//cylinder.obj");
		CYLINDER_Glass.LoadShaders("shaders//Refraction.vertexshader", "shaders//Refraction.fragmentshader");
		CYLINDER_Glass.PrepareReflectionRefraction();
		CYLINDER_Glass.Move(0.0, -1.0, -3.0);

		CYLINDER_Mirror = OBJECT("3dmodels//cylinder.obj");
		CYLINDER_Mirror.LoadShaders("shaders//Reflection.vertexshader", "shaders//Reflection.fragmentshader");
		CYLINDER_Mirror.PrepareReflectionRefraction();
		CYLINDER_Mirror.Move(0.0, -4.0, -3.0);

		SKYBOX = OBJECT();
		SKYBOX.LoadShaders("shaders//SkyBox.vertexshader", "shaders//SkyBox.fragmentshader");
		SKYBOX.PrepareSkyBox();

		CUBE_Glass.cubemapTexture = SKYBOX.cubemapTexture;
		CUBE_Mirror.cubemapTexture = SKYBOX.cubemapTexture;

		SPHERE_Glass.cubemapTexture = SKYBOX.cubemapTexture;
		SPHERE_Mirror.cubemapTexture = SKYBOX.cubemapTexture;

		CYLINDER_Glass.cubemapTexture = SKYBOX.cubemapTexture;
		CYLINDER_Mirror.cubemapTexture = SKYBOX.cubemapTexture;*/

		Objects[0] = OBJECT(0, 0, "3dmodels//cube.obj");
		Objects[0].Prepare();
		Objects[0].setSolidColor(0.9, 0.0, 0.5);
		Objects[0].setPosition(0.0, 0.0, 10.0);

		Objects[1] = OBJECT(1, 0, "3dmodels//cube.obj");
		Objects[1].Prepare();
		Objects[1].setSolidColor(1.0, 0.6, 0.5);
		Objects[1].setPosition(0.0, 0.0, -10.0);

		Objects[2] = OBJECT(2, 0, "3dmodels//cube.obj");
		Objects[2].Prepare();
		Objects[2].setSolidColor(0.3, 0.8, 0.5);
		Objects[2].setPosition(5.0, 0.0, 0.0);

		Objects[3] = OBJECT(3, 0, "3dmodels//cube.obj");
		Objects[3].Prepare();
		Objects[3].setSolidColor(0.5, 0.0, 0.9);
		Objects[3].setPosition(-5.0, 0.0, 0.0);

		Objects[4] = OBJECT(4, 0, "3dmodels//cube.obj");
		Objects[4].Prepare();
		Objects[4].setSolidColor(0.1, 0.3, 0.5);
		Objects[4].setPosition(0.0, 15.0, 0.0);

		Objects[5] = OBJECT(5, 0, "3dmodels//cube.obj");
		Objects[5].Prepare();
		Objects[5].setSolidColor(0.5, 0.8, 0.9);
		Objects[5].setPosition(0.0, -15.0, 0.0);

		Objects[6] = OBJECT(6, 2, "3dmodels//sphere.obj");
		Objects[6].Prepare();
		Objects[6].setScale(2.0);

		/*Objects[0] = OBJECT(0, 2, "3dmodels//sphere.obj");
		Objects[0].Prepare();
		Objects[0].setPosition(0.0, 0.0, 3.0);
		Objects[0].setScale(2.0);

		Objects[1] = OBJECT(1, 2, "3dmodels//sphere.obj");
		Objects[1].Prepare();
		Objects[1].setPosition(0.0, 0.0, -3.0);
		Objects[1].setScale(2.0);
		Objects[1].cubemapTexture = SKYBOX.cubemapTexture;*/
	};

	~SCENE() 
	{
		/*glDeleteProgram(CUBE_Solid.ShaderID);
		glDeleteProgram(CUBE_Gradient.ShaderID);
		glDeleteProgram(CUBE_Glass.ShaderID);
		glDeleteProgram(CUBE_Mirror.ShaderID);

		glDeleteProgram(SPHERE_Solid.ShaderID);
		glDeleteProgram(SPHERE_Gradient.ShaderID);
		glDeleteProgram(SPHERE_Glass.ShaderID);
		glDeleteProgram(SPHERE_Mirror.ShaderID);

		glDeleteProgram(CYLINDER_Solid.ShaderID);
		glDeleteProgram(CYLINDER_Gradient.ShaderID);
		glDeleteProgram(CYLINDER_Glass.ShaderID);
		glDeleteProgram(CYLINDER_Mirror.ShaderID);
		glDeleteProgram(CYLINDER.ShaderID);*/

		for (int i = 0; i < ObjectsCount; i++)
		{
			glDeleteProgram(Objects[i].ShaderID);
		}

		glDeleteProgram(Skybox.ShaderID);
		glDeleteProgram(Axes.ShaderID);
	};

	void Render()
	{		
		camPos = camera.ComputeViewMatrix(window, cameramode);
		ViewMatrix = camera.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*CUBE_Solid.RenderSolidColor(ProjectionMatrix, ViewMatrix);
		CUBE_Gradient.RenderGradientColor(ProjectionMatrix, ViewMatrix);
		CUBE_Glass.RenderReflectionRefraction(camera, ProjectionMatrix, ViewMatrix);
		CUBE_Mirror.RenderReflectionRefraction(camera, ProjectionMatrix, ViewMatrix);

		SPHERE_Solid.RenderSolidColor(ProjectionMatrix, ViewMatrix);
		SPHERE_Gradient.RenderGradientColor(ProjectionMatrix, ViewMatrix);
		SPHERE_Glass.RenderReflectionRefraction(camera, ProjectionMatrix, ViewMatrix);
		SPHERE_Mirror.RenderReflectionRefraction(camera, ProjectionMatrix, ViewMatrix);

		CYLINDER_Solid.RenderSolidColor(ProjectionMatrix, ViewMatrix);
		CYLINDER_Gradient.RenderGradientColor(ProjectionMatrix, ViewMatrix);
		CYLINDER_Glass.RenderReflectionRefraction(camera, ProjectionMatrix, ViewMatrix);
		CYLINDER_Mirror.RenderReflectionRefraction(camera, ProjectionMatrix, ViewMatrix);
		CYLINDER.RenderCylinder(ProjectionMatrix, ViewMatrix);

		SKYBOX.RenderSkyBox(ProjectionMatrix, ViewMatrix);

		CUBE_Solid.Rotate(-CUBEangle, vec3(0.0, 1.0, 1.0));
		CUBE_Gradient.Rotate(CUBEangle, vec3(1.0, 1.0, 0.0));
		CUBE_Glass.Rotate(-CUBEangle, vec3(1.0, 0.0, 0.0));
		CUBE_Mirror.Rotate(CUBEangle, vec3(1.0, 0.0, 0.0));

		//SPHERE_Glass.Resize(SPHEREsize);

		CYLINDER_Solid.Rotate(CYLINDERangle, vec3(0.0, 1.0, 0.0));
		CYLINDER_Gradient.Rotate(-CYLINDERangle, vec3(0.0, 1.0, 0.0));
		CYLINDER_Glass.Rotate(CYLINDERangle, vec3(0.0, 1.0, 0.0));
		CYLINDER_Mirror.Rotate(-CYLINDERangle, vec3(0.0, 1.0, 0.0));
		CYLINDER.Rotate(CYLINDERangle, vec3(0.0, 1.0, 0.0));

		AXES.RenderAxes(ViewMatrix);

		if ((SPHEREsize > SPHEREsizeMin) && SPHEREdecreaseSize) SPHEREsize -= SPHEREsizeDelta;
		else
		{
			SPHEREdecreaseSize = false;
			SPHEREincreaseSize = true;
		}

		if ((SPHEREsize < SPHEREsizeMax) && SPHEREincreaseSize) SPHEREsize += SPHEREsizeDelta;
		else
		{
			SPHEREdecreaseSize = true;
			SPHEREincreaseSize = false;
		}*/

		ProjectionMatrix = perspective(radians(FOV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);

		for (int i = 0; i < ObjectsCount; i++)
		{
			if (Objects[i].Material == 2)
			{			
				Objects[i].cubemapTexture = PrepareCubemap(i, camPos, ViewMatrix);
			}
		}

		for (int i = 0; i < ObjectsCount; i++)
		{
			Objects[i].Render(camPos, ProjectionMatrix, ViewMatrix);
			if (Objects[i].Material == 2)
			{
				glDeleteTextures(1, &(Objects[i].cubemapTexture));
			}
		}

		Skybox.RenderSkyBox(ProjectionMatrix, ViewMatrix);
		Axes.RenderAxes(ViewMatrix);

		/*Objects[0].setRotation(CUBEangle, vec3(0.0, 1.0, 1.0));
		Objects[1].setRotation(-CUBEangle, vec3(0.0, 1.0, 1.0));
		Objects[2].setRotation(CUBEangle, vec3(0.0, 1.0, 1.0));
		Objects[3].setRotation(-CUBEangle, vec3(0.0, 1.0, 1.0));
		Objects[4].setRotation(CUBEangle, vec3(0.0, 1.0, 1.0));*/
		//Objects[5].setRotation(-CUBEangle, vec3(0.0, 1.0, 1.0));
	}

private:
	bool SPHEREdecreaseSize = true, SPHEREincreaseSize = false;
	float SPHEREsize = 1.0, SPHEREsizeDelta = 0.006, SPHEREsizeMin = 0.8, SPHEREsizeMax = 1.0;
	float CUBEangle = 0.05, CYLINDERangle = -0.01;

	vec3 camPos;
	mat4 ProjectionMatrix, ViewMatrix;

	GLuint VAO;
	GLuint ShaderID;
	GLuint timeID, texID;
	GLuint framebuffer, renderedTexture, depthbuffer;
	//GLuint cubemap;

	bool LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
	{
		// Создание шейдеров
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER); // Вершинный
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); // Фрагментный

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
		printf("Compiling vertex shader: %s...\n", vertex_file_path);
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
		printf("Compiling fragment shader: %s...\n", fragment_file_path);
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
		printf("Linking program...\n");

		ShaderID = glCreateProgram();
		glAttachShader(ShaderID, VertexShaderID);
		glAttachShader(ShaderID, FragmentShaderID);
		glLinkProgram(ShaderID);

		// Проверка
		glGetProgramiv(ShaderID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ShaderID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("\nError: %s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(ShaderID, VertexShaderID);
		glDetachShader(ShaderID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return true;
	}

	/*void PrepareRenderToTexture()
	{
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenRenderbuffers(1, &depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 800);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("Framebuffer problem.\n");
		else printf("Framebuffer OK.\n");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// The fullscreen quad's FBO
		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint quad_vertexbuffer;
		glGenBuffers(1, &quad_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		// Create and compile our GLSL program from the shaders
		LoadShaders("shaders//RenderToTexture.vertexshader", "shaders//RenderToTexture.fragmentshader");
		texID = glGetUniformLocation(ShaderID, "renderedTexture");
		timeID = glGetUniformLocation(ShaderID, "time");
	}

	void RenderToTexture(vec3 camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, WindowWidth, WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CUBE_Solid.RenderSolidColor(ProjectionMatrix, ViewMatrix);
		CUBE_Mirror.RenderReflectionRefraction(camera, ProjectionMatrix, ViewMatrix);
		SKYBOX.RenderSkyBox(ProjectionMatrix, ViewMatrix);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, WindowWidth, WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glUniform1i(texID, 0);
		glUniform1f(timeID, (float)(glfwGetTime()*10.0f));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	*/

	GLuint PrepareCubemap(int id, vec3 camera, mat4 ViewMatrix)
	{
		GLuint cubemap;

		/*glDeleteTextures(1, &cubemap);
		glDeleteRenderbuffers(1, &depthbuffer);
		glDeleteFramebuffers(1, &framebuffer);*/

		mat4 ProjectionMatrix = perspective(radians(90.0f), 1.0f, 0.1f, 100.0f);
		//glEnable(GL_CULL_FACE);
		//glDepthFunc(GL_LEQUAL);

		glGenTextures(1, &cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureSize, textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		for (int i = 0; i < 6; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		}

		glGenRenderbuffers(1, &depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureSize, textureSize);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("Framebuffer init problem.\n");

		for (int i = 0; i < 6; i++)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("Framebuffer face %d problem.\n", i);

			/*
			GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right	0
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left	1
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top		2
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom	3
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back	4
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front	5
			*/

			switch (i)
			{
			case 0:
				//ViewMatrix = lookAt(vec3(0.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
				break;
			case 1:
				//ViewMatrix = lookAt(vec3(0.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
				break;
			case 2:
				//ViewMatrix = lookAt(vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
				break;
			case 3:
				//ViewMatrix = lookAt(vec3(0.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0));
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0));
				break;
			case 4:
				//ViewMatrix = lookAt(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0));
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0));
				break;
			case 5:
				//ViewMatrix = lookAt(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0));
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0));
			default:
				break;
			};

			glViewport(0, 0, textureSize, textureSize);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int j = 0; j < ObjectsCount; j++)
			{
				if (Objects[j].ID != id)
				{
					Objects[j].Render(camera, ProjectionMatrix, ViewMatrix);
				}
			}
			Skybox.RenderSkyBox(ProjectionMatrix, ViewMatrix);
		}
		glViewport(0, 0, WindowWidth, WindowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteRenderbuffers(1, &depthbuffer);
		glDeleteFramebuffers(1, &framebuffer);

		return cubemap;
	}
};

/* Обработка ошибок */
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* Обработка клавиатуры */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
	{ 
		cameramode++;
		if (cameramode > 2) cameramode = 1;
	}
	
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) cameramode = 3;

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) 
	{ 
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = true;
		}
	}
}

/* Вывод ошибок в консоль */
void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
{
	printf("OpenGL Debug Output message : ");

	if (source == GL_DEBUG_SOURCE_API_ARB)					printf("Source : API; ");
	else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)	printf("Source : WINDOW_SYSTEM; ");
	else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)	printf("Source : SHADER_COMPILER; ");
	else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)		printf("Source : THIRD_PARTY; ");
	else if (source == GL_DEBUG_SOURCE_APPLICATION_ARB)		printf("Source : APPLICATION; ");
	else if (source == GL_DEBUG_SOURCE_OTHER_ARB)			printf("Source : OTHER; ");

	if (type == GL_DEBUG_TYPE_ERROR_ARB)					printf("Type : ERROR; ");
	else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)	printf("Type : DEPRECATED_BEHAVIOR; ");
	else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)	printf("Type : UNDEFINED_BEHAVIOR; ");
	else if (type == GL_DEBUG_TYPE_PORTABILITY_ARB)			printf("Type : PORTABILITY; ");
	else if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB)			printf("Type : PERFORMANCE; ");
	else if (type == GL_DEBUG_TYPE_OTHER_ARB)				printf("Type : OTHER; ");

	if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)				printf("Severity : HIGH; ");
	else if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)		printf("Severity : MEDIUM; ");
	else if (severity == GL_DEBUG_SEVERITY_LOW_ARB)			printf("Severity : LOW; ");

	printf("Message : %s\n", message);
}

void main()
{
	int nbFrames = 0;
	double currentTime, lastTime;
	char text[100];

	glfwSetErrorCallback(error_callback);								//Обработка ошибок

	if (!glfwInit())
	{
		printf("Can't init GLFW.\n");
		getchar();
	}

	glfwWindowHint(GLFW_SAMPLES, 4);									// 4x сглаживание
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);						// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
	
	window = glfwCreateWindow(WindowWidth, WindowHeight, "Diploma", NULL, NULL);	// Ширина, высота, название окна, монитор (Fullscrean, NUll - оконный, обмен ресурсами с окном (NULL - нет такого))
	
	const GLFWvidmode *Screen = glfwGetVideoMode(glfwGetPrimaryMonitor());						// Информация об экране, разрешение
	
	glfwSetWindowPos(window, Screen->width / 2 - WindowWidth / 2, Screen->height / 2 - WindowHeight / 2);	// Окно в центр экрана

	if (!window)
	{
		printf("Can't open GLFW window.\n");
		getchar();
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);							// Обработка клавиатуры

	glewExperimental = true;
	if (glewInit() != GLEW_OK) 
	{
		printf("Can't init GLEW.\n");
		getchar();
		glfwTerminate();
	}

	if (GLEW_ARB_debug_output) 
	{
		printf("OpenGL debug output Ok.\n");
		glDebugMessageCallbackARB(&DebugOutputCallback, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	}
	else 
	{
		printf("OpenGL debug output doesn't work.\n");
	}
	
	glViewport(0, 0, WindowWidth, WindowHeight);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//Скрыть курсор
	glfwSetCursorPos(window, WindowWidth / 2, WindowHeight / 2);
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);								// Цвет фона, RGBA		
	glEnable(GL_DEPTH_TEST);											// Включаем буфер глубины	
	glDepthFunc(GL_LESS);												// Выбираем фрагмент, ближайший к камере						  
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);												
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	SCENE Scene = SCENE();

	initText2D("Text.DDS");

	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		nbFrames++;
		char text2[30];

		if (currentTime - lastTime >= 0.01)
		{
			//sprintf(text, "%.3f ms for frame. %d frames\n", 1000.0 / double(nbFrames), nbFrames);
			sprintf(text, "%d", nbFrames);
			sprintf(text2, "Diploma at %d FPS", nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwSetWindowTitle(window, text2);

		glfwPollEvents();

		Scene.Render();
		//printText2D(text, 0, 568, 20);

		glfwSwapBuffers(window); // Меняем буферы															  
	}

	// Чистка
	cleanupText2D();
		
	glfwTerminate(); // Закрываем OpenGL окно и убиваем GLFW
}
