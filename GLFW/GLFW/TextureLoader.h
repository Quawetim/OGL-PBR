#ifndef TextureLoader_H
#define TextureLoader_H

#include "Includes.h"

/* �������� BMP-�������� */
/* Image - ���� � �������� */
GLuint LoadBMP(const char *Image);

/* �������� DDS-�������� */
/* Image - ���� � �������� */
GLuint LoadDDS(const char *Image);

#endif