#include "Shader.h"

///<summary>Проверка на ошибки компиляции шейдера.</summary>
///<param name = 'id'>Идентификатор шейдера.</param>
///<param name = 'type'>Тип шейдера.</param>
void Shader::checkCompileErrors(unsigned int id, std::string type)
{
    int success;
    char log[1024];
    
    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, NULL, log);
            logger.log("SHADER::checkCompileErrors", errorType::error, std::string("Shader compilation error. Type: " + type + "\n" + log));
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, log);
            logger.log("SHADER::checkCompileErrors", errorType::error, std::string("Program linking error. Type: " + type + "\n" + log));
        }
    }
}

///<summary>Конструктор.</summary>
///<param name = 'vs_path'>Путь к вершинному шейдеру.</param>
///<param name = 'fs_path'>Путь к фрагментному шейдеру.</param>
Shader::Shader(const std::string vs_path, const std::string fs_path)
{
    // Reading
    std::string vs_code, fs_code;
    std::ifstream vs_file, fs_file;

    vs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vs_file.open(vs_path);
        fs_file.open(fs_path);

        std::stringstream vs_stream, fs_stream;

        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();

        vs_file.close();
        fs_file.close();
        
        vs_code = vs_stream.str();
        fs_code = fs_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        logger.log("SHADER::Shader", errorType::error, "Can't read shader file.");
        logger.log("SHADER::Shader", errorType::info, std::string("VS_PATH: " + vs_path));
        logger.log("SHADER::Shader", errorType::info, std::string("FS_PATH: " + fs_path));
    }

    const char* vShaderCode = vs_code.c_str();
    const char * fShaderCode = fs_code.c_str();

    // Compilation
    unsigned int vertex, fragment;
    
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // Full shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {};

///<summary>Возвращает идентификатор шейдера.</summary>
unsigned int Shader::getID()
{
    return this->ID;
}

///<summary>Активация шейдера.</summary>
void Shader::activate()
{
    glUseProgram(this->ID);
}

///<summary>Задаёт (передаёт) значение переменной типа bool в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'value'>Задаваемое значение.</param>
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

///<summary>Задаёт (передаёт) значение переменной типа int в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'value'>Задаваемое значение.</param>
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

///<summary>Задаёт (передаёт) значение переменной типа float в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'value'>Задаваемое значение.</param>
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}