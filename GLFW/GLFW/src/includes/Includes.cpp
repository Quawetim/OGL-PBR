#include "Includes.h"
#include "..\renderer\Renderer.h"

///<summary>������. �������� ������ �� ����� � � ���� "log.txt".</summary>
Logger logger;

Renderer* renderer;

QInputHandle *QInputHandle::inputHandle_;

///<summary>����� ������������ QTextureType � string.</summary>
const std::map<QTextureType, std::string> mapTextureType = 
{ 
    {QTextureType::diffuse, "diffuseMap"}, 
    {QTextureType::specular, "specularMap"}, 
    {QTextureType::normal, "normalMap"} 
};

///<summary>Field of view.</summary>
float FOV = 60.0f;

///<summary>������ ������������ ����� ���������.</summary>
int reflectionsResolution = 128;

///<summary>�����, ��������� ����� ������� ������ � ����������.</summary>
float deltaTime = 0.0;

///<summary>�����, ����������� �� ��������� ����������� �����.</summary>
float lastFrameTime = 0.0;

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
QTexture::QTexture()
{
	this->id_ = 0;
	this->path_ = "resources/textures/test.png";
	this->type_ = QTextureType::diffuse;
}

///<summary>�����������.</summary>
///<param name = 'path'>���� � ��������.</param>
///<param name = 'type'>��� ��������.</param>
QTexture::QTexture(std::string path, QTextureType type)
{
	this->path_ = path;
	this->type_ = type;
	this->id_ = textureLoader::loadTexture(this->path_);
}

///<summary>����� ��� ��������.</summary>
///<param name = 'type'>��� ��������.</param>
void QTexture::setType(const QTextureType type)
{
	this->type_ = type;
}

///<summary>����� ��� ��������.</summary>
///<param name = 'name'>��� ��������.</param>
void QTexture::setName(const std::string name)
{
	this->name_ = name;
}

///<summary>���������� ������������� ��������..</summary>
unsigned int QTexture::getID() const
{
	return this->id_;
}

///<summary>���������� ��� ��������.</summary>
QTextureType QTexture::getType() const
{
	return this->type_;
}

///<summary>���������� ��� ��������.</summary>
std::string QTexture::getName() const
{
	return this->name_;
}

////////////////////////////////////////////////////////////QMaterial////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
QMaterial::QMaterial()
{
	this->ambientColor_ = glm::vec3(0.1f, 0.1f, 0.1f);
	this->diffuseColor_ = glm::vec3(0.4f, 0.4f, 0.4f);
	this->specularColor_ = glm::vec3(0.6f, 0.6f, 0.6f);

	this->shininess_ = 8.0f;
}

///<summary>����� � �������� ���������.</summary>
void QMaterial::reset()
{
	this->ambientColor_ = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor_ = glm::vec3(0.5f, 0.5f, 0.5f);
	this->specularColor_ = glm::vec3(0.7f, 0.7f, 0.7f);
	this->shininess_ = 8.0f;
	
	this->textures_.clear();
	std::vector<QTexture>(this->textures_).swap(this->textures_);	
}

///<summary>����� ambient ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void QMaterial::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� diffuse ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void QMaterial::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� specular ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void QMaterial::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� ���� (�������) �����.</summary>
///<param name = 'shininess'>��������.</param>
void QMaterial::setShininess(const float shininess)
{
	this->shininess_ = shininess;
}

///<summary>����� ��������� ��������.</summary>
///<param name = 'texture'>��������.</param>
void QMaterial::addTexture(QTexture texture)
{
	this->textures_.push_back(texture);
}

///<summary>���������� ambient ���� � RGB �������.</summary>
glm::vec3 QMaterial::getAmbientColor() const
{
	return this->ambientColor_;
}

///<summary>���������� diffuse ���� � RGB �������.</summary>
glm::vec3 QMaterial::getDiffuseColor() const
{
	return this->diffuseColor_;
}

///<summary>���������� specular ���� � RGB �������.</summary>
glm::vec3 QMaterial::getSpecularColor() const
{
	return this->specularColor_;
}

///<summary>���������� ���� (�������) �����.</summary>
float QMaterial::getShininess() const
{
	return this->shininess_;
}

///<summary>���������� ������ �������.</summary>
std::vector<QTexture> QMaterial::getTextures() const
{
	return this->textures_;
}

///<summary>��������� ������ ������� �� �������.</summary>
bool QMaterial::isTexturesEmpty() const
{
	return this->textures_.empty();
}