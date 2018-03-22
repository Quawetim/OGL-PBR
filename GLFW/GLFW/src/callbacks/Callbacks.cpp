#include "Callbacks.h"

///<summary>��������� ����������.</summary>
///<param name = 'window'>��������� �� ����.</param>
///<param name = 'key'>�������.</param>
///<param name = 'scancode'>Scancode.</param>
///<param name = 'action'>��������.</param>
///<param name = 'mods'>������������.</param>
void Callbacks::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static int PrevCamera;
    
    // ������� ��� ������ �� ����������
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    //if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    //if (key == GLFW_KEY_KP_ENTER && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

    // FOV ��-���������
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) { FOV = 60.0f; }

    // ����������� ������� ��� ����������� CTRL
    //if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    //{
    //    windowInfo.setShowCursor(true);
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //}

    //// ������� ������� ��� ���������� CTRL
    //if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    //{
    //    windowInfo.setShowCursor(false);
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //    glfwSetCursorPos(window, windowInfo.getHalfWidth(), windowInfo.getHalfHeight());
    //}
}

///<summary>��������� ������� �����.</summary>
///<param name = 'window'>��������� �� ����.</param>
///<param name = 'xoffset'>�������� �� ��� X.</param>
///<param name = 'yoffset'>�������� �� ��� Y.</param>
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

///<summary>��������� ��������� ������� ����.</summary>
///<param name = 'window'>��������� �� ����.</param>
///<param name = 'width'>����� ������.</param>
///<param name = 'height'>����� ������.</param>
void Callbacks::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowInfo.setWidth(width);
    windowInfo.setHeight(height);
}

///<summary>����� ������ � �������.</summary>
///<param name = 'source'>�������� ������.</param>
///<param name = 'type'>��� ������.</param>
///<param name = 'id'>��� ������.</param>
///<param name = 'severity'>�������� ������.</param>
///<param name = 'length'>����� ���������.</param>
///<param name = 'message'>���������.</param>
///<param name = 'userParam'>���������.</param>
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

///<summary>��������� ������ GLFW.</summary>
///<param name = 'errorCode'>��� ������.</param>
///<param name = 'message'>���������.</param>
void Callbacks::GLFWErrorCallback(int errorCode, const char* message)
{
    std::cout << "GLFW Error: " << std::endl;
    std::cout << "Message: " << message << std::endl;
}

///<summary>���������� ������ glGetError().</summary>
///<param name = 'file'>��� ����� � �������.</param>
///<param name = 'line'>����� ������ � �������.</param>
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