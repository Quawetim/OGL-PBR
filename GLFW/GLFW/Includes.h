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

#include <string.h>

using namespace std;
using namespace glm;

struct windowInfo
{
	GLFWwindow* Window;
	int Width;
	int Height;
	bool FullScreen;
	bool ShowCursor;
};

#endif