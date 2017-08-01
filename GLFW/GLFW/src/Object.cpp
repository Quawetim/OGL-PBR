#include "Object.h"
#include "VboIndexer.h"
#include "TextureLoader.h"

/* Подготовка данных для объекта сплошного цвета */
void OBJECT::PrepareSolidColor()
{
	LoadShaders("resources//shaders//SolidColor.vs", NULL, "resources//shaders//SolidColor.fs");

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
/* Camera - позиция камеры */
/* ProjectionMatrix - матрица проекции */
/* ViewMatrix - матрица вида */
void OBJECT::RenderSolidColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
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
void OBJECT::PrepareGradientColor()
{
	LoadShaders("resources//shaders//GradientColor.vs", NULL, "resources//shaders//GradientColor.fs");

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
/* Camera - позиция камеры */
/* ProjectionMatrix - матрица проекции */
/* ViewMatrix - матрица вида */
void OBJECT::RenderGradientColor(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
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
void OBJECT::PrepareReflectionRefraction()
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
void OBJECT::RenderReflectionRefraction(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
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

/* Подготовка данных для объекта с картой нормалей */
void OBJECT::PrepareNormalMapping()
{
	LoadShaders("resources//shaders//NormalMapping.vs", NULL, "resources//shaders//NormalMapping.fs");

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

/* Рендеринг объекта с картой нормалей */
/* Camera - позиция камеры */
/* ProjectionMatrix - матрица проекции */
/* ViewMatrix - матрица вида */
void OBJECT::RenderNormalMapping(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
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
bool OBJECT::LoadOBJ(const char *Path, vector<vec3> &Vertices, vector<vec2> &UVs, vector<vec3> &Normals)
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
/* GeometryShader - путь к геометрическому шейдеру */
/* FragmentShader - путь к фрагментному (пиксельному) шейдеру */
bool OBJECT::LoadShaders(const char *VertexShader, const char* GeometryShader, const char *FragmentShader)
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
void OBJECT::ComputeTBT(vector<vec3> Vertices, vector<vec2> UVs, vector<vec3> Normals, vector<vec3> &Tangents, vector<vec3> &Bitangents)
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
/* faces - список изображений для каждой грани. Порядок: +X (Право), -X (Лево), +Y (Верх), -Y (Низ), +Z (Перед), -Z (Зад) */
GLuint OBJECT::LoadCubeMap(vector<const GLchar*> faces)
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

/* Конструктор по-умолчанию */
OBJECT::OBJECT() {};

/* Конструктор для Скайбокса и координтных осей */
OBJECT::OBJECT(windowInfo Winfo) { WindowInfo = Winfo; }

OBJECT::OBJECT(int lightscount)
{
	LightsCount = lightscount;
	PointLight = new pointLight[LightsCount];
};

/* Конструктор для объектов */
/* materialID - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
/* path - путь к модели */
OBJECT::OBJECT(int materialID, const char *path)
{
	sprintf(Path, path);
	Material.ID = materialID;
	if (Material.ID == 4)
	{
		LightsCount = 1;
		PointLight = new pointLight[LightsCount];
	}
	ModelLoadedFlag = LoadOBJ(path, vertices, uvs, normals);
}

/* Конструктор для объектов */
/* materialID - материал: 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
/* lightscount - число источников света */
/* path - путь к модели */
OBJECT::OBJECT(int materialID, int lightscount, bool blinn, const char *path)
{
	sprintf(Path, path);
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
	Blinn = blinn;
	ModelLoadedFlag = LoadOBJ(path, vertices, uvs, normals);
}

/* Деструктор */
OBJECT::~OBJECT()
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
GLuint OBJECT::getShaderID() { return ShaderID; }

/* Создаёт матрицу модели */
/* position - позиция объекта */
/* initialangle - начальный угол поворота */
/* axis - ось поворота */
/* size - исходный размер */
void OBJECT::createModelMatrix(vec3 position, float initialangle, char axis[], float size)
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
void OBJECT::createModelMatrix(vec3 position, float initialangle, vec3 axis, float size)
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
float OBJECT::getAngle() { return Angle; }

/* Возвращает ось поворота */
vec3 OBJECT::getAxis() { return Axis; }

/* Задаёт размер объекта */
void OBJECT::increaseScale(float value)
{
	Scale += value;
	ModelMatrix *= scale(vec3(Scale, Scale, Scale));
}

/* Возвращает размер объекта */
float OBJECT::getScale()
{
	return Scale;
}

/* Вращение объекта на заданный угол вокруг заданной оси */
/* angle - угол в градусах */
/* axis - ось вращения: X, Y, Z, XY, XZ, YZ, XYZ */
void OBJECT::increaseRotation(float angle, char axis[])
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
void OBJECT::increaseRotation(float angle, vec3 axis)
{
	ModelMatrix *= rotate(radians(angle), axis);
	Angle += angle;
	Axis = axis;
}

/* Задаёт позицию объекта */
void OBJECT::setPosition(float x, float y, float z)
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
void OBJECT::setPosition(vec3 position)
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
vec3 OBJECT::getPosition()
{
	//return vec3(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);
	return Position;
}

/* Задаёт материал объекта */
/* 0 - сплошной цвет, 1 - градиентный цвет, 2 - стекло, 3 - зеркало,  4 - Normal Mapping */
void OBJECT::setMaterial(int value) { Material.ID = value; }

/* Возвращает материал объекта */
int OBJECT::getMaterial() { return Material.ID; }

/* Задаёт фоновый цвет объекта */
void OBJECT::setAmbientColor(float r, float g, float b) { Material.AmbientColor = vec3(r, g, b); }

/* Задаёт фоновый цвет объекта */
void OBJECT::setAmbientColor(vec3 color) { Material.AmbientColor = color; }

/* Возвращает фоновый цвет объекта */
vec3 OBJECT::getAmbientColor() { return Material.AmbientColor; }

/* Задаёт диффузный цвет объекта */
void OBJECT::setDiffuseColor(float r, float g, float b) { Material.DiffuseColor = vec3(r, g, b); }

/* Задаёт диффузный цвет объекта */
void OBJECT::setDiffuseColor(vec3 color) { Material.DiffuseColor = color; }

/* Возвращает диффузный цвет объекта */
vec3 OBJECT::getDiffuseColor() { return Material.DiffuseColor; }

/* Задаёт отражённый цвет объекта */
void OBJECT::setSpecularColor(float r, float g, float b) { Material.SpecularColor = vec3(r, g, b); }

/* Задаёт отражённый цвет объекта */
void OBJECT::setSpecularColor(vec3 color) { Material.SpecularColor = color; }

/* Возвращает отражённый цвет объекта */
vec3 OBJECT::getSpecularColor() { return Material.SpecularColor; }

/* Задаёт силу блеска объекта */
void OBJECT::setShinePower(float value) { Material.Shine = value; }

/* Возвращает силу блеска объекта */
float OBJECT::getShinePower() { return Material.Shine; }

/* Задаёт индекс преломления объекта >= 1 */
/* 1.0 - Воздух, 1.309 - лёд, 1.33 - вода, 1.52 - стекло (по-умолчанию), 2.42 - алмаз */
void OBJECT::setRefractiveIndex(float value) { if (value < 1.0f) value = 1.0f; Material.RefractiveIndex = value; }

/* Возвращает индекс преломления объекта */
float OBJECT::getRefractiveIndex() { return Material.RefractiveIndex; }

/* Задаёт диффузную текстуру объекта */
void OBJECT::setDiffuseTexture(const char *path, bool DDS)
{
	if (DDS) DiffuseTexture = LoadDDS(path);
	else DiffuseTexture = LoadBMP(path);
	if (DiffuseTexture > 0) DiffuseTextureFlag = true;
}

/* Задаёт карту отражений объекта */
void OBJECT::setSpecularTexture(const char *path, bool DDS)
{
	if (DDS) SpecularTexture = LoadDDS(path);
	else SpecularTexture = LoadBMP(path);
	if (SpecularTexture > 0) SpecularTextureFlag = true;
}

/* задаёт карту нормалей объекта */
void OBJECT::setNormalTexture(const char *path)
{
	NormalTexture = LoadBMP(path);
	if (NormalTexture > 0) NormalTextureFlag = true;
}

/* Задаёт позицию источников света по его ID */
void OBJECT::setLightPosition(int id, float x, float y, float z) { PointLight[id].Position = vec3(x, y, z); }

/* Задаёт позицию источников света по его ID */
void OBJECT::setLightPosition(int id, vec3 position) { PointLight[id].Position = position; }

/* Задаёт позицию источников света */
void OBJECT::setLightsPositions(vec3 positions[]) { for (int i = 0; i < LightsCount; i++) PointLight[i].Position = positions[i]; }

/* Возвращает позицию источника света по его ID */
vec3 OBJECT::getLightPosition(int id) { return PointLight[id].Position; }

/* Задаёт цвет источника света по его ID */
void OBJECT::setLightColor(int id, float r, float g, float b) { PointLight[id].Color = vec3(r, g, b); }

/* Задаёт цвет источника света по его ID */
void OBJECT::setLightColor(int id, vec3 color) { PointLight[id].Color = color; }

/* Задаёт цвета всех источников света */
void OBJECT::setLightsColors(vec3 color[]) { for (int i = 0; i < LightsCount; i++) PointLight[i].Color = color[i]; }

/* Возвращает цвет источника света по его ID */
vec3 OBJECT::getLightColor(int id) { return PointLight[id].Color; }

/* Задаёт свойства источника света по его ID */
/* power - сила света */
/* constcoeff - постоянный коэффициент затухания */
/* lincoeff - линейный коэффициент затухания */
/* quadcoeff - квадратичный коэффициент затухания */
void OBJECT::setLightProperties(int id, float power, float constcoeff, float lincoeff, float quadcoeff)
{
	PointLight[id].Power = power;
	PointLight[id].Constant = constcoeff;
	PointLight[id].Linear = lincoeff;
	PointLight[id].Quadratic = quadcoeff;
}

/* Задаёт свойства источника света по его ID */
/* Порядок: power, constant, linear, quadratic */
void OBJECT::setLightProperties(int id, vec4 properties)
{
	PointLight[id].Power = properties.x;
	PointLight[id].Constant = properties.y;
	PointLight[id].Linear = properties.z;
	PointLight[id].Quadratic = properties.w;
}

/* Задаёт свойства всех источников света */
void OBJECT::setLightsProperties(vec4 properties[])
{
	for (int i = 0; i < LightsCount; i++)
	{
		PointLight[i].Power = properties[i].x;
		PointLight[i].Constant = properties[i].y;
		PointLight[i].Linear = properties[i].z;
		PointLight[i].Quadratic = properties[i].w;
	}
}

/* Задаёт модель освещения */
void OBJECT::setBlinn(bool value) { Blinn = value; }

/* Возвращает свойства источника света по его ID */
vec4 OBJECT::getlightProperties(int id) { return vec4(PointLight[id].Power, PointLight[id].Constant, PointLight[id].Linear, PointLight[id].Quadratic); }

/* Задаёт CubeMap-текстуру */
void OBJECT::setCubeMapTexture(GLuint value) { CubeMapTexture = value; }

/* Возврщает указатель на CubeMap-текстуру */
GLuint OBJECT::getCubeMapTexture() { return CubeMapTexture; }

/* Выполняет инициализацию объекта */
void OBJECT::Prepare()
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
				LoadShaders("resources//shaders//Refraction.vs", NULL, "resources//shaders//Refraction.fs");
				PrepareReflectionRefraction();
				break;
			}
			case 3:
			{
				LoadShaders("resources//shaders//Reflection.vs", NULL, "resources//shaders//Reflection.fs");
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
		printf("Model %s is not loaded.\n", Path);
	}
}

/* Рендеринг объекта */
/* CameraPosition - позиция камеры */
/* ProjectionMatrix - матрица проекции */
/* ViewMatrix - матрица вида */
void OBJECT::Render(vec3 CameraPosition, mat4 ProjectionMatrix, mat4 ViewMatrix)
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
		printf("Model %s is not loaded.\n", Path);
	}
}

/* Инициализация координатных осей */
void OBJECT::PrepareAxes()
{
	LoadShaders("resources//shaders//GradientColor.vs", NULL, "resources//shaders//GradientColor.fs");

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
void OBJECT::RenderAxes(mat4 ViewMatrix)
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
void OBJECT::PrepareSkyBox(float SkyBoxHalfSide)
{
	LoadShaders("resources//shaders//SkyBox.vs", NULL, "resources//shaders//SkyBox.fs");

	ProjectionMatrixID = glGetUniformLocation(ShaderID, "P");
	ViewMatrixID = glGetUniformLocation(ShaderID, "V");
	ModelMatrixID = glGetUniformLocation(ShaderID, "M");
	cubemapTextureID = glGetUniformLocation(ShaderID, "cubemap");

	GLfloat skyboxVerticesNormals[] = {
		/* Право */
		-SkyBoxHalfSide,  SkyBoxHalfSide, -SkyBoxHalfSide, 0.0f, 0.0f, 1.0f,
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
	faces.push_back("resources//textures//skybox//right.jpg");
	faces.push_back("resources//textures//skybox//left.jpg");
	faces.push_back("resources//textures//skybox//top.jpg");
	faces.push_back("resources//textures//skybox//bottom.jpg");
	faces.push_back("resources//textures//skybox//back.jpg");
	faces.push_back("resources//textures//skybox//front.jpg");
	CubeMapTexture = LoadCubeMap(faces);
}

/* Рендеринг Скайбокса */
/* Camera - позиция камеры */
/* ProjectionMatrix - матрица проекции */
/* ViewMatrix - матрица вида */
void OBJECT::RenderSkyBox(vec3 Camera, mat4 ProjectionMatrix, mat4 ViewMatrix)
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