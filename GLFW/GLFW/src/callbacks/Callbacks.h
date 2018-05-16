#pragma once
#include "..\includes\Includes.h"

///<summary>Default callbacks</summary>
class InputHandler : public InputHandle
{
private:
	///<summary>Хранит флаги нажатия клавиш мышки.</summary>
	bool mouseKeys_[5];

	///<summary>Хранит флаги нажатия клавиш клавиатуры.</summary>
	bool keyboardKeys_[1024];

	///<summary>Позиция курсора.</summary>
	double cursorX_, cursorY_;

public:
	///<summary>Кнструктор по-умолчанию.</summary>
	InputHandler();

	///<summary>Обработка позиции курсора.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'xpos'>Координата по оси x.</param>
	///<param name = 'ypos'>Координата по оси y.</param>
	void handleCursorPosition(GLFWwindow* window, double xpos, double ypos);

	///<summary>Обработка клавиш мышки.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'button'>Клавиша.</param>
	///<param name = 'action'>Событие.</param>
	///<param name = 'mods'>Модификаторы.</param>
	void handleMouseButtons(GLFWwindow* window, int button, int action, int mods);

	///<summary>Обработка колёсика мышки.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'xoffset'>Смещение по оси X.</param>
	///<param name = 'yoffset'>Смещение по оси Y.</param>
	void handleScroll(GLFWwindow* window, double xoffset, double yoffset);

	///<summary>Обработка клавиатуры.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'key'>Клавиша.</param>
	///<param name = 'scancode'>Scancode.</param>
	///<param name = 'action'>Действие.</param>
	///<param name = 'mods'>Модификаторы.</param>
	void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

	///<summary>Обработка изменения размера окна.</summary>
	///<param name = 'window'>Указатель на окно.</param>
	///<param name = 'width'>Новая ширина.</param>
	///<param name = 'height'>Новая высота.</param>
	void handleFramebufferSize(GLFWwindow* window, int width, int height);

	///<summary>Задаёт флаг нажатия клавиши мышки.</summary>
	///<param name = 'key'>Номер клавиши.</param>
	///<param name = 'value'>Значение флага.</param>
	void setMouseKeyState(const int key, const bool value);

	///<summary>Возвращает флаг нажатия клавиши мышки.</summary>
	///<param name = 'key'>Номер клавиши.</param>
	bool getMouseKeyState(const int key) const;

	///<summary>Возвращает флаг нажатия клавиши клавиатуры.</summary>
	///<param name = 'key'>Номер клавиши.</param>
	bool getKeyboardKeyState(const int key) const;

	///<summary>Возвращает позицию курсора по оси X.</summary>
	double getCursorX() const;

	///<summary>Возвращает позицию курсора по оси Y.</summary>
	double getCursorY() const;
};

namespace callbacks
{
	///<summary>Вывод ошибок в консоль.</summary>
	///<param name = 'source'>Источник ошибки.</param>
	///<param name = 'type'>Тип ошибки.</param>
	///<param name = 'id'>Код ошибки.</param>
	///<param name = 'severity'>Важность ошибки.</param>
	///<param name = 'length'>Длина сообщения.</param>
	///<param name = 'message'>Сообщение.</param>
	///<param name = 'userParam'>Параметры.</param>
	void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	///<summary>Обработка ошибок GLFW.</summary>
	///<param name = 'errorCode'>Код ошибки.</param>
	///<param name = 'message'>Сообщение.</param>
	void glfwErrorCallback(int errorCode, const char* message);

	///<summary>Возвращает ошибку glGetError().</summary>
	///<param name = 'file'>Имя файла с ошибкой.</param>
	///<param name = 'line'>Номер строки с ошибкой.</param>
	GLenum glCheckError(const char *file, int line);
}