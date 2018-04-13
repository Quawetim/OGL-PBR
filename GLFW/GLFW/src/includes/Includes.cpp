#include "Includes.h"
#include "..\renderer\Renderer.h"

///<summary>Логгер. Логирует ошибки на экран и в файл "log.txt".</summary>
Logger logger;

Renderer* renderer;

InputHandle *InputHandle::inputHandle_;

///<summary>Задаёт соответствие TextureType и её TextureKeys в шейдере.</summary>
const std::map<TextureType, TextureKeys> mapTextureType = 
{
	{ TextureType::diffuse, {"diffuseMaps", "useDiffuseMaps", "diffuseMapsCount"} },
	{ TextureType::specular, {"specularMaps", "useSpecularMaps", "specularMapsCount" } },
	{ TextureType::normal, {"normalMaps", "useNormalMaps", "normalMapsCount" } }
};

///<summary>Размер генерируемой карты отражений.</summary>
int reflectionsResolution = 128;

///<summary>Время, прошедшее между текущим кадром и предыдущим.</summary>
float deltaTime = 0.0;

///<summary>Время, затраченное на отрисовку предыдущего кадра.</summary>
float lastFrameTime = 0.0;

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>Конструктор.</summary>
Texture::Texture()
{
	this->id_ = 0;
	this->path_ = "resources/textures/test.png";
	this->type_ = TextureType::diffuse;
}

///<summary>Конструктор.</summary>
///<param name = 'path'>Путь к текстуре.</param>
///<param name = 'type'>Тип текстуры.</param>
Texture::Texture(std::string path, TextureType type)
{
	this->path_ = path;
	this->type_ = type;
	this->id_ = textureLoader::loadTexture(this->path_);
}

///<summary>Задаёт тип текстуры.</summary>
///<param name = 'type'>Тип текстуры.</param>
void Texture::setType(const TextureType type)
{
	this->type_ = type;
}

///<summary>Задаёт имя текстуры.</summary>
///<param name = 'name'>Имя текстуры.</param>
void Texture::setName(const std::string name)
{
	this->name_ = name;
}

///<summary>Возвращает идентификатор текстуры..</summary>
unsigned int Texture::getID() const
{
	return this->id_;
}

///<summary>Возвращает тип текстуры.</summary>
TextureType Texture::getType() const
{
	return this->type_;
}

///<summary>Возвращает имя текстуры.</summary>
std::string Texture::getName() const
{
	return this->name_;
}

////////////////////////////////////////////////////////////QMaterial////////////////////////////////////////////////////////////

///<summary>Конструктор.</summary>
Material::Material()
{
	this->ambientColor_ = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor_ = glm::vec3(0.3f, 0.3f, 0.3f);
	this->specularColor_ = glm::vec3(0.4f, 0.4f, 0.4f);

	this->shininess_ = 8.0f;
}

///<summary>Деструктор.</summary>
Material::~Material()
{
	for (size_t i = 0; i < this->textures_.size(); i++)
	{
		//textureLoader::deleteTexture(this->textures_[i].getID());
	}
}

///<summary>Сброс к дефолным значениям.</summary>
void Material::reset()
{
	this->ambientColor_ = glm::vec3(0.05f, 0.05f, 0.05f);
	this->diffuseColor_ = glm::vec3(0.5f, 0.5f, 0.5f);
	this->specularColor_ = glm::vec3(0.7f, 0.7f, 0.7f);
	this->shininess_ = 8.0f;
	
	this->textures_.clear();
	std::vector<Texture>(this->textures_).swap(this->textures_);	
}

///<summary>Задаёт ambient цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Material::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->ambientColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт diffuse цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Material::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->diffuseColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт specular цвет в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Material::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->specularColor_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт силу (яркость) блика.</summary>
///<param name = 'shininess'>Значение.</param>
void Material::setShininess(const float shininess)
{
	this->shininess_ = shininess;
}

///<summary>Задаёт диффузную текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void Material::addTexture(Texture texture)
{
	this->textures_.push_back(texture);
}

///<summary>Возвращает ambient цвет в RGB формате.</summary>
glm::vec3 Material::getAmbientColor() const
{
	return this->ambientColor_;
}

///<summary>Возвращает diffuse цвет в RGB формате.</summary>
glm::vec3 Material::getDiffuseColor() const
{
	return this->diffuseColor_;
}

///<summary>Возвращает specular цвет в RGB формате.</summary>
glm::vec3 Material::getSpecularColor() const
{
	return this->specularColor_;
}

///<summary>Возвращает силу (яркость) блика.</summary>
float Material::getShininess() const
{
	return this->shininess_;
}

///<summary>Возвращает список текстур.</summary>
std::vector<Texture> Material::getTextures() const
{
	return this->textures_;
}

///<summary>Проверяет список текстур на пустоту.</summary>
bool Material::isTexturesEmpty() const
{
	return this->textures_.empty();
}