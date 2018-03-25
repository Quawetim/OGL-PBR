#include "CoordinateAxes.h"

CoordinateAxes::CoordinateAxes()
{
	this->modelMatrix = glm::mat4(1.0f);
	this->projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.05f, 500.0f);
	//this->projectionMatrix = glm::ortho(-800.0f, 800.0f, -600.0f, 600.0f, 0.001f, 100.0f);
	//glOrtho(-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -10.0, 1.0);

	float vertex_data[] =
	{
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);

	// Setup VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	// Push positions to layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

CoordinateAxes::~CoordinateAxes()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}

void CoordinateAxes::draw(const Shader shader, const glm::mat4 viewMatrix)
{
	shader.activate();

	//viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 V = glm::lookAt(glm::vec3(2.0f * cos(45), 2.0f * cos(0), 2.0 * sin(45)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setProjectionViewModelMatrices(this->projectionMatrix, V, this->modelMatrix);
	
	glViewport(5, 5, 60, 60);

	glBindVertexArray(this->VAO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	glViewport(0, 0, windowInfo.getWidth(), windowInfo.getHeight());
}