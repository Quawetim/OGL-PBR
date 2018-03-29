#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"
#include "Model.h"

///<summary>Основная сущность. Контейнер моделей.</summary>
class Object : public IEntity
{
	///<summary>Модели, из которых состоит объект.</summary>
	std::vector<Model*> models_;									// Мб лучше список? Который можно отсортировать по приоритету отрисовки...

	///<summary>Использовать материал этого объекта.<./summary>
	bool useObjectMaterial_;

public:
	///<summary>Конструктор.</summary>
	Object();

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
	///<param name = 'projection_matrix'>Матрица проекции.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void draw(Shader shader, glm::mat4 projection_matrix, glm::mat4 view_matrix, glm::vec3 camera_position);

	///<summary>Задаёт материал объекта.</summary>
	///<param name = 'material'>Материал.</param>
	void setMaterial(QMaterial material);

	///<summary>Задаёт флаг использования материала этого объекта для всех моделей, 
	///<para>принадлежащих объекту.</para>
	///<para>Приоритет выше флага модели, но ниже флага текстур.</para>
	///</summary>
	void useObjectMaterial();

	///<summary>Добавляет модель к объекту.</summary>
	///<param name = 'model'>Модель.</param>
	void addModel(Model *model);

	///<summary>Извлекает модель из объекта пл имени.</summary>
	///<param name = 'name'>Имя извлекаемой модели.</param>
	Model* removeModel(const std::string name);
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