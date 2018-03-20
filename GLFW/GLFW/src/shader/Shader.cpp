#include "Shader.h"

///<summary>�������� �� ������ ���������� �������.</summary>
///<param name = 'id'>������������� �������.</param>
///<param name = 'type'>��� �������.</param>
void Shader::checkCompilationErrors(unsigned int id, std::string type)
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

///<summary>�����������.</summary>
///<param name = 'vs_path'>���� � ���������� �������.</param>
///<param name = 'fs_path'>���� � ������������ �������.</param>
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
    checkCompilationErrors(vertex, "VERTEX");
    
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompilationErrors(fragment, "FRAGMENT");
    
    // Full shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompilationErrors(ID, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

///<summary>���������� ������������� �������.</summary>
unsigned int Shader::getID()
{
    return this->ID;
}

///<summary>��������� �������.</summary>
void Shader::activate()
{
    glUseProgram(this->ID);
}

///<summary>����� (�������) �������� ���������� ���� bool � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setBool(const std::string &name, const bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

///<summary>����� (�������) �������� ���������� ���� int � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setInt(const std::string &name, const int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

///<summary>����� (�������) �������� ���������� ���� float � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setFloat(const std::string &name, const float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

///<summary>����� (�������) �������� ���������� ���� vec3 � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

///<summary>����� (�������) �������� ���������� ���� mat4 � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'mat'>���������� ��������.</param>
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

///<summary>����� (�������) ������� ��������.</summary>
///<param name = 'projectionMatrix'>������� ��������.</param>
void Shader::setProjectionMatrix(const glm::mat4 projectionMatrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
}

///<summary>����� (�������) ������� ����.</summary>
///<param name = 'viewMatrix'>������� ����.</param>
void Shader::setViewMatrix(const glm::mat4 viewMatrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, "viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
}

///<summary>����� (�������) ������� ������.</summary>
///<param name = 'modelMatrix'>������� ������.</param>
void Shader::setModelMatrix(const glm::mat4 modelMatrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, "modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
}

///<summary>����� (�������) ������� ��������, ���� � ������.</summary>
///<param name = 'projectionMatrix'>������� ��������.</param>
///<param name = 'viewMatrix'>������� ����.</param>
///<param name = 'modelMatrix'>������� ������.</param>
void Shader::setProjectionViewModelMatrices(const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix, const glm::mat4 modelMatrix) const
{
    this->setProjectionMatrix(projectionMatrix);
    this->setViewMatrix(viewMatrix);
    this->setModelMatrix(modelMatrix);
}