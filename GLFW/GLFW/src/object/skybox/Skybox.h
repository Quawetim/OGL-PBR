#pragma once
#include "..\Object.h"

class Skybox : public Object
{
private:
	///<summary>������������� ��������.</summary>
	unsigned int envMapID_;

public:
	///<summary>�����������.</summary>
	///<param name = 'size'>������ ���������.</param>
	///<param name = 'envMap'>������������� environment map.</param>
	Skybox(const float size, const unsigned int envMap);
	
	///<summary>����������.</summary>
	~Skybox();

	///<summary>���������� ������������� CubeMap.</summary>
	unsigned int getEnvMapID() const;
};