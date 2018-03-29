#pragma once
#include "../../includes/Includes.h"

///<summary>������ ������������ ���.
///<para>x - �������.</para>
///<para>y - ������.</para>
///<para>z - �����.</para>
///</summary>
class CoordinateAxes
{
private:
	///<summary>Vertex array object.</summary>
	unsigned int VAO_;

	///<summary>Vertex buffer object.</summary>
	unsigned int VBO_;

	///<summary>�������.</summary>
	std::vector<QVertexData> vertices_;

	///<summary>�������.</summary>
	std::vector<unsigned int> indices_;

	///<summary>������� ������.</summary>
	glm::mat4 modelMatrix_;

	///<summary>������� ��������.</summary>
	glm::mat4 projectionMatrix_;
	
public:
	///<summary>�����������.</summary>
	CoordinateAxes();

	///<summary>����������.</summary>
	~CoordinateAxes();

	///<summary>��������� ����.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	void draw(const Shader shader, const glm::mat4 view_matrix);
};
