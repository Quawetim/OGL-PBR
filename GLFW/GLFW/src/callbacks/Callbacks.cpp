#include "Callbacks.h"

///<summary>Обработка клавиатуры.</summary>
///<param name = 'window'>Указатель на окно.</param>
///<param name = 'key'>Клавиша.</param>
///<param name = 'scancode'>Scancode.</param>
///<param name = 'action'>Действие.</param>
///<param name = 'mods'>Модификаторы.</param>
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static int PrevCamera;
    
    // Клавиши для выхода из приложения
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    //if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    //if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

    // FOV по-умолчанию
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) { FOV = 90.0f; }

    // Переключение камер №1 и №2
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        CameraMode++;
        if (CameraMode > 2) CameraMode = 1;
    }

    // Включение камеры №3
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        if (CameraMode != 3)
        {
            PrevCamera = CameraMode;
            CameraMode = 3;
        }
        else CameraMode = PrevCamera;
    }

    // Отображение курсора при удерживании CTRL
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        WindowInfo.ShowCursor = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // Скрытие курсора при отпускании CTRL
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        WindowInfo.ShowCursor = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, WindowInfo.Width / 2, WindowInfo.Height / 2);
    }

    // Отображение справки при удерживании F1
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        ShowHelp = true;
    }

    // Скрытие справки при отпускании F1
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE)
    {
        ShowHelp = false;
    }
}

///<summary>Обработка колёсика мышки.</summary>
///<param name = 'window'>Указатель на окно.</param>
///<param name = 'xoffset'>Смещение по оси X.</param>
///<param name = 'yoffset'>Смещение по оси Y.</param>
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
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

///<summary>Обработка изменения размера окна.</summary>
///<param name = 'window'>Указатель на окно.</param>
///<param name = 'width'>Новая ширина.</param>
///<param name = 'height'>Новая высота.</param>
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

///<summary>Вывод ошибок в консоль.</summary>
///<param name = 'source'>Источник ошибки.</param>
///<param name = 'type'>Тип ошибки.</param>
///<param name = 'id'>Код ошибки.</param>
///<param name = 'severity'>Важность ошибки.</param>
///<param name = 'length'>Длина сообщения.</param>
///<param name = 'message'>Сообщение.</param>
///<param name = 'userParam'>Параметры.</param>
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << "**************************" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:               std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:       std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:             std::cout << "Source: Other"; break;
        default:
            {
                std::cout << "Source: Unknown"; break;
            }
    }

    std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Perfomance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        default:
            {
                std::cout << "Type: Unknown"; break;
            }
    }

    std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION:    std::cout << "Severity: Notification"; break;
        case GL_DEBUG_SEVERITY_LOW:             std::cout << "Severity: Low"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:          std::cout << "Severity: Medium"; break;
        case GL_DEBUG_SEVERITY_HIGH:            std::cout << "Severity: High"; break;
        default:
            {
                std::cout << "Severity: Unknown"; break;
            }
    }

    std::cout << std::endl;
}

///<summary>Обработка ошибок GLFW.</summary>
///<param name = 'errorCode'>Код ошибки.</param>
///<param name = 'message'>Сообщение.</param>
void GLFWErrorCallback(int errorCode, const char* message)
{
    std::cout << "GLFW Error: " << std::endl;
    std::cout << "Message: " << message << std::endl;
}

///<summary>Возвращает ошибку glGetError().</summary>
///<param name = 'file'>Имя файла с ошибкой.</param>
///<param name = 'line'>Номер строки с ошибкой.</param>
GLenum glCheckError(const char *file, int line)
{
    GLenum errorCode;

    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;

        switch (errorCode)
        {
            case GL_INVALID_ENUM:                   error = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                  error = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:              error = "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY:                  error = "GL_OUT_OF_MEMORY"; break;
            default:
                {
                    std::stringstream ss;
                    ss << "Unknown error. Code = " << errorCode;
                    error = ss.str();
                    break;
                }
        }

        std::cout << error << " | file:" << file << " | line:" << line << std::endl;
    }

    return errorCode;
}