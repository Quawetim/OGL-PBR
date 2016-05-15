#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ViewMatrix.h"


vec3 position;																							// Позиция камеры
float Pi = pi<float>();
float deltaTime, radius = 20.0, radiusMin = 2.0, radiusMax = 100.0, cameraheight = 1.0;					// Время, радиус и высота полёта второй камеры
float horizontalAngle = 0.0, verticalAngle = 0.0;														// Горизонтальный и вертикальный углы
float FoV = 90.0, speed = 6.0, speed2 = 6.0, mouseSpeed = 0.005;										// FOV, скорость движения камеры, скорость мышки

mat4 ProjectionMatrix, ViewMatrix, ViewMatrixAxes;

mat4 getProjectionMatrix() { return ProjectionMatrix; }
mat4 getViewMatrix() { return ViewMatrix; }
mat4 getViewMatrixAxes() { return ViewMatrixAxes; }

/*
Обработка клавиатуры для движения камер
*/
void checkmove(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { if (radius >= radiusMin) radius -= deltaTime * speed; }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { if (radius <= radiusMax) radius += deltaTime * speed; }
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { if (cameraheight <= 10.0) cameraheight += deltaTime * speed; }
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { if (cameraheight >= -10.0) cameraheight -= deltaTime * speed; }
}

void checkmove(GLFWwindow* window, vec3 direction, vec3 right)
{
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) { if (speed2 < 20.0) speed2 += 0.2; }
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) { if (speed2 > 1.0) speed2 -= 0.2; }
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { position += direction * deltaTime * speed2; }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { position -= direction * deltaTime * speed2; }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { position += right * deltaTime * speed2; }
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { position -= right * deltaTime * speed2; }
}
/*
Вычисление матриц вида и проекции
*/
vec3 ComputeViewMatrix(GLFWwindow* window, int cameramode)
{
	int width, height;
	float width_float, height_float;
	double currentTime, mouseX, mouseY;

	static double lastTime = glfwGetTime();

	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

	glfwGetCursorPos(window, &mouseX, &mouseY);

	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	horizontalAngle += mouseSpeed * float(width / 2 - mouseX);
	verticalAngle += mouseSpeed * float(height / 2 - mouseY);

	width_float = float(width);
	height_float = float(height);

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

		ViewMatrixAxes = lookAt(vec3(5*cos(horizontalAngle), cameraheight, 5*sin(horizontalAngle)), direction, up);
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
		
	ProjectionMatrix = perspective(radians(FoV), width_float / height_float, 0.1f, 100.0f); // FOV, ratio, range : 0.1 unit <-> 100 units	 	
	lastTime = currentTime;

	return position;
}