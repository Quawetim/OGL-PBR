#pragma once
#include "..\includes\Includes.h"

class UiElement
{
public:
	unsigned int VAO_;
	unsigned int VBO_;

protected:
	///<summary>Позиция X левого нижнего угла в пикселях.</summary>
	int x_;

	///<summary>Позиция Y левого нижнего угла в пикселях.</summary>
	int y_;

	///<summary>Ширина элемента.</summary>
	int width_;

	///<summary>Высота элемента.</summary>
	int height_;

	///<summary>Цвет.</summary>
	glm::vec3 bgColor_;

	///<summary>Флаг использования текстуры.</summary>
	bool useBgTexture_;

	///<summary>Текстура.</summary>
	std::shared_ptr<Texture> bgTexture_;

	///<summary>Шейдер.</summary>
	std::shared_ptr<Shader> shader_;

public:
	///<summary>Констркутор.</summary>
	UiElement();

	///<summary>Отрисовывает элемент.</summary>
	virtual void draw() = 0;

	///<summary>Задаёт цвет в RGB формате.</summary>
	void setBgColor(const unsigned int red, const unsigned int green, const unsigned int blue);
	
	///<summary>Задаёт цвет в float формате.</summary>
	void setBgColor(const glm::vec3 color);

	///<summary>Задаёт текстуру.</summary>
	void setBgTexture(const std::shared_ptr<Texture> texture);

	///<summary>Возвращает позицию X левого нижнего угла в пикселях.</summary>
	int getX() const;

	///<summary>Возвращает позицию Y левого нижнего угла в пикселях.</summary>
	int getY() const;

	///<summary>Возвращает ширину элемента.</summary>
	int getWidth() const;

	///<summary>Возвращает высоту элемента.</summary>
	int getHeight() const;

	///<summary>Возвращает цвет.</summary>
	glm::vec3 getBgColor() const;

	///<summary>Возвращает признак использования bgTexture.</summary>
	bool useBgTexture() const;

	///<summary>Возвращает текстуру.</summary>
	std::shared_ptr<Texture> getBgTexture() const;

	///<summary>Возвращает шейдер.</summary>
	std::shared_ptr<Shader> getShader() const;
};

class Panel : public UiElement
{
public:
	///<summary>Конструктор по-умолчанию.</summary>
	Panel() {};

	///<summary>Констркутор.</summary>
	///<param name = 'x'>Позиция X левого нижнего угла в пикселях.</param>
	///<param name = 'y'>Позиция Y левого нижнего угла в пикселях.</param>
	///<param name = 'width'>Ширина элемента.</param>
	///<param name = 'height'>Высота элемента.</param>
	Panel(const int x, const int y, const int width, const int height);

	///<summary>Отрисовывает элемент.</summary>
	void draw();
};

class Button : public Panel
{

};