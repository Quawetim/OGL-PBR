#pragma once
#include "..\includes\Includes.h"
#include "..\callbacks\Callbacks.h"
#include "..\scene\IScene.h"

class UiElement
{
	friend class OpenGLRenderer;
private:
	///<summary>Vertex Array Object.</summary>
	unsigned int VAO_;

	///<summary>Vertex Buffer Object.</summary>
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

	///<summary>Текущий цвет (bgColor || hoverColor).</summary>
	glm::vec3 color_;

	///<summary>Цвет элемента.</summary>
	glm::vec3 bgColor_;

	///<summary>Флаг использования текстуры.</summary>
	bool useBgTexture_;

	///<summary>Текстура.</summary>
	std::shared_ptr<Texture> bgTexture_;

	///<summary>Шейдер.</summary>
	std::shared_ptr<Shader> shader_;

	std::vector<std::shared_ptr<UiElement>> childs_;

	///<summary>Хранит букву и соответствующую ей текстуру.</summary>
	std::map<wchar_t, Character> characters_;

	///<summary>Текст.</summary>
	std::string text_;

	///<summary>Unicode.</summary>
	wchar_t* wtext_;

public:
	///<summary>Констркутор.</summary>
	UiElement();

	///<summary>Задаёт основной цвет в RGB формате.</summary>
	///<param name = 'red'>Красная компонента цвета.</param>
	///<param name = 'green'>Зелёная компонента цвета.</param>
	///<param name = 'blue'>Синяя компонента цвета.</param>
	void setBgColor(const unsigned int red, const unsigned int green, const unsigned int blue);
	
	///<summary>Задаёт основной цвет в float формате.</summary>
	void setBgColor(const glm::vec3 color);

	///<summary>Задаёт текстуру.</summary>
	void setBgTexture(const std::shared_ptr<Texture> texture);

	///<summary>Задаёт текст.</summary>
	///<param name = 'text'>Текст.</param>
	void setText(const std::string text);

	///<summary>Возвращает позицию X левого нижнего угла в пикселях.</summary>
	int getX() const;

	///<summary>Возвращает позицию Y левого нижнего угла в пикселях.</summary>
	int getY() const;

	///<summary>Возвращает ширину элемента.</summary>
	int getWidth() const;

	///<summary>Возвращает высоту элемента.</summary>
	int getHeight() const;

	///<summary>Возвращает цвет.</summary>
	glm::vec3 getColor() const;

	///<summary>Возвращает признак использования bgTexture.</summary>
	bool useBgTexture() const;

	///<summary>Возвращает текстуру.</summary>
	std::shared_ptr<Texture> getBgTexture() const;

	///<summary>Возвращает текст.</summary>
	std::string getText() const;

	///<summary>Возвращает текст в Unicode.</summary>
	wchar_t* getWText() const;

	///<summary>Возвращает шейдер.</summary>
	std::shared_ptr<Shader> getShader() const;	
};

class UiPanel : public UiElement
{
public:
	///<summary>Конструктор по-умолчанию.</summary>
	UiPanel() {};

	///<summary>Конструктор.</summary>
	///<param name = 'x'>Позиция X левого нижнего угла в пикселях.</param>
	///<param name = 'y'>Позиция Y левого нижнего угла в пикселях.</param>
	///<param name = 'width'>Ширина элемента.</param>
	///<param name = 'height'>Высота элемента.</param>
	UiPanel(const int x, const int y, const int width, const int height);

	void addChild(std::shared_ptr<UiElement> ui_element);
};

class UiButton : public UiElement
{
private:
	///<summary>Функция клика.</summary>
	void(*click)(std::shared_ptr<IScene>);

public:
	///<summary>Конструктор по-умолчанию.</summary>
	UiButton() {};

	///<summary>Конструктор.</summary>
	///<param name = 'x'>Позиция X левого нижнего угла в пикселях.</param>
	///<param name = 'y'>Позиция Y левого нижнего угла в пикселях.</param>
	///<param name = 'width'>Ширина элемента.</param>
	///<param name = 'height'>Высота элемента.</param>
	UiButton(const int x, const int y, const int width, const int height);

	///<summary>Задаёт функцию клика.</summary>
	///<param name = 'function'>Указатель на функцию.</param>
	void setClickFunction(void(*function)(std::shared_ptr<IScene>));

	///<summary>проверяет взаимодействие с курсором.</summary>
	///<param name = 'input_handler'>Обработчик ввода.</param>
	///<param name = 'scene'>Сцена.</param>
	void checkActions(std::shared_ptr<InputHandler> input_handler, std::shared_ptr<IScene> scene);
};

class UiLabel : public UiElement
{
private:
	FT_Library ft_;
	FT_Face face_;
	
public:
	///<summary>Конструктор по-умолчанию.</summary>
	UiLabel() {};

	///<summary>Конструктор.</summary>
	///<param name = 'font'>Название шрифта.</param>
	///<param name = 'size'>Кегль.</param>
	///<param name = 'x'>Позиция X левого нижнего угла в пикселях.</param>
	///<param name = 'y'>Позиция Y левого нижнего угла в пикселях.</param>
	UiLabel(const std::string font, const int size, const int x, const int y);	
};