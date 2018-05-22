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
	///<summary>������� X ������ ������� ���� � ��������.</summary>
	int x_;

	///<summary>������� Y ������ ������� ���� � ��������.</summary>
	int y_;

	///<summary>������ ��������.</summary>
	int width_;

	///<summary>������ ��������.</summary>
	int height_;

	///<summary>������� ���� (bgColor || hoverColor).</summary>
	glm::vec3 color_;

	///<summary>���� ��������.</summary>
	glm::vec3 bgColor_;

	///<summary>���� ������������� ��������.</summary>
	bool useBgTexture_;

	///<summary>��������.</summary>
	std::shared_ptr<Texture> bgTexture_;

	///<summary>������.</summary>
	std::shared_ptr<Shader> shader_;

	std::vector<std::shared_ptr<UiElement>> childs_;

	///<summary>������ ����� � ��������������� �� ��������.</summary>
	std::map<wchar_t, Character> characters_;

	///<summary>�����.</summary>
	std::string text_;

	///<summary>Unicode.</summary>
	wchar_t* wtext_;

public:
	///<summary>�����������.</summary>
	UiElement();

	///<summary>����� �������� ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setBgColor(const unsigned int red, const unsigned int green, const unsigned int blue);
	
	///<summary>����� �������� ���� � float �������.</summary>
	void setBgColor(const glm::vec3 color);

	///<summary>����� ��������.</summary>
	void setBgTexture(const std::shared_ptr<Texture> texture);

	///<summary>����� �����.</summary>
	///<param name = 'text'>�����.</param>
	void setText(const std::string text);

	///<summary>���������� ������� X ������ ������� ���� � ��������.</summary>
	int getX() const;

	///<summary>���������� ������� Y ������ ������� ���� � ��������.</summary>
	int getY() const;

	///<summary>���������� ������ ��������.</summary>
	int getWidth() const;

	///<summary>���������� ������ ��������.</summary>
	int getHeight() const;

	///<summary>���������� ����.</summary>
	glm::vec3 getColor() const;

	///<summary>���������� ������� ������������� bgTexture.</summary>
	bool useBgTexture() const;

	///<summary>���������� ��������.</summary>
	std::shared_ptr<Texture> getBgTexture() const;

	///<summary>���������� �����.</summary>
	std::string getText() const;

	///<summary>���������� ����� � Unicode.</summary>
	wchar_t* getWText() const;

	///<summary>���������� ������.</summary>
	std::shared_ptr<Shader> getShader() const;	
};

class UiPanel : public UiElement
{
public:
	///<summary>����������� ��-���������.</summary>
	UiPanel() {};

	///<summary>�����������.</summary>
	///<param name = 'x'>������� X ������ ������� ���� � ��������.</param>
	///<param name = 'y'>������� Y ������ ������� ���� � ��������.</param>
	///<param name = 'width'>������ ��������.</param>
	///<param name = 'height'>������ ��������.</param>
	UiPanel(const int x, const int y, const int width, const int height);

	void addChild(std::shared_ptr<UiElement> ui_element);
};

class UiButton : public UiElement
{
private:
	///<summary>������� �����.</summary>
	void(*click)(std::shared_ptr<IScene>);

public:
	///<summary>����������� ��-���������.</summary>
	UiButton() {};

	///<summary>�����������.</summary>
	///<param name = 'x'>������� X ������ ������� ���� � ��������.</param>
	///<param name = 'y'>������� Y ������ ������� ���� � ��������.</param>
	///<param name = 'width'>������ ��������.</param>
	///<param name = 'height'>������ ��������.</param>
	UiButton(const int x, const int y, const int width, const int height);

	///<summary>����� ������� �����.</summary>
	///<param name = 'function'>��������� �� �������.</param>
	void setClickFunction(void(*function)(std::shared_ptr<IScene>));

	///<summary>��������� �������������� � ��������.</summary>
	///<param name = 'input_handler'>���������� �����.</param>
	///<param name = 'scene'>�����.</param>
	void checkActions(std::shared_ptr<InputHandler> input_handler, std::shared_ptr<IScene> scene);
};

class UiLabel : public UiElement
{
private:
	FT_Library ft_;
	FT_Face face_;
	
public:
	///<summary>����������� ��-���������.</summary>
	UiLabel() {};

	///<summary>�����������.</summary>
	///<param name = 'font'>�������� ������.</param>
	///<param name = 'size'>�����.</param>
	///<param name = 'x'>������� X ������ ������� ���� � ��������.</param>
	///<param name = 'y'>������� Y ������ ������� ���� � ��������.</param>
	UiLabel(const std::string font, const int size, const int x, const int y);	
};