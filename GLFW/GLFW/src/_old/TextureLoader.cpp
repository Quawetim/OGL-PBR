#include "TextureLoader.h"

/* Загрузка BMP-текстуры */
/* Image - путь к текстуре */
GLuint LoadBMP(const char *Image) 
{
	printf("Searching BMP texture %s...", Image);

	unsigned int DataPos, Size, Width, Height;
	unsigned char Header[54], *Data;	

	FILE *Fin = fopen(Image, "rb");
	
	if (!Fin) 
	{ 
		printf("BMP texture %s not found.\n", Image);
		return 0; 
	}
	else
	{
		printf("BMP texture found, OK.\n");
	}

	/* Читаем заколовок 54 байта */
	if (fread(Header, 1, 54, Fin) != 54) 
	{
		printf("Wrong BMP texture.\n");
		fclose(Fin);
		return 0;
	}

	/* Проверяем заголовок */
	if (Header[0] != 'B' || Header[1] != 'M') 
	{
		printf("Wrong BMP texture.\n");
		fclose(Fin);
		return 0;
	}

	if (*(int*)&(Header[0x1E]) != 0) 
	{ 
		printf("Wrong BMP texture.\n"); 
		fclose(Fin);
		return 0; 
	}

	if (*(int*)&(Header[0x1C]) != 24) 
	{ 
		printf("Wrong BMP texture. Use 24-bit texture, instead of %d\n", *(int*)&(Header[0x1C]));
		fclose(Fin);
		return 0; 
	}

	/* Считываем информацию о файле */
	DataPos = *(int*)&(Header[0x0A]);
	Size = *(int*)&(Header[0x22]);
	Width = *(int*)&(Header[0x12]);
	Height = *(int*)&(Header[0x16]);

	/* Добавляем потерявшиеся данные */
	if (Size == 0) Size = Width * Height * 3;
	if (DataPos == 0) DataPos = 54;

	Data = new unsigned char[Size];

	/* Читаем оставшиеся данные */
	fread(Data, 1, Size, Fin);

	fclose(Fin);

	/* Создаём текстуру */
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, Data);

	delete[] Data;

	/* Билинейная фильтрация */
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

	/* Трилинейная фильтрация */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

#define FOURCC_DXT1 0x31545844 // "DXT1" в ASCII
#define FOURCC_DXT3 0x33545844 // "DXT3" в ASCII
#define FOURCC_DXT5 0x35545844 // "DXT5" в ASCII

/* Загрузка DDS-текстуры */
/* Image - путь к текстуре */
GLuint LoadDDS(const char * Image)
{
	printf("Searching DDS texture %s...", Image);

	unsigned int LinearSize, Width, Height, Size, MipMapCount, FourCC;
	unsigned char Header[124], *Data;
	char FileCode[4];
	
	FILE *Fin = fopen(Image, "rb");

	if (Fin == NULL) 
	{
		printf("DDS texture %s not found.\n", Image);
		return 0;
	}
	else
	{
		printf("DDS texture found, OK.\n");
	}

	fread(FileCode, 1, 4, Fin);

	if (strncmp(FileCode, "DDS ", 4) != 0) 
	{
		printf("Wrong DDS texture.\n");
		fclose(Fin);
		return 0;
	}

	/* Считываем заголовок */
	fread(&Header, 124, 1, Fin);

	Height = *(unsigned int*)&(Header[8]);
	Width = *(unsigned int*)&(Header[12]);
	LinearSize = *(unsigned int*)&(Header[16]);
	MipMapCount = *(unsigned int*)&(Header[24]);
	FourCC = *(unsigned int*)&(Header[80]);

	Size = MipMapCount > 1 ? LinearSize * 2 : LinearSize;
	Data = (unsigned char*)malloc(Size * sizeof(unsigned char));
	fread(Data, 1, Size, Fin);

	fclose(Fin);

	/* Определяем алгоритм сжатия */
	unsigned int components = (FourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	
	switch (FourCC)
	{
		case (FOURCC_DXT1):
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		}
		case (FOURCC_DXT3):
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		}
		case (FOURCC_DXT5):
		{
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		}
		default:
		{
			printf("Wrong DDS texture.\n");
			free(Data);
			return 0;
		}
	}

	/* Создаём текстуру */
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	for (unsigned int level = 0; level < MipMapCount && (Width || Height); ++level)
	{
		unsigned int size = ((Width + 3) / 4)*((Height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, Width, Height, 0, size, Data + offset);

		offset += size;
		Width /= 2;
		Height /= 2;

		/* Для текстур не степени двойки */
		if (Width < 1) Width = 1;
		if (Height < 1) Height = 1;
	}

	free(Data);

	return textureID;
}