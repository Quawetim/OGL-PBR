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

/* WindowWidth, WindowHeight - ширина и высота окна */
/* FOV - field of view */
/* CameraMode - выбранная камера */
/* GenTextureSize - размер генерируемой текстуры */
/* Wireframe - отображение сетки объектов, переключение по F1 */
/* MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map */
int WindowWidth = 1280, WindowHeight = 800;
float FOV = 90.0;
int CameraMode = 2;
int GenTextureSize = 512;
bool Wireframe = false;
bool MirrorExample = true;
bool FullSceen = true;

class CAMERA
{
private:
	/* Position - позиция камеры */
	/* Radius, RadiusMin, RadiusMax - радиус полёта камеры от начала координат для камеры №2 и его минимальное и максимальное значения */
	/* CameraHeight - высота полёта камеры №2 */
	/* HorizontalAngle, VerticalAngle - uоризонтальный и вертикальный углы для камеры №2 */
	/* Speed, Speed2 - скорость движения камер №1 и №2  */
	/* MouseSpeed - скорость движения мышки */
	vec3 Position;
	float Pi = pi<float>();
	float DeltaTime, Radius = 20.0, RadiusMin = 2.0, RadiusMax = 100.0, CameraHeight = 1.0;
	float HorizontalAngle = 0.0, VerticalAngle = 0.0;
	float Speed = 6.0, Speed2 = 6.0, MouseSpeed = 0.005;

	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	/* ViewMatrix - матрица вида для координатных осей */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* Обработка клавиатуры для движения камеры №2 */
	/* window - указатель на окно */
	void checkmove(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { if (Radius >= RadiusMin) Radius -= DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { if (Radius <= RadiusMax) Radius += DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { if (CameraHeight <= 10.0) CameraHeight += DeltaTime * Speed; }
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { if (CameraHeight >= -10.0) CameraHeight -= DeltaTime * Speed; }
	}

	/* Обработка клавиатуры для движения камеры №1 */
	/* window - указатель на окно */
	/* direction - направление камеры */
	/* right - вектор "вправо" для камеры */
	void checkmove(GLFWwindow* window, vec3 Direction, vec3 Right)
	{
		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) { if (Speed2 < 20.0) Speed2 += 0.2; }
		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) { if (Speed2 > 1.0) Speed2 -= 0.2; }
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { Position += Direction * DeltaTime * Speed2; }
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { Position -= Direction * DeltaTime * Speed2; }
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { Position += Right * DeltaTime * Speed2; }
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { Position -= Right * DeltaTime * Speed2; }
	}

public:
	/* Возвращает матрицу проекции */
	mat4 getProjectionMatrix() { return ProjectionMatrix; }
	/* Возвращает матрицу вида */
	mat4 getViewMatrix() { return ViewMatrix; }
	/* Возвращает матрицу вида для координатных осей */
	mat4 getViewMatrixAxes() { return ViewMatrixAxes; }

	/* Вычисление матриц вида и проекции, возвращает позицию камеры */
	/* window - указатель окна */
	/* CameraMode - выбранная камера */
	vec3 ComputeViewMatrix(GLFWwindow* window, int CameraMode)
	{
		double CurrentTime, MouseX, MouseY;
		static double LastTime = glfwGetTime();

		CurrentTime = glfwGetTime();
		DeltaTime = float(CurrentTime - LastTime);

		glfwGetCursorPos(window, &MouseX, &MouseY);
		glfwSetCursorPos(window, WindowWidth / 2.0, WindowHeight / 2.0);

		HorizontalAngle += MouseSpeed * float(WindowWidth / 2.0 - MouseX);
		VerticalAngle += MouseSpeed * float(WindowHeight / 2.0 - MouseY);

		if (CameraMode == 1)
		{
			vec3 Direction, Right;

			// Direction : Spherical coordinates to Cartesian coordinates conversion
			Direction = vec3(cos(VerticalAngle) * sin(HorizontalAngle), sin(VerticalAngle), cos(VerticalAngle) * cos(HorizontalAngle));
			Right = vec3(sin(HorizontalAngle - Pi / 2.0), 0, cos(HorizontalAngle - Pi / 2.0));

			checkmove(window, Direction, Right);

			ViewMatrix = lookAt(Position, Position + Direction, vec3(0.0, 1.0, 0.0));

			ViewMatrixAxes = ViewMatrix;
		}

		if (CameraMode == 2)
		{
			Position = vec3(Radius*cos(HorizontalAngle), CameraHeight, Radius*sin(HorizontalAngle));

			checkmove(window);

			ViewMatrix = lookAt(Position, vec3(0.0, -1.5, 0.0), vec3(0.0, 1.0, 0.0));

			ViewMatrixAxes = lookAt(vec3(5 * cos(HorizontalAngle), CameraHeight, 5 * sin(HorizontalAngle)), vec3(0.0, -1.5, 0.0), vec3(0.0, 1.0, 0.0));
		}

		if (CameraMode == 3)
		{
			ViewMatrix = lookAt(vec3(5.0, 3.0, 4.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

			ViewMatrixAxes = ViewMatrix;
		}

		ProjectionMatrix = perspective(radians(FOV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f); 	
		LastTime = CurrentTime;

		return Position;
	}
};

class OBJECT
{
private:
	/* Material - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - "цилиндр"*/
	int Material;

	/* Vertex Array Object */
	GLuint VAO;

	/* ModelMatrix - матрица модели */
	mat4 ModelMatrix = mat4(1.0), ModelViewMatrix, MVP;
	mat3 ModelView3x3Matrix;

	/* Идентификаторы шейдера, источника света, матриц и текстур для шейдеров*/
	GLuint ShaderID, CameraPosID, LightID, UserSolidColorID;
	GLuint MatrixID, ProjectionMatrixID, ViewMatrixID, ModelMatrixID, ModelView3x3MatrixID;
	GLuint DiffuseTextureID, NormalTextureID, SpecularTextureID, cubemapTextureID;
	/* Текстуры */
	GLuint DiffuseTexture, SpecularTexture, NormalTexture, CubeMapTexture;
	/* Буферы */
	GLuint vertexbuffer, colorbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer, elementbuffer;

	/* UserSolidColor - пользовательский цвет объекта */
	vec3 UserSolidColor;
	vector<vec3> vertices, normals, tangents, bitangents;
	vector<vec2> uvs;
	vector<unsigned short> indices;

	/* Подготовка данных для объекта сплошного цвета */
	void PrepareSolidColor()
	{
		LoadShaders("shaders//SolidColor.vertexshader", "shaders//SolidColor.fragmentshader");

		MatrixID = glGetUniformLocation(ShaderID, "MVP");
		UserSolidColorID = glGetUniformLocation(ShaderID, "userColor");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	/* Рендеринг объекта сплошного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSolidColor(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));
		glUniform3f(UserSolidColorID, UserSolidColor.x, UserSolidColor.y, UserSolidColor.z);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для объекта градиентного цвета */
	void PrepareGradientColor()
	{
		LoadShaders("shaders//GradientColor.vertexshader", "shaders//GradientColor.fragmentshader");

		MatrixID = glGetUniformLocation(ShaderID, "MVP");
		UserSolidColorID = glGetUniformLocation(ShaderID, "userColor");

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

	/* Рендеринг объекта градиентного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderGradientColor(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для стекла/зеркала */
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

	/* Рендеринг стекла/зеркала */
	/* Camera - позиция камеры */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderReflectionRefraction(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniform3f(CameraPosID, Camera.x, Camera.y, Camera.z);

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для цилиндра с картой нормалей */
	void PrepareCylinder()
	{
		LoadShaders("shaders//Cylinder.vertexshader", "shaders//Cylinder.fragmentshader");

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

		ComputeTBT(vertices, uvs, normals, tangents, bitangents);

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

	/* Рендеринг цилиндра с картой нормалей */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
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

		vec3 LightPos = vec3(0.0, -7.0, 0.0);
		glUniform3f(LightID, LightPos.x, LightPos.y, LightPos.z);

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

	/* Загрузка модели из obj-файла, возвращает успех/ошибку */
	/* Path - путь к файлу */
	/* Vertices - список координат вершин, возвращаемое значение */
	/* UVs - список текстурных координат вершин, возвращаемое значение */
	/* Normals - список нормалей вершин, возвращаемое значение */
	bool LoadOBJ(const char *Path, vector<vec3> &Vertices, vector<vec2> &UVs, vector<vec3> &Normals)
	{
		char Buf[128];
		vector<int> VertexIndices, UVIndices, NormalIndices;
		vector<vec3> tmp_Vertices;
		vector<vec2> tmp_UVs;
		vector<vec3> tmp_Normals;

		printf("Loading OBJ file %s...\n", Path);		

		FILE *Fin = fopen(Path, "r");		

		if (Fin == NULL)
		{
			printf("OBJ file not found.\n");
			return false;
		}

		while (true)
		{
			if (fscanf(Fin, "%s", Buf) == EOF) break;

			if (strcmp(Buf, "v") == 0)
			{
				/* Координаты вершины */
				vec3 vertex;

				fscanf(Fin, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tmp_Vertices.push_back(vertex);
			}
			else
			{
				if (strcmp(Buf, "vt") == 0)
				{
					/* Текстурные координаты */
					vec2 uv;

					fscanf(Fin, "%f %f\n", &uv.x, &uv.y);
					uv.y = -uv.y; // ивертируем для DDS текстуры. Убрать для TGA или BMP.
					tmp_UVs.push_back(uv);
				}
				else
				{
					if (strcmp(Buf, "vn") == 0)
					{
						/* Нормаль */
						vec3 normal;

						fscanf(Fin, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
						tmp_Normals.push_back(normal);
					}
					else
					{
						/* Грань (полигон) */
						if (strcmp(Buf, "f") == 0)
						{
							int VertexIndex[3], UVIndex[3], NormalIndex[3];

							int count = fscanf(Fin, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &VertexIndex[0], &UVIndex[0], &NormalIndex[0], &VertexIndex[1], &UVIndex[1], &NormalIndex[1], &VertexIndex[2], &UVIndex[2], &NormalIndex[2]);

							if (count != 9)
							{
								printf("Can't read OBJ file.\n");
								return false;
							}

							VertexIndices.push_back(VertexIndex[0]);
							VertexIndices.push_back(VertexIndex[1]);
							VertexIndices.push_back(VertexIndex[2]);

							UVIndices.push_back(UVIndex[0]);
							UVIndices.push_back(UVIndex[1]);
							UVIndices.push_back(UVIndex[2]);

							NormalIndices.push_back(NormalIndex[0]);
							NormalIndices.push_back(NormalIndex[1]);
							NormalIndices.push_back(NormalIndex[2]);
						}
						else
						{
							/* Что-то ещё, не нужное */
							char Buf2[1000];
							fgets(Buf2, 1000, Fin);
						}
					}
				}
			}

		}

		/* Для каждой вершины в каждом треугольнике достаём данные на выход */
		for (int i = 0; i < VertexIndices.size(); i++)
		{
			int vertexIndex = VertexIndices[i];
			int uvIndex = UVIndices[i];
			int normalIndex = NormalIndices[i];

			vec3 vertex = tmp_Vertices[vertexIndex - 1];
			vec2 uv = tmp_UVs[uvIndex - 1];
			vec3 normal = tmp_Normals[normalIndex - 1];

			vertices.push_back(vertex);
			uvs.push_back(uv);
			normals.push_back(normal);
		}
		return true;
	}

	/* Вычисление касательных и бикасательных */
	/* Vertices - список координат вершин */
	/* UVs - список текстурных координат вершин */
	/* Normals - список нормалей вершин */
	/* Tangents - список касательных нормалей, возвращаемое значение */
	/* Bitangents - список бикасательных нормалей, возвращаемое значение */
	void ComputeTBT(vector<vec3> Vertices, vector<vec2> UVs, vector<vec3> Normals, vector<vec3> &Tangents, vector<vec3> &Bitangents)
	{
		float R;
		vec3 V0, V1, V2, DeltaPos1, DeltaPos2, Tangent, Bitangent, Normal;
		vec2 UV0, UV1, UV2, DeltaUV1, DeltaUV2;

		for (int i = 0; i < vertices.size(); i += 3)
		{
			V0 = Vertices[i + 0];
			V1 = Vertices[i + 1];
			V2 = Vertices[i + 2];

			UV0 = UVs[i + 0];
			UV1 = UVs[i + 1];
			UV2 = UVs[i + 2];

			DeltaPos1 = V1 - V0;
			DeltaPos2 = V2 - V0;

			DeltaUV1 = UV1 - UV0;
			DeltaUV2 = UV2 - UV0;

			R = 1.0f / (DeltaUV1.x * DeltaUV2.y - DeltaUV1.y * DeltaUV2.x);

			Tangent = R * (DeltaPos1 * DeltaUV2.y - DeltaPos2 * DeltaUV1.y);
			Bitangent = R * (DeltaPos2 * DeltaUV1.x - DeltaPos1 * DeltaUV2.x);

			Tangents.push_back(Tangent);
			Tangents.push_back(Tangent);
			Tangents.push_back(Tangent);

			Bitangents.push_back(Bitangent);
			Bitangents.push_back(Bitangent);
			Bitangents.push_back(Bitangent);
		}

		for (int i = 0; i < vertices.size(); i++)
		{
			Normal = Normals[i];
			Tangent = Tangents[i];
			Bitangent = Bitangents[i];

			/* Ортогонализация */
			Tangent = normalize(Tangent - Normal * dot(Normal, Tangent));

			/* Направление */
			if (dot(cross(Normal, Tangent), Bitangent) < 0.0f) Tangent = Tangent * -1.0f;
		}
	}

	/* Загрузка 6 CubeMap текстур, возвращает идентификатор загруженной текстуры */
	/* Порядок: +X (Право), -X (Лево), +Y (Верх), -Y (Низ), +Z (Перед), -Z (Зад) */
	/* faces - список изображений для каждой грани */
	GLuint LoadCubeMap(vector<const GLchar*> faces)
	{
		GLuint TextureID;
		glGenTextures(1, &TextureID);

		int Width, Height;
		unsigned char* Image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

		for (int i = 0; i < faces.size(); i++)
		{
			Image = SOIL_load_image(faces[i], &Width, &Height, 0, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
			SOIL_free_image_data(Image);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return TextureID;
	}

public:
	/* Пустой конструктор для Скайбокса и координтных осей */
	OBJECT() {};

	/* Конструктор для объектов*/
	/* material - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - "цилиндр" */
	OBJECT(int material, const char *path)
	{
		Material = material;
		LoadOBJ(path, vertices, uvs, normals);
	}

	/* Деструктор */
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

	/* Возвращает указатель на шейдер */
	GLuint getShaderID() { return ShaderID; }

	/* Возвращает материал */
	int getMaterial() { return Material; }

	/* Задаёт материал */
	void setMaterial(int value) { Material = value; }
	
	/* Возврщает указатель на CubeMap-текстуру */
	GLuint getCubeMapTexture() { return CubeMapTexture; }

	/* Задаёт CubeMap-текстуру */
	void setCubeMapTexture(GLuint value){ CubeMapTexture = value; }

	/* Загрузка вершинного и фрагментного шейдеров */
	bool LoadShaders(const char *VertexShader, const char *FragmentShader)
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
			return false;
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
			return false;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		printf("Compiling vertex shader: %s...\n", VertexShader);
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

		printf("Compiling fragment shader: %s...\n", FragmentShader);
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

		printf("Linking program...\n");

		ShaderID = glCreateProgram();
		glAttachShader(ShaderID, VertexShaderID);
		glAttachShader(ShaderID, FragmentShaderID);
		glLinkProgram(ShaderID);

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

	/* Задаёт размер объекта */
	void setScale(float size)
	{
		/* Если умножить, то радиус сферы -> 0 */
		ModelMatrix = scale(vec3(size, size, size));
	}

	/* Вращение объекта */
	void setRotation(float angle, vec3 axis) { ModelMatrix *= rotate(angle, axis); }

	/* Возвращает позицию объекта */
	vec3 getPosition() { return vec3(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z); }

	/* Задаёт позицию объекта */
	void setPosition(float X, float Y, float Z) { ModelMatrix *= translate(vec3(X, Y, Z)); }	

	/* Задаёт сплошной цвет объекта */
	void setSolidColor(float R, float G, float B)
	{
		UserSolidColor.x = R;
		UserSolidColor.y = G;
		UserSolidColor.z = B;
	}

	/* Выполняет инициализацию объекта */
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
			LoadShaders("shaders//Refraction.vertexshader", "shaders//Refraction.fragmentshader");
			PrepareReflectionRefraction();
			break;
		}
		case 3:
		{
			LoadShaders("shaders//Reflection.vertexshader", "shaders//Reflection.fragmentshader");
			PrepareReflectionRefraction();
			break;
		}
		case 4:
		{
			PrepareCylinder();
			break;
		}
		default:
			break;
		}
	}

	/* Рендеринг объекта */
	/* CameraPosition - позиция камеры */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void Render(vec3 CameraPosition, mat4 ProjectionMatrix, mat4 ViewMatrix)
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
			RenderReflectionRefraction(CameraPosition, ProjectionMatrix, ViewMatrix);
			break;
		}
		case 3:
		{
			RenderReflectionRefraction(CameraPosition, ProjectionMatrix, ViewMatrix);
			break;
		}
		case 4:
		{
			RenderCylinder(ProjectionMatrix, ViewMatrix);
			break;
		}
		default:
			break;
		}
	}

	/* Инициализация координатных осей */
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

	/* Рендеринг координатных осей */
	/* ViewMatrix - матрица вида для осей */
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

	/* Инициализация Скайбокса */
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
		CubeMapTexture = LoadCubeMap(faces);
	}

	/* Рендеринг Скайбокса */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSkyBox(mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glDepthFunc(GL_LEQUAL);

		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
};

class SCENE
{
private:
	/* Число объектов на сцене */
	int ObjectsCount;
	OBJECT Axes, Skybox, *Objects;
	CAMERA Camera;

	bool SPHEREdecreaseSize = true, SPHEREincreaseSize = false;
	float SPHEREsize = 1.0, SPHEREsizeDelta = 0.006, SPHEREsizeMin = 0.8, SPHEREsizeMax = 1.0;
	float CUBEangle = 0.05, CYLINDERangle = -0.01;

	/* Пзиция камеры */
	vec3 CameraPos;
	/* Матрицы проекции и вида */
	mat4 ProjectionMatrix, ViewMatrix;

	/* Генерирует CubeMap-текстуру для зеркального объекта */
	/* id - идентификатор объекта, позиция в массиве всех объектов */
	/* camera - позиция камеры */
	/* ViewMatrix - матрица вида */
	GLuint MakeCubemap(int id, vec3 camera, mat4 ViewMatrix)
	{
		GLuint VAO;
		GLuint cubemap;
		GLuint framebuffer, renderedTexture, depthbuffer;
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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GenTextureSize, GenTextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
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
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GenTextureSize, GenTextureSize);
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
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
				break;
			}
			case 1:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0));
				break;
			}
			case 2:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));
				break;
			}
			case 3:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0));
				break;
			}
			case 4:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0));
				break;
			}
			case 5:
			{
				ViewMatrix = lookAt(Objects[id].getPosition(), vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0));
				break;
			}
			default:
				break;
			};

			glViewport(0, 0, GenTextureSize, GenTextureSize);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int j = 0; j < ObjectsCount; j++)
			{
				if (j != id)
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

public:
	/* Конструктор, инициализирует объекты сцены */
	SCENE()
	{
		Axes = OBJECT();
		Axes.PrepareAxes();

		Skybox = OBJECT();
		Skybox.PrepareSkyBox();

		if (MirrorExample)
		{
			ObjectsCount = 7;
			Objects = new OBJECT[ObjectsCount];

			Objects[0] = OBJECT(1, "3dmodels//cube.obj");
			Objects[0].Prepare();
			Objects[0].setSolidColor(0.9, 0.0, 0.5);
			Objects[0].setPosition(0.0, 0.0, 10.0);
			
			Objects[1] = OBJECT(1, "3dmodels//cube.obj");
			Objects[1].Prepare();
			Objects[1].setSolidColor(1.0, 0.6, 0.5);
			Objects[1].setPosition(0.0, 0.0, -10.0);
			
			Objects[2] = OBJECT(0, "3dmodels//cube.obj");
			Objects[2].Prepare();
			Objects[2].setSolidColor(0.3, 0.8, 0.5);
			Objects[2].setPosition(5.0, 0.0, 0.0);
			
			Objects[3] = OBJECT(0, "3dmodels//cube.obj");
			Objects[3].Prepare();
			Objects[3].setSolidColor(0.5, 0.0, 0.9);
			Objects[3].setPosition(-5.0, 0.0, 0.0);
			
			Objects[4] = OBJECT(0, "3dmodels//cube.obj");
			Objects[4].Prepare();
			Objects[4].setSolidColor(0.1, 0.3, 0.5);
			Objects[4].setPosition(0.0, 15.0, 0.0);
			
			Objects[5] = OBJECT(0, "3dmodels//cube.obj");
			Objects[5].Prepare();
			Objects[5].setSolidColor(0.5, 0.8, 0.9);
			Objects[5].setPosition(0.0, -15.0, 0.0);
			
			Objects[6] = OBJECT(3, "3dmodels//sphere.obj");
			Objects[6].Prepare();
			Objects[6].setScale(2.0);
		}
		else
		{
			ObjectsCount = 13;
			Objects = new OBJECT[ObjectsCount];

			Objects[0] = OBJECT(0, "3dmodels//cube.obj");
			Objects[0].Prepare();
			Objects[0].setSolidColor(0.9, 0.0, 0.5);
			Objects[0].setPosition(0.0, 6.0, 3.0);

			Objects[1] = OBJECT(1, "3dmodels//cube.obj");
			Objects[1].Prepare();
			Objects[1].setPosition(0.0, 3.0, 3.0);

			Objects[2] = OBJECT(2, "3dmodels//cube.obj");
			Objects[2].Prepare();
			Objects[2].setPosition(0.0, 0.0, 3.0);

			Objects[3] = OBJECT(3, "3dmodels//cube.obj");
			Objects[3].Prepare();
			Objects[3].setPosition(0.0, -3.0, 3.0);

			Objects[4] = OBJECT(0, "3dmodels//sphere.obj");
			Objects[4].Prepare();
			Objects[4].setSolidColor(0.6, 0.3, 0.9);
			Objects[4].setPosition(0.0, 6.0, 0.0);

			Objects[5] = OBJECT(1, "3dmodels//sphere.obj");
			Objects[5].Prepare();
			Objects[5].setPosition(0.0, 3.0, 0.0);

			Objects[6] = OBJECT(2, "3dmodels//sphere.obj");
			Objects[6].Prepare();

			Objects[7] = OBJECT(3, "3dmodels//sphere.obj");
			Objects[7].Prepare();
			Objects[7].setPosition(0.0, -3.0, 0.0);

			Objects[8] = OBJECT(4, "3dmodels//cylinder.obj");
			Objects[8].Prepare();
			Objects[8].setPosition(0.0, -7.0, -3.0);

			Objects[9] = OBJECT(0, "3dmodels//cylinder.obj");
			Objects[9].Prepare();
			Objects[9].setSolidColor(0.1, 0.9, 0.8);
			Objects[9].setPosition(0.0, 5.0, -3.0);

			Objects[10] = OBJECT(1, "3dmodels//cylinder.obj");
			Objects[10].Prepare();
			Objects[10].setPosition(0.0, 2.0, -3.0);

			Objects[11] = OBJECT(2, "3dmodels//cylinder.obj");
			Objects[11].Prepare();
			Objects[11].setPosition(0.0, -1.0, -3.0);

			Objects[12] = OBJECT(3, "3dmodels//cylinder.obj");
			Objects[12].Prepare();
			Objects[12].setPosition(0.0, -4.0, -3.0);

			Objects[2].setCubeMapTexture(Skybox.getCubeMapTexture());
			Objects[3].setCubeMapTexture(Skybox.getCubeMapTexture());

			Objects[6].setCubeMapTexture(Skybox.getCubeMapTexture());
			Objects[7].setCubeMapTexture(Skybox.getCubeMapTexture());

			Objects[11].setCubeMapTexture(Skybox.getCubeMapTexture());
			Objects[12].setCubeMapTexture(Skybox.getCubeMapTexture());
		}
	};

	/* Деструктор, удаляет шейдеры */
	~SCENE()
	{
		for (int i = 0; i < ObjectsCount; i++)
		{
			glDeleteProgram(Objects[i].getShaderID());
		}

		glDeleteProgram(Skybox.getShaderID());
		glDeleteProgram(Axes.getShaderID());
	};

	/* Рендеринг сцены */
	void Render()
	{
		CameraPos = Camera.ComputeViewMatrix(window, CameraMode);
		ProjectionMatrix = Camera.getProjectionMatrix();
		ViewMatrix = Camera.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (MirrorExample)
		{
			for (int i = 0; i < ObjectsCount; i++)
			{
				if (Objects[i].getMaterial() == 3) Objects[i].setCubeMapTexture(MakeCubemap(i, CameraPos, ViewMatrix));
			}

			for (int i = 0; i < ObjectsCount; i++)
			{
				Objects[i].Render(CameraPos, ProjectionMatrix, ViewMatrix);
				if (Objects[i].getMaterial() == 3)
				{
					GLuint tex = Objects[i].getCubeMapTexture();
					glDeleteTextures(1, &tex);
				}
			}

			Skybox.RenderSkyBox(ProjectionMatrix, ViewMatrix);
			Axes.RenderAxes(ViewMatrix);

			Objects[0].setRotation(CUBEangle, vec3(0.0, 1.0, 1.0));
			Objects[1].setRotation(-CUBEangle, vec3(0.0, 1.0, 1.0));
			Objects[2].setRotation(CUBEangle, vec3(0.0, 1.0, 1.0));
			Objects[3].setRotation(-CUBEangle, vec3(0.0, 1.0, 1.0));
			Objects[4].setRotation(CUBEangle, vec3(0.0, 1.0, 1.0));
			Objects[5].setRotation(-CUBEangle, vec3(0.0, 1.0, 1.0));
		}
		else
		{
			for (int i = 0; i < ObjectsCount; i++)
			{
				Objects[i].Render(CameraPos, ProjectionMatrix, ViewMatrix);
			}

			Skybox.RenderSkyBox(ProjectionMatrix, ViewMatrix);
			Axes.RenderAxes(ViewMatrix);

			Objects[0].setRotation(-CUBEangle, vec3(0.0, 1.0, 1.0));
			Objects[1].setRotation(CUBEangle, vec3(1.0, 1.0, 0.0));
			Objects[2].setRotation(-CUBEangle, vec3(1.0, 0.0, 0.0));
			Objects[3].setRotation(CUBEangle, vec3(1.0, 0.0, 0.0));

			Objects[6].setScale(SPHEREsize);

			Objects[8].setRotation(CYLINDERangle, vec3(0.0, 1.0, 0.0));
			Objects[9].setRotation(CYLINDERangle, vec3(0.0, 1.0, 0.0));
			Objects[10].setRotation(-CYLINDERangle, vec3(0.0, 1.0, 0.0));
			Objects[11].setRotation(CYLINDERangle, vec3(0.0, 1.0, 0.0));
			Objects[12].setRotation(-CYLINDERangle, vec3(0.0, 1.0, 0.0));

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
	/* Клавиши для выхода из приложения */
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	/* Переключение камер №1 и №2 */
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		CameraMode++;
		if (CameraMode > 2) CameraMode = 1;
	}

	/* Включение камеры №3 */
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) CameraMode = 3;

	/* Переключение отображения полигональной сетки */
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		if (Wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			Wireframe = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Wireframe = true;
		}
	}
}

/* Вывод ошибок в консоль */
void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	printf("OpenGL Debug Output message : ");

	if (source == GL_DEBUG_SOURCE_API_ARB) printf("Source : API; ");
	else
	{
		if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB) printf("Source : WINDOW_SYSTEM; ");
		else
		{
			if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB) printf("Source : SHADER_COMPILER; ");
			else
			{
				if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB) printf("Source : THIRD_PARTY; ");
				else
				{
					if (source == GL_DEBUG_SOURCE_APPLICATION_ARB) printf("Source : APPLICATION; ");
					else
					{
						if (source == GL_DEBUG_SOURCE_OTHER_ARB) printf("Source : OTHER; ");
					}
				}
			}
		}
	}

	if (type == GL_DEBUG_TYPE_ERROR_ARB) printf("Type : ERROR; ");
	else
	{
		if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB) printf("Type : DEPRECATED_BEHAVIOR; ");
		else
		{
			if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB) printf("Type : UNDEFINED_BEHAVIOR; ");
			else
			{
				if (type == GL_DEBUG_TYPE_PORTABILITY_ARB) printf("Type : PORTABILITY; ");
				else
				{
					if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB) printf("Type : PERFORMANCE; ");
					else
					{
						if (type == GL_DEBUG_TYPE_OTHER_ARB) printf("Type : OTHER; ");
					}
				}
			}
		}
	}

	if (severity == GL_DEBUG_SEVERITY_HIGH_ARB) printf("Severity : HIGH; ");
	else
	{
		if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB) printf("Severity : MEDIUM; ");
		else
		{
			if (severity == GL_DEBUG_SEVERITY_LOW_ARB) printf("Severity : LOW; ");
		}
	}

	printf("Message : %s\n", message);
}

void main()
{
	int nbFrames = 0;
	double currentTime, lastTime;
	char text[30], text2[30];

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		printf("Can't init GLFW.\n");
		getchar();
	}

	/* 4x сглаживание, OpenGL 3.3 */
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);


	GLFWmonitor *Screen = glfwGetPrimaryMonitor();

	if (FullSceen)
	{
		WindowWidth = 1920; WindowHeight = 1080;

		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		window = glfwCreateWindow(WindowWidth, WindowHeight, "Diploma", Screen, NULL);
	}
	else
	{
		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		window = glfwCreateWindow(WindowWidth, WindowHeight, "Diploma", NULL, NULL);

		/* Информация об экране, разрешение */
		const GLFWvidmode *VidMode = glfwGetVideoMode(Screen);
		/* Окно в центр экрана */
		glfwSetWindowPos(window, VidMode->width / 2 - WindowWidth / 2, VidMode->height / 2 - WindowHeight / 2);
	}

	if (!window)
	{
		printf("Can't open GLFW window.\n");
		getchar();
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

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

	/* Скрыть курсор */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		
	glfwSetCursorPos(window, WindowWidth / 2, WindowHeight / 2);
	/* Цвет фона, RGBA */
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
	/* Включаем буфер глубины */
	glEnable(GL_DEPTH_TEST);
	/* Выбираем фрагмент, ближайший к камере */
	glDepthFunc(GL_LESS);																		  
	//glEnable(GL_CULL_FACE);
	/*
	glEnable(GL_BLEND);												
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	*/

	SCENE Scene = SCENE();

	initText2D("Text.DDS");

	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		nbFrames++;

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
		printText2D(text, 0, 568, 20);

		glfwSwapBuffers(window);														  
	}

	cleanupText2D();
	glfwTerminate();
}