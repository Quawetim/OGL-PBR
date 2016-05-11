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
#include "ViewMatrix.h"
#include "VboIndexer.h"
#include "Text2D.h"

int cameramode = 2;
bool wireframe = false;

class OBJECT
{
public:
	GLuint ShaderID;
	GLuint  cubemapTexture;

	OBJECT() {};

	OBJECT(const char * path)
	{
		loadOBJ(path, vertices, uvs, normals);
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
	void Resize(float size)
	{
		ModelMatrix = scale(vec3(size));
	}

	/* Вращение объекта */
	void Rotate(float angle, vec3 axis)
	{
		ModelMatrix *= rotate(angle, axis);
	}

	/* Перемещение объекта */
	void Move(float X, float Y, float Z)
	{
		ModelMatrix *= translate(vec3(X, Y, Z));
	}	

	void PrepareAxes()
	{
		LoadShaders("shaders//Axes.vertexshader", "shaders//Axes.fragmentshader");

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexbuffer_data), vertexbuffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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

	void RenderAxes()
	{
		int width, height;

		glfwGetWindowSize(window, &width, &height);

		glUseProgram(ShaderID);

		glViewport(5, 5, 60, 60);

		ComputeViewMatrix(window, cameramode);
		ProjectionMatrix = perspective(45.0f, 1.0f, 0.1f, 100.0f);
		ViewMatrix = getViewMatrixAxes();
		ModelViewMatrix = ViewMatrix * ModelMatrix;
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glBindVertexArray(VAO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);

		glViewport(0, 0, width, height);
	}

	void PrepareSkyBox()
	{				
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
		faces.push_back("skybox/right.jpg");
		faces.push_back("skybox/left.jpg");
		faces.push_back("skybox/top.jpg");
		faces.push_back("skybox/bottom.jpg");
		faces.push_back("skybox/back.jpg");
		faces.push_back("skybox/front.jpg");
		cubemapTexture = loadCubemap(faces);
	}

	void RenderSkyBox()
	{
		glDepthFunc(GL_LEQUAL);

		glUseProgram(ShaderID);
		
		ComputeViewMatrix(window, cameramode);
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelViewMatrix = ViewMatrix * ModelMatrix;
		ModelView3x3Matrix = mat3(ModelViewMatrix);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
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

	void RenderReflectionRefraction()
	{
		glUseProgram(ShaderID);

		vec3 camera = ComputeViewMatrix(window, cameramode);
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelViewMatrix = ViewMatrix * ModelMatrix;
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniform3f(CameraPosID, camera.x, camera.y, camera.z);

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
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

	void RenderCylinder()
	{
		glUseProgram(ShaderID);

		ComputeViewMatrix(window, cameramode);
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelViewMatrix = ViewMatrix * ModelMatrix;
		ModelView3x3Matrix = mat3(ModelViewMatrix);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, value_ptr(ModelView3x3Matrix));

		vec3 lightPos = vec3(0.0, 0.0, 0.0);
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

private:
	mat4 ModelMatrix = mat4(1.0), ProjectionMatrix, ViewMatrix, ModelViewMatrix, MVP;
	mat3 ModelView3x3Matrix;

	/* Vertex Array Object */
	GLuint VAO;

	/* Идентификаторы источника света, матриц и текстур для шейдеров*/
	GLuint CameraPosID, LightID, MatrixID, ProjectionMatrixID, ViewMatrixID, ModelMatrixID, ModelView3x3MatrixID, DiffuseTextureID, NormalTextureID, SpecularTextureID, cubemapTextureID;
	/* Текстуры */
	GLuint DiffuseTexture, NormalTexture, SpecularTexture;
	/* Буферы */
	GLuint vertexbuffer, colorbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer, elementbuffer;

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
};

class SCENE
{
public:
	OBJECT CUBE, SPHERE, CYLINDER, SKYBOX;

	SCENE() 
	{
		CUBE = OBJECT("3dmodels//cube.obj");
		CUBE.LoadShaders("shaders//Refraction.vertexshader", "shaders//Refraction.fragmentshader");
		CUBE.PrepareReflectionRefraction();
		CUBE.Move(0.0, 0.0, 3.0);

		SPHERE = OBJECT("3dmodels//sphere.obj");
		SPHERE.LoadShaders("shaders//Reflection.vertexshader", "shaders//Reflection.fragmentshader");
		SPHERE.PrepareReflectionRefraction();

		CYLINDER = OBJECT("3dmodels//cylinder.obj");
		CYLINDER.LoadShaders("shaders//Cylinder.vertexshader", "shaders//Cylinder.fragmentshader");
		CYLINDER.PrepareCylinder();
		CYLINDER.Move(0.0, -1.0, -3.0);

		SKYBOX = OBJECT();
		SKYBOX.LoadShaders("shaders//SkyBox.vertexshader", "shaders//SkyBox.fragmentshader");
		SKYBOX.PrepareSkyBox();

		CUBE.cubemapTexture = SKYBOX.cubemapTexture;
		SPHERE.cubemapTexture = SKYBOX.cubemapTexture;
	};

	~SCENE() 
	{
		glDeleteProgram(CUBE.ShaderID);
		glDeleteProgram(SPHERE.ShaderID);
		glDeleteProgram(CYLINDER.ShaderID);
		glDeleteProgram(SKYBOX.ShaderID);
	};

	void Render()
	{
		CUBE.RenderReflectionRefraction();
		SPHERE.RenderReflectionRefraction();
		CYLINDER.RenderCylinder();
		SKYBOX.RenderSkyBox();

		SPHERE.Resize(SPHEREsize);
		CUBE.Rotate(CUBEangle, vec3(0.0, 1.0, 1.0));
		CYLINDER.Rotate(CYLINDERangle, vec3(0.0, 1.0, 0.0));

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
		}

	}
private:
	bool SPHEREdecreaseSize = true, SPHEREincreaseSize = false;
	float SPHEREsize = 1.0, SPHEREsizeDelta = 0.006, SPHEREsizeMin = 0.8, SPHEREsizeMax = 1.0;
	float CUBEangle = 0.05, CYLINDERangle = -0.01;
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

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { cameramode = 1; }	
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) { cameramode = 2; }
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) { cameramode = 3; }

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
	int nbFrames = 0, width = 1280, height = 800;
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

	window = glfwCreateWindow(width, height, "Diploma", NULL, NULL);	// Ширина, высота, название окна, монитор (Fullscrean, NUll - оконный, обмен ресурсами с окном (NULL - нет такого))

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
	
	glViewport(0, 0, width, height);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//Скрыть курсор
	glfwSetCursorPos(window, width / 2, height / 2);
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);								// Цвет фона, RGBA		
	glEnable(GL_DEPTH_TEST);											// Включаем буфер глубины	
	glDepthFunc(GL_LESS);												// Выбираем фрагмент, ближайший к камере						  
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);												
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	SCENE Scene = SCENE();

	OBJECT Axes = OBJECT();
	Axes.PrepareAxes();	

	initText2D("Text.DDS");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);						// Сетка
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистили экран		

		Scene.Render();
		Axes.RenderAxes();
		printText2D(text, 0, 568, 20);

		glfwSwapBuffers(window); // Меняем буферы															  
	}

	// Чистка
	cleanupText2D();
		
	glfwTerminate(); // Закрываем OpenGL окно и убиваем GLFW
}
