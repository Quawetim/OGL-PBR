#pragma once
#include "..\includes\Includes.h"

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

	///<summary>����.</summary>
	glm::vec3 bgColor_;

	///<summary>���� ������������� ��������.</summary>
	bool useBgTexture_;

	///<summary>��������.</summary>
	std::shared_ptr<Texture> bgTexture_;

	///<summary>������.</summary>
	std::shared_ptr<Shader> shader_;

	std::vector<std::shared_ptr<UiElement>> childs_;

public:
	///<summary>�����������.</summary>
	UiElement();

	///<summary>����� ���� � RGB �������.</summary>
	void setBgColor(const unsigned int red, const unsigned int green, const unsigned int blue);
	
	///<summary>����� ���� � float �������.</summary>
	void setBgColor(const glm::vec3 color);

	///<summary>����� ��������.</summary>
	void setBgTexture(const std::shared_ptr<Texture> texture);

	///<summary>���������� ������� X ������ ������� ���� � ��������.</summary>
	int getX() const;

	///<summary>���������� ������� Y ������ ������� ���� � ��������.</summary>
	int getY() const;

	///<summary>���������� ������ ��������.</summary>
	int getWidth() const;

	///<summary>���������� ������ ��������.</summary>
	int getHeight() const;

	///<summary>���������� ����.</summary>
	glm::vec3 getBgColor() const;

	///<summary>���������� ������� ������������� bgTexture.</summary>
	bool useBgTexture() const;

	///<summary>���������� ��������.</summary>
	std::shared_ptr<Texture> getBgTexture() const;

	///<summary>���������� ������.</summary>
	std::shared_ptr<Shader> getShader() const;
};

class UiPanel : public UiElement
{
private:
	

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
	void(*click_function_)();

public:
	///<summary>����������� ��-���������.</summary>
	UiButton() {};

	///<summary>�����������.</summary>
	///<param name = 'x'>������� X ������ ������� ���� � ��������.</param>
	///<param name = 'y'>������� Y ������ ������� ���� � ��������.</param>
	///<param name = 'width'>������ ��������.</param>
	///<param name = 'height'>������ ��������.</param>
	UiButton(const int x, const int y, const int width, const int height);

	void setClickFunction(void(*function)());

	void click();
};