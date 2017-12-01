#ifndef Includes_H
#define Includes_H

#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

using namespace std;
using namespace glm;

/* Информация об окне */
/* Window - указатель */
/* Width, Height - ширина, высота */
/* FullScreen, Vsync, ShowCursor - во весь экран, вертикальная синхронизация, отображение курсора */
struct windowInfo
{
	GLFWwindow* Window;
	int Width, Height;
	float HalfWidth, HalfHeight;
	bool FullScreen, Vsync, ShowCursor;
};

#endif