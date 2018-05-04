#include "TextureLoader.h"

///<summary>Загрузка текстуры из файла.</summary>
///<param name = 'path'>Путь к текстуре.</param>
///<param name = 'type'>Тип текстуры.</param>
unsigned int textureLoader::loadTexture(const std::string path, TextureType type)
{
    unsigned int ID;

    int width, height, color_channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &color_channels, 0);

    if (data)
    {
		std::string msg = "Texture found. PATH: " + path;
        logger.log(__FUNCTION__, ErrorType::info, msg);

		GLint internalFormat;

		if (type == TextureType::diffuse)
		{
			internalFormat = GL_SRGB;
		}
		else
		{
			internalFormat = GL_RGB;
		}

        GLenum format;

        switch (color_channels)
        {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
        }

		glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
		glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
    }
    else
    {
		stbi_image_free(data);

		std::string msg = "Texture not found. PATH: " + path;
        logger.log(__FUNCTION__, ErrorType::error, msg);
    }    

    return ID;
}

///<summary>Загрузка HDR текстуры из файла.</summary>
///<param name = 'path'>Путь к текстуре после resources/textures/.</param>
unsigned int textureLoader::loadTextureHDR(const std::string path)
{
	unsigned int ID;

	int width, height, color_channels;

	std::string s = "resources/textures/" + path;

	stbi_set_flip_vertically_on_load(true);
	float *data = stbi_loadf(s.c_str(), &width, &height, &color_channels, 0);

	if (data)
	{
		std::string msg = "Texture found. PATH: " + path;
		logger.log(__FUNCTION__, ErrorType::info, msg);

		GLint internalFormat = GL_RGB16F;
		GLenum format;

		switch (color_channels)
		{
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);

		std::string msg = "Texture not found. PATH: " + path;
		logger.log(__FUNCTION__, ErrorType::error, msg);
	}

	return ID;
}

///<summary>Загрузка кубической текстуры.</summary>
///<param name = 'folder'>Папка с текстурами граней.</param>
unsigned int textureLoader::loadCubeMap(const std::string folder)
{
	unsigned int ID;

	int width, height, colorChannels;
	unsigned char *data;

	std::string faces[6];

	std::string s = "resources/textures/" + folder;

	faces[0] = s + "/x+.jpg";
	faces[1] = s + "/x-.jpg";
	faces[2] = s + "/y+.jpg";
	faces[3] = s + "/y-.jpg";
	faces[4] = s + "/z+.jpg";
	faces[5] = s + "/z-.jpg";

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (size_t i = 0; i < 6; i++)
	{
		data = stbi_load(faces[i].c_str(), &width, &height, &colorChannels, 0);

		if (data)
		{
			GLint internalFormat = GL_SRGB;
			GLenum format;

			switch (colorChannels)
			{
				case 1: format = GL_RED; break;
				case 3: format = GL_RGB; break;
				case 4: format = GL_RGBA; break;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);

			std::string msg = "Texture not found. PATH: " + faces[i];
			logger.log(__FUNCTION__, ErrorType::error, msg);
		}		
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return ID;
}

///<summary>Загрузка кубической HDR текстуры.</summary>
///<param name = 'folder'>Папка с текстурами граней.</param>
///<param name = 'resolution'>Разрешение.</param>
unsigned int textureLoader::loadCubeMapHDR(const std::string folder, const int resolution)
{
	unsigned int ID;

	int width, height, colorChannels;
	float *data;

	std::string faces[6];

	std::string s = "resources/textures/" + folder + "/" + std::to_string(resolution);

	faces[0] = s + "/x+.hdr";
	faces[1] = s + "/x-.hdr";
	faces[2] = s + "/y+.hdr";
	faces[3] = s + "/y-.hdr";
	faces[4] = s + "/z+.hdr";
	faces[5] = s + "/z-.hdr";

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (size_t i = 0; i < 6; i++)
	{
		stbi_set_flip_vertically_on_load(true);
		data = stbi_loadf(faces[i].c_str(), &width, &height, &colorChannels, 0);

		if (data)
		{
			GLint internalFormat = GL_RGB16F;
			GLenum format;

			switch (colorChannels)
			{
				case 1: format = GL_RED; break;
				case 3: format = GL_RGB; break;
				case 4: format = GL_RGBA; break;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_FLOAT, data);

			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);

			std::string msg = "Texture not found. PATH: " + faces[i];
			logger.log(__FUNCTION__, ErrorType::error, msg);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return ID;
}

///<summary>Удаляет текстуру из памяти.</summary>
///<param name = 'ID'>Идентификатор текстуры.</param>
void textureLoader::deleteTexture(unsigned int ID)
{
	glDeleteTextures(1, &ID);
}