#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"
#include "../object/skybox/Skybox.h"
#include "../object/light/Light.h"

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

	///<summary>FOV.</summary>
	float fov_;

	///<summary>Разрешение карт отражений.</summary>
	int reflectionsResolution_;

	///<summary>Матрица проекции.</summary>
	glm::mat4 projectionMatrix_;

	///<summary>Отрисовка модели.</summary>
	///<param name = 'model'>Модель.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'material'>Материал.</param>
	virtual void drawModel(Model* model, Shader shader, Material material) = 0;

public:
	///<summary>Конструктор.</summary>
	Renderer();

	///<summary>Деструктор.</summary>
	~Renderer();

	///<summary>Отрисовка объекта.</summary>
	///<param name = 'object'>Объект.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	virtual void drawObject(Object* object, Shader shader, std::vector<std::shared_ptr<PointLight>> lights, glm::mat4 view_matrix, glm::vec3 camera_position) = 0;

	///<summary>Отрисовка скайбокса.</summary>
	///<param name = 'skybox'>Скайбокс.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	virtual void drawSkybox(std::shared_ptr<Skybox> skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position) = 0;

	///<summary>Отрисовка точечного источника освещения.</summary>
	///<param name = 'light'>Точечный источник освещения.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	virtual void drawPointLight(std::shared_ptr<PointLight> light, glm::mat4 view_Matrix, glm::vec3 camera_position) = 0;

	///<summary>Отрисовка осей координат.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	//virtual void drawCoordinateAxes(Shader shader, glm::mat4 view_matrix) = 0;

	///<summary>Очистка экрана.</summary>
	virtual void clearScreen() const = 0;

	///<summary>Поменять передний и задний буфферы местами.</summary>
	virtual void swapBuffers() const = 0;

	///<summary>Обработка ивентов.</summary>
	virtual void pollEvents() const = 0;

	///<summary>Флаг выхода из главного цикла.</summary>
	virtual bool quit() const = 0;

	///<summary>Задаёт параметры вьюпорта.</summary>
	///<param name = 'x'>Координата x левого нижнего угла.</param>
	///<param name = 'y'>Координата y левого нижнего угла.</param>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	virtual void setViewport(const int x, const int y, const int width, const int height) = 0;

	///<summary>Возвращает размер вьюпорта к дефолным настройкам.</summary>
	virtual void restoreViewPort() = 0;

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

	///<summary>Задаёт текущее значение FOV.</summary>
	///<param name = 'fov'>FOV.</param>
	void setFOV(const int fov);

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

	///<summary>Возвращает текущее значение FOV.</summary>
	int getFOV() const;

	///<summary>Возвращает матрицу проекции.</summary>
	glm::mat4 getProjectionMatrix() const;
};

///<summary>Рендерер.</summary>
extern Renderer* renderer;

///<summary>OpenGL рендерер.</summary>
class OpenGLRenderer : public Renderer
{
private:
	///<summary>Отрисовка модели.</summary>
	///<param name = 'model'>Модель.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'material'>Материал.</param>
	void drawModel(Model* model, Shader shader, Material material);

public:
	///<summary>Конструктор.</summary>
	OpenGLRenderer();

	///<summary>Деструктор.</summary>
	~OpenGLRenderer();

	///<summary>Отрисовка объекта.</summary>
	///<param name = 'object'>Объект.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void drawObject(Object* object, Shader shader, std::vector<std::shared_ptr<PointLight>> lights, glm::mat4 view_matrix, glm::vec3 camera_position);

	///<summary>Отрисовка скайбокса.</summary>
	///<param name = 'skybox'>Скайбокс.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void drawSkybox(std::shared_ptr<Skybox> skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position);

	///<summary>Отрисовка точечного источника освещения.</summary>
	///<param name = 'light'>Точечный источник освещения.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	///<param name = 'camera_position'>Позиция камеры.</param>
	void drawPointLight(std::shared_ptr<PointLight> light, glm::mat4 view_Matrix, glm::vec3 camera_position);

	///<summary>Отрисовка осей координат.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'view_matrix'>Матрица вида.</param>
	//void drawCoordinateAxes(Shader shader, glm::mat4 view_matrix);

	///<summary>Очистка экрана.</summary>
	void clearScreen() const;

	///<summary>Поменять передний и задний буфферы местами.</summary>
	void swapBuffers() const;

	///<summary>Обработка ивентов.</summary>
	void pollEvents() const;

	///<summary>Флаг выхода из главного цикла.</summary>
	bool quit() const;

	///<summary>Задаёт параметры вьюпорта.</summary>
	///<param name = 'x'>Координата x левого нижнего угла.</param>
	///<param name = 'y'>Координата y левого нижнего угла.</param>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	void setViewport(const int x, const int y, const int width, const int height);

	///<summary>Возвращает размер вьюпорта к дефолным настройкам.</summary>
	void restoreViewPort();

	///<summary>Возвращает указатель на окно.</summary>
	QWindow getWindow() const;

	///<summary>Задаёт имя окна.</summary>
	///<param name = 'title'>Имя.</param>
	void setWindowTitle(const std::string title);
};