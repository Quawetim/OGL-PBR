#include "TextureLoader.h"

///<summary>�������� �������� �� �����.</summary>
///<param name = 'path'>���� � ��������.</param>
unsigned int textureLoader::loadTexture(std::string path)
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
    }
    else
    {
		std::string msg = "Texture not found. PATH: " + path;
        logger.log("TextureLoader::loadTexture", QErrorType::error, msg);
    }

    stbi_image_free(data);

    return ID;
}