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
	std::vector<std::shared_ptr<Model>> models_;

public:
	///<summary>Конструктор по-умолчанию.</summary>
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

	///<summary>Возвращает модель по имени. Или первую, если не найдено.</summary>
	///<param name = 'name'>Имя модели.</param>
	const std::shared_ptr<Model> getModelByName(const std::string name) const;
};