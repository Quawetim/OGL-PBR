#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

///<summary>��������� �������.</summary>
namespace TextureLoader
{
    ///<summary>�������� �������� �� �����.</summary>
    ///<param name = 'path'>���� � ��������.</param>
    unsigned int loadTexture(std::string path);
};