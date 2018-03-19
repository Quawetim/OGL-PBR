#include "Shader.h"

extern Logger logger;

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
            logger.log("SHADER::checkCompileErrors", QErrorType::error, std::string("Shader compilation error. Type: " + type + "\n" + log));
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, log);
            logger.log("SHADER::checkCompileErrors", QErrorType::error, std::string("Program linking error. Type: " + type + "\n" + log));
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
        logger.log("SHADER::Shader", QErrorType::error, "Shader file not found.");
        logger.log("SHADER::Shader", QErrorType::info, std::string("VS_PATH: " + vs_path));
        logger.log("SHADER::Shader", QErrorType::info, std::string("FS_PATH: " + fs_path));
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

///<summary>Задаёт (передаёт) значение переменной типа mat4 в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'mat'>Задаваемое значение.</param>
void Shader::setMat4(const std::string &name, const glm::mat4 mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}