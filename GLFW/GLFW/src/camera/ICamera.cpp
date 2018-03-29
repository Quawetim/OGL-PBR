#include "ICamera.h"

///<summary>�����������.</summary>
ICamera::ICamera()
{
	this->position_ = glm::vec3(0.0f, 0.0f, 20.0f);
	this->front_ = glm::vec3(0.0f, 0.0f, -1.0f);;
	this->up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right_ = glm::normalize(glm::cross(this->front_, this->up_));

	this->pitch_ = 0.0f;
	this->yaw_ = 0.0f;
	this->roll_ = 0.0f;

	this->viewMatrix_ = glm::mat4(1.0f);
	this->viewMatrixAxes_ = glm::mat4(1.0f);
}

///<summary>��������� � ���������� ������� ���� ������.</summary>
glm::mat4 ICamera::getViewMatrix()
{
	computeViewMatrix();
	return this->viewMatrix_;
}

///<summary>��������� � ���������� ������� ���� ���� ���������.</summary>
glm::mat4 ICamera::getViewMatrixAxes()
{
	computeViewMatrixAxes();
	return this->viewMatrixAxes_;
}

///<summary>���������� ������� ������.</summary>
glm::vec3 ICamera::getPosition() const
{
	return this->position_;
}

////////////////////////////////////////////////////////////FirstPersonCamera////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
FirstPersonCamera::FirstPersonCamera()
{
	this->pitch_ = 0.0f;
	this->yaw_ = 0.0f;
	this->roll_ = 0.0f;
}

///<summary>��������� ������� ���� ������.</summary>
void FirstPersonCamera::computeViewMatrix()
{
	this->viewMatrix_ = glm::lookAt(this->position_, this->position_ + this->front_, this->up_);
}

///<summary>��������� ������� ���� ���� ���������.</summary>
void FirstPersonCamera::computeViewMatrixAxes()
{
	float angle = glm::radians(this->yaw_ + 90.0f);
	float height = glm::normalize(this->position_).y;

	this->viewMatrixAxes_ = glm::lookAt(glm::vec3(this->axesRadius_ * cos(angle), height, this->axesRadius_ * sin(angle)), glm::vec3(0.0f), this->up_);
}

void FirstPersonCamera::handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

///<summary>��������� ����������.</summary>
///<param name = 'windowInfo'>��������� �� ����.</param>
void FirstPersonCamera::handleInput(QWindowInfo windowInfo)
{
	double cursorX, cursorY;
	float offsetX, offsetY;

	glfwGetCursorPos(windowInfo.getWindowPointer(), &cursorX, &cursorY);

	offsetX = static_cast<float>(cursorX) - windowInfo.getHalfWidth();
	offsetY = windowInfo.getHalfHeight() - static_cast<float>(cursorY);

	this->pitch_ += offsetY * deltaTime * this->cameraVelocities_.pitch;

	if (this->pitch_ > 60.0f) this->pitch_ = 60.0f;
	if (this->pitch_ < -60.0f) this->pitch_ = -60.0f;

	this->yaw_ += offsetX * deltaTime * this->cameraVelocities_.yaw;

	if (this->yaw_ > 360.0f) this->yaw_ -= 360.0f;
	if (this->yaw_ < -360.0f) this->yaw_ += 360.0f;

	//if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_E) == GLFW_PRESS) this->roll_ -= deltaTime * this->cameraVelocities_.roll;
	//if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_Q) == GLFW_PRESS) this->roll_ += deltaTime * this->cameraVelocities_.roll;

	//if (this->roll_ > 360.0f) this->roll_ -= 360.0f;
	//if (this->roll_ < -360.0f) this->roll_ += 360.0f;

	this->front_ = glm::normalize(glm::vec3(sin(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_)), sin(glm::radians(this->pitch_)),
		-1.0f * cos(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_))));
	//this->up_ = glm::normalize(glm::vec3(cos(glm::radians(this->roll_)), sin(glm::radians(this->roll_)), cos(glm::radians(this->roll_))));
	this->right_ = glm::normalize(glm::cross(this->front_, this->up_));

	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_W) == GLFW_PRESS) this->position_ += deltaTime * this->cameraVelocities_.forward * this->front_;
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_S) == GLFW_PRESS) this->position_ -= deltaTime * this->cameraVelocities_.backward * this->front_;
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_A) == GLFW_PRESS) this->position_ -= deltaTime * this->cameraVelocities_.strafe * this->right_;
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_D) == GLFW_PRESS) this->position_ += deltaTime * this->cameraVelocities_.strafe * this->right_;
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) this->position_.y += deltaTime * this->cameraVelocities_.strafe;
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) this->position_.y -= deltaTime * this->cameraVelocities_.strafe;

	glfwSetCursorPos(windowInfo.getWindowPointer(), windowInfo.getHalfWidth(), windowInfo.getHalfHeight());
}

////////////////////////////////////////////////////////////ThirdPersonCamera////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
ThirdPersonCamera::ThirdPersonCamera()
{
	this->pitch_ = 0.0f;
	this->yaw_ = 90.0f;
	this->roll_ = 0.0f;
}

///<summary>��������� ������� ���� ������.</summary>
void ThirdPersonCamera::computeViewMatrix()
{
	this->viewMatrix_ = glm::lookAt(this->position_, glm::vec3(0.0f), this->up_);
}

///<summary>��������� ������� ���� ���� ���������.</summary>
void ThirdPersonCamera::computeViewMatrixAxes()
{
	float angle = glm::radians(this->yaw_);
	float height = glm::normalize(this->position_).y;

	this->viewMatrixAxes_ = glm::lookAt(glm::vec3(this->axesRadius_ * cos(angle), height, this->axesRadius_ * sin(angle)), glm::vec3(0.0f), this->up_);
}

void ThirdPersonCamera::handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_SPACE) == GLFW_PRESS) std::cout << "TP CAM" << std::endl;
}

///<summary>��������� ����������.</summary>
///<param name = 'windowInfo'>��������� �� ����.</param>
void ThirdPersonCamera::handleInput(QWindowInfo windowInfo)
{
	double cursorX, cursorY;
	float offsetX, offsetY;

	glfwGetCursorPos(windowInfo.getWindowPointer(), &cursorX, &cursorY);

	offsetX = static_cast<float>(cursorX) - windowInfo.getHalfWidth();
	offsetY = windowInfo.getHalfHeight() - static_cast<float>(cursorY);

	this->pitch_ += offsetY * deltaTime * this->cameraVelocities_.pitch;

	if (this->pitch_ > 40.0f) this->pitch_ = 40.0f;
	if (this->pitch_ < -40.0f) this->pitch_ = -40.0f;

	this->yaw_ -= offsetX * deltaTime * this->cameraVelocities_.yaw;

	if (this->yaw_ > 360.0f) this->yaw_ -= 360.0f;
	if (this->yaw_ < -360.0f) this->yaw_ += 360.0f;

	this->position_ = glm::vec3(this->radius_ * cos(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_)), this->radius_ * sin(glm::radians(this->pitch_)), 
		this->radius_ * sin(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_)));

	glfwSetCursorPos(windowInfo.getWindowPointer(), windowInfo.getHalfWidth(), windowInfo.getHalfHeight());
}

////////////////////////////////////////////////////////////StaticCamera////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
StaticCamera::StaticCamera()
{
	this->direction_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

///<summary>�����������.</summary>
///<param name = 'position'>�������.</param>
///<param name = 'direction'>�����������.</param>
///<param name = 'up'>������ �����.</param>
StaticCamera::StaticCamera(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	this->position_ = position;
	this->direction_ = direction;
	this->up_ = up;
}

///<summary>��������� ������� ���� ������.</summary>
void StaticCamera::computeViewMatrix()
{
	this->viewMatrix_ = glm::lookAt(this->position_, this->direction_, this->up_);
}

///<summary>��������� ������� ���� ���� ���������.</summary>
void StaticCamera::computeViewMatrixAxes()
{
	this->viewMatrixAxes_ = glm::lookAt(glm::normalize(this->position_), glm::vec3(0.0f), this->up_);
}

void StaticCamera::handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_SPACE) == GLFW_PRESS) std::cout << "STATIC CAM" << std::endl;
}

///<summary>��������� ����������.</summary>
///<param name = 'windowInfo'>��������� �� ����.</param>
void StaticCamera::handleInput(QWindowInfo windowInfo)
{

}

////////////////////////////////////////////////////////////FreeCamera////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
FreeCamera::FreeCamera()
{
	this->pitch_ = 0.0f;
	this->yaw_ = 0.0f;
	this->roll_ = 0.0f;
}

///<summary>��������� ������� ���� ������.</summary>
void FreeCamera::computeViewMatrix()
{

}

///<summary>��������� ������� ���� ���� ���������.</summary>
void FreeCamera::computeViewMatrixAxes()
{

}

void FreeCamera::handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(windowInfo.getWindowPointer(), GLFW_KEY_SPACE) == GLFW_PRESS) std::cout << "FREE CAM" << std::endl;
}

///<summary>��������� ����������.</summary>
///<param name = 'windowInfo'>��������� �� ����.</param>
void FreeCamera::handleInput(QWindowInfo windowInfo)
{

}

//glm::vec3 m_pos;
//glm::quat m_orient;
//public:
//	camera(void) = default;
//	camera(const camera &) = default;
//
//	camera(const glm::vec3 &pos) : m_pos(pos) {}
//	camera(const glm::vec3 &pos, const glm::quat &orient) : m_pos(pos), m_orient(orient) {}
//
//	camera &operator =(const camera &) = default;
//
//	const glm::vec3 &position(void) const { return m_pos; }
//	const glm::quat &orientation(void) const { return m_orient; }
//
//	glm::mat4 view(void) const { return glm::translate(glm::mat4_cast(m_orient), m_pos); }
//
//	void translate(const glm::vec3 &v) { m_pos += v * m_orient; }
//	void translate(float x, float y, float z) { m_pos += glm::vec3(x, y, z) * m_orient; }
//
//	void rotate(float angle, const glm::vec3 &axis) { m_orient *= glm::angleAxis(angle, axis * m_orient); }
//	void rotate(float angle, float x, float y, float z) { m_orient *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orient); }
//
//	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
//	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
//	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }