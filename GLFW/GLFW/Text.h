#ifndef TEXT_H
#define TEXT_H

/* ������������� */
/* TexturePath - ���� � �������� */
void InitText(const char *TexturePath);

/* ������� ����� �� ����� */
/* Text - ����� */
/* X, Y - ���������� ��������� �� ������ */
/* Size - ������ */
void PrintText(const char *Text, int X, int Y, int Size);

/* ������ */
void DeleteText();

#endif