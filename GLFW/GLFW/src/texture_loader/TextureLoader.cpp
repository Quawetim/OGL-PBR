#include "TextureLoader.h"

///<summary>Загрузка текстуры из файла.</summary>
///<param name = 'path'>Путь к текстуре.</param>
unsigned int textureLoader::loadTexture(const std::string path)
{
    unsigned int ID;

    glGenTextures(1, &ID);

    int width, height, color_channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &color_channels, 0);

    if (data)
    {
		std::string msg = "Texture found. PATH: " + path;
        logger.log("TextureLoader::loadTexture", QErrorType::info, msg);

        GLenum format;

        switch (color_channels)
        {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
        }

        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
    }
    else
    {
		stbi_image_free(data);

		std::string msg = "Texture not found. PATH: " + path;
        logger.log("TextureLoader::loadTexture", QErrorType::error, msg);
    }    

    return ID;
}

///<summary>Загрузка кубической текстуры.</summary>
///<param name = 'folder'>Папка с текстурами граней.</param>
unsigned int textureLoader::loadCubeMap(const std::string folder)
{
	unsigned int ID;

	int width, height, color_channels;
	unsigned char *data;

	std::string faces[6];

	faces[0] = folder + "/x+.jpg";
	faces[1] = folder + "/x-.jpg";
	faces[2] = folder + "/y+.jpg";
	faces[3] = folder + "/y-.jpg";
	faces[4] = folder + "/z+.jpg";
	faces[5] = folder + "/z-.jpg";

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (size_t i = 0; i < 6; i++)
	{
		data = stbi_load(faces[i].c_str(), &width, &height, &color_channels, 0);

		if (data)
		{
			GLenum format;

			switch (color_channels)
			{
				case 1: format = GL_RED; break;
				case 3: format = GL_RGB; break;
				case 4: format = GL_RGBA; break;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			stbi_image_free(data);

			std::string msg = "Texture not found. PATH: " + faces[i];
			logger.log("TextureLoader::loadCubeMap", QErrorType::error, msg);
		}		
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return ID;
}

///<summary>Удаляет текстуру из памяти.</summary>
///<param name = 'ID'>Идентификатор текстуры.</param>
void textureLoader::deleteTexture(unsigned int ID)
{
	glDeleteTextures(1, &ID);
}