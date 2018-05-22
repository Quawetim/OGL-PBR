#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

///<summary>Загрузчик текстур.</summary>
namespace textureLoader
{
    ///<summary>Загрузка текстуры из файла.</summary>
    ///<param name = 'path'>Путь к текстуре.</param>
	///<param name = 'type'>Тип текстуры.</param>
	///<param name = 'flip'>Нужно ли перевернуть текстуру.</param>
    unsigned int loadTexture(const std::string path, TextureType type, const bool flip);

	///<summary>Загрузка HDR текстуры из файла.</summary>
	///<param name = 'path'>Путь к текстуре после resources/textures/.</param>
	unsigned int loadTextureHDR(const std::string path);

	///<summary>Загрузка кубической текстуры.</summary>
	///<param name = 'folder'>Папка с текстурами граней.</param>
	unsigned int loadCubeMap(const std::string folder);

	///<summary>Загрузка кубической HDR текстуры.</summary>
	///<param name = 'folder'>Папка с текстурами граней.</param>
	///<param name = 'resolution'>Разрешение.</param>
	unsigned int loadCubeMapHDR(const std::string folder, const int resolution);

	///<summary>Удаляет текстуру из памяти.</summary>
	///<param name = 'ID'>Идентификатор текстуры.</param>
	void deleteTexture(unsigned int ID);
};