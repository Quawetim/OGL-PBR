#include "Scene.h"

/* Генерирует CubeMap-текстуру для зеркального объекта */
/* id - идентификатор объекта, позиция в массиве всех объектов */
/* camera - позиция камеры */
/* ViewMatrix - матрица вида */
GLuint SCENE::MakeCubemap(int GenTextureSize, int id, vec3 camera, mat4 ViewMatrix)
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

/* Конструктор по-умолчанию */
SCENE::SCENE() {};

/* Конструктор, инициализирует объекты сцены */
SCENE::SCENE(bool blinn, windowInfo Winfo)
{
	WindowInfo = Winfo;

	Axes = OBJECT(WindowInfo);
	Axes.PrepareAxes();

	LightsCount = 3;

	Skybox = OBJECT(0, "3dmodels//skybox.obj");
	Skybox.setLightsPositions(LightsPositions);
	Skybox.setLightsColors(LightsColors);
	Skybox.setLightsProperties(LightsProperties);
	Skybox.PrepareSkyBox(100.0f);
	Skybox.createModelMatrix(vec3(0.0f, 0.0f, 0.0f), NULL, NULL, 100.0f * 2.0);

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

	Objects[0] = OBJECT(0, LightsCount, blinn, "3dmodels//cube.obj");
	Objects[0].Prepare();
	//Objects[0].setDiffuseTexture("textures//batman_diffuse.bmp", false);
	Objects[0].setSpecularTexture("textures//batman_specular.bmp", false);
	//Objects[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
	Objects[0].createModelMatrix(vec3(0.0f, 6.0f, 3.0f), NULL, NULL, 0.5f);

	Objects[1] = OBJECT(0, LightsCount, blinn, "3dmodels//sphere_lowpoly.obj");
	Objects[1].Prepare();
	Objects[1].setDiffuseColor(0.6f, 0.3f, 0.9f);
	Objects[1].setPosition(0.0f, 6.0f, 0.0f);

	Objects[2] = OBJECT(0, LightsCount, blinn, "3dmodels//cylinder.obj");
	Objects[2].Prepare();
	Objects[2].setDiffuseColor(0.1f, 0.9f, 0.8f);
	Objects[2].setPosition(0.0f, 6.0f, -3.0f);

	Objects[3] = OBJECT(1, LightsCount, blinn, "3dmodels//cube.obj");
	Objects[3].Prepare();
	Objects[3].setPosition(0.0f, 3.0f, 3.0f);

	Objects[4] = OBJECT(1, LightsCount, blinn, "3dmodels//sphere_lowpoly.obj");
	Objects[4].Prepare();
	Objects[4].setPosition(0.0f, 3.0f, 0.0f);

	Objects[5] = OBJECT(1, LightsCount, blinn, "3dmodels//cylinder.obj");
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

	ObjectsMirror[0] = OBJECT(0, LightsCount, blinn, "3dmodels//cube.obj");
	ObjectsMirror[0].Prepare();
	ObjectsMirror[0].setLightsPositions(LightsPositions);
	ObjectsMirror[0].setLightsColors(LightsColors);
	ObjectsMirror[0].setLightsProperties(LightsProperties);
	ObjectsMirror[0].setDiffuseColor(0.9f, 0.0f, 0.5f);
	ObjectsMirror[0].setPosition(0.0f, 0.0f, 10.0f);

	ObjectsMirror[1] = OBJECT(0, LightsCount, blinn, "3dmodels//cube.obj");
	ObjectsMirror[1].Prepare();
	ObjectsMirror[1].setLightsPositions(LightsPositions);
	ObjectsMirror[1].setLightsColors(LightsColors);
	ObjectsMirror[1].setLightsProperties(LightsProperties);
	ObjectsMirror[1].setDiffuseColor(0.0f, 0.5f, 0.9f);
	ObjectsMirror[1].setPosition(0.0f, 0.0f, -10.0f);

	ObjectsMirror[2] = OBJECT(0, LightsCount, blinn, "3dmodels//cube.obj");
	ObjectsMirror[2].Prepare();
	ObjectsMirror[2].setLightsPositions(LightsPositions);
	ObjectsMirror[2].setLightsColors(LightsColors);
	ObjectsMirror[2].setLightsProperties(LightsProperties);
	ObjectsMirror[2].setDiffuseColor(0.3f, 0.8f, 0.5f);
	ObjectsMirror[2].setPosition(5.0f, 0.0f, 0.0f);

	ObjectsMirror[3] = OBJECT(0, LightsCount, blinn, "3dmodels//cube.obj");
	ObjectsMirror[3].Prepare();
	ObjectsMirror[3].setLightsPositions(LightsPositions);
	ObjectsMirror[3].setLightsColors(LightsColors);
	ObjectsMirror[3].setLightsProperties(LightsProperties);
	ObjectsMirror[3].setDiffuseColor(0.5f, 0.0f, 0.9f);
	ObjectsMirror[3].setDiffuseTexture("textures//deadpool.bmp", false);
	//ObjectsMirror[3].setPosition(-5.0f, 0.0f, 0.0f);
	ObjectsMirror[3].createModelMatrix(vec3(-5.0f, 0.0f, 0.0f), NULL, NULL, 1.0f);

	ObjectsMirror[4] = OBJECT(1, LightsCount, blinn, "3dmodels//cylinder.obj");
	ObjectsMirror[4].Prepare();
	ObjectsMirror[4].setLightsPositions(LightsPositions);
	ObjectsMirror[4].setLightsColors(LightsColors);
	ObjectsMirror[4].setLightsProperties(LightsProperties);
	ObjectsMirror[4].setDiffuseColor(0.1f, 0.3f, 0.5f);
	//ObjectsMirror[4].setPosition(0.0f, 15.0f, 0.0f);
	ObjectsMirror[4].createModelMatrix(vec3(0.0f, 15.0f, 0.0f), NULL, NULL, 3.0f);

	ObjectsMirror[5] = OBJECT(0, LightsCount, blinn, "3dmodels//cylinder.obj");
	ObjectsMirror[5].setLightsPositions(LightsPositions);
	ObjectsMirror[5].setLightsColors(LightsColors);
	ObjectsMirror[5].setLightsProperties(LightsProperties);
	ObjectsMirror[5].Prepare();
	ObjectsMirror[5].setDiffuseColor(0.0f, 1.0f, 1.0f);
	ObjectsMirror[5].setPosition(0.0f, -15.0f, 0.0f);
	//ObjectsMirror[5].createModelMatrix(vec3(0.0f, -15.0f, 0.0f), NULL, NULL, 3.0f);

	ObjectsMirror[6] = OBJECT(3, "3dmodels//sphere_highpoly.obj");
	//ObjectsMirror[6].createModelMatrix(vec3(0.0f, 0.0f, 0.0f), NULL, NULL, 0.5f);
	ObjectsMirror[6].Prepare();
	//ObjectsMirror[6].setRefractiveIndex(1.1f);
	//ObjectsMirror[6].setScale(2.0f);
	//ObjectsMirror[6].setRotation(90.0f, "Y");
};

/* Деструктор, удаляет шейдеры */
SCENE::~SCENE()
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
void SCENE::Render(windowInfo Winfo, int cameramode, int texturesize, float fov, bool mirrorexample, bool stoprotations, bool showlights, bool blinn)
{
	WindowInfo = Winfo;

	CameraPosition = Camera.ComputeViewMatrix(WindowInfo, cameramode, fov);
	ProjectionMatrix = Camera.getProjectionMatrix();
	ViewMatrix = Camera.getViewMatrix();
	ViewMatrixAxes = Camera.getViewMatrixAxes();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Skybox.RenderSkyBox(CameraPosition, ProjectionMatrix, ViewMatrix);

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

	if (mirrorexample)
	{
		for (int i = 0; i < ObjectsCountMirror; i++)
		{
			int mat = ObjectsMirror[i].getMaterial();
			if ((mat == 2) || (mat == 3)) ObjectsMirror[i].setCubeMapTexture(MakeCubemap(texturesize, i, CameraPosition, ViewMatrix));
		}

		for (int i = 0; i < ObjectsCountMirror; i++)
		{
			if (i < 6)
			{
				ObjectsMirror[i].setBlinn(blinn);
				ObjectsMirror[i].setLightsPositions(LightsPositions);
				ObjectsMirror[i].setLightsColors(LightsColors);
				ObjectsMirror[i].setLightsProperties(LightsProperties);
			}

			ObjectsMirror[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
			int mat = ObjectsMirror[i].getMaterial();
			if ((mat == 2) || (mat == 3))
			{
				GLuint tex = ObjectsMirror[i].getCubeMapTexture();
				glDeleteTextures(1, &tex);
			}
		}

		if (!stoprotations)
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
		for (int i = 0; i < ObjectsCount; i++)
		{
			if (i < 6)
			{
				Objects[i].setBlinn(blinn);
				Objects[i].setLightsPositions(LightsPositions);
				Objects[i].setLightsColors(LightsColors);
				Objects[i].setLightsProperties(LightsProperties);
			}
			Objects[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
		}

		if (!stoprotations)
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

		if (showlights) for (int i = 0; i < LightsCount; i++) Lights[i].Render(CameraPosition, ProjectionMatrix, ViewMatrix);
	}

	//Skybox.RenderSkyBox(CameraPosition, ProjectionMatrix, ViewMatrix);
	Axes.RenderAxes(ViewMatrixAxes);
}