#include "CoordinateAxes.h"
#include "..\..\renderer\Renderer.h"

///<summary>Конструктор.</summary>
CoordinateAxes::CoordinateAxes()
{
	this->modelMatrix_ = glm::mat4(1.0f);
	this->projectionMatrix_ = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	float vertexData[] =
	{
		//3x positions			//3x colors
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &this->VAO_);
	glGenBuffers(1, &this->VBO_);

	glBindVertexArray(this->VAO_);

	// Setup VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Push positions to layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Push colors to layout 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

///<summary>Деструктор.</summary>
CoordinateAxes::~CoordinateAxes()
{
	glDeleteVertexArrays(1, &this->VAO_);
	glDeleteBuffers(1, &this->VBO_);
}

///<summary>Возвращает VAO.</summary>
unsigned int CoordinateAxes::getVAO() const
{
	return this->VAO_;
}

///<summary>Возвращает матрицу модели.</summary>
glm::mat4 CoordinateAxes::getModelMatrix() const
{
	return this->modelMatrix_;
}

///<summary>Возвращает матрицу проекции.</summary>
glm::mat4 CoordinateAxes::getProjectionMatrix() const
{
	return this->projectionMatrix_;
}