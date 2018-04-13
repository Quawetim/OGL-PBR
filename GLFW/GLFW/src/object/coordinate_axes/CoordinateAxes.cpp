#include "CoordinateAxes.h"
#include "..\..\renderer\Renderer.h"

///<summary>Конструктор.</summary>
CoordinateAxes::CoordinateAxes()
{
	this->modelMatrix_ = glm::mat4(1.0f);
	this->projectionMatrix_ = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	float vertexData[] =
	{
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

///<summary>Отрисовка осей.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
void CoordinateAxes::draw(const Shader shader, const glm::mat4 view_matrix)
{
	shader.activate();
	shader.setProjectionViewModelMatrices(this->projectionMatrix_, view_matrix, this->modelMatrix_);
	
	glViewport(5, 5, 60, 60);

	glBindVertexArray(this->VAO_);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	glViewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());
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