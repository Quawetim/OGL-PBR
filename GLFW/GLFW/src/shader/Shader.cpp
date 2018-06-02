#include "Shader.h"

///<summary>�������� �� ������ ���������� �������.</summary>
///<param name = 'id'>������������� �������.</param>
///<param name = 'type'>��� �������.</param>
void Shader::checkCompilationErrors(const unsigned int id, const int type) const
{
    int success;
    char log[1024];
    
    if (type != 0)
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
			std::string msg;

			glGetShaderInfoLog(id, 1024, NULL, log);

			switch (type)
			{
				case 1: msg = "Shader compilation error.\nType: Vertex.\nName: " + this->name_ + ".vs\n" + std::string(log); break;
				case 2: msg = "Shader compilation error.\nType: Fragment.\nName: " + this->name_ + ".fs\n" + std::string(log); break;
				case 3: msg = "Shader compilation error.\nType: Geometry.\nName: " + this->name_ + ".gs\n" + std::string(log); break;
			}
            
            logger.log(__FUNCTION__, ErrorType::error, msg);
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, log);
			std::string msg = "Program linking error.\nType: Program.\n" + std::string(log);
            logger.log(__FUNCTION__, ErrorType::error, msg);
        }
    }
}

///<summary>�����������.</summary>
///<param name = 'name'>�������� ����� �������.</param>
Shader::Shader(std::string name)
{
	this->folder_ = "resources/shaders/";
	this->name_ = name;

	std::string path = this->folder_ + this->name_;

	this->vsPath_ = path + ".vs";
	this->fsPath_ = path + ".fs";
	this->gsPath_ = path + ".gs";
    
	// Reading
    std::string vsCode, fsCode, gsCode;
    std::ifstream vsFin, fsFin, gsFin;

	vsFin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fsFin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gsFin.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
		vsFin.open(this->vsPath_);
		fsFin.open(this->fsPath_);

        std::stringstream vsStream, fsStream;

		vsStream << vsFin.rdbuf();
		fsStream << fsFin.rdbuf();

		vsFin.close();
		fsFin.close();
        
		vsCode = vsStream.str();
		fsCode = fsStream.str();
    }
    catch (std::ifstream::failure e)
    {
		std::string msg;
        logger.log(__FUNCTION__, ErrorType::info, "Shader file not found.");

		msg = "VS_PATH: " + this->vsPath_;
        logger.log(__FUNCTION__, ErrorType::info, msg);

		msg = "FS_PATH: " + this->fsPath_;
        logger.log(__FUNCTION__, ErrorType::error, msg);
    }

	bool gsExist = false;
	try
	{
		gsFin.open(this->gsPath_);

		std::stringstream gsStream;

		gsStream << gsFin.rdbuf();

		gsFin.close();

		gsCode = gsStream.str();
		gsExist = true;
	}
	catch (std::ifstream::failure e)
	{
		std::string msg;
		logger.log(__FUNCTION__, ErrorType::info, "Shader file not found.");

		msg = "GS_PATH: " + this->gsPath_;
		logger.log(__FUNCTION__, ErrorType::info, msg);
	}

    const char* vShaderCode = vsCode.c_str();
    const char* fShaderCode = fsCode.c_str();
	const char* gShaderCode;

	if (gsExist)
	{
		gShaderCode = gsCode.c_str();
	}

    // Compilation
    
    // Vertex shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompilationErrors(vertex, 1);
    
    // Fragment Shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompilationErrors(fragment, 2);

	unsigned int geometry;
	if (gsExist)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompilationErrors(geometry, 3);
	}
    
    // Full shader
    this->id_ = glCreateProgram();
    glAttachShader(this->id_, vertex);
    glAttachShader(this->id_, fragment);

	if (gsExist) glAttachShader(this->id_, geometry);

    glLinkProgram(this->id_);
    checkCompilationErrors(this->id_, 0);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
	if (gsExist) glDeleteShader(geometry);
}

///<summary>���������� ������������� �������.</summary>
unsigned int Shader::getID() const
{
    return this->id_;
}

///<summary>��������� �������.</summary>
void Shader::activate() const
{
    glUseProgram(this->id_);
}

///<summary>����� (�������) �������� ���������� ���� bool � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setBool(const std::string &name, const bool value) const
{
	this->activate();
	glUniform1i(glGetUniformLocation(this->id_, name.c_str()), static_cast<int>(value));
}

///<summary>����� (�������) �������� ���������� ���� int � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setInt(const std::string &name, const int value) const
{
	this->activate();
    glUniform1i(glGetUniformLocation(this->id_, name.c_str()), value);
}

///<summary>����� (�������) �������� ���������� ���� float � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setFloat(const std::string &name, const float value) const
{
	this->activate();
    glUniform1f(glGetUniformLocation(this->id_, name.c_str()), value);
}

///<summary>����� (�������) �������� ���������� ���� vec2 � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	this->activate();
	glUniform2fv(glGetUniformLocation(this->id_, name.c_str()), 1, &value[0]);
}

///<summary>����� (�������) �������� ���������� ���� vec3 � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'value'>���������� ��������.</param>
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	this->activate();
    glUniform3fv(glGetUniformLocation(this->id_, name.c_str()), 1, &value[0]);
}

///<summary>����� (�������) �������� ���������� ���� mat4 � ������� �� �����.</summary>
///<param name = 'name'>��� ���������� � �������.</param>
///<param name = 'mat'>���������� ��������.</param>
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

///<summary>����� (�������) ������� ��������.</summary>
///<param name = 'projectionMatrix'>������� ��������.</param>
void Shader::setProjectionMatrix(const glm::mat4 projectionMatrix) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
}

///<summary>����� (�������) ������� ����.</summary>
///<param name = 'viewMatrix'>������� ����.</param>
void Shader::setViewMatrix(const glm::mat4 viewMatrix) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, "viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
}

///<summary>����� (�������) ������� ������.</summary>
///<param name = 'modelMatrix'>������� ������.</param>
void Shader::setModelMatrix(const glm::mat4 modelMatrix) const
{
	this->activate();
    glUniformMatrix4fv(glGetUniformLocation(this->id_, "modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
}

///<summary>����� (�������) ������� ��������, ���� � ������.</summary>
///<param name = 'projection_matrix'>������� ��������.</param>
///<param name = 'view_matrix'>������� ����.</param>
///<param name = 'model_matrix'>������� ������.</param>
void Shader::setProjectionViewModelMatrices(const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::mat4 model_matrix) const
{
	this->activate();
    this->setProjectionMatrix(projection_matrix);
    this->setViewMatrix(view_matrix);
    this->setModelMatrix(model_matrix);
}