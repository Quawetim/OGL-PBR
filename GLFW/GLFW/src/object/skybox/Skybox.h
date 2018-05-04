#pragma once
#include "..\Object.h"

class Skybox : public Object
{
private:
	///<summary>Идентификатор текстуры.</summary>
	unsigned int envMapID_;

public:
	///<summary>Конструктор.</summary>
	///<param name = 'size'>Размер скайбокса.</param>
	///<param name = 'envMap'>Идентификатор environment map.</param>
	Skybox(const float size, const unsigned int envMap);
	
	///<summary>Деструктор.</summary>
	~Skybox();

	///<summary>Задаёт environment map.</summary>
	///<param name = 'envMap'>Идентификатор environment map.</param>
	void setEnvMap(const unsigned int envMap);

	///<summary>Возвращает идентификатор CubeMap.</summary>
	unsigned int getEnvMapID() const;
};