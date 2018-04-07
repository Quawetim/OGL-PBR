#pragma once
#include "../includes/Includes.h"

struct QWindow
{
	GLFWwindow* OGLwindow;
	//DXwindow* DXwindow;
};

///<summary>Абсрактный класс рендерер.</summary>
class Renderer
{
private:
	///<summary>Читает параметры из файла config.</summary>
	void readConfig();

protected:
	///<summary>Указатель на окно.</summary>
	QWindow window_;

	///<summary>Тип рендерера.</summary>
	bool ogl_;

	///<summary>Ширина окна.</summary>
	int windowWidth_;

	///<summary>Высота окна.</summary>
	int windowHeight_;

	///<summary>Полный экран.</summary>
	bool isFullScreen_;

	///<summary>Вертикальная синхронизация.</summary>
	bool isVSync_;

	///<summary>Отображение курсора.</summary>
	bool isShowCursor_;

	///<summary>FPS.</summary>
	int fps_;

	///<summary>Разрешение карт отражений.</summary>
	int reflectionsResolution_;

	///<summary>Матрица проекции.</summary>
	glm::mat4 projectionMatrix_;

public:
	///<summary>Конструктор.</summary>
	Renderer();

	///<summary>Деструктор.</summary>
	~Renderer();

	///<summary>Очистка экрана.</summary>
	virtual void clearScreen() const = 0;

	///<summary>Поменять передний и задний буфферы местами.</summary>
	virtual void swapBuffers() const = 0;

	///<summary>Обработка ивентов.</summary>
	virtual void pollEvents() const = 0;

	///<summary>Флаг выхода из главного цикла.</summary>
	virtual bool quit() const = 0;

	///<summary>Возвращает указатель на окно.</summary>
	virtual QWindow getWindow() const = 0;

	///<summary>Возвращает тип рендерера.</summary>
	bool isOgl() const;

	///<summary>Задаёт ширину окна.</summary>
	///<param name = 'width'>Ширина.</param>
	void setWindowWidth(const int width);

	///<summary>Задаёт высоту окна.</summary>
	///<param name = 'height'>Высота.</param>
	void setWindowHeight(const int height);

	///<summary>Задаёт имя окна.</summary>
	///<param name = 'title'>Имя.</param>
	virtual void setWindowTitle(const std::string title) = 0;

	///<summary>Задаёт полноэкранный режим.</summary>
	///<param name = 'fullScreen'>Полноэкранный режим.</param>
	void setFullScreen(const bool fullScreen);

	///<summary>Включает/отключает вертикальную синхронизацию.</summary>
	///<param name = 'vsync'>Вертикальная инхронизация.</param>
	void setVsync(const bool vsync);

	///<summary>Задаёт отображение курсора.</summary>
	///<param name = 'showCursor'>Отображать курсор.</param>
	void setShowCursor(const bool showCursor);

	///<summary>Задаёт текущее число кадров в секунду.</summary>
	///<param name = 'fps'>FPS.</param>
	void setFPS(const int fps);

	///<summary>Возвращает ширину окна.</summary>
	int getWindowWidth() const;

	///<summary>Возвращает высоту окна.</summary>
	int getWindowHeight() const;

	///<summary>Возвращает полуширину окна.</summary>
	float getWindowHalfWidth() const;

	///<summary>Возвращает полувысоту окна.</summary>
	float getWindowHalfHeight() const;

	///<summary>Возвращает признак полноэкранности.</summary>
	bool isFullScreen() const;

	///<summary>Возвращает включена вертикальная синхронизация или нет.</summary>
	bool isVsyncEnabled() const;

	///<summary>Возвращает признак отображаемости курсора.</summary>
	bool isShowCursor() const;

	///<summary>Возвращает текущее число кадров в секунду.</summary>
	int getFPS() const;

	///<summary>Возвращает матрицу проекции.</summary>
	glm::mat4 getProjectionMatrix() const;
};

///<summary>Рендерер.</summary>
extern Renderer* renderer;

///<summary>OpenGL рендерер.</summary>
class OpenGLRenderer : public Renderer
{
public:
	///<summary>Конструктор.</summary>
	OpenGLRenderer();

	///<summary>Деструктор.</summary>
	~OpenGLRenderer();

	///<summary>Очистка экрана.</summary>
	void clearScreen() const override;

	///<summary>Поменять передний и задний буфферы местами.</summary>
	void swapBuffers() const override;

	///<summary>Обработка ивентов.</summary>
	void pollEvents() const override;

	///<summary>Флаг выхода из главного цикла.</summary>
	bool quit() const override;

	///<summary>Возвращает указатель на окно.</summary>
	QWindow getWindow() const override;

	///<summary>Задаёт имя окна.</summary>
	///<param name = 'title'>Имя.</param>
	void setWindowTitle(const std::string title) override;
};