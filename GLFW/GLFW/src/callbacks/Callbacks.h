#pragma once
#include "..\includes\Includes.h"

///<summary>Default callbacks</summary>
namespace callbacks
{
	///<summary>��������� ����������.</summary>
	///<param name = 'window'>��������� �� ����.</param>
	///<param name = 'key'>�������.</param>
	///<param name = 'scancode'>Scancode.</param>
	///<param name = 'action'>��������.</param>
	///<param name = 'mods'>������������.</param>
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	///<summary>��������� ������� �����.</summary>
	///<param name = 'window'>��������� �� ����.</param>
	///<param name = 'xoffset'>�������� �� ��� X.</param>
	///<param name = 'yoffset'>�������� �� ��� Y.</param>
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	///<summary>��������� ��������� ������� ����.</summary>
	///<param name = 'window'>��������� �� ����.</param>
	///<param name = 'width'>����� ������.</param>
	///<param name = 'height'>����� ������.</param>
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	///<summary>����� ������ � �������.</summary>
	///<param name = 'source'>�������� ������.</param>
	///<param name = 'type'>��� ������.</param>
	///<param name = 'id'>��� ������.</param>
	///<param name = 'severity'>�������� ������.</param>
	///<param name = 'length'>����� ���������.</param>
	///<param name = 'message'>���������.</param>
	///<param name = 'userParam'>���������.</param>
	void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	///<summary>��������� ������ GLFW.</summary>
	///<param name = 'errorCode'>��� ������.</param>
	///<param name = 'message'>���������.</param>
	void glfwErrorCallback(int errorCode, const char* message);

	///<summary>���������� ������ glGetError().</summary>
	///<param name = 'file'>��� ����� � �������.</param>
	///<param name = 'line'>����� ������ � �������.</param>
	GLenum glCheckError(const char *file, int line);
}