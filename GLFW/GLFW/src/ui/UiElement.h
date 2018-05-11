#pragma once
#include "..\includes\Includes.h"

class UiElement
{
public:
	unsigned int VAO_;
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

public:
	///<summary>�����������.</summary>
	UiElement();

	///<summary>������������ �������.</summary>
	virtual void draw() = 0;

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

class Panel : public UiElement
{
public:
	///<summary>����������� ��-���������.</summary>
	Panel() {};

	///<summary>�����������.</summary>
	///<param name = 'x'>������� X ������ ������� ���� � ��������.</param>
	///<param name = 'y'>������� Y ������ ������� ���� � ��������.</param>
	///<param name = 'width'>������ ��������.</param>
	///<param name = 'height'>������ ��������.</param>
	Panel(const int x, const int y, const int width, const int height);

	///<summary>������������ �������.</summary>
	void draw();
};

class Button : public Panel
{

};