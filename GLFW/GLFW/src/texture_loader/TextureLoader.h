#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

class TextureLoader
{
public:
    TextureLoader() {};

    ///<summary>�������� �������� �� �����.</summary>
    ///<para name ='path'>���� � ��������.</para>
    unsigned int loadTexture(std::string path);
};