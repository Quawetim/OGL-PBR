#pragma once
#include "..\Object.h"

class Skybox : public Object
{
public:
	///<summary>Конструктор.</summary>
	///<param name = 'size'>Размер скайбокса.</param>
	Skybox(const float size);
	
	///<summary>Деструктор.</summary>
	~Skybox();
};