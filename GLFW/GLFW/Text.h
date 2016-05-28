#ifndef TEXT_H
#define TEXT_H

#include "Includes.h"

/* Инициализация */
/* TexturePath - путь к текстуре */
void InitText(const char *TexturePath);

/* Выводит текст на экран */
/* Text - буфер */
/* X, Y - координаты положения на экране */
/* Size - размер */
void PrintText(const char *Text, int X, int Y, int Size);

/* Чистка */
void DeleteText();

#endif