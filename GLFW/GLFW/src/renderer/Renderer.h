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

	int initialWidth_, initialHeight_;

	///<summary>������ ����.</summary>
	int windowWidth_;

	///<summary>������ ����.</summary>
	int windowHeight_;

	///<summary>������������ ������� ui �� ���������.</summary>
	float uiScaleX_, uiScaleY_;

	///<summary>����������� ������ ������.</summary>
	float aspectRatio_;

	///<summary>������ �����.</summary>
	bool isFullScreen_;

	///<summary>������������ �������������.</summary>
	bool isVSync_;

	///<summary>����������� �������.</summary>
	bool isShowCursor_;

	///<summary>FOV.</summary>
	float fov_;

	///<summary>������������� ������� ��������.</summary>
	glm::mat4 perspectiveProjection_;

	///<summary>��������������� ������� ��������.</summary>
	glm::mat4 orthoProjection_;

	///<summary>��������� Aspect Ratio 
	///<para>� ��������� projectionPerspective � orthoPerspective.</para>
	///</summary>
	void updateAspectRatio();

	///<summary>������������� ������ frame ��������.</summary>
	virtual void updateFrameSize() = 0;

	///<summary>���������� irradiance map.</summary>
	virtual void generateIrradianceMap() = 0;

	///<summary>���������� pre-filtered environment map.</summary>
	virtual void generatePrefilteredMap() = 0;

	///<summary>���������� BRDF LUT map.</summary>
	virtual void generateBrdfLutMap() = 0;

	///<summary>��������� ������.</summary>
	///<param name = 'model'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'material'>��������.</param>
	virtual void drawModel(Model* model, Shader shader, Material material) = 0;	

	virtual void renderCube() = 0;

	virtual void renderQuad() = 0;

public:
	///<summary>�����������.</summary>
	Renderer();

	///<summary>����������.</summary>
	~Renderer();	

	////////////////////////////////////////////// draw-������� //////////////////////////////////////////////

	///<summary>��������� ����� �� ���� �����.</summary>
	///<param name = 'shader'>������.</param>
	virtual void drawFrame(Shader shader) = 0;

	///<summary>��������� �������.</summary>
	///<param name = 'object'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void drawObject(Object* object, Shader shader, std::vector<std::shared_ptr<PointLight>> lights, glm::mat4 view_matrix, glm::vec3 camera_position) = 0;

	///<summary>��������� ���������.</summary>
	///<param name = 'skybox'>��������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void drawSkybox(std::shared_ptr<Skybox> skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position) = 0;

	///<summary>��������� ��������� ��������� ���������.</summary>
	///<param name = 'light'>�������� �������� ���������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	virtual void drawPointLight(std::shared_ptr<PointLight> light, glm::mat4 view_Matrix, glm::vec3 camera_position) = 0;

	///<summary>��������� UI ��������.</summary>
	///<param name = 'ui_element'>�������.</param>
	virtual void drawUiElement(std::shared_ptr<UiElement> ui_element) = 0;

	///<summary>��������� ���� ���������.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	virtual void drawCoordinateAxes(std::shared_ptr<CoordinateAxes> axes, glm::mat4 view_matrix) = 0;

	virtual void drawDebugQuad(unsigned int textureID, Shader shader) = 0;

	////////////////////////////////////////////// ��������� ������� //////////////////////////////////////////////

	///<summary>������� ������.</summary>
	virtual void clearScreen() const = 0;

	///<summary>�������� �������� � ������ ������� �������.</summary>
	virtual void swapBuffers() const = 0;

	///<summary>��������� �������.</summary>
	virtual void pollEvents() const = 0;

	///<summary>���� ������ �� �������� �����.</summary>
	virtual bool shouldExit() const = 0;

	///<summary>���������� ������.</summary>
	virtual void exit() const = 0;

	///<summary>����� ��������� ��������.</summary>
	///<param name = 'x'>���������� x ������ ������� ����.</param>
	///<param name = 'y'>���������� y ������ ������� ����.</param>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	virtual void setViewport(const int x, const int y, const int width, const int height) = 0;

	///<summary>���������� ������ �������� � �������� ����������.</summary>
	virtual void restoreViewPort() = 0;

	///<summary>������ �������� RED.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	virtual unsigned int generateTexture2D_RED(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>������ �������� RGB.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	virtual unsigned int generateTexture2D_RGB(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>������ �������� RGB16F.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	virtual unsigned int generateTexture2D_RGB16F(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>������ �������� RG16F.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	virtual unsigned int generateTexture2D_RG16F(const int width, const int height, const unsigned char* buffer) = 0;

	///<summary>������ float CubeMap.</summary>
	///<param name = 'size'>������.</param>
	///<param name = 'generate_mipmap'>����� �� ������������ mipmap.</param>
	virtual unsigned int generateCubeMap16F(const int size, const unsigned char* buffer, const bool generate_mipmap) = 0;

	///<summary>����� �������� ��������.</summary>
	///<param name = 'ID'>������������� ��������.</param>
	virtual void bindTexture2D(const unsigned int ID) = 0;

	///<summary>������� ��������.</summary>
	///<param name = 'ID'>������������� ��������.</param>
	virtual void deleteTexture2D(const unsigned int ID) = 0;

	///<summary>������ ������������.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	virtual unsigned int generateRenderBuffer(const int width, const int height) = 0;

	///<summary>������ �����������.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	///<param name = 'textureID'>������������� ��������, �������� �������� ������������.</param>
	virtual unsigned int generateFrameBuffer(const unsigned int renderBuffer, const unsigned int textureID) = 0;

	///<summary>������ ���������� �����������.</summary>
	///<param name = 'renderBuffer'>������������.</param>
	virtual unsigned int generateFrameBufferCube(const unsigned int renderBuffer) = 0;

	///<summary>����� �������� ����������� ��� �����.</summary>
	virtual void bindFrameBuffer() = 0;

	///<summary>����� �������� �����������.</summary>
	///<param name = 'ID'>������������� ������������.</param>
	virtual void bindFrameBuffer(const unsigned int ID) = 0;

	///<summary>������� �����������.</summary>
	///<param name = 'ID'>������������� ������������.</param>
	virtual void deleteFrameBuffer(const unsigned int ID) = 0;

	///<summary>�������� ��� ��������� ����� �������.</summary>
	///<param name = 'use'>���/����.</param>
	virtual void useDepthTesting(const bool use) = 0;

	///<summary>���������� ��������� �� ����.</summary>
	virtual QWindow getWindow() const = 0;

	///<summary>���������� ��� ���������.</summary>
	bool isOgl() const;

	////////////////////////////////////////////// set-������� //////////////////////////////////////////////

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
	virtual void setFullScreen(const bool fullScreen) = 0;

	///<summary>��������/��������� ������������ �������������.</summary>
	///<param name = 'vsync'>������������ ������������.</param>
	virtual void setVsync(const bool vsync) = 0;

	///<summary>����� ����������� �������.</summary>
	///<param name = 'showCursor'>���������� ������.</param>
	virtual void setShowCursor(const bool showCursor) = 0;

	///<summary>����� ������� �������� FOV.</summary>
	///<param name = 'fov'>FOV.</param>
	void setFOV(const int fov);

	///<summary>����� environment map.</summary>
	///<param name = 'ID'>�������������.</param>
	virtual void setEnvironmentMap(const unsigned int ID) = 0;

	////////////////////////////////////////////// get-������� //////////////////////////////////////////////

	///<summary>���������� ������ ����.</summary>
	int getWindowWidth() const;

	///<summary>���������� ������ ����.</summary>
	int getWindowHeight() const;

	///<summary>���������� ���������� ����.</summary>
	float getWindowHalfWidth() const;

	///<summary>���������� ���������� ����.</summary>
	float getWindowHalfHeight() const;

	///<summary>���������� ����������� ������� ui �� ��� X.</summary>
	float getUiScaleX() const;

	///<summary>���������� ����������� ������� ui �� ��� Y.</summary>
	float getUiScaleY() const;

	///<summary>���������� ������� ���������������.</summary>
	bool isFullScreen() const;

	///<summary>���������� �������� ������������ ������������� ��� ���.</summary>
	bool isVsyncEnabled() const;

	///<summary>���������� ������� �������������� �������.</summary>
	bool isShowCursor() const;

	///<summary>���������� ������� �������� FOV.</summary>
	int getFOV() const;

	///<summary>���������� ������������� ������� ��������.</summary>
	glm::mat4 getPerspectiveProjectionMatrix() const;

	///<summary>���������� ��������������� ������� ��������.</summary>
	glm::mat4 getOrthoProjectionMatrix() const;
};

///<summary>��������.</summary>
extern Renderer* renderer;

///<summary>OpenGL ��������.</summary>
class OpenGLRenderer : public Renderer
{
private:
	///<summary>VAO/VBO ��� ��������� ��������������.</summary>
	unsigned int quadVAO_, quadVBO_;

	///<summary>VAO/VBO ��� ��������� ����.</summary>
	unsigned int cubeVAO_, cubeVBO_;

	///<summary>����, � ������� �������� ����� � ��� �������.</summary>
	unsigned int frame_, frameRenderBuffer_, frameFrameBuffer_;

	///<summary>Environment map.</summary>
	unsigned int environmentMap_;

	///<summary>Irradiance map.</summary>
	unsigned int irradianceMap_;

	///<summary>Pre-filtered environment map.</summary>
	unsigned int prefilteredMap_;

	///<summary>BRDF LUT map.</summary>
	unsigned int brdfLutMap_;

	///<summary>��������� ������������.</summary>
	unsigned int tempRenderBuffer_;

	///<summary>��������� �����������.</summary>
	unsigned int tempFrameBuffer_;

	///<summary>������ ��� ��������� irradiance map.</summary>
	Shader irradianceShader_;

	///<summary>������ ��� ��������� pre-filtered map.</summary>
	Shader prefilteringShader_;

	///<summary>������ ��� ��������� BRDF LUT map.</summary>
	Shader brdfLutShader_;

	///<summary>������ ��� ��������� ������������ ����.</summary>
	Shader coordinateAxesShader_;

	///<summary>������������� ������ frame ��������.</summary>
	void updateFrameSize();

	///<summary>���������� irradiance map.</summary>
	void generateIrradianceMap();

	///<summary>���������� pre-filtered map.</summary>
	void generatePrefilteredMap();

	///<summary>���������� BRDF LUT map.</summary>
	void generateBrdfLutMap();

	///<summary>��������� ������.</summary>
	///<param name = 'model'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'material'>��������.</param>
	void drawModel(Model* model, Shader shader, Material material);

	void renderQuad();

	void renderCube();

public:
	///<summary>�����������.</summary>
	OpenGLRenderer();

	///<summary>����������.</summary>
	~OpenGLRenderer();		

	////////////////////////////////////////////// draw-������� //////////////////////////////////////////////

	///<summary>��������� ����� �� ���� �����.</summary>
	///<param name = 'shader'>������.</param>
	void drawFrame(Shader shader);

	///<summary>��������� �������.</summary>
	///<param name = 'object'>������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void drawObject(Object* object, Shader shader, std::vector<std::shared_ptr<PointLight>> lights, glm::mat4 view_matrix, glm::vec3 camera_position);

	///<summary>��������� ���������.</summary>
	///<param name = 'skybox'>��������.</param>
	///<param name = 'shader'>������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void drawSkybox(std::shared_ptr<Skybox> skybox, Shader shader, glm::mat4 view_matrix, glm::vec3 camera_position);

	///<summary>��������� ��������� ��������� ���������.</summary>
	///<param name = 'light'>�������� �������� ���������.</param>
	///<param name = 'view_matrix'>������� ����.</param>
	///<param name = 'camera_position'>������� ������.</param>
	void drawPointLight(std::shared_ptr<PointLight> light, glm::mat4 view_Matrix, glm::vec3 camera_position);

	///<summary>��������� UI ��������.</summary>
	///<param name = 'ui_element'>�������.</param>
	void drawUiElement(std::shared_ptr<UiElement> ui_element);

	///<summary>��������� ���� ���������.</summary>
	///<param name = 'view_matrix'>������� ����.</param>
	void drawCoordinateAxes(std::shared_ptr<CoordinateAxes> axes, glm::mat4 view_matrix);

	void drawDebugQuad(unsigned int textureID, Shader shader);

	////////////////////////////////////////////// ��������� ������� //////////////////////////////////////////////

	///<summary>������� ������.</summary>
	void clearScreen() const;

	///<summary>�������� �������� � ������ ������� �������.</summary>
	void swapBuffers() const;

	///<summary>��������� �������.</summary>
	void pollEvents() const;

	///<summary>���� ������ �� �������� �����.</summary>
	bool shouldExit() const;

	///<summary>���������� ������.</summary>
	void exit() const;

	///<summary>����� ��������� ��������.</summary>
	///<param name = 'x'>���������� x ������ ������� ����.</param>
	///<param name = 'y'>���������� y ������ ������� ����.</param>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	void setViewport(const int x, const int y, const int width, const int height);

	///<summary>���������� ������ �������� � �������� ����������.</summary>
	void restoreViewPort();

	///<summary>������ �������� RED.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	unsigned int generateTexture2D_RED(const int width, const int height, const unsigned char* buffer);

	///<summary>������ �������� RGB.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	unsigned int generateTexture2D_RGB(const int width, const int height, const unsigned char* buffer);

	///<summary>������ �������� RGB16F.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	unsigned int generateTexture2D_RGB16F(const int width, const int height, const unsigned char* buffer);

	///<summary>������ �������� RG16F.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	unsigned int generateTexture2D_RG16F(const int width, const int height, const unsigned char* buffer);

	///<summary>������ float CubeMap.</summary>
	///<param name = 'size'>������.</param>
	///<param name = 'generate_mipmap'>����� �� ������������ mipmap.</param>
	unsigned int generateCubeMap16F(const int size, const unsigned char* buffer, const bool generate_mipmap);

	///<summary>����� �������� ��������.</summary>
	///<param name = 'textureID'>������������� ��������.</param>
	void bindTexture2D(const unsigned int textureID);

	///<summary>������� ��������.</summary>
	///<param name = 'textureID'>������������� ��������.</param>
	void deleteTexture2D(const unsigned int textureID);

	///<summary>������ ������������.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	unsigned int generateRenderBuffer(const int width, const int height);

	///<summary>������ �����������.</summary>
	///<param name = 'width'>������.</param>
	///<param name = 'height'>������.</param>
	///<param name = 'textureID'>������������� ��������, �������� �������� ������������.</param>
	unsigned int generateFrameBuffer(const unsigned int renderBuffer, const unsigned int textureID);

	///<summary>������ ���������� �����������.</summary>
	///<param name = 'renderBuffer'>������������.</param>
	unsigned int generateFrameBufferCube(const unsigned int renderBuffer);

	///<summary>����� �������� ����������� ��� �����.</summary>
	void bindFrameBuffer();

	///<summary>����� �������� �����������.</summary>
	///<param name = 'frameBufferID'>������������� ������������.</param>
	void bindFrameBuffer(const unsigned int frameBufferID);

	///<summary>������� �����������.</summary>
	///<param name = 'frameBufferID'>������������� ������������.</param>
	void deleteFrameBuffer(const unsigned int frameBufferID);

	///<summary>�������� ��� ��������� ����� �������.</summary>
	///<param name = 'use'>���/����.</param>
	void useDepthTesting(const bool use);

	///<summary>���������� ��������� �� ����.</summary>
	QWindow getWindow() const;

	////////////////////////////////////////////// set-������� //////////////////////////////////////////////

	///<summary>����� ������������� �����.</summary>
	///<param name = 'fullScreen'>������������� �����.</param>
	void setFullScreen(const bool fullScreen);

	///<summary>��������/��������� ������������ �������������.</summary>
	///<param name = 'vsync'>������������ ������������.</param>
	void setVsync(const bool vsync);

	///<summary>����� ����������� �������.</summary>
	///<param name = 'showCursor'>���������� ������.</param>
	void setShowCursor(const bool showCursor);

	///<summary>����� ��� ����.</summary>
	///<param name = 'title'>���.</param>
	void setWindowTitle(const std::string title);

	///<summary>����� environment map.</summary>
	///<param name = 'ID'>�������������.</param>
	void setEnvironmentMap(const unsigned int ID);
};