#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"
#include "../object/skybox/Skybox.h"

struct QWindow
{
	GLFWwindow* OGLwindow;
	//DXwindow* DXwindow;
};

///<summary>���������� ����� ��������.</summary>
class Renderer
{
private:
	///<summary>������ ��������� �� ����� config.</summary>
	void readConfig();

protected:
	///<summary>��������� �� ����.</summary>
	QWindow window_;

	///<summary>��� ���������.</summary>
	bool ogl_;

	///<summary>������ ����.</summary>
	int windowWidth_;

	///<summary>������ ����.</summary>
	int windowHeight_;

	///<summary>������ �����.</summary>
	bool isFullScreen_;

	///<summary>������������ �������������.</summary>
	bool isVSync_;

	///<summary>����������� �������.</summary>
	bool isShowCursor_;

	///<summary>FPS.</summary>
	int fps_;

	///<summary>���������� ���� ���������.</summary>
	int reflectionsResolution_;

	///<summary>������� ��������.</summary>
	glm::mat4 projectionMatrix_;

	virtual void drawModel(Model* model, Shader shader, QMaterial material) = 0;

public:
	///<summary>�����������.</summary>
	Renderer();

	///<summary>����������.</summary>
	~Renderer();

	///<summary>��������� �������.</summary>
	///<param name = 'object'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void drawObject(Object* object, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position) = 0;

	///<summary>��������� ���������.</summary>
	///<param name = 'skybox'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void drawSkybox(Skybox* skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position) = 0;

	///<summary>������� ������.</summary>
	virtual void clearScreen() const = 0;

	///<summary>�������� �������� � ������ ������� �������.</summary>
	virtual void swapBuffers() const = 0;

	///<summary>��������� �������.</summary>
	virtual void pollEvents() const = 0;

	///<summary>���� ������ �� �������� �����.</summary>
	virtual bool quit() const = 0;

	///<summary>����� ��������� ��������.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	virtual void setViewport(const int width, const int height) = 0;

	///<summary>���������� ��������� �� ����.</summary>
	virtual QWindow getWindow() const = 0;

	///<summary>���������� ��� ���������.</summary>
	bool isOgl() const;

	///<summary>����� ������ ����.</summary>
	///<param name = 'width'>������.</param>
	void setWindowWidth(const int width);

	///<summary>����� ������ ����.</summary>
	///<param name = 'height'>������.</param>
	void setWindowHeight(const int height);

	///<summary>����� ��� ����.</summary>
	///<param name = 'title'>���.</param>
	virtual void setWindowTitle(const std::string title) = 0;

	///<summary>����� ������������� �����.</summary>
	///<param name = 'fullScreen'>������������� �����.</param>
	void setFullScreen(const bool fullScreen);

	///<summary>��������/��������� ������������ �������������.</summary>
	///<param name = 'vsync'>������������ ������������.</param>
	void setVsync(const bool vsync);

	///<summary>����� ����������� �������.</summary>
	///<param name = 'showCursor'>���������� ������.</param>
	void setShowCursor(const bool showCursor);

	///<summary>����� ������� ����� ������ � �������.</summary>
	///<param name = 'fps'>FPS.</param>
	void setFPS(const int fps);

	///<summary>���������� ������ ����.</summary>
	int getWindowWidth() const;

	///<summary>���������� ������ ����.</summary>
	int getWindowHeight() const;

	///<summary>���������� ���������� ����.</summary>
	float getWindowHalfWidth() const;

	///<summary>���������� ���������� ����.</summary>
	float getWindowHalfHeight() const;

	///<summary>���������� ������� ���������������.</summary>
	bool isFullScreen() const;

	///<summary>���������� �������� ������������ ������������� ��� ���.</summary>
	bool isVsyncEnabled() const;

	///<summary>���������� ������� �������������� �������.</summary>
	bool isShowCursor() const;

	///<summary>���������� ������� ����� ������ � �������.</summary>
	int getFPS() const;

	///<summary>���������� ������� ��������.</summary>
	glm::mat4 getProjectionMatrix() const;
};

///<summary>��������.</summary>
extern Renderer* renderer;

///<summary>OpenGL ��������.</summary>
class OpenGLRenderer : public Renderer
{
private:
	void drawModel(Model* model, Shader shader, QMaterial material);

public:
	///<summary>�����������.</summary>
	OpenGLRenderer();

	///<summary>����������.</summary>
	~OpenGLRenderer();

	///<summary>��������� �������.</summary>
	///<param name = 'object'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void drawObject(Object* object, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position);

	///<summary>��������� ���������.</summary>
	///<param name = 'skybox'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void drawSkybox(Skybox* skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position);

	///<summary>������� ������.</summary>
	void clearScreen() const;

	///<summary>�������� �������� � ������ ������� �������.</summary>
	void swapBuffers() const;

	///<summary>��������� �������.</summary>
	void pollEvents() const;

	///<summary>���� ������ �� �������� �����.</summary>
	bool quit() const;

	///<summary>����� ��������� ��������.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	void setViewport(const int width, const int height);

	///<summary>���������� ��������� �� ����.</summary>
	QWindow getWindow() const;

	///<summary>����� ��� ����.</summary>
	///<param name = 'title'>���.</param>
	void setWindowTitle(const std::string title);
};