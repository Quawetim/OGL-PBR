#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

///<summary>Загрузчик текстур.</summary>
namespace TextureLoader
{
    ///<summary>Загрузка текстуры из файла.</summary>
    ///<param name = 'path'>Путь к текстуре.</param>
    unsigned int loadTexture(std::string path);
};