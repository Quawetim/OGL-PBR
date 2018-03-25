#pragma once
#include "../includes/Includes.h"

class CoordinateAxes
{
private:
	///<summary>Vertex array object.</summary>
	unsigned int VAO;

	///<summary>Vertex buffer object.</summary>
	unsigned int VBO;

	std::vector<QVertexData> vertices;
	std::vector<unsigned int> indices;

	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrix;
	
public:
	CoordinateAxes();
	~CoordinateAxes();
	void draw(const Shader shader, const glm::mat4 viewMatrix);
};
