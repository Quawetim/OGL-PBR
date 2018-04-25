#include "Includes.h"
#include "..\texture_loader\TextureLoader.h"
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

///<summary>Значение гаммы для гамма-коррекции.</summary>
float gamma = 2.2f;

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
	this->id_ = textureLoader::loadTexture(path, type);
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

///<summary>Конструктор.
///<para>Значения по-умолчанию:</para>
///<para>ambient = (0.05, 0.05, 0.05)</para>
///<para>diffuse = (0.5, 0.5, 0.5)</para>
///<para>specular = (0.7, 0.7, 0.7)</para>
///<para>shininess = 8</para>
///</summary>
Material::Material()
{
	this->setDefault();
}

///<summary>Деструктор.</summary>
Material::~Material()
{
	for (size_t i = 0; i < this->textures_.size(); i++)
	{
		//textureLoader::deleteTexture(this->textures_[i].getID());
	}
}

///<summary>Сброс к дефолным значениям:
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

///<summary>Задаёт индекс отражения.</summary>
///<param name = 'reflectiveIndex'>Индекс отражения.</param>
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

///<summary>Задаёт индекс преломления.</summary>
///<param name = 'refractiveIndex'>Индекс преломления.</param>
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

///<summary>Возвращает индекс отражения.</summary>
float Material::getReflectiveIndex() const
{
	return this->reflectiveIndex_;
}

///<summary>Возвращает индекс преломления.</summary>
float Material::getRefractiveIndex() const
{
	return this->refractiveIndex_;
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