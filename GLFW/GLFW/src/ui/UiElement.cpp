#include "UiElement.h"
#include "../renderer/Renderer.h"

//////////////////////////////////////UiElement//////////////////////////////////////
///<summary>�����������.</summary>
UiElement::UiElement()
{
	this->VAO_ = 0;
	this->VBO_ = 0;

	this->bgColor_ = glm::vec3(0.5f);
	this->color_ = this->bgColor_;

	this->x_ = 0;
	this->y_ = 0;
	this->width_ = 100;
	this->height_ = 100;
	this->useBgTexture_ = false;

	this->shader_ = std::shared_ptr<Shader>(new Shader("ui"));

	this->text_ = "";
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

///<summary>����� ��������.</summary>
void UiElement::setBgTexture(const std::shared_ptr<Texture> texture)
{
	this->bgTexture_ = texture;
	this->useBgTexture_ = true;
}

///<summary>����� �����.</summary>
///<param name = 'text'>�����.</param>
void UiElement::setText(const std::string text)
{
	this->text_ = text;
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

///<summary>���������� �����.</summary>
std::string UiElement::getText() const
{
	return this->text_;
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

void UiButton::checkActions(std::shared_ptr<InputHandler> input_handler, std::shared_ptr<IScene> scene)
{
	float scaleX = renderer->getUiScaleX();
	float scaleY = renderer->getUiScaleY();

	float left = static_cast<int>(static_cast<float>(this->x_)* scaleX);
	float right = left + static_cast<int>(this->width_* scaleX);

	float bottom = static_cast<int>(static_cast<float>(this->y_) * scaleY);
	float top = bottom + static_cast<int>(this->height_ * scaleY);

	double mouseX = input_handler->getCursorX();
	double mouseY = renderer->getWindowHeight() - input_handler->getCursorY();

	if (input_handler->getMouseKeyState(GLFW_MOUSE_BUTTON_2))
	{
		int abc = 0;
		input_handler->setMouseKeyState(GLFW_MOUSE_BUTTON_2, false);
	}

	if (left <= mouseX && mouseX <= right && bottom <= mouseY && mouseY <= top)
	{
		this->color_ = this->bgColor_ * glm::vec3(2.0f);

		if (this->click != nullptr)
		{
			if (input_handler->getMouseKeyState(GLFW_MOUSE_BUTTON_1))
			{
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

//////////////////////////////////////Label//////////////////////////////////////

UiLabel::UiLabel(const std::string font, const int size, const int x, const int y)
{
	this->x_ = x;
	this->y_ = y;

	if (FT_Init_FreeType(&this->ft_))
	{
		std::string msg = "Can't init FreeType lib.";
		logger.log(__FUNCTION__, ErrorType::error, msg);
	}
	else
	{
		std::string path = "resources/fonts/" + font + ".ttf";

		if (FT_New_Face(this->ft_, path.c_str(), 0, &this->face_))
		{
			std::string msg = "Can't find font " + path;
			logger.log(__FUNCTION__, ErrorType::error, msg);
		}
		else
		{
			FT_Set_Pixel_Sizes(this->face_, 0, size);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			//��������
			for (int i = 32; i < 127; i++) // ���� unicode
			{
				if (FT_Load_Char(this->face_, i, FT_LOAD_RENDER))
				{
					std::string msg = "Can't load character";
					logger.log(__FUNCTION__, ErrorType::warning, msg);
					continue;
				}

				unsigned int texture = renderer->generateTexture2D_RED(this->face_->glyph->bitmap.width, this->face_->glyph->bitmap.rows, this->face_->glyph->bitmap.buffer);

				Character character =
				{
					texture,
					glm::ivec2(this->face_->glyph->bitmap.width, this->face_->glyph->bitmap.rows),
					glm::ivec2(this->face_->glyph->bitmap_left, this->face_->glyph->bitmap_top),
					static_cast<unsigned int>(this->face_->glyph->advance.x)
				};

				this->characters_.insert(std::pair<wchar_t, Character>(i, character));
			}

			//���������
			for (int i = 1040; i < 1104; i++) // ���� unicode
			{
				if (FT_Load_Char(this->face_, i, FT_LOAD_RENDER))
				{
					std::string msg = "Can't load character";
					logger.log(__FUNCTION__, ErrorType::warning, msg);
					continue;
				}

				unsigned int texture = renderer->generateTexture2D_RED(this->face_->glyph->bitmap.width, this->face_->glyph->bitmap.rows, this->face_->glyph->bitmap.buffer);

				Character character =
				{
					texture,
					glm::ivec2(this->face_->glyph->bitmap.width, this->face_->glyph->bitmap.rows),
					glm::ivec2(this->face_->glyph->bitmap_left, this->face_->glyph->bitmap_top),
					static_cast<unsigned int>(this->face_->glyph->advance.x)
				};

				characters_.insert(std::pair<wchar_t, Character>(i, character));
			}

			FT_Done_Face(this->face_);
			FT_Done_FreeType(this->ft_);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
	}
}