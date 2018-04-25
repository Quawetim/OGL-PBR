#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

///<summary>��������� �������.</summary>
namespace textureLoader
{
    ///<summary>�������� �������� �� �����.</summary>
    ///<param name = 'path'>���� � ��������.</param>
    unsigned int loadTexture(const std::string path, TextureType type);

	///<summary>�������� ���������� ��������.</summary>
	///<param name = 'folder'>����� � ���������� ������.</param>
	unsigned int loadCubeMap(const std::string folder);

	///<summary>������� �������� �� ������.</summary>
	///<param name = 'ID'>������������� ��������.</param>
	void deleteTexture(unsigned int ID);
};