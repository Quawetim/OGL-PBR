#include "Callbacks.h"

///<summary>Обработка клавиатуры.</summary>
///<param name = 'window'>Указатель на окно.</param>
///<param name = 'key'>Клавиша.</param>
///<param name = 'scancode'>Scancode.</param>
///<param name = 'action'>Действие.</param>
///<param name = 'mods'>Модификаторы.</param>
void Callbacks::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static int PrevCamera;
    
    // Клавиши для выхода из приложения
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    //if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    //if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

    // FOV по-умолчанию
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) { FOV = 60.0f; }

    // Отображение курсора при удерживании CTRL
    //if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    //{
    //    windowInfo.setShowCursor(true);
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //}

    //// Скрытие курсора при отпускании CTRL
    //if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    //{
    //    windowInfo.setShowCursor(false);
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //    glfwSetCursorPos(window, windowInfo.getHalfWidth(), windowInfo.getHalfHeight());
    //}
}

///<summary>Обработка колёсика мышки.</summary>
///<param name = 'window'>Указатель на окно.</param>
///<param name = 'xoffset'>Смещение по оси X.</param>
///<param name = 'yoffset'>Смещение по оси Y.</param>
void Callbacks::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
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
void Callbacks::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowInfo.setWidth(width);
    windowInfo.setHeight(height);
}

///<summary>Вывод ошибок в консоль.</summary>
///<param name = 'source'>Источник ошибки.</param>
///<param name = 'type'>Тип ошибки.</param>
///<param name = 'id'>Код ошибки.</param>
///<param name = 'severity'>Важность ошибки.</param>
///<param name = 'length'>Длина сообщения.</param>
///<param name = 'message'>Сообщение.</param>
///<param name = 'userParam'>Параметры.</param>
void APIENTRY Callbacks::glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::stringstream ss;

    ss << "**************************" << std::endl;
    ss << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:               ss << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     ss << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   ss << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       ss << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:       ss << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:             ss << "Source: Other"; break;
        default:
            {
                ss << "Source: Unknown"; break;
            }
    }

    ss << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               ss << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "Type: Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "Type: Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         ss << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         ss << "Type: Perfomance"; break;
        case GL_DEBUG_TYPE_MARKER:              ss << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           ss << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               ss << "Type: Other"; break;
        default:
            {
                ss << "Type: Unknown"; break;
            }
    }

    ss << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION:    ss << "Severity: Notification"; break;
        case GL_DEBUG_SEVERITY_LOW:             ss << "Severity: Low"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:          ss << "Severity: Medium"; break;
        case GL_DEBUG_SEVERITY_HIGH:            ss << "Severity: High"; break;
        default:
            {
                ss << "Severity: Unknown"; break;
            }
    }

    ss << std::endl;
    ss << "**************************" << std::endl;


}

///<summary>Обработка ошибок GLFW.</summary>
///<param name = 'errorCode'>Код ошибки.</param>
///<param name = 'message'>Сообщение.</param>
void Callbacks::GLFWErrorCallback(int errorCode, const char* message)
{
    std::cout << "GLFW Error: " << std::endl;
    std::cout << "Message: " << message << std::endl;
}

///<summary>Возвращает ошибку glGetError().</summary>
///<param name = 'file'>Имя файла с ошибкой.</param>
///<param name = 'line'>Номер строки с ошибкой.</param>
GLenum Callbacks::glCheckError(const char *file, int line)
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