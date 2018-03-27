#include "Shader.h"

///<summary>Проверка на ошибки компиляции шейдера.</summary>
///<param name = 'id'>Идентификатор шейдера.</param>
///<param name = 'type'>Тип шейдера.</param>
void Shader::checkCompilationErrors(const unsigned int id, const std::string type) const
{
    int success;
    char log[1024];
    
    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, NULL, log);
			std::string msg = "Shader compilation error. Type: " + type + "\n" + log;
            logger.log("Shader::checkCompileErrors", QErrorType::error, msg);
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, log);
			std::string msg = "Program linking error. Type: " + type + "\n" + log;
            logger.log("Shader::checkCompileErrors", QErrorType::error, msg);
        }
    }
}

///<summary>Конструктор.</summary>
///<param name = 'vs_path'>Путь к вершинному шейдеру.</param>
///<param name = 'fs_path'>Путь к фрагментному шейдеру.</param>
Shader::Shader(std::string vs_path, std::string fs_path)
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
		std::string msg;
        logger.log("Shader::Shader", QErrorType::error, "Shader file not found.");

		msg = "VS_PATH: " + vs_path;
        logger.log("Shader::Shader", QErrorType::info, msg);

		msg = "FS_PATH: " + fs_path;
        logger.log("Shader::Shader", QErrorType::info, msg);
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
    this->id_ = glCreateProgram();
    glAttachShader(this->id_, vertex);
    glAttachShader(this->id_, fragment);
    glLinkProgram(this->id_);
    checkCompilationErrors(this->id_, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

///<summary>Возвращает идентификатор шейдера.</summary>
unsigned int Shader::getID() const
{
    return this->id_;
}

///<summary>Активация шейдера.</summary>
void Shader::activate() const
{
    glUseProgram(this->id_);
}

///<summary>Задаёт (передаёт) значение переменной типа bool в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'value'>Задаваемое значение.</param>
void Shader::setBool(const std::string &name, const bool value) const
{
	this->activate();
    glUniform1i(glGetUniformLocation(this->id_, name.c_str()), (int)value);
}

///<summary>Задаёт (передаёт) значение переменной типа int в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'value'>Задаваемое значение.</param>
void Shader::setInt(const std::string &name, const int value) const
{
	this->activate();
    glUniform1i(glGetUniformLocation(this->id_, name.c_str()), value);
}

///<summary>Задаёт (передаёт) значение переменной типа float в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'value'>Задаваемое значение.</param>
void Shader::setFloat(const std::string &name, const float value) const
{
	this->activate();
    glUniform1f(glGetUniformLocation(this->id_, name.c_str()), value);
}

///<summary>Задаёт (передаёт) значение переменной типа vec3 в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'value'>Задаваемое значение.</param>
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	this->activate();
    glUniform3fv(glGetUniformLocation(this->id_, name.c_str()), 1, &value[0]);
}

///<summary>Задаёт (передаёт) значение переменной типа mat4 в шейдере по имени.</summary>
///<param name = 'name'>Имя переменной в шейдере.</param>
///<param name = 'mat'>Задаваемое значение.</param>
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

///<summary>Задаёт (передаёт) матрицу проекции.</summary>
///<param name = 'projectionMatrix'>Матрица проекции.</param>
void Shader::setProjectionMatrix(const glm::mat4 projectionMatrix) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
}

///<summary>Задаёт (передаёт) матрицу вида.</summary>
///<param name = 'viewMatrix'>Матрица вида.</param>
void Shader::setViewMatrix(const glm::mat4 viewMatrix) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, "viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
}

///<summary>Задаёт (передаёт) матрицу модели.</summary>
///<param name = 'modelMatrix'>Матрица модели.</param>
void Shader::setModelMatrix(const glm::mat4 modelMatrix) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, "modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
}

///<summary>Задаёт (передаёт) матрицы проекции, вида и модели.</summary>
///<param name = 'projection_matrix'>Матрица проекции.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'model_matrix'>Матрица модели.</param>
void Shader::setProjectionViewModelMatrices(const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::mat4 model_matrix) const
{
	this->activate();
    this->setProjectionMatrix(projection_matrix);
    this->setViewMatrix(view_matrix);
    this->setModelMatrix(model_matrix);
}