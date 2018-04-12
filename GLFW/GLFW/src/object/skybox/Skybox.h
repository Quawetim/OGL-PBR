#pragma once
#include "..\Object.h"

class Skybox : public Object
{
private:
	///<summary>������������� ��������.</summary>
	unsigned int cubeMapID;

public:
	///<summary>�����������.</summary>
	///<param name = 'size'>������ ���������.</param>
	Skybox(const float size);
	
	///<summary>����������.</summary>
	~Skybox();

	///<summary>���������� ������������� CubeMap.</summary>
	unsigned int getCubeMapID() const;
};