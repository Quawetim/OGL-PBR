#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

///<summary>Загрузчик текстур.</summary>
namespace textureLoader
{
    ///<summary>Загрузка текстуры из файла.</summary>
    ///<param name = 'path'>Путь к текстуре.</param>
    unsigned int loadTexture(const std::string path);

	///<summary>Загрузка кубической текстуры.</summary>
	///<param name = 'folder'>Папка с текстурами граней.</param>
	unsigned int loadCubeMap(const std::string folder);

	///<summary>Удаляет текстуру из памяти.</summary>
	///<param name = 'ID'>Идентификатор текстуры.</param>
	void deleteTexture(unsigned int ID);
};