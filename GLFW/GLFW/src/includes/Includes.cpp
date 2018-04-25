#include "Includes.h"
#include "..\texture_loader\TextureLoader.h"
#include "..\renderer\Renderer.h"

///<summary>������. �������� ������ �� ����� � � ���� "log.txt".</summary>
Logger logger;

Renderer* renderer;

InputHandle *InputHandle::inputHandle_;

///<summary>����� ������������ TextureType � � TextureKeys � �������.</summary>
const std::map<TextureType, TextureKeys> mapTextureType = 
{
	{ TextureType::diffuse, {"diffuseMaps", "useDiffuseMaps", "diffuseMapsCount"} },
	{ TextureType::specular, {"specularMaps", "useSpecularMaps", "specularMapsCount" } },
	{ TextureType::normal, {"normalMaps", "useNormalMaps", "normalMapsCount" } }
};

///<summary>������ ������������ ����� ���������.</summary>
int reflectionsResolution = 128;

///<summary>�����, ��������� ����� ������� ������ � ����������.</summary>
float deltaTime = 0.0;

///<summary>�����, ����������� �� ��������� ����������� �����.</summary>
float lastFrameTime = 0.0;

///<summary>�������� ����� ��� �����-���������.</summary>
float gamma = 2.2f;

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
Texture::Texture()
{
	this->id_ = 0;
	this->path_ = "resources/textures/test.png";
	this->type_ = TextureType::diffuse;
}

///<summary>�����������.</summary>
///<param name = 'path'>���� � ��������.</param>
///<param name = 'type'>��� ��������.</param>
Texture::Texture(std::string path, TextureType type)
{
	this->path_ = path;
	this->type_ = type;
	this->id_ = textureLoader::loadTexture(path, type);
}

///<summary>����� ��� ��������.</summary>
///<param name = 'type'>��� ��������.</param>
void Texture::setType(const TextureType type)
{
	this->type_ = type;
}

///<summary>����� ��� ��������.</summary>
///<param name = 'name'>��� ��������.</param>
void Texture::setName(const std::string name)
{
	this->name_ = name;
}

///<summary>���������� ������������� ��������..</summary>
unsigned int Texture::getID() const
{
	return this->id_;
}

///<summary>���������� ��� ��������.</summary>
TextureType Texture::getType() const
{
	return this->type_;
}

///<summary>���������� ��� ��������.</summary>
std::string Texture::getName() const
{
	return this->name_;
}

////////////////////////////////////////////////////////////QMaterial////////////////////////////////////////////////////////////

///<summary>�����������.
///<para>�������� ��-���������:</para>
///<para>ambient = (0.05, 0.05, 0.05)</para>
///<para>diffuse = (0.5, 0.5, 0.5)</para>
///<para>specular = (0.7, 0.7, 0.7)</para>
///<para>shininess = 8</para>
///</summary>
Material::Material()
{
	this->setDefault();
}

///<summary>����������.</summary>
Material::~Material()
{
	for (size_t i = 0; i < this->textures_.size(); i++)
	{
		//textureLoader::deleteTexture(this->textures_[i].getID());
	}
}

///<summary>����� � �������� ���������:
///<para>ambient = (0.05, 0.05, 0.05)</para>
///<para>diffuse = (0.5, 0.5, 0.5)</para>
///<para>specular = (0.7, 0.7, 0.7)</para>
///<para>shininess = 8</para>
///</summary>
void Material::setDefault()
{
	this->ambientColor_ = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor_ = glm::vec3(0.5f, 0.5f, 0.5f);
	this->specularColor_ = glm::vec3(0.7f, 0.7f, 0.7f);

	this->shininess_ = 8.0f;

	this->reflectiveIndex_ = 0.0f;
	this->refractiveIndex_ = 0.0f;	
	
	if (this->textures_.size() > 0)
	{
		this->textures_.clear();
		std::vector<Texture>(this->textures_).swap(this->textures_);
	}
}

///<summary>����� ambient ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Material::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� diffuse ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Material::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� specular ���� � RGB �������.</summary>
///<param name = 'red'>������� ���������� �����.</param>
///<param name = 'green'>������ ���������� �����.</param>
///<param name = 'blue'>����� ���������� �����.</param>
void Material::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� ���� (�������) �����.</summary>
///<param name = 'shininess'>��������.</param>
void Material::setShininess(const float shininess)
{
	this->shininess_ = shininess;
}

///<summary>����� ������ ���������.</summary>
///<param name = 'reflectiveIndex'>������ ���������.</param>
void Material::setReflectiveIndex(const float reflectiveIndex)
{
	if (reflectiveIndex < 0.0f)
	{
		std::string msg = "Reflective index < 0.0 (" + std::to_string(reflectiveIndex) + "). Set to 0.0.";
		logger.log(__FUNCTION__, ErrorType::warning, msg);

		this->reflectiveIndex_ = 0.0f;
	}
	else
	{
		if (reflectiveIndex > 1.0f)
		{
			std::string msg = "Reflective index > 1.0 (" + std::to_string(reflectiveIndex) + "). Set to 1.0.";
			logger.log(__FUNCTION__, ErrorType::warning, msg);

			this->reflectiveIndex_ = 1.0f;
		}
		else this->reflectiveIndex_ = reflectiveIndex;
	}
}

///<summary>����� ������ �����������.</summary>
///<param name = 'refractiveIndex'>������ �����������.</param>
void Material::setRefractiveIndex(const float refractiveIndex)
{
	if (refractiveIndex < 0.0f)
	{
		std::string msg = "Refractive index < 0.0 (" + std::to_string(refractiveIndex) + "). Set to 0.0.";
		logger.log(__FUNCTION__, ErrorType::warning, msg);

		this->refractiveIndex_ = 0.0f;
	}
	else
	{
		if (refractiveIndex > 4.05f)
		{
			std::string msg = "Refractive index > 4.05 (" + std::to_string(refractiveIndex) + "). Set to 4.05.";
			logger.log(__FUNCTION__, ErrorType::warning, msg);

			this->refractiveIndex_ = 4.05f;
		}
		else this->refractiveIndex_ = refractiveIndex;
	}
}

///<summary>����� ��������� ��������.</summary>
///<param name = 'texture'>��������.</param>
void Material::addTexture(Texture texture)
{
	this->textures_.push_back(texture);
}

///<summary>���������� ambient ���� � RGB �������.</summary>
glm::vec3 Material::getAmbientColor() const
{
	return this->ambientColor_;
}

///<summary>���������� diffuse ���� � RGB �������.</summary>
glm::vec3 Material::getDiffuseColor() const
{
	return this->diffuseColor_;
}

///<summary>���������� specular ���� � RGB �������.</summary>
glm::vec3 Material::getSpecularColor() const
{
	return this->specularColor_;
}

///<summary>���������� ���� (�������) �����.</summary>
float Material::getShininess() const
{
	return this->shininess_;
}

///<summary>���������� ������ ���������.</summary>
float Material::getReflectiveIndex() const
{
	return this->reflectiveIndex_;
}

///<summary>���������� ������ �����������.</summary>
float Material::getRefractiveIndex() const
{
	return this->refractiveIndex_;
}

///<summary>���������� ������ �������.</summary>
std::vector<Texture> Material::getTextures() const
{
	return this->textures_;
}

///<summary>��������� ������ ������� �� �������.</summary>
bool Material::isTexturesEmpty() const
{
	return this->textures_.empty();
}