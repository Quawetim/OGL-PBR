#include "UiElement.h"

//////////////////////////////////////UiElement//////////////////////////////////////
///<summary>�����������.</summary>
UiElement::UiElement()
{
	this->VAO_ = 0;
	this->VBO_ = 0;

	this->bgColor_ = glm::vec3(0.5f);
	this->x_ = 0;
	this->y_ = 0;
	this->width_ = 100;
	this->height_ = 100;
	this->useBgTexture_ = false;

	this->shader_ = std::shared_ptr<Shader>(new Shader("ui"));
}

///<summary>����� ���� � RGB �������.</summary>
void UiElement::setBgColor(const unsigned int red, const unsigned int green, const unsigned int blue)
{
	this->bgColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� ���� � float �������.</summary>
void UiElement::setBgColor(const glm::vec3 color)
{
	this->bgColor_ = color;
}

///<summary>����� ��������.</summary>
void UiElement::setBgTexture(const std::shared_ptr<Texture> texture)
{
	this->bgTexture_ = texture;
}

///<summary>���������� ������� X ������ ������� ���� � ��������.</summary>
int UiElement::getX() const
{
	return this->x_;
}

///<summary>���������� ������� Y ������ ������� ���� � ��������.</summary>
int UiElement::getY() const
{
	return this->y_;
}

///<summary>���������� ������ ��������.</summary>
int UiElement::getWidth() const
{
	return this->width_;
}

///<summary>���������� ������ ��������.</summary>
int UiElement::getHeight() const
{
	return this->height_;
}

///<summary>���������� ����.</summary>
glm::vec3 UiElement::getBgColor() const
{
	return this->bgColor_;
}

///<summary>���������� ������� ������������� bgTexture.</summary>
bool UiElement::useBgTexture() const
{
	return this->useBgTexture_;
}

///<summary>���������� ��������.</summary>
std::shared_ptr<Texture> UiElement::getBgTexture() const
{
	return this->bgTexture_;
}

///<summary>���������� ������.</summary>
std::shared_ptr<Shader> UiElement::getShader() const
{
	return this->shader_;
}

//////////////////////////////////////UiPanel//////////////////////////////////////
///<summary>�����������.</summary>
///<param name = 'x'>������� X ������ ������� ���� � ��������.</param>
///<param name = 'y'>������� Y ������ ������� ���� � ��������.</param>
///<param name = 'width'>������.</param>
///<param name = 'height'>������.</param>
UiPanel::UiPanel(const int x, const int y, const int width, const int height)
{
	this->x_ = x;
	this->y_ = y;
	this->width_ = width;
	this->height_ = height;	
}

void UiPanel::addChild(std::shared_ptr<UiElement> ui_element)
{
	this->childs_.push_back(ui_element);
}

//////////////////////////////////////UiButton//////////////////////////////////////
void UiButton::setClickFunction(void(*function)())
{
	this->click_function_ = function;
}

void UiButton::click()
{
	this->click_function_();
}