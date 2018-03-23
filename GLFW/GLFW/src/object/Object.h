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
	void useObjectColors();

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