#pragma once
#include "..\includes\Includes.h"
#include <stb_image\stb_image.h>

///<summary>��������� �������.</summary>
namespace textureLoader
{
    ///<summary>�������� �������� �� �����.</summary>
    ///<param name = 'path'>���� � ��������.</param>
	///<param name = 'type'>��� ��������.</param>
    unsigned int loadTexture(const std::string path, TextureType type);

	///<summary>�������� HDR �������� �� �����.</summary>
	///<param name = 'path'>���� � �������� ����� resources/textures/.</param>
	unsigned int loadTextureHDR(const std::string path);

	///<summary>�������� ���������� ��������.</summary>
	///<param name = 'folder'>����� � ���������� ������.</param>
	unsigned int loadCubeMap(const std::string folder);

	///<summary>�������� ���������� HDR ��������.</summary>
	///<param name = 'folder'>����� � ���������� ������.</param>
	///<param name = 'resolution'>����������.</param>
	unsigned int loadCubeMapHDR(const std::string folder, const int resolution);

	///<summary>������� �������� �� ������.</summary>
	///<param name = 'ID'>������������� ��������.</param>
	void deleteTexture(unsigned int ID);
};