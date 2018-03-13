#ifndef TextureLoader_H
#define TextureLoader_H

#include "Includes.h"

/* Загрузка BMP-текстуры */
/* Image - путь к текстуре */
GLuint LoadBMP(const char *Image);

/* Загрузка DDS-текстуры */
/* Image - путь к текстуре */
GLuint LoadDDS(const char *Image);

#endif