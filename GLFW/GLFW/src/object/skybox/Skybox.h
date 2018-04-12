#pragma once
#include "..\Object.h"

class Skybox : public Object
{
private:
	///<summary>Идентификатор текстуры.</summary>
	unsigned int cubeMapID;

public:
	///<summary>Конструктор.</summary>
	///<param name = 'size'>Размер скайбокса.</param>
	Skybox(const float size);
	
	///<summary>Деструктор.</summary>
	~Skybox();

	///<summary>Возвращает идентификатор CubeMap.</summary>
	unsigned int getCubeMapID() const;
};