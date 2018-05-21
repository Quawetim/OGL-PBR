#pragma once
#include "..\includes\Includes.h"
#include "Model.h"

///<summary>Основная сущность. Контейнер моделей.</summary>
class Object : public IEntity
{
private:
	///<summary>Материал.</summary>
	Material material_;

protected:
	///<summary>Модели, из которых состоит объект.</summary>
	std::vector<std::shared_ptr<Model>> models_;									// Мб лучше список? Который можно отсортировать по приоритету отрисовки...

public:
	///<summary>Конструктор.</summary>
	Object();

	///<summary>Конструктор.</summary>
	///<param name = 'name'>Имя объекта.</param>
	///<param name = 'model'>Модель, составляющая объек.</param>
	Object(std::string name, std::shared_ptr<Model> model);

	///<summary>Конструктор.</summary>
	///<param name = 'name'>Имя объекта.</param>
	///<param name = 'models'>Список моделей, составляющих объект.</param>
	Object(std::string name, std::vector<std::shared_ptr<Model>> models);

	///<summary>Деструктор.</summary>
	~Object();

	///<summary>Добавляет модель к объекту.</summary>
	///<param name = 'model'>Модель.</param>
	void addModel(std::shared_ptr<Model> model);

	///<summary>Извлекает модель из объекта по имени.</summary>
	///<param name = 'name'>Имя извлекаемой модели.</param>
	std::shared_ptr<Model> removeModel(const std::string name);

	///<summary>Задаёт материал.</summary>
	void setMaterial(Material material);

	///<summary>Возвращает материал.</summary>
	Material getMaterial() const;

	///<summary>Возвращает модели, из которых состоит объект.</summary>
	const std::vector<std::shared_ptr<Model>> getModels() const;

	const std::shared_ptr<Model> getModelByName(const std::string name) const;
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