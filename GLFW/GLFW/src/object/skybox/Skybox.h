#pragma once
#include "..\Object.h"

class Skybox : public Object
{
public:
	///<summary>�����������.</summary>
	///<param name = 'size'>������ ���������.</param>
	Skybox(const float size);
	
	///<summary>����������.</summary>
	~Skybox();
};