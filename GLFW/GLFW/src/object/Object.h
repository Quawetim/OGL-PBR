#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"
#include "Model.h"

///<summary>Основная сущность. Контейнер моделей.</summary>
class Object : public IEntity
{
	///<summary>Модели, из которых состоит объект.</summary>
	std::vector<Model*> models;									// Мб лучше список? Который можно отсортировать по приоритету отрисовки...

	///<summary>Использовать цвет этого объекта.<./summary>
	bool object_colors_flag = false;

public:
	///<summary>Конструктор.</summary>
	Object() {};

	///<summary>Конструктор.</summary>
	///<param name = 'name'>Имя объекта.</param>
	///<param name = 'model'>Модель, составляющая объек.</param>
	Object(std::string name, Model *model);

	///<summary>Конструктор.</summary>
	///<param name = 'name'>Имя объекта.</param>
	///<param name = 'models'>Список моделей, составляющих объект.</param>
	Object(std::string name, std::vector<Model*> models);

	///<summary>Деструктор.</summary>
	~Object();

	///<summary>Отрисовка объекта.
	///<para>Если задан флаг "object_colors_flag", то все модели рисуются с цветом объекта, </para>
	///<para>иначе - с заданным цветом модели.</para>
	///</summary>
	///<param name = 'shader'>Шейдер.</param>
	void draw(Shader shader);

	///<summary>Задаёт флаг использования цвета этого объекта для всех моделей, 
	///<para>принадлежащих объекту.</para>
	///<para>Приоритет выше флага модели, но ниже флага текстур.</para>
	///</summary>
	///<param name = 'use'>Использовать цвет объекта или нет.</param>
	void setObjectColorsFlag(const bool use);

	///<summary>Задаёт ambient цвет всем мешам модели в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>Задаёт diffuse цвет всем мешам модели в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>Задаёт specular цвет всем мешам модели в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

	///<summary>Задаёт силу (яркость) блика всем мешам модели.</summary>
	///<param name = 'value'>Значение.</param>
	void setShinePower(const float value);

	///<summary>Добавляет модель к объекту.</summary>
	///<param name = 'model'>Модель.</param>
	void addModel(Model *model);

	///<summary>Извлекает модель из объекта пл имени.</summary>
	///<param name = 'name'>Имя извлекаемой модели.</param>
	Model* deleteModel(const std::string name);

	///<summary>Возвращает флаг использования цвета этого объекта для всех моделей, 
	///<para>принадлежащих объекту.</para>
	///</summary>
	bool getObjectColorsFlag() const;
};

class StarDestroyer : public Object
{

};

class StarFighter : public Object
{

};

class ImperialStarDestroyer : public StarDestroyer
{

};

class Venator : public StarDestroyer
{

};

class TieFighter : public StarFighter
{

};

class ARC170 : public StarFighter
{

};