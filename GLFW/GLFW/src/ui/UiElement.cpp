#include "UiElement.h"

//////////////////////////////////////UiElement//////////////////////////////////////
///<summary>�����������.</summary>
UiElement::UiElement()
{
	this->VAO_ = 0;
	this->VBO_ = 0;

	this->bgColor_ = glm::vec3(0.5f);
	this->hoverColor_ = glm::vec3(0.7f);
	this->color_ = this->bgColor_;

	this->x_ = 0;
	this->y_ = 0;
	this->width_ = 100;
	this->height_ = 100;
	this->useBgTexture_ = false;

	this->shader_ = std::shared_ptr<Shader>(new Shader("ui"));
}

///<summary>����� ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void UiElement::setBgColor(const unsigned int red, const unsigned int green, const unsigned int blue)
{
	this->bgColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
	this->color_ = this->bgColor_;
}

///<summary>����� ���� � float �������.</summary>
///<param name = 'color'>����.</param>
void UiElement::setBgColor(const glm::vec3 color)
{
	this->bgColor_ = color;
	this->color_ = this->bgColor_;
}

///<summary>����� hover ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void UiElement::setHoverColor(const unsigned int red, const unsigned int green, const unsigned int blue)
{
	this->hoverColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� hover ���� � float �������.</summary>
///<param name = 'color'>����.</param>
void UiElement::setHoverColor(const glm::vec3 color)
{
	this->hoverColor_ = color;
}

///<summary>����� ��������.</summary>
void UiElement::setBgTexture(const std::shared_ptr<Texture> texture)
{
	this->bgTexture_ = texture;
	this->useBgTexture_ = true;
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
glm::vec3 UiElement::getColor() const
{
	return this->color_;
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
///<summary>�����������.</summary>
///<param name = 'x'>������� X ������ ������� ���� � ��������.</param>
///<param name = 'y'>������� Y ������ ������� ���� � ��������.</param>
///<param name = 'width'>������ ��������.</param>
///<param name = 'height'>������ ��������.</param>
UiButton::UiButton(const int x, const int y, const int width, const int height)
{
	this->x_ = x;
	this->y_ = y;
	this->width_ = width;
	this->height_ = height;

	this->click = nullptr;
}

void UiButton::setClickFunction(void(*function)(std::shared_ptr<IScene>))
{
	this->click = function;
}

void UiButton::checkActions(std::shared_ptr<InputHandler> input_handler, std::shared_ptr<IScene> scene, const float scaleX, const float scaleY)
{
	int left = static_cast<int>(this->x_ * scaleX);
	int right = left + static_cast<int>(this->width_ * scaleX);
	
	int bottom = static_cast<int>(this->y_ * scaleY);
	int top = bottom + static_cast<int>(this->height_ * scaleY);

	double mouseX = input_handler->getCursorX();
	double mouseY = input_handler->getCursorY();

	if (left <= mouseX && mouseX <= right && bottom <= mouseY && mouseY <= top)
	{
		//this->color_ = this->hoverColor_;
		this->color_ = this->bgColor_ * glm::vec3(2.0f);

		if (this->click != nullptr)
		{
			if (input_handler->getMouseKeyState(GLFW_MOUSE_BUTTON_1))
			{
				this->color_ = this->bgColor_ / glm::vec3(2.0f);
				this->click(scene);
				input_handler->setMouseKeyState(GLFW_MOUSE_BUTTON_1, false);
			}
		}
	}
	else
	{
		this->color_ = this->bgColor_;
	}
}