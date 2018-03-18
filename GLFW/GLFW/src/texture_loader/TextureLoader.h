#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

class TextureLoader
{
public:
    TextureLoader() {};

    ///<summary>Загрузка текстуры из файла.</summary>
    ///<para name ='path'>Путь к текстуре.</para>
    unsigned int loadTexture(std::string path);
};