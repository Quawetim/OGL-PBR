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

	glm::vec3 color_;

	///<summary>����.</summary>
	glm::vec3 bgColor_;

	glm::vec3 hoverColor_;

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

	///<summary>����� �������� ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setBgColor(const unsigned int red, const unsigned int green, const unsigned int blue);
	
	///<summary>����� �������� ���� � float �������.</summary>
	void setBgColor(const glm::vec3 color);

	///<summary>����� hover ���� � RGB �������.</summary>
	///<param name = 'red'>������� ���������� �����.</param>
	///<param name = 'green'>������ ���������� �����.</param>
	///<param name = 'blue'>����� ���������� �����.</param>
	void setHoverColor(const unsigned int red, const unsigned int green, const unsigned int blue);

	///<summary>����� hover ���� � float �������.</summary>
	void setHoverColor(const glm::vec3 color);

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
	glm::vec3 getColor() const;

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

	void setClickFunction(void(*function)(std::shared_ptr<IScene>));

	void checkActions(std::shared_ptr<InputHandler> input_handler, std::shared_ptr<IScene> scene, const float scaleX, const float scaleY);
};