#include "Camera.h"

/* ��������� ���������� ��� �������� ������ �2 */
/* window - ��������� �� ���� */
void CAMERA::CheckMove(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { if (Radius > RadiusMin) Radius -= DeltaTime * Speed; }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { if (Radius < RadiusMax) Radius += DeltaTime * Speed; }

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) Speed += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) if (Speed > 1.0f) Speed -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) Speed = 15.0f;
}

/* ��������� ���������� ��� �������� ������ �1 */
/* window - ��������� �� ���� */
/* Position - ����� ������� ������ */
/* Direction - ����������� ������ */
/* Right - ������ "������" ��� ������ */
void CAMERA::CheckMove(GLFWwindow* window, vec3 &Position, vec3 Direction, vec3 Right)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { Position += normalize(Direction) * DeltaTime * Speed; }
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { Position -= normalize(Direction) * DeltaTime * Speed; }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { Position += normalize(Right) * DeltaTime * Speed; }
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { Position -= normalize(Right) * DeltaTime * Speed; }
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { Position.y += DeltaTime * Speed; }
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { Position.y -= DeltaTime * Speed; }

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) Speed += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) if (Speed > 1.0f) Speed -= 1.0f;

	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) Speed = 15.0f;

	if (Position.x > Side) Position.x = Side - 0.1f;
	if (Position.x < -Side) Position.x = -Side + 0.1f;
	if (Position.y > Side) Position.y = Side - 0.1f;
	if (Position.y < -Side) Position.y = -Side + 0.1f;
	if (Position.z > Side) Position.z = Side - 0.1f;
	if (Position.z < -Side) Position.z = -Side + 0.1f;
}

/* ����������� */
/* �� ���� ������� �������� ����� ������� ��������� */
CAMERA::CAMERA(float value) { Side = value; }
	
/* ���������� ������� �������� */
mat4 CAMERA::getProjectionMatrix() { return ProjectionMatrix; }

/* ���������� ������� ���� */
mat4 CAMERA::getViewMatrix() { return ViewMatrix; }

/* ���������� ������� ���� ��� ������������ ���� */
mat4 CAMERA::getViewMatrixAxes() { return ViewMatrixAxes; }

/* ���������� ������ ���� � ��������, ���������� ������� ������ */
/* WindowInfo - ���������� �� ���� */
/* CameraMode - ��������� ������ */
/* FOV - field of view */
vec3 CAMERA::ComputeViewMatrix(windowInfo WindowInfo, int CameraMode, float FOV)
{
	double CurrentTime, MouseX, MouseY;
	static double LastTime = glfwGetTime();

	CurrentTime = glfwGetTime();
	DeltaTime = float(CurrentTime - LastTime);

	glfwGetCursorPos(WindowInfo.Window, &MouseX, &MouseY);

	switch (CameraMode)
	{
		/* �� ������� ���� */
		case 1:
		{
			static vec3 Position = vec3(Radius, 0.0f, 0.0f), Direction, Right;
			static float HorizontalAngle = radians(180.0f), VerticalAngle = radians(0.0f);

			if (degrees(VerticalAngle) > 90.0f) VerticalAngle = radians(89.9f);
			if (degrees(VerticalAngle) < -90.0f) VerticalAngle = radians(-89.9f);

			Direction = normalize(vec3(cos(HorizontalAngle) * cos(VerticalAngle), sin(VerticalAngle), sin(-HorizontalAngle) * cos(VerticalAngle)));
			Right = cross(Direction, CameraUp);

			CheckMove(WindowInfo.Window, Position, Direction, Right);

			ViewMatrix = lookAt(Position, Position + Direction, CameraUp);

			ViewMatrixAxes = lookAt(vec3(-5.0f * cos(HorizontalAngle), Position.y, 5.0f * sin(HorizontalAngle)), CameraLookTo, CameraUp);

			CameraPosition = Position;

			if (!WindowInfo.ShowCursor)
			{
				HorizontalAngle += MouseSpeed * (float)(WindowInfo.HalfWidth - MouseX);
				VerticalAngle += MouseSpeed * (float)(WindowInfo.HalfHeight - MouseY);
				glfwSetCursorPos(WindowInfo.Window, WindowInfo.HalfWidth, WindowInfo.HalfHeight);
			}

			break;
		}
		/* �� �������� ���� */
		case 2:
		{
			static vec3 Position;
			static float HorizontalAngle = radians(30.0f), VerticalAngle = radians(-90.0f);

			if (degrees(VerticalAngle) > 0.0f) VerticalAngle = radians(-0.9f);
			if (degrees(VerticalAngle) < -180.0f) VerticalAngle = radians(-179.9f);

			/* ������� �� ����������� ������� � ��������� */
			Position = vec3(-Radius * sin(VerticalAngle) * cos(HorizontalAngle), Radius * cos(VerticalAngle), Radius * sin(VerticalAngle) * sin(-HorizontalAngle));

			CheckMove(WindowInfo.Window);

			ViewMatrix = lookAt(Position, CameraLookTo, CameraUp);

			ViewMatrixAxes = lookAt(vec3(5.0f * cos(HorizontalAngle), 5.0f * cos(VerticalAngle), 5.0 * sin(HorizontalAngle)), CameraLookTo, CameraUp);

			CameraPosition = Position;

			if (!WindowInfo.ShowCursor)
			{
				HorizontalAngle += MouseSpeed * (float)(WindowInfo.Width / 2.0f - MouseX);
				VerticalAngle += MouseSpeed * (float)(WindowInfo.Height / 2.0f - MouseY);
				glfwSetCursorPos(WindowInfo.Window, WindowInfo.Width / 2.0f, WindowInfo.Height / 2.0f);
			}
			break;
		}
		/* ������������� */
		case 3:
		{
			ViewMatrix = lookAt(CameraStaticPosition, CameraLookTo, CameraUp);

			ViewMatrixAxes = ViewMatrix;

			CameraPosition = CameraStaticPosition;
			break;
		}
		default:
			break;
	}

	ProjectionMatrix = perspective(radians(FOV), (float)WindowInfo.Width / (float)WindowInfo.Height, 0.05f, 500.0f);

	LastTime = CurrentTime;

	return CameraPosition;
}