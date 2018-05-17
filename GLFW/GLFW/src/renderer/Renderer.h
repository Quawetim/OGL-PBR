#pragma once
#include "../includes/Includes.h"
#include "../object/Object.h"
#include "../object/skybox/Skybox.h"
#include "../object/light/Light.h"
#include "../ui/UiElement.h"
#include "../object/coordinate_axes/CoordinateAxes.h"

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

	int initialWidth_, initialHeight_;

	///<summary>Ширина окна.</summary>
	int windowWidth_;

	///<summary>Высота окна.</summary>
	int windowHeight_;

	///<summary>Коэффициенты размера ui от исходного.</summary>
	float uiScaleX_, uiScaleY_;

	///<summary>Соотношение сторон экрана.</summary>
	float aspectRatio_;

	///<summary>Полный экран.</summary>
	bool isFullScreen_;

	///<summary>Вертикальная синхронизация.</summary>
	bool isVSync_;

	///<summary>Отображение курсора.</summary>
	bool isShowCursor_;

	///<summary>FOV.</summary>
	float fov_;

	///<summary>Перспективная матрица проекции.</summary>
	glm::mat4 perspectiveProjection_;

	///<summary>Ортографическая матрица проекции.</summary>
	glm::mat4 orthoProjection_;

	///<summary>Считывает Aspect Ratio 
	///<para>и обновляет projectionPerspective и orthoPerspective.</para>
	///</summary>
	void updateAspectRatio();

	///<summary>Пересчитывает размер frame текстуры.</summary>
	virtual void updateFrameSize() = 0;

	///<summary>Генерирует irradiance map.</summary>
	virtual void generateIrradianceMap() = 0;

	///<summary>Генерирует pre-filtered environment map.</summary>
	virtual void generatePrefilteredMap() = 0;

	///<summary>Генерирует BRDF LUT map.</summary>
	virtual void generateBrdfLutMap() = 0;

	///<summary>Отрисовка модели.</summary>
	///<param name = 'model'>Модель.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'material'>Материал.</param>
	virtual void drawModel(Model* model, Shader shader, Material material) = 0;	

	virtual void renderCube() = 0;

	virtual void renderQuad() = 0;

public:
	///<summary>Конструктор.</summary>
	Renderer();

	///<summary>Деструктор.</summary>
	~Renderer();	

	////////////////////////////////////////////// draw-функции //////////////////////////////////////////////

	///<summary>Отрисовка кадра во весь экран.</summary>
	///<param name = 'shader'>Шейдер.</param>
	virtual void drawFrame(Shader shader) = 0;

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

	///<summary>Отрисовка UI элемента.</summary>
	///<param name = 'ui_element'>Элемент.</param>
	virtual void drawUiElement(std::shared_ptr<UiElement> ui_element) = 0;

	///<summary>Отрисовка осей координат.</summary>
	///<param name = 'view_matrix'>Матрица вида.</param>
	virtual void drawCoordinateAxes(std::shared_ptr<CoordinateAxes> axes, glm::mat4 view_matrix) = 0;

	virtual void drawDebugQuad(unsigned int textureID, Shader shader) = 0;

	////////////////////////////////////////////// служебные функции //////////////////////////////////////////////

	///<summary>Очистка экрана.</summary>
	virtual void clearScreen() const = 0;

	///<summary>Поменять передний и задний буфферы местами.</summary>
	virtual void swapBuffers() const = 0;

	///<summary>Обработка ивентов.</summary>
	virtual void pollEvents() const = 0;

	///<summary>Флаг выхода из главного цикла.</summary>
	virtual bool shouldExit() const = 0;

	///<summary>Завершение работы.</summary>
	virtual void exit() const = 0;

	///<summary>Задаёт параметры вьюпорта.</summary>
	///<param name = 'x'>Координата x левого нижнего угла.</param>
	///<param name = 'y'>Координата y левого нижнего угла.</param>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	virtual void setViewport(const int x, const int y, const int width, const int height) = 0;

	///<summary>Возвращает размер вьюпорта к дефолным настройкам.</summary>
	virtual void restoreViewPort() = 0;

	///<summary>Создаёт текстуру RED.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	virtual unsigned int generateTexture2D_RED(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>Создаёт текстуру RGB.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	virtual unsigned int generateTexture2D_RGB(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>Создаёт текстуру RGB16F.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	virtual unsigned int generateTexture2D_RGB16F(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>Создаёт текстуру RG16F.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	virtual unsigned int generateTexture2D_RG16F(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>Создаёт float CubeMap.</summary>
	///<param name = 'size'>Размер.</param>
	///<param name = 'generate_mipmap'>Нужно ли генерировать mipmap.</param>
	virtual unsigned int generateCubeMap16F(const int size, const unsigned char* buffer, const bool generate_mipmap) = 0;

	///<summary>Задаёт активную текстуру.</summary>
	///<param name = 'ID'>Идентификатор текстуры.</param>
	virtual void bindTexture2D(const unsigned int ID) = 0;

	///<summary>Удаляет текстуру.</summary>
	///<param name = 'ID'>Идентификатор текстуры.</param>
	virtual void deleteTexture2D(const unsigned int ID) = 0;

	///<summary>Создаёт рендербуффер.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	virtual unsigned int generateRenderBuffer(const int width, const int height) = 0;

	///<summary>Создаёт фреймбуффер.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	///<param name = 'textureID'>Идентификатор текстуры, хранящей значения фреймбуффера.</param>
	virtual unsigned int generateFrameBuffer(const unsigned int renderBuffer, const unsigned int textureID) = 0;

	///<summary>Создаёт кубический фреймбуффер.</summary>
	///<param name = 'renderBuffer'>Рендербуффер.</param>
	virtual unsigned int generateFrameBufferCube(const unsigned int renderBuffer) = 0;

	///<summary>Задаёт активный фреймбуффер для кадра.</summary>
	virtual void bindFrameBuffer() = 0;

	///<summary>Задаёт активный фреймбуффер.</summary>
	///<param name = 'ID'>Идентификатор фреймбуффера.</param>
	virtual void bindFrameBuffer(const unsigned int ID) = 0;

	///<summary>Удаляет фреймбуффер.</summary>
	///<param name = 'ID'>Идентификатор фреймбуффера.</param>
	virtual void deleteFrameBuffer(const unsigned int ID) = 0;

	///<summary>Включает или отключает буфер глубины.</summary>
	///<param name = 'use'>Вкл/выкл.</param>
	virtual void useDepthTesting(const bool use) = 0;

	///<summary>Возвращает указатель на окно.</summary>
	virtual QWindow getWindow() const = 0;

	///<summary>Возвращает тип рендерера.</summary>
	bool isOgl() const;

	////////////////////////////////////////////// set-функции //////////////////////////////////////////////

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
	virtual void setFullScreen(const bool fullScreen) = 0;

	///<summary>Включает/отключает вертикальную синхронизацию.</summary>
	///<param name = 'vsync'>Вертикальная инхронизация.</param>
	virtual void setVsync(const bool vsync) = 0;

	///<summary>Задаёт отображение курсора.</summary>
	///<param name = 'showCursor'>Отображать курсор.</param>
	virtual void setShowCursor(const bool showCursor) = 0;

	///<summary>Задаёт текущее значение FOV.</summary>
	///<param name = 'fov'>FOV.</param>
	void setFOV(const int fov);

	///<summary>Задаёт environment map.</summary>
	///<param name = 'ID'>Идентификатор.</param>
	virtual void setEnvironmentMap(const unsigned int ID) = 0;

	////////////////////////////////////////////// get-функции //////////////////////////////////////////////

	///<summary>Возвращает ширину окна.</summary>
	int getWindowWidth() const;

	///<summary>Возвращает высоту окна.</summary>
	int getWindowHeight() const;

	///<summary>Возвращает полуширину окна.</summary>
	float getWindowHalfWidth() const;

	///<summary>Возвращает полувысоту окна.</summary>
	float getWindowHalfHeight() const;

	///<summary>Возвращает коэффициент размера ui по оси X.</summary>
	float getUiScaleX() const;

	///<summary>Возвращает коэффициент размера ui по оси Y.</summary>
	float getUiScaleY() const;

	///<summary>Возвращает признак полноэкранности.</summary>
	bool isFullScreen() const;

	///<summary>Возвращает включена вертикальная синхронизация или нет.</summary>
	bool isVsyncEnabled() const;

	///<summary>Возвращает признак отображаемости курсора.</summary>
	bool isShowCursor() const;

	///<summary>Возвращает текущее значение FOV.</summary>
	int getFOV() const;

	///<summary>Возвращает перспективную матрицу проекции.</summary>
	glm::mat4 getPerspectiveProjectionMatrix() const;

	///<summary>Возвращает ортографическую матрицу проекции.</summary>
	glm::mat4 getOrthoProjectionMatrix() const;
};

///<summary>Рендерер.</summary>
extern Renderer* renderer;

///<summary>OpenGL рендерер.</summary>
class OpenGLRenderer : public Renderer
{
private:
	///<summary>VAO/VBO для отрисовки прямоугольника.</summary>
	unsigned int quadVAO_, quadVBO_;

	///<summary>VAO/VBO для отрисовки куба.</summary>
	unsigned int cubeVAO_, cubeVBO_;

	///<summary>Кадр, в который рисуется сцена и его буфферы.</summary>
	unsigned int frame_, frameRenderBuffer_, frameFrameBuffer_;

	///<summary>Environment map.</summary>
	unsigned int environmentMap_;

	///<summary>Irradiance map.</summary>
	unsigned int irradianceMap_;

	///<summary>Pre-filtered environment map.</summary>
	unsigned int prefilteredMap_;

	///<summary>BRDF LUT map.</summary>
	unsigned int brdfLutMap_;

	///<summary>Временный рендербуффер.</summary>
	unsigned int tempRenderBuffer_;

	///<summary>Временный фреймбуффер.</summary>
	unsigned int tempFrameBuffer_;

	///<summary>Шейдер для генерации irradiance map.</summary>
	Shader irradianceShader_;

	///<summary>Шейдер для генерации pre-filtered map.</summary>
	Shader prefilteringShader_;

	///<summary>Шейдер для генерации BRDF LUT map.</summary>
	Shader brdfLutShader_;

	///<summary>Шейдер для отрисовки координатных осей.</summary>
	Shader coordinateAxesShader_;

	///<summary>Пересчитывает размер frame текстуры.</summary>
	void updateFrameSize();

	///<summary>Генерирует irradiance map.</summary>
	void generateIrradianceMap();

	///<summary>Генерирует pre-filtered map.</summary>
	void generatePrefilteredMap();

	///<summary>Генерирует BRDF LUT map.</summary>
	void generateBrdfLutMap();

	///<summary>Отрисовка модели.</summary>
	///<param name = 'model'>Модель.</param>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'material'>Материал.</param>
	void drawModel(Model* model, Shader shader, Material material);

	void renderQuad();

	void renderCube();

public:
	///<summary>Конструктор.</summary>
	OpenGLRenderer();

	///<summary>Деструктор.</summary>
	~OpenGLRenderer();		

	////////////////////////////////////////////// draw-функции //////////////////////////////////////////////

	///<summary>Отрисовка кадра во весь экран.</summary>
	///<param name = 'shader'>Шейдер.</param>
	void drawFrame(Shader shader);

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

	///<summary>Отрисовка UI элемента.</summary>
	///<param name = 'ui_element'>Элемент.</param>
	void drawUiElement(std::shared_ptr<UiElement> ui_element);

	///<summary>Отрисовка осей координат.</summary>
	///<param name = 'view_matrix'>Матрица вида.</param>
	void drawCoordinateAxes(std::shared_ptr<CoordinateAxes> axes, glm::mat4 view_matrix);

	void drawDebugQuad(unsigned int textureID, Shader shader);

	////////////////////////////////////////////// служебные функции //////////////////////////////////////////////

	///<summary>Очистка экрана.</summary>
	void clearScreen() const;

	///<summary>Поменять передний и задний буфферы местами.</summary>
	void swapBuffers() const;

	///<summary>Обработка ивентов.</summary>
	void pollEvents() const;

	///<summary>Флаг выхода из главного цикла.</summary>
	bool shouldExit() const;

	///<summary>Завершение работы.</summary>
	void exit() const;

	///<summary>Задаёт параметры вьюпорта.</summary>
	///<param name = 'x'>Координата x левого нижнего угла.</param>
	///<param name = 'y'>Координата y левого нижнего угла.</param>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	void setViewport(const int x, const int y, const int width, const int height);

	///<summary>Возвращает размер вьюпорта к дефолным настройкам.</summary>
	void restoreViewPort();

	///<summary>Создаёт текстуру RED.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	unsigned int generateTexture2D_RED(const int width, const int height, const unsigned char* buffer);

	///<summary>Создаёт текстуру RGB.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	unsigned int generateTexture2D_RGB(const int width, const int height, const unsigned char* buffer);

	///<summary>Создаёт текстуру RGB16F.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	unsigned int generateTexture2D_RGB16F(const int width, const int height, const unsigned char* buffer);

	///<summary>Создаёт текстуру RG16F.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	unsigned int generateTexture2D_RG16F(const int width, const int height, const unsigned char* buffer);

	///<summary>Создаёт float CubeMap.</summary>
	///<param name = 'size'>Размер.</param>
	///<param name = 'generate_mipmap'>Нужно ли генерировать mipmap.</param>
	unsigned int generateCubeMap16F(const int size, const unsigned char* buffer, const bool generate_mipmap);

	///<summary>Задаёт активную текстуру.</summary>
	///<param name = 'textureID'>Идентификатор текстуры.</param>
	void bindTexture2D(const unsigned int textureID);

	///<summary>Удаляет текстуру.</summary>
	///<param name = 'textureID'>Идентификатор текстуры.</param>
	void deleteTexture2D(const unsigned int textureID);

	///<summary>Создаёт рендербуффер.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	unsigned int generateRenderBuffer(const int width, const int height);

	///<summary>Создаёт фреймбуффер.</summary>
	///<param name = 'width'>Ширина.</param>
	///<param name = 'height'>Высота.</param>
	///<param name = 'textureID'>Идентификатор текстуры, хранящей значения фреймбуффера.</param>
	unsigned int generateFrameBuffer(const unsigned int renderBuffer, const unsigned int textureID);

	///<summary>Создаёт кубический фреймбуффер.</summary>
	///<param name = 'renderBuffer'>Рендербуффер.</param>
	unsigned int generateFrameBufferCube(const unsigned int renderBuffer);

	///<summary>Задаёт активный фреймбуффер для кадра.</summary>
	void bindFrameBuffer();

	///<summary>Задаёт активный фреймбуффер.</summary>
	///<param name = 'frameBufferID'>Идентификатор фреймбуффера.</param>
	void bindFrameBuffer(const unsigned int frameBufferID);

	///<summary>Удаляет фреймбуффер.</summary>
	///<param name = 'frameBufferID'>Идентификатор фреймбуффера.</param>
	void deleteFrameBuffer(const unsigned int frameBufferID);

	///<summary>Включает или отключает буфер глубины.</summary>
	///<param name = 'use'>Вкл/выкл.</param>
	void useDepthTesting(const bool use);

	///<summary>Возвращает указатель на окно.</summary>
	QWindow getWindow() const;

	////////////////////////////////////////////// set-функции //////////////////////////////////////////////

	///<summary>Задаёт полноэкранный режим.</summary>
	///<param name = 'fullScreen'>Полноэкранный режим.</param>
	void setFullScreen(const bool fullScreen);

	///<summary>Включает/отключает вертикальную синхронизацию.</summary>
	///<param name = 'vsync'>Вертикальная инхронизация.</param>
	void setVsync(const bool vsync);

	///<summary>Задаёт отображение курсора.</summary>
	///<param name = 'showCursor'>Отображать курсор.</param>
	void setShowCursor(const bool showCursor);

	///<summary>Задаёт имя окна.</summary>
	///<param name = 'title'>Имя.</param>
	void setWindowTitle(const std::string title);

	///<summary>Задаёт environment map.</summary>
	///<param name = 'ID'>Идентификатор.</param>
	void setEnvironmentMap(const unsigned int ID);
};