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

	///<summary>������� ������.</summary>
	glm::mat4 modelMatrix_;

	///<summary>������� ��������.</summary>
	glm::mat4 projectionMatrix_;
	
public:
	///<summary>�����������.</summary>
	CoordinateAxes();

	///<summary>����������.</summary>
	~CoordinateAxes();

	///<summary>���������� VAO.</summary>
	unsigned int getVAO() const;

	///<summary>���������� ������� ������.</summary>
	glm::mat4 getModelMatrix() const;

	///<summary>���������� ������� ��������.</summary>
	glm::mat4 getProjectionMatrix() const;
};
