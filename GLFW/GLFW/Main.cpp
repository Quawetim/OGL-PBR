#include "Includes.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "VboIndexer.h"
#include "Text.h"

//GLFWwindow* window;
windowInfo WindowInfo;

/* WindowWidth, WindowHeight - ширина и высота окна */
/* FOV - field of view */
/* CameraMode - выбранная камера: 1 - от первого лица, 2 - от третьего лица, 3 - фиксированная */
/* GenTextureSize - размер генерируемой текстуры */
/* Wireframe - отображение сетки объектов, переключение по F1 */
/* StopRotations - переключение вращений по F2 */
/* ShowLights - переключение отображения источников света по F3 */
/* Blinn - переключение модели освещения по F4 */
/* MirrorExample - true = пример зеркального шарика с Reflection Map, false = все объекты без Reflection Map */
//int WindowWidth = 1280, WindowHeight = 800;
int CameraMode = 2;
int GenTextureSize = 512;
float FOV = 90.0f;
float SkyBoxHalfSide = 100.0f;
bool Wireframe = false;
bool StopRotations = true;
bool ShowLights = false;
bool Blinn = false;
bool MirrorExample = false;
//bool FullSceen = false;
//bool ShowCursor = false;

/* Обработка ошибок */
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* Обработка клавиатуры */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static int PrevCamera;
	/* Клавиши для выхода из приложения */
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

	/* FOV по-молчанию */
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) { FOV = 90.0f; }

	/* Переключение камер №1 и №2 */
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		CameraMode++;
		if (CameraMode > 2) CameraMode = 1;
	}

	/* Включение камеры №3 */
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		if (CameraMode != 3)
		{
			PrevCamera = CameraMode;
			CameraMode = 3;
		}
		else CameraMode = PrevCamera;
	}

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

	/* Включение/отключение вращений */
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		if (StopRotations)
		{
			StopRotations = false;
		}
		else
		{
			StopRotations = true;
		}
	}

	/* Включение/отключение отображения источников света */
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		if (ShowLights)
		{
			ShowLights = false;
		}
		else
		{
			ShowLights = true;
		}
	}

	/* Переключение модели освещения */
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		if (Blinn)
		{
			Blinn = false;
		}
		else
		{
			Blinn = true;
		}
	}

	/* Переключение сцен */
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (MirrorExample)
		{
			MirrorExample = false;
		}
		else
		{
			MirrorExample = true;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		WindowInfo.ShowCursor = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		WindowInfo.ShowCursor = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, WindowInfo.Width / 2, WindowInfo.Height / 2);
	}
}

/* Обработка колёсика мышки */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
	{
		if (FOV > 10.0f) FOV -= (float)yoffset * 10.0f;
		else FOV = 10.0f;
	}
	else
	{
		if (FOV < 90.0f) FOV -= (float)yoffset * 10.0f;
		else FOV = 90.0f;
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

class OBJECT
{
private:
	/* ModelLoadedFlag - флаг успешной/неуспешной загрузки модели */
	/* DiffuseTextureFlag - флаг наличия/отсутствия диффузной текстуры */
	/* SpecularTextureFlag - флаг наличия/отсутствия карты отражений */
	/* NormalTextureFlag - флаг наличия/отсутствия карты нормалей */
	bool ModelLoadedFlag = false;
	bool DiffuseTextureFlag = false;
	bool SpecularTextureFlag = false;
	bool NormalTextureFlag = false;

	char Name[50];
	/* Material - материал*/
	/* ID - тип: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping*/
	/* AmbientColor - фоновый цвет */
	/* DiffuseColor - диффузный цвет */
	/* SpecularColor - отражённый цвет */
	/* Shine - сила блеска */
	/* RefractionIndex - индекс преломления: 1.0 - Воздух, 1.309 - лёд, 1.33 - вода, 1.52 - стекло (по-умолчанию), 2.42 - алмаз */
	struct material
	{
		int ID;
		vec3 AmbientColor = vec3(0.05f, 0.05f, 0.05f);
		vec3 DiffuseColor = vec3(0.5f, 0.5f, 0.5f);
		vec3 SpecularColor = vec3(0.7f, 0.7f, 0.7f);
		float Shine = 8.0f;
		float RefractiveIndex = 1.52f;
	} Material;

	/* LightsCount - число источников освещения */
	int LightsCount = 0;	

	/* Position - позиция объекта */
	/* Axis - ось последнего вращения */
	/* Angle - угол поворота */
	/* Scale - размер */
	vec3 Position = vec3(0.0f, 0.0f, 0.0f), Axis = vec3(0.0f, 1.0f, 0.0f);
	float Angle = 0.0f, Scale = 1.0f;

	/* PointLight - точечный источник света */
	/* Position - позиция */
	/* Color - цвет */
	/* Power - сила света */
	/* Constant - постоянный коэффициент затухания */
	/* Linear - линейный коэффициент затухания */
	/* Quadratic - квадратичный коэффициент затухания */
	struct pointLight
	{
		vec3 Position = vec3(5.0f, 0.0f, 0.0f);
		vec3 Color = vec3(1.0f, 1.0f, 1.0f);
		float Power = 2.0f;
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;
	} *PointLight;

	/* Vertex Array Object */
	GLuint VAO;

	/* ModelMatrix - матрица модели */
public:	mat4 ModelMatrix = mat4(1.0f);
private:
	/* Идентификаторы шейдера, источника света, матриц и текстур для шейдеров*/
	GLuint ProjectionMatrixID, ViewMatrixID, ModelMatrixID;
	GLuint BlinnID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MaterialShineID, RefractiveIndexID;
	GLuint *PointLightPositionsIDs, *PointLightColorsIDs, *PointLightPowersIDs, *PointLightConstantsIDs, *PointLightLinearsIDs, *PointLightQuadraticsIDs;
	GLuint ShaderID, CameraPositionID, LightsCountID;	
	GLuint DiffuseTextureFlagID, NormalTextureFlagID, SpecularTextureFlagID;
	GLuint DiffuseTextureID, NormalTextureID, SpecularTextureID, cubemapTextureID;
	/* Текстуры */
	GLuint DiffuseTexture = 0, SpecularTexture = 0, NormalTexture = 0, CubeMapTexture = 0;
	/* Буферы */
	GLuint vertexbuffer, colorbuffer, uvbuffer, normalbuffer, tangentbuffer, bitangentbuffer, elementbuffer;

	vector<vec3> vertices, normals, tangents, bitangents;
	vector<vec2> uvs;
	vector<unsigned short> indices;

	/* Подготовка данных для объекта сплошного цвета */
	void PrepareSolidColor()
	{
		LoadShaders("shaders//SolidColor.vs", NULL, "shaders//SolidColor.fs");

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");

		LightsCountID = glGetUniformLocation(ShaderID, "LightsCount");
		CameraPositionID = glGetUniformLocation(ShaderID, "CameraPosition");

		BlinnID = glGetUniformLocation(ShaderID, "Blinn");
		MaterialAmbientColorID = glGetUniformLocation(ShaderID, "Material.AmbientColor");
		MaterialDiffuseColorID = glGetUniformLocation(ShaderID, "Material.DiffuseColor");
		MaterialSpecularColorID = glGetUniformLocation(ShaderID, "Material.SpecularColor");
		MaterialShineID = glGetUniformLocation(ShaderID, "Material.Shine");

		DiffuseTextureFlagID = glGetUniformLocation(ShaderID, "DiffuseTextureFlag");
		NormalTextureFlagID = glGetUniformLocation(ShaderID, "NormalTextureFlag");
		SpecularTextureFlagID = glGetUniformLocation(ShaderID, "SpecularTextureFlag");

		DiffuseTextureID = glGetUniformLocation(ShaderID, "DiffuseTexture");
		NormalTextureID = glGetUniformLocation(ShaderID, "NormalTexture");
		SpecularTextureID = glGetUniformLocation(ShaderID, "SpecularTexture");

		PointLightPositionsIDs = new GLuint[LightsCount];
		PointLightColorsIDs = new GLuint[LightsCount];
		PointLightPowersIDs = new GLuint[LightsCount];
		PointLightConstantsIDs = new GLuint[LightsCount];
		PointLightLinearsIDs = new GLuint[LightsCount];
		PointLightQuadraticsIDs = new GLuint[LightsCount];

		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			PointLightPositionsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Color", i);
			PointLightColorsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Power", i);
			PointLightPowersIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Constant", i);
			PointLightConstantsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Linear", i);
			PointLightLinearsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			PointLightQuadraticsIDs[i] = glGetUniformLocation(ShaderID, buf);
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	/* Рендеринг объекта сплошного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSolidColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		/*glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniform1i(BlinnID, Blinn);
		glUniform3f(MaterialAmbientColorID, Material.AmbientColor.x, Material.AmbientColor.y, Material.AmbientColor.z);
		glUniform3f(MaterialDiffuseColorID, Material.DiffuseColor.x, Material.DiffuseColor.y, Material.DiffuseColor.z);
		glUniform3f(MaterialSpecularColorID, Material.SpecularColor.x, Material.SpecularColor.y, Material.SpecularColor.z);
		glUniform1f(MaterialShineID, Material.Shine);
		glUniform1i(DiffuseTextureFlagID, DiffuseTextureFlag);
		glUniform1i(SpecularTextureFlagID, SpecularTextureFlag);
		glUniform1i(NormalTextureFlagID, NormalTextureFlag);
		
		glUniform1i(LightsCountID, LightsCount);
		glUniform3f(CameraPositionID, Camera.x, Camera.y, Camera.z);
		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			glUniform3f(PointLightPositionsIDs[i], PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);
			sprintf(buf, "PointLight[%d].Color", i);
			glUniform3f(PointLightColorsIDs[i], PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
			sprintf(buf, "PointLight[%d].Power", i);
			glUniform1f(PointLightPowersIDs[i], PointLight[i].Power);
			sprintf(buf, "PointLight[%d].Constant", i);
			glUniform1f(PointLightConstantsIDs[i], PointLight[i].Constant);
			sprintf(buf, "PointLight[%d].Linear", i);
			glUniform1f(PointLightLinearsIDs[i], PointLight[i].Linear);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			glUniform1f(PointLightQuadraticsIDs[i], PointLight[i].Quadratic);
		}

		glBindVertexArray(VAO);

		if (DiffuseTextureFlag)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
			glUniform1i(DiffuseTextureID, 0);
		}

		if (NormalTextureFlag)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, NormalTexture);
			glUniform1i(NormalTextureID, 1);
		}

		if (SpecularTextureFlag)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, SpecularTexture);
			glUniform1i(SpecularTextureID, 2);
		}

		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));


		if (DiffuseTextureFlag)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (NormalTextureFlag)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (SpecularTextureFlag)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindVertexArray(0);
		/*glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);*/
	}

	/* Подготовка данных для объекта градиентного цвета */
	void PrepareGradientColor()
	{
		LoadShaders("shaders//GradientColor.vs", NULL, "shaders//GradientColor.fs");

		float *colorbuffer_data = new float[vertices.size() * sizeof(vec3) * 3];

		for (unsigned int i = 0; i < vertices.size() * sizeof(vec3) * 3; i += 3)
		{
			if (i % 2 == 0)
			{
				colorbuffer_data[i] = 0.33f;
				colorbuffer_data[i + 1] = 0.66f;
				colorbuffer_data[i + 2] = 0.99f;
			}
			else
			{
				colorbuffer_data[i] = 0.24f;
				colorbuffer_data[i + 1] = 0.16f;
				colorbuffer_data[i + 2] = 0.71f;
			}
		}

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");	
		LightsCountID = glGetUniformLocation(ShaderID, "LightsCount");
		CameraPositionID = glGetUniformLocation(ShaderID, "CameraPosition");
		BlinnID = glGetUniformLocation(ShaderID, "Blinn");
		MaterialAmbientColorID = glGetUniformLocation(ShaderID, "Material.AmbientColor");
		//MaterialDiffuseColorID = glGetUniformLocation(ShaderID, "Material.DiffuseColor");
		MaterialSpecularColorID = glGetUniformLocation(ShaderID, "Material.SpecularColor");
		MaterialShineID = glGetUniformLocation(ShaderID, "Material.Shine");

		PointLightPositionsIDs = new GLuint[LightsCount];
		PointLightColorsIDs = new GLuint[LightsCount];
		PointLightPowersIDs = new GLuint[LightsCount];
		PointLightConstantsIDs = new GLuint[LightsCount];
		PointLightLinearsIDs = new GLuint[LightsCount];
		PointLightQuadraticsIDs = new GLuint[LightsCount];

		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			PointLightPositionsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Color", i);
			PointLightColorsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Power", i);
			PointLightPowersIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Constant", i);
			PointLightConstantsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Linear", i);
			PointLightLinearsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			PointLightQuadraticsIDs[i] = glGetUniformLocation(ShaderID, buf);
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

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	/* Рендеринг объекта градиентного цвета */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderGradientColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniform1i(BlinnID, Blinn);
		glUniform3f(MaterialAmbientColorID, Material.AmbientColor.x, Material.AmbientColor.y, Material.AmbientColor.z);
		//glUniform3f(MaterialDiffuseColorID, Material.DiffuseColor.x, Material.DiffuseColor.y, Material.DiffuseColor.z);
		glUniform3f(MaterialSpecularColorID, Material.SpecularColor.x, Material.SpecularColor.y, Material.SpecularColor.z);
		glUniform1f(MaterialShineID, Material.Shine);
		glUniform1i(LightsCountID, LightsCount);
		glUniform3f(CameraPositionID, Camera.x, Camera.y, Camera.z);
		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			glUniform3f(PointLightPositionsIDs[i], PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);
			sprintf(buf, "PointLight[%d].Color", i);
			glUniform3f(PointLightColorsIDs[i], PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
			sprintf(buf, "PointLight[%d].Power", i);
			glUniform1f(PointLightPowersIDs[i], PointLight[i].Power);
			sprintf(buf, "PointLight[%d].Constant", i);
			glUniform1f(PointLightConstantsIDs[i], PointLight[i].Constant);
			sprintf(buf, "PointLight[%d].Linear", i);
			glUniform1f(PointLightLinearsIDs[i], PointLight[i].Linear);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			glUniform1f(PointLightQuadraticsIDs[i], PointLight[i].Quadratic);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для стекла/зеркала */
	void PrepareReflectionRefraction()
	{
		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");	
		CameraPositionID = glGetUniformLocation(ShaderID, "CameraPosition");
		RefractiveIndexID = glGetUniformLocation(ShaderID, "RefractiveIndex");

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

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));			
		glUniform3f(CameraPositionID, Camera.x, Camera.y, Camera.z);
		glUniform1f(RefractiveIndexID, Material.RefractiveIndex);

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);
	}

	/* Подготовка данных для цилиндра с картой нормалей */
	void PrepareNormalMapping()
	{
		LoadShaders("shaders//NormalMapping.vs", NULL, "shaders//NormalMapping.fs");

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");
		DiffuseTextureID = glGetUniformLocation(ShaderID, "DiffuseTexture");
		NormalTextureID = glGetUniformLocation(ShaderID, "NormalTexture");
		SpecularTextureID = glGetUniformLocation(ShaderID, "SpecularTexture");
		//LightsCountID = glGetUniformLocation(ShaderID, "LightsCount");
		MaterialShineID = glGetUniformLocation(ShaderID, "Material.Shine");
		CameraPositionID = glGetUniformLocation(ShaderID, "CameraPosition");
		BlinnID = glGetUniformLocation(ShaderID, "Blinn");

		PointLightPositionsIDs = new GLuint[LightsCount];
		PointLightColorsIDs = new GLuint[LightsCount];
		PointLightPowersIDs = new GLuint[LightsCount];
		PointLightConstantsIDs = new GLuint[LightsCount];
		PointLightLinearsIDs = new GLuint[LightsCount];
		PointLightQuadraticsIDs = new GLuint[LightsCount];

		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			PointLightPositionsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Color", i);
			PointLightColorsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Power", i);
			PointLightPowersIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Constant", i);
			PointLightConstantsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Linear", i);
			PointLightLinearsIDs[i] = glGetUniformLocation(ShaderID, buf);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			PointLightQuadraticsIDs[i] = glGetUniformLocation(ShaderID, buf);
		}

		ComputeTBT(vertices, uvs, normals, tangents, bitangents);

		vector<vec3> indexed_vertices;
		vector<vec2> indexed_uvs;
		vector<vec3> indexed_normals;
		vector<vec3> indexed_tangents;
		vector<vec3> indexed_bitangents;
		IndexVBO(vertices, uvs, normals, tangents, bitangents, indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(vec3), &indexed_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(vec3), &indexed_normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(vec2), &indexed_uvs[0], GL_STATIC_DRAW);		

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
	void RenderNormalMapping(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));			
		glUniform3f(CameraPositionID, Camera.x, Camera.y, Camera.z);
		//glUniform1i(LightsCountID, LightsCount);
		glUniform1f(MaterialShineID, Material.Shine);
		glUniform1i(BlinnID, Blinn);

		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			glUniform3f(PointLightPositionsIDs[i], PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);
			sprintf(buf, "PointLight[%d].Color", i);
			glUniform3f(PointLightColorsIDs[i], PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
			sprintf(buf, "PointLight[%d].Power", i);
			glUniform1f(PointLightPowersIDs[i], PointLight[i].Power);
			sprintf(buf, "PointLight[%d].Constant", i);
			glUniform1f(PointLightConstantsIDs[i], PointLight[i].Constant);
			sprintf(buf, "PointLight[%d].Linear", i);
			glUniform1f(PointLightLinearsIDs[i], PointLight[i].Linear);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			glUniform1f(PointLightQuadraticsIDs[i], PointLight[i].Quadratic);
		}
		
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

		bool texcoords = false;

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
					texcoords = true;
					vec2 uv;

					fscanf(Fin, "%f %f\n", &uv.x, &uv.y);
					//uv.y = -uv.y; // ивертируем для DDS текстуры. Убрать для TGA или BMP.
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
							int count, VertexIndex[3], UVIndex[3], NormalIndex[3];

							if (texcoords)
							{						
								count = fscanf(Fin, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &VertexIndex[0], &UVIndex[0], &NormalIndex[0], &VertexIndex[1], &UVIndex[1], &NormalIndex[1], &VertexIndex[2], &UVIndex[2], &NormalIndex[2]);

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
								count = fscanf(Fin, "%d//%d %d//%d %d//%d\n", &VertexIndex[0], &NormalIndex[0], &VertexIndex[1], &NormalIndex[1], &VertexIndex[2], &NormalIndex[2]);

								if (count != 6)
								{
									printf("Can't read OBJ file.\n");
									return false;
								}

								VertexIndices.push_back(VertexIndex[0]);
								VertexIndices.push_back(VertexIndex[1]);
								VertexIndices.push_back(VertexIndex[2]);

								//UVIndices.push_back(0);
								//UVIndices.push_back(0);
								//UVIndices.push_back(0);

								NormalIndices.push_back(NormalIndex[0]);
								NormalIndices.push_back(NormalIndex[1]);
								NormalIndices.push_back(NormalIndex[2]);
							}
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
		for (unsigned int i = 0; i < VertexIndices.size(); i++)
		{
			int vertexIndex, uvIndex, normalIndex;
			vec2 uv;
			vec3 vertex, normal;

			vertexIndex = VertexIndices[i];
			normalIndex = NormalIndices[i];

			vertex = tmp_Vertices[vertexIndex - 1];
			normal = tmp_Normals[normalIndex - 1];

			vertices.push_back(vertex);
			normals.push_back(normal);

			if (texcoords)
			{
				uvIndex = UVIndices[i];
				uv = tmp_UVs[uvIndex - 1];
				uvs.push_back(uv);
			}
		}
		return true;
	}

	/* Загрузка вершинного и фрагментного шейдеров */
	/* VertexShader - путь к вершинному шейдеру */
	/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
	bool LoadShaders(const char *VertexShader, const char* GeometryShader, const char *FragmentShader)
	{
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
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
		char const *VertexSourcePointer = VertexShaderCode.c_str();
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

		if (GeometryShader != NULL)
		{
			string GeometryShaderCode;
			ifstream GeometryShaderStream(GeometryShader, ios::in);

			if (GeometryShaderStream.is_open())
			{
				string Line = "";
				while (getline(GeometryShaderStream, Line)) GeometryShaderCode += "\n" + Line;
				GeometryShaderStream.close();

			}
			else
			{
				printf("File %s not found.\n", GeometryShader);
				return false;
			}

			printf("Compiling geometry shader: %s...\n", GeometryShader);
			char const *GeometrySourcePointer = GeometryShaderCode.c_str();
			glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
			glCompileShader(GeometryShaderID);

			glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

			if (InfoLogLength > 0)
			{
				vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
				glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
				printf("\nError: %s\n", &GeometryShaderErrorMessage[0]);
			}
		}

		printf("Linking program...\n");

		ShaderID = glCreateProgram();
		glAttachShader(ShaderID, VertexShaderID);
		if (GeometryShader != NULL) glAttachShader(ShaderID, GeometryShaderID);
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
		if (GeometryShader != NULL) glDetachShader(ShaderID, GeometryShaderID);
		glDetachShader(ShaderID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(GeometryShaderID);
		glDeleteShader(FragmentShaderID);

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

		for (unsigned int i = 0; i < vertices.size(); i += 3)
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

		for (unsigned int i = 0; i < vertices.size(); i++)
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

		for (unsigned int i = 0; i < faces.size(); i++)
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

	OBJECT(int lightscount) 
	{ 
		LightsCount = lightscount; 
		PointLight = new pointLight[LightsCount];
	};

	/* Конструктор для объектов*/
	/* materialID - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
	/* path - путь к модели */
	OBJECT(int materialID, const char *path)
	{
		sprintf(Name, path);
		Material.ID = materialID;
		if (Material.ID == 4)
		{
			LightsCount = 1;
			PointLight = new pointLight[LightsCount];
		}
		ModelLoadedFlag = LoadOBJ(path, vertices, uvs, normals);
	}

	/* Конструктор для объектов*/
	/* materialID - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
	/* lightscount - число источников света */
	/* path - путь к модели */
	OBJECT(int materialID, int lightscount, const char *path)
	{
		sprintf(Name, path);
		Material.ID = materialID;
		if (Material.ID == 4)
		{
			LightsCount = 1;
			PointLight = new pointLight[LightsCount];
		}
		else
		{
			LightsCount = lightscount;
			PointLight = new pointLight[LightsCount];
		}
		ModelLoadedFlag = LoadOBJ(path, vertices, uvs, normals);
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

	/* Создаёт матрицу модели */
	/* position - позиция объекта */
	/* initialangle - начальный угол поворота */
	/* axis - ось поворота */
	/* size - исходный размер */
	void createModelMatrix(vec3 position, float initialangle, char axis[], float size)
	{
		ModelMatrix = mat4(1.0f);
		ModelMatrix *= translate(position);

		if (axis != NULL)
		{
			if (strcmp(axis, "X") == 0) Axis = vec3(1.0f, 0.0f, 0.0f);
			else
			{
				if (strcmp(axis, "Y") == 0) Axis = vec3(0.0f, 1.0f, 0.0f);
				else
				{
					if (strcmp(axis, "Z") == 0) Axis = vec3(0.0f, 0.0f, 1.0f);
					else
					{
						if ((strcmp(axis, "XY") == 0) || (strcmp(axis, "YX") == 0)) Axis = vec3(1.0f, 1.0f, 0.0f);
						else
						{
							if ((strcmp(axis, "XZ") == 0) || (strcmp(axis, "ZX") == 0)) Axis = vec3(1.0f, 0.0f, 1.0f);
							else
							{
								if ((strcmp(axis, "YZ") == 0) || (strcmp(axis, "ZY") == 0)) Axis = vec3(0.0f, 1.0f, 1.0f);
								else
								{
									if ((strcmp(axis, "XYZ") == 0) || (strcmp(axis, "XZY") == 0) ||
										(strcmp(axis, "YXZ") == 0) || (strcmp(axis, "YZX") == 0) ||
										(strcmp(axis, "ZXY") == 0) || (strcmp(axis, "ZYX") == 0)) Axis = vec3(1.0f, 1.0f, 1.0f);
									else return;
								}
							}
						}
					}
				}
			}

			ModelMatrix *= rotate(radians(initialangle), Axis);
		}

		ModelMatrix *= scale(vec3(size, size, size));

		Position = position;
		Angle = initialangle;
		Scale = size;
	}

	/* Создаёт матрицу модели */
	/* position - позиция объекта */
	/* initialangle - начальный угол поворота */
	/* axis - ось поворота */
	/* size - исходный размер */
	void createModelMatrix(vec3 position, float initialangle, vec3 axis, float size)
	{
		ModelMatrix = mat4(1.0f);
		ModelMatrix *= translate(position);		
		ModelMatrix *= rotate(radians(initialangle), axis);
		ModelMatrix *= scale(vec3(size, size, size));

		Position = position;
		Angle = initialangle;
		Axis = axis;
		Scale = size;
	}

	/* Возвращает угол поворота */
	float getAngle() { return Angle; }

	/* Возвращает ось поворота */
	vec3 getAxis() { return Axis; }

	/* Задаёт размер объекта */
	void increaseScale(float value)
	{
		Scale += value;
		ModelMatrix *= scale(vec3(Scale, Scale, Scale));
	}

	/* Возвращает размер объекта */
	float getScale()
	{
		return Scale;
	}

	/* Вращение объекта на заданный угол вокруг заданной оси */
	/* angle - угол в градусах */
	/* axis - ось вращения: X, Y, Z, XY, XZ, YZ, XYZ */
	void increaseRotation(float angle, char axis[])
	{
		if (strcmp(axis, "X") == 0) Axis = vec3(1.0f, 0.0f, 0.0f);
		else
		{
			if (strcmp(axis, "Y") == 0) Axis = vec3(0.0f, 1.0f, 0.0f);
			else
			{
				if (strcmp(axis, "Z") == 0) Axis = vec3(0.0f, 0.0f, 1.0f);
				else
				{
					if ((strcmp(axis, "XY") == 0) || (strcmp(axis, "YX") == 0)) Axis = vec3(1.0f, 1.0f, 0.0f);
					else
					{
						if ((strcmp(axis, "XZ") == 0) || (strcmp(axis, "ZX") == 0)) Axis = vec3(1.0f, 0.0f, 1.0f);
						else
						{
							if ((strcmp(axis, "YZ") == 0) || (strcmp(axis, "ZY") == 0)) Axis = vec3(0.0f, 1.0f, 1.0f);
							else
							{
								if ((strcmp(axis, "XYZ") == 0) || (strcmp(axis, "XZY") == 0) ||
									(strcmp(axis, "YXZ") == 0) || (strcmp(axis, "YZX") == 0) ||
									(strcmp(axis, "ZXY") == 0) || (strcmp(axis, "ZYX") == 0)) Axis = vec3(1.0f, 1.0f, 1.0f);
								else return;
							}
						}
					}
				}
			}
		}
		ModelMatrix *= rotate(radians(angle), Axis);
		Angle += angle;
	}

	/* Вращение объекта на заданный угол вокруг заданной оси */
	/* angle - угол в градусах */
	/* axis - ось вращения: X, Y, Z, XY, XZ, YZ, XYZ */
	void increaseRotation(float angle, vec3 axis)
	{	
		ModelMatrix *= rotate(radians(angle), axis);
		Angle += angle;
		Axis = axis;
	}

	/* Задаёт позицию объекта */
	void setPosition(float x, float y, float z)
	{
		float scale = getScale();
		ModelMatrix = mat4(1.0);
		ModelMatrix *= translate(vec3(x, y, z));
		ModelMatrix[0].x = scale;
		ModelMatrix[1].y = scale;
		ModelMatrix[2].z = scale;
		Position = vec3(x, y, z);
	}

	/* Задаёт позицию объекта */
	void setPosition(vec3 position)
	{
		float scale = getScale();
		ModelMatrix = mat4(1.0);
		ModelMatrix *= translate(vec3(position.x, position.y, position.z));
		ModelMatrix[0].x = scale;
		ModelMatrix[1].y = scale;
		ModelMatrix[2].z = scale;
		Position = position;
	}

	/* Возвращает позицию объекта */
	vec3 getPosition() 
	{
		//return vec3(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);
		return Position;	 
	}

	/* Задаёт материал */
	/* 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
	void setMaterial(int value) { Material.ID = value; }
		
	/* Возвращает материал */
	int getMaterial() { return Material.ID; }

	/* Задаёт фоновый цвет */
	void setAmbientColor(float r, float g, float b) { Material.AmbientColor = vec3(r, g, b); }

	/* Задаёт фоновый цвет */
	void setAmbientColor(vec3 color) { Material.AmbientColor = color; }

	/* Возвращает фоновый */
	vec3 getAmbientColor() { return Material.AmbientColor; }

	/* Задаёт диффузный цвет */
	void setDiffuseColor(float r, float g, float b) { Material.DiffuseColor = vec3(r, g, b); }

	/* Задаёт диффузный цвет */
	void setDiffuseColor(vec3 color) { Material.DiffuseColor = color; }

	/* Возвращает диффузный цвет */
	vec3 getDiffuseColor() { return Material.DiffuseColor; }

	/* Задаёт отражённый цвет */
	void setSpecularColor(float r, float g, float b) { Material.SpecularColor = vec3(r, g, b); }

	/* Задаёт отражённый цвет */
	void setSpecularColor(vec3 color) { Material.SpecularColor = color; }

	/* Возвращает отражённый цвет */
	vec3 getSpecularColor() { return Material.SpecularColor; }

	/* Задаёт силу блеска */
	void setShinePower(float value) { Material.Shine = value; }

	/* Возвращает силу блеска */
	float getShinePower() { return Material.Shine; }

	/* Задаёт индекс преломления >= 1 */
	/* 1.0 - Воздух, 1.309 - лёд, 1.33 - вода, 1.52 - стекло (по-умолчанию), 2.42 - алмаз */
	void setRefractiveIndex(float value) { if (value < 1.0f) value = 1.0f; Material.RefractiveIndex = value; }

	/* Возвращает индекс преломления */
	float getRefractiveIndex() { return Material.RefractiveIndex; }

	/* Задаёт диффузную текстуру */
	void setDiffuseTexture(const char *path, bool DDS)
	{
		if (DDS) DiffuseTexture = LoadDDS(path);
		else DiffuseTexture = LoadBMP(path);
		if (DiffuseTexture > 0) DiffuseTextureFlag = true;
	}

	/* Задаёт карту отражений */
	void setSpecularTexture(const char *path, bool DDS)
	{
		if (DDS) SpecularTexture = LoadDDS(path);
		else SpecularTexture = LoadBMP(path);
		if (SpecularTexture > 0) SpecularTextureFlag = true;
	}

	/* задаёт карту нормалей */
	void setNormalTexture(const char *path)
	{
		NormalTexture = LoadBMP(path);
		if (NormalTexture > 0) NormalTextureFlag = true;
	}

	/* Задаёт позицию источников света по его ID */
	void setLightPosition(int id, float x, float y, float z) { PointLight[id].Position = vec3(x, y, z); }

	/* Задаёт позицию источников света по его ID */
	void setLightPosition(int id, vec3 position) { PointLight[id].Position = position; }

	/* Задаёт позицию источников света */
	void setLightsPositions(vec3 positions[]) { for (int i = 0; i < LightsCount; i++) PointLight[i].Position = positions[i]; }

	/* Возвращает позицию источника света по его ID */
	vec3 getLightPosition(int id) { return PointLight[id].Position; }

	/* Задаёт цвет источника света по его ID */
	void setLightColor(int id, float r, float g, float b) { PointLight[id].Color = vec3(r, g, b); }

	/* Задаёт цвет источника света по его ID */
	void setLightColor(int id, vec3 color) { PointLight[id].Color = color; }

	/* Задаёт цвета всех источников света */
	void setLightsColors(vec3 color[]) { for (int i = 0; i < LightsCount; i++) PointLight[i].Color = color[i]; }

	/* Возвращает цвет источника света по его ID */
	vec3 getLightColor(int id) { return PointLight[id].Color; }

	/* Задаёт свойства источника света по его ID */
	void setLightProperties(int id, float power, float constcoeff, float lincoeff, float quadcoeff)
	{
		PointLight[id].Power = power;
		PointLight[id].Constant = constcoeff;
		PointLight[id].Linear = lincoeff;
		PointLight[id].Quadratic = quadcoeff;
	}

	/* Задаёт свойства источника света по его ID */
	/* Порядок: power, constant, linear, quadratic */
	void setLightProperties(int id, vec4 properties)
	{
		PointLight[id].Power = properties.x;
		PointLight[id].Constant = properties.y;
		PointLight[id].Linear = properties.z;
		PointLight[id].Quadratic = properties.w;
	}

	/* Задаёт свойства всех источников света */
	void setLightsProperties(vec4 properties[])
	{
		for (int i = 0; i < LightsCount; i++)
		{
			PointLight[i].Power = properties[i].x;
			PointLight[i].Constant = properties[i].y;
			PointLight[i].Linear = properties[i].z;
			PointLight[i].Quadratic = properties[i].w;
		}
	}

	/* Возвращает свойства источника света по его ID */
	vec4 getlightProperties(int id) { return vec4(PointLight[id].Power, PointLight[id].Constant, PointLight[id].Linear, PointLight[id].Quadratic); }

	/* Задаёт CubeMap-текстуру */
	void setCubeMapTexture(GLuint value){ CubeMapTexture = value; }

	/* Возврщает указатель на CubeMap-текстуру */
	GLuint getCubeMapTexture() { return CubeMapTexture; }

	/* Выполняет инициализацию объекта */
	void Prepare()
	{
		if (ModelLoadedFlag)
		{
			switch (Material.ID)
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
				LoadShaders("shaders//Refraction.vs", NULL, "shaders//Refraction.fs");
				PrepareReflectionRefraction();
				break;
			}
			case 3:
			{
				LoadShaders("shaders//Reflection.vs", NULL, "shaders//Reflection.fs");
				PrepareReflectionRefraction();
				break;
			}
			case 4:
			{
				PrepareNormalMapping();
				break;
			}
			default:
				break;
			}
		}
		else
		{
			printf("Model %s is not loaded.\n", Name);
		}
	}

	/* Рендеринг объекта */
	/* CameraPosition - позиция камеры */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void Render(vec3 CameraPosition, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		if (ModelLoadedFlag)
		{
			switch (Material.ID)
			{
			case 0:
			{
				RenderSolidColor(CameraPosition, ProjectionMatrix, ViewMatrix);
				break;
			}
			case 1:
			{
				RenderGradientColor(CameraPosition, ProjectionMatrix, ViewMatrix);
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
				RenderNormalMapping(CameraPosition, ProjectionMatrix, ViewMatrix);
				break;
			}
			default:
				break;
			}
		}
		else
		{
			printf("Model %s is not loaded.\n", Name);
		}
	}

	/* Инициализация координатных осей */
	void PrepareAxes()
	{
		LoadShaders("shaders//GradientColor.vs", NULL, "shaders//GradientColor.fs");

		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");

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

		mat4 ProjectionMatrix = perspective(radians(45.0f), 1.0f, 0.1f, 10.0f);
		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));

		glBindVertexArray(VAO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);

		glViewport(0, 0, WindowInfo.Width, WindowInfo.Height);
	}

	/* Инициализация Скайбокса */
	void PrepareSkyBox()
	{
		LoadShaders("shaders//SkyBox.vs", NULL, "shaders//SkyBox.fs");
	
		ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
		ViewMatrixID = glGetUniformLocation(ShaderID, "V");
		ModelMatrixID = glGetUniformLocation(ShaderID, "M");
		cubemapTextureID = glGetUniformLocation(ShaderID, "cubemap");

		GLfloat skyboxVerticesNormals[] = {
			/* Право */
			-SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 0.0f, 1.0f,	//или -
			-SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 0.0f, 1.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 0.0f, 1.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 0.0f, 1.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 0.0f, 1.0f,
			-SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 0.0f, 1.0f,

			/* Зад */
			-SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 1.0f, 0.0f, 0.0f,
			-SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, 1.0f, 0.0f, 0.0f,
			-SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 1.0f, 0.0f, 0.0f,
			-SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 1.0f, 0.0f, 0.0f,
			-SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, 1.0f, 0.0f, 0.0f,
			-SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 1.0f, 0.0f, 0.0f,

			/* Перед */
			SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, -1.0f, 0.0f, 0.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, -1.0f, 0.0f, 0.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, -1.0f, 0.0f, 0.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, -1.0f, 0.0f, 0.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, -1.0f, 0.0f, 0.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, -1.0f, 0.0f, 0.0f,

			/* Лево */
			-SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 0.0f, -1.0f,
			-SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 0.0f, -1.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 0.0f, -1.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 0.0f, -1.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 0.0f, -1.0f,
			-SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 0.0f, -1.0f,

			/* Верх */
			-SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, -1.0f, 0.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, -1.0f, 0.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, -1.0f, 0.0f,
			SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, -1.0f, 0.0f,
			-SkyBoxHalfSide,  SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, -1.0f, 0.0f,
			-SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, -1.0f, 0.0f,

			/* Низ */
			-SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 1.0f, 0.0f,
			-SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 1.0f, 0.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 1.0f, 0.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 1.0f, 0.0f,
			-SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 1.0f, 0.0f,
			SkyBoxHalfSide, -SkyBoxHalfSide,  SkyBoxHalfSide, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		/*
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerticesNormals), skyboxVerticesNormals, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);
		*/

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		vector<const GLchar*> faces;
		faces.push_back("textures//skybox//right.jpg");
		faces.push_back("textures//skybox//left.jpg");
		faces.push_back("textures//skybox//top.jpg");
		faces.push_back("textures//skybox//bottom.jpg");
		faces.push_back("textures//skybox//back.jpg");
		faces.push_back("textures//skybox//front.jpg");
		CubeMapTexture = LoadCubeMap(faces);
	}

	/* Рендеринг Скайбокса */
	/* ProjectionMatrix - матрица проекции */
	/* ViewMatrix - матрица вида */
	void RenderSkyBox(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
	{
		glDepthFunc(GL_LEQUAL);

		glUseProgram(ShaderID);

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, value_ptr(ViewMatrix));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value_ptr(ModelMatrix));
		glUniform1i(glGetUniformLocation(ShaderID, "LightsCount"), LightsCount);
		glUniform3f(glGetUniformLocation(ShaderID, "CameraPosition"), Camera.x, Camera.y, Camera.z);
		for (int i = 0; i < LightsCount; i++)
		{
			char buf[30];
			sprintf(buf, "PointLight[%d].Position", i);
			glUniform3f(glGetUniformLocation(ShaderID, buf), PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);
			sprintf(buf, "PointLight[%d].Color", i);
			glUniform3f(glGetUniformLocation(ShaderID, buf), PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
			sprintf(buf, "PointLight[%d].Power", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Power);
			sprintf(buf, "PointLight[%d].Constant", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Constant);
			sprintf(buf, "PointLight[%d].Linear", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Linear);
			sprintf(buf, "PointLight[%d].Quadratic", i);
			glUniform1f(glGetUniformLocation(ShaderID, buf), PointLight[i].Quadratic);
		}

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vec3));
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
};

class SCENE
{
private:
	/* LightsCount - число источников света */
	/* ObjectsCount, ObjectsCountMirror - число объектов */
	int LightsCount = 0, ObjectsCount = 0, ObjectsCountMirror = 0;
	OBJECT *Lights, Axes, Skybox, Plane, *Objects, *ObjectsMirror;
	CAMERA Camera;

	bool SphereDecrease = true, SphereIncrease = false;
	float SpheresSize = 1.0f, SpheresSizeDelta = 0.004f, SpheresSizeMin = 0.8f, SpheresSizeMax = 1.0f;
	float CubeAngle = 1.0f, CylinderAngle = -0.2f;
	float Radius, Angle = 0.0f, Angle2 = 0.0f, Angle3 = 0.0f, LightAngle = 0.0f;
	float AngleDelta = 2.0f, AngleDelta2 = 1.0f, AngleDelta3 = 1.0f, LightAngleDelta = 0.5f;
	vec3 Position, NewPosition;

	/* Позиция камеры */
	vec3 CameraPosition;

	/* LightsPositions - позиции источников света */
	/* LightsColors - цвета источников света */
	vec3 *LightsPositions, *LightsColors;

	/* Свойства источников света */
	vec4 *LightsProperties;

	/* Матрицы проекции и вида */
	mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

	/* Генерирует CubeMap-текстуру для зеркального объекта */
	/* id - идентификатор объекта, позиция в массиве всех объектов */
	/* camera - позиция камеры */
	/* ViewMatrix - матрица вида */
	GLuint MakeCubemap(int id, vec3 camera, mat4 ViewMatrix)
	{
		GLuint cubemap, framebuffer, depthbuffer;
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
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 1:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 2:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
				break;
			}
			case 3:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
				break;
			}
			case 4:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			case 5:
			{
				ViewMatrix = lookAt(ObjectsMirror[id].getPosition(), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f));
				break;
			}
			default:
				break;
			};

			glViewport(0, 0, GenTextureSize, GenTextureSize);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int j = 0; j < ObjectsCountMirror; j++)
			{
				if (j != id)
				{
					ObjectsMirror[j].Render(camera, ProjectionMatrix, ViewMatrix);
				}
			}
			Skybox.RenderSkyBox(camera, ProjectionMatrix, ViewMatrix);
		}
		glViewport(0, 0, WindowInfo.Width, WindowInfo.Height);
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

		LightsCount = 3;

		Skybox = OBJECT(0, "3dmodels//skybox.obj");
		Skybox.setLightsPositions(LightsPositions);
		Skybox.setLightsColors(LightsColors);
		Skybox.setLightsProperties(LightsProperties);
		Skybox.PrepareSkyBox();
		Skybox.createModelMatrix(vec3(0.0f, 0.0f, 0.0f), NULL, NULL, SkyBoxHalfSide * 2.0);

		if (LightsCount > 0)
		{
			LightsPositions = new vec3[LightsCount];
			LightsPositions[0] = vec3(-60.0f, 50.0f, -80.0f);
			LightsPositions[1] = vec3(-60.0f, -60.0f, -80.0f);
			LightsPositions[2] = vec3(60.0f, 6.0f, 60.0f);

			LightsColors = new vec3[LightsCount];
			LightsColors[0] = vec3(1.0f, 1.0f, 1.0f);
			LightsColors[1] = vec3(1.0f, 1.0f, 1.0f);
			LightsColors[2] = vec3(1.0f, 1.0f, 1.0f);

			LightsProperties = new vec4[LightsCount];
			LightsProperties[0] = vec4(100.0f, 1.0f, 0.8f, 0.002f);
			LightsProperties[1] = vec4(100.0f, 1.0f, 0.7f, 0.008f);
			LightsProperties[2] = vec4(50.0f, 1.0f, 0.2f, 0.03f);

			Lights = new OBJECT[LightsCount];
			for (int i = 0; i < LightsCount; i++)
			{
				Lights[i] = OBJECT(0, "3dmodels//sphere_lowpoly.obj");
				Lights[i].Prepare();
				Lights[i].setDiffuseColor(LightsColors[i]);
				Lights[i].setPosition(LightsPositions[i]);
				Lights[i].createModelMatrix(LightsPositions[i], NULL, NULL, 0.5f);
			}
		}

		ObjectsCount = 15;
		Objects = new OBJECT[ObjectsCount];

		Objects[0] = OBJECT(0, LightsCount,"3dmodels//cube.obj");
		Objects[0].Prepare();
		//Objects[0].setDiffuseTexture("textures//batman_diffuse.bmp", false);
		Objects[0].setSpecularTexture("textures//batman_specular.bmp", false);		
		//Objects[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
		Objects[0].createModelMatrix(vec3(0.0f, 6.0f, 3.0f), NULL, NULL, 0.5f);

		Objects[1] = OBJECT(0, LightsCount, "3dmodels//sphere_lowpoly.obj");
		Objects[1].Prepare();
		Objects[1].setDiffuseColor(0.6f, 0.3f, 0.9f);
		Objects[1].setPosition(0.0f, 6.0f, 0.0f);

		Objects[2] = OBJECT(0, LightsCount, "3dmodels//cylinder.obj");
		Objects[2].Prepare();
		Objects[2].setDiffuseColor(0.1f, 0.9f, 0.8f);
		Objects[2].setPosition(0.0f, 6.0f, -3.0f);

		Objects[3] = OBJECT(1, LightsCount, "3dmodels//cube.obj");
		Objects[3].Prepare();
		Objects[3].setPosition(0.0f, 3.0f, 3.0f);

		Objects[4] = OBJECT(1, LightsCount, "3dmodels//sphere_lowpoly.obj");
		Objects[4].Prepare();
		Objects[4].setPosition(0.0f, 3.0f, 0.0f);

		Objects[5] = OBJECT(1, LightsCount, "3dmodels//cylinder.obj");
		Objects[5].Prepare();
		Objects[5].setPosition(0.0f, 3.0f, -3.0f);

		Objects[6] = OBJECT(2, "3dmodels//cube.obj");
		Objects[6].Prepare();
		Objects[6].setRefractiveIndex(1.1f);
		Objects[6].setPosition(0.0f, 0.0f, 3.0f);

		Objects[7] = OBJECT(2, "3dmodels//sphere_highpoly.obj");
		Objects[7].Prepare();
		Objects[7].setRefractiveIndex(2.42f);

		Objects[8] = OBJECT(2, "3dmodels//cylinder.obj");
		Objects[8].Prepare();
		Objects[8].setRefractiveIndex(1.33f);
		Objects[8].setPosition(0.0f, 0.0f, -3.0f);

		Objects[9] = OBJECT(3, "3dmodels//cube.obj");
		Objects[9].Prepare();
		Objects[9].setPosition(0.0f, -3.0f, 3.0f);		

		Objects[10] = OBJECT(3, "3dmodels//sphere_lowpoly.obj");
		Objects[10].Prepare();
		Objects[10].setPosition(0.0f, -3.0f, 0.0f);	

		Objects[11] = OBJECT(3, "3dmodels//cylinder.obj");
		Objects[11].Prepare();
		Objects[11].setPosition(0.0f, -3.0f, -3.0f);

		Objects[12] = OBJECT(4, "3dmodels//cube.obj");
		Objects[12].setLightPosition(0, 5.0f, -6.0f, 3.0f);
		Objects[12].Prepare();	
		Objects[12].setDiffuseTexture("textures//brick_diffuse.bmp", false);
		Objects[12].setSpecularTexture("textures//brick_specular.bmp", false);
		Objects[12].setNormalTexture("textures//brick_normal.bmp");		
		Objects[12].setShinePower(32.0f);
		Objects[12].setPosition(0.0f, -6.0f, 3.0f);	

		Objects[13] = OBJECT(4, "3dmodels//sphere_lowpoly.obj");
		Objects[13].setLightPosition(0, 5.0f, -6.0f, 0.0f);
		Objects[13].Prepare();		
		Objects[13].setDiffuseTexture("textures//brick2_diffuse.bmp", false);
		Objects[13].setSpecularTexture("textures//brick2_specular.bmp", false);
		Objects[13].setNormalTexture("textures//brick2_normal.bmp");		
		Objects[13].setPosition(0.0f, -6.0f, 0.0f);		

		Objects[14] = OBJECT(4, "3dmodels//cylinder.obj");
		Objects[14].setLightPosition(0, 5.0f, -6.0f, -3.0f);
		Objects[14].Prepare();		
		Objects[14].setDiffuseTexture("textures//diffuse.dds", true);
		Objects[14].setSpecularTexture("textures//specular.dds", true);
		Objects[14].setNormalTexture("textures//normal.bmp");
		Objects[14].setPosition(0.0f, -6.0f, -3.0f);		

		Objects[6].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[7].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[8].setCubeMapTexture(Skybox.getCubeMapTexture());

		Objects[9].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[10].setCubeMapTexture(Skybox.getCubeMapTexture());
		Objects[11].setCubeMapTexture(Skybox.getCubeMapTexture());

		ObjectsCountMirror = 7;
		ObjectsMirror = new OBJECT[ObjectsCountMirror];

		ObjectsMirror[0] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[0].Prepare();
		ObjectsMirror[0].setLightsPositions(LightsPositions);
		ObjectsMirror[0].setLightsColors(LightsColors);
		ObjectsMirror[0].setLightsProperties(LightsProperties);		
		ObjectsMirror[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
		ObjectsMirror[0].setPosition(0.0f, 0.0f, 10.0f);

		ObjectsMirror[1] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[1].Prepare();
		ObjectsMirror[1].setLightsPositions(LightsPositions);
		ObjectsMirror[1].setLightsColors(LightsColors);
		ObjectsMirror[1].setLightsProperties(LightsProperties);		
		ObjectsMirror[1].setDiffuseColor(0.0f, 0.5f, 0.9f);
		ObjectsMirror[1].setPosition(0.0f, 0.0f, -10.0f);

		ObjectsMirror[2] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[2].Prepare();
		ObjectsMirror[2].setLightsPositions(LightsPositions);
		ObjectsMirror[2].setLightsColors(LightsColors);
		ObjectsMirror[2].setLightsProperties(LightsProperties);	
		ObjectsMirror[2].setDiffuseColor(0.3f, 0.8f, 0.5f);
		ObjectsMirror[2].setPosition(5.0f, 0.0f, 0.0f);

		ObjectsMirror[3] = OBJECT(0, LightsCount, "3dmodels//cube.obj");
		ObjectsMirror[3].Prepare();
		ObjectsMirror[3].setLightsPositions(LightsPositions);
		ObjectsMirror[3].setLightsColors(LightsColors);
		ObjectsMirror[3].setLightsProperties(LightsProperties);		
		ObjectsMirror[3].setDiffuseColor(0.5f, 0.0f, 0.9f);
		ObjectsMirror[3].setDiffuseTexture("textures//deadpool.bmp", false);
		//ObjectsMirror[3].setPosition(-5.0f, 0.0f, 0.0f);
		ObjectsMirror[3].createModelMatrix(vec3(-5.0f, 0.0f, 0.0f), NULL, NULL, 1.0f);

		ObjectsMirror[4] = OBJECT(1, LightsCount, "3dmodels//cylinder.obj");
		ObjectsMirror[4].Prepare();
		ObjectsMirror[4].setLightsPositions(LightsPositions);
		ObjectsMirror[4].setLightsColors(LightsColors);
		ObjectsMirror[4].setLightsProperties(LightsProperties);	
		ObjectsMirror[4].setDiffuseColor(0.1f, 0.3f, 0.5f);
		//ObjectsMirror[4].setPosition(0.0f, 15.0f, 0.0f);
		ObjectsMirror[4].createModelMatrix(vec3(0.0f, 15.0f, 0.0f), NULL, NULL, 3.0f);

		ObjectsMirror[5] = OBJECT(0, LightsCount, "3dmodels//cylinder.obj");
		ObjectsMirror[5].setLightsPositions(LightsPositions);
		ObjectsMirror[5].setLightsColors(LightsColors);
		ObjectsMirror[5].setLightsProperties(LightsProperties);
		ObjectsMirror[5].Prepare();
		ObjectsMirror[5].setDiffuseColor(0.0f, 1.0f, 1.0f);
		ObjectsMirror[5].setPosition(0.0f, -15.0f, 0.0f);
		//ObjectsMirror[5].createModelMatrix(vec3(0.0f, -15.0f, 0.0f), NULL, NULL, 3.0f);

		ObjectsMirror[6] = OBJECT(3, "3dmodels//teapot.obj"); 
		//ObjectsMirror[6].createModelMatrix(vec3(0.0f, 0.0f, 0.0f), NULL, NULL, 0.5f);
		ObjectsMirror[6].Prepare();
		//ObjectsMirror[6].setRefractiveIndex(1.1f);
		//ObjectsMirror[6].setScale(2.0f);
		//ObjectsMirror[6].setRotation(90.0f, "Y");
	};

	/* Деструктор, удаляет шейдеры */
	~SCENE()
	{
		for (int i = 0; i < ObjectsCount; i++)
		{
			glDeleteProgram(Objects[i].getShaderID());
		}

		for (int i = 0; i < ObjectsCountMirror; i++)
		{
			glDeleteProgram(ObjectsMirror[i].getShaderID());
		}

		glDeleteProgram(Skybox.getShaderID());
		glDeleteProgram(Axes.getShaderID());
	};
	
	/* Рендеринг сцены */
	void Render()
	{
		CameraPosition = Camera.ComputeViewMatrix(WindowInfo, CameraMode, FOV);
		ProjectionMatrix = Camera.getProjectionMatrix();
		ViewMatrix = Camera.getViewMatrix();
		ViewMatrixAxes = Camera.getViewMatrixAxes();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Skybox.RenderSkyBox(CameraPosition, ProjectionMatrix, ViewMatrix);
		
		if (MirrorExample)
		{
			for (int i = 0; i < ObjectsCountMirror; i++)
			{
				int mat = ObjectsMirror[i].getMaterial();
				if ((mat == 2) || (mat == 3)) ObjectsMirror[i].setCubeMapTexture(MakeCubemap(i, CameraPosition, ViewMatrix));
			}

			for (int i = 0; i < ObjectsCountMirror; i++)
			{
				ObjectsMirror[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
				int mat = ObjectsMirror[i].getMaterial();
				if ((mat == 2) || (mat == 3))
				{
					GLuint tex = ObjectsMirror[i].getCubeMapTexture();
					glDeleteTextures(1, &tex);
				}
			}
					
			if (!StopRotations)
			{
				Angle += AngleDelta;
				Angle2 += AngleDelta2;
				Angle3 += AngleDelta3;
				if (Angle > 360.0f) Angle = 0.0f;
				if (Angle2 > 360.0f) Angle2 = 0.0f;
				if (Angle3 > 360.0f) Angle3 = 0.0f;

				Position = ObjectsMirror[0].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(Radius * sin(radians(Angle)), 0.0f, Radius * cos(radians(Angle)));
				ObjectsMirror[0].createModelMatrix(NewPosition, -Angle, "Y", ObjectsMirror[0].getScale());

				Position = ObjectsMirror[1].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(-Radius * sin(radians(Angle)), 0.0f, -Radius * cos(radians(Angle)));
				ObjectsMirror[1].createModelMatrix(NewPosition, -Angle, "Y", ObjectsMirror[1].getScale());

				ObjectsMirror[2].increaseRotation(CubeAngle, "YZ");
				ObjectsMirror[3].increaseRotation(-CubeAngle, "XZ");

				Position = ObjectsMirror[4].getPosition();
				Radius = sqrt(Position.y * Position.y + Position.x * Position.x);
				NewPosition = vec3(Radius * sin(radians(Angle2)), Radius * cos(radians(Angle2)), 0.0f);
				ObjectsMirror[4].createModelMatrix(NewPosition, Angle3, NewPosition, ObjectsMirror[4].getScale());
				ObjectsMirror[4].increaseRotation(-Angle2, "Z");

				Position = ObjectsMirror[5].getPosition();
				Radius = sqrt(Position.y * Position.y + Position.x * Position.x);
				NewPosition = vec3(-Radius * sin(radians(Angle2)), -Radius * cos(radians(Angle2)), 0.0f);
				ObjectsMirror[5].createModelMatrix(NewPosition, Angle3, NewPosition, ObjectsMirror[5].getScale());
				ObjectsMirror[5].increaseRotation(-Angle2, "Z");

				ObjectsMirror[6].increaseRotation(1.0f, "Y");				
			}	
		}
		else
		{
			static vec3 color = vec3(1.0f, 0.0f, 0.0f);
			static bool flag = false;

			if ((color.r > 0.0f) && (!flag))
			{
				color.r -= 0.001f;
				color.g += 0.001f;
			}
			else
			{
				if (color.g > 0.0f)
				{
					color.g -= 0.001f;
					color.b += 0.001f;
				}
				else
				{
					if (color.b > 0.0f)
					{
						color.b -= 0.001f;
						color.r += 0.001f;
						if (!flag) flag = true;
					}
					else flag = false;
				}
			}

			LightsColors[0] = color;
			Lights[0].setDiffuseColor(color);

			for (int i = 0; i < ObjectsCount; i++)
			{
				if (i < 6)
				{
					Objects[i].setLightsPositions(LightsPositions);
					Objects[i].setLightsColors(LightsColors);
					Objects[i].setLightsProperties(LightsProperties);				
				}
				Objects[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			}

			if (LightsCount > 0)
			{
				LightAngle += LightAngleDelta;
				if (LightAngle > 360.0f) LightAngle = 0;

				Position = Lights[0].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(-Radius * sin(radians(LightAngle)), Position.y, -Radius * cos(radians(LightAngle)));
				LightsPositions[0] = NewPosition;
				Lights[0].createModelMatrix(NewPosition, -LightAngle, "Y", Lights[0].getScale());		

				Position = Lights[1].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(-Radius * sin(radians(LightAngle)), Position.y, -Radius * cos(radians(LightAngle)));
				LightsPositions[1] = NewPosition;
				Lights[1].createModelMatrix(NewPosition, -LightAngle, "Y", Lights[1].getScale());

				Position = Lights[2].getPosition();
				Radius = sqrt(Position.x * Position.x + Position.z * Position.z);
				NewPosition = vec3(Radius * sin(radians(LightAngle)), Position.y, Radius * cos(radians(LightAngle)));
				LightsPositions[2] = NewPosition;
				Lights[2].createModelMatrix(NewPosition, -LightAngle, "Y", Lights[2].getScale());

				if (ShowLights) for (int i = 0; i < LightsCount; i++) Lights[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			}

			if (!StopRotations)
			{
				Objects[0].increaseRotation(-CubeAngle, "Y");
				Objects[3].increaseRotation(CubeAngle, "XY");
				Objects[6].increaseRotation(-CubeAngle, "X");
				Objects[9].increaseRotation(CubeAngle, "X");
				Objects[12].increaseRotation(CylinderAngle * 3.0f, "Y");

				Objects[1].createModelMatrix(Objects[1].getPosition(), Objects[1].getAngle(), Objects[1].getAxis(), SpheresSize);
				Objects[4].createModelMatrix(Objects[4].getPosition(), Objects[4].getAngle(), Objects[4].getAxis(), SpheresSize);
				Objects[7].createModelMatrix(Objects[7].getPosition(), Objects[7].getAngle(), Objects[7].getAxis(), SpheresSize);
				Objects[10].createModelMatrix(Objects[10].getPosition(), Objects[10].getAngle(), Objects[10].getAxis(), SpheresSize);
				
				Objects[13].createModelMatrix(Objects[13].getPosition(), Objects[13].getAngle(), Objects[13].getAxis(), SpheresSize);
				Objects[13].increaseRotation(-0.6f, "XYZ");			

				Objects[2].increaseRotation(-CylinderAngle, "Y");
				Objects[5].increaseRotation(CylinderAngle, "Y");
				Objects[8].increaseRotation(-CylinderAngle, "Y");
				Objects[11].increaseRotation(CylinderAngle, "Y");
				Objects[14].increaseRotation(-CylinderAngle, "Y");

				if ((SpheresSize > SpheresSizeMin) && SphereDecrease) SpheresSize -= SpheresSizeDelta;
				else
				{
					SphereDecrease = false;
					SphereIncrease = true;
				}

				if ((SpheresSize < SpheresSizeMax) && SphereIncrease) SpheresSize += SpheresSizeDelta;
				else
				{
					SphereDecrease = true;
					SphereIncrease = false;
				}
			}
		}
		
		if ((LightsCount > 0) && (ShowLights))
		{
			for (int i = 0; i < LightsCount; i++) Lights[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);		
		}

		//Skybox.RenderSkyBox(CameraPosition, ProjectionMatrix, ViewMatrix);
		Axes.RenderAxes(ViewMatrixAxes);
	}
};

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

	WindowInfo.Width = 1280;
	WindowInfo.Height = 800;
	WindowInfo.ShowCursor = false;
	WindowInfo.FullScreen = false;

	if (WindowInfo.FullScreen)
	{
		WindowInfo.Width = 1920; WindowInfo.Height = 1080;

		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", Screen, NULL);
	}
	else
	{
		/* Ширина, высота, название окна, монитор (FullSreen , NUll - оконный), обмен ресурсами с окном (NULL - нет такого) */
		WindowInfo.Window = glfwCreateWindow(WindowInfo.Width, WindowInfo.Height, "Diploma", NULL, NULL);

		/* Информация об экране, разрешение */
		const GLFWvidmode *VidMode = glfwGetVideoMode(Screen);
		/* Окно в центр экрана */
		glfwSetWindowPos(WindowInfo.Window, VidMode->width / 2 - WindowInfo.Width / 2, VidMode->height / 2 - WindowInfo.Height / 2);
	}

	if (!WindowInfo.Window)
	{
		printf("Can't open GLFW window.\n");
		getchar();
		glfwTerminate();
	}

	glfwMakeContextCurrent(WindowInfo.Window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(WindowInfo.Window, key_callback);
	glfwSetScrollCallback(WindowInfo.Window, scroll_callback);

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

	glViewport(0, 0, WindowInfo.Width, WindowInfo.Height);

	/* Скрыть курсор */
	glfwSetInputMode(WindowInfo.Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(WindowInfo.Window, WindowInfo.Width / 2, WindowInfo.Height / 2);
	/* Цвет фона, RGBA */
	glClearColor(0.0f, 0.6f, 0.8f, 0.0f);
	/* Включаем буфер глубины */
	glEnable(GL_DEPTH_TEST);
	/* Выбираем фрагмент, ближайший к камере */
	glDepthFunc(GL_LESS);																		  
	glEnable(GL_CULL_FACE);

	SCENE Scene = SCENE();

	InitText("textures//Text.DDS");

	lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(WindowInfo.Window))
	{
		currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 0.01)
		{
			//sprintf(text, "%.3f ms for frame. %d frames\n", 1000.0 / double(nbFrames), nbFrames);
			sprintf(text, "%d FPS", nbFrames);
			sprintf(text2, "Diploma at %d FPS", nbFrames);
			glfwSetWindowTitle(WindowInfo.Window, text2);
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwPollEvents();

		Scene.Render();
		PrintText(text, 0, 580, 12);

		glfwSwapBuffers(WindowInfo.Window);
	}

	DeleteText();
	glfwTerminate();
}