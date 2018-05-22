#include "Includes.h"
#include "..\texture_loader\TextureLoader.h"
#include "..\renderer\Renderer.h"

///<summary>Логгер. Логирует ошибки на экран и в файл "log.txt".</summary>
Logger logger;

///<summary>Рендерер.</summary>
Renderer* renderer;

///<summary>Обработчик ввода.</summary>
InputHandle *InputHandle::inputHandle_;

///<summary>Задаёт соответствие TextureType и её TextureKeys в шейдере.</summary>
const std::map<TextureType, TextureKeys> mapTextureType = 
{
	{ TextureType::albedo, {"albedoMaps", "useAlbedoMaps", "albedoMapsCount"} },
	{ TextureType::smoothness, {"smoothnessMaps", "useSmoothnessMaps", "smoothnessMapsCount" } },
	{ TextureType::metallic, { "metallicMaps", "useMetallicMaps", "metallicMapsCount" } },
	{ TextureType::ambientOcclusion, { "ambientOcclusionMaps", "useAmbientOcclusionMaps", "ambientOcclusionMapsCount" } },
	{ TextureType::normal, { "normalMaps", "useNormalMaps", "normalMapsCount" } },
	{ TextureType::height, { "heightMaps", "useHeightMaps", "heightMapsCount" } }
};

///<summary>Значение гаммы для гамма-коррекции.</summary>
float gamma = 2.2f;

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>Конструктор.</summary>
Texture::Texture()
{
	this->id_ = 0;
	this->path_ = "";
	this->type_ = TextureType::albedo;
}

///<summary>Конструктор.</summary>
///<param name = 'path'>Путь к текстуре.</param>
///<param name = 'type'>Тип текстуры.</param>
Texture::Texture(std::string path, TextureType type, const bool flip)
{
	this->path_ = path;
	this->type_ = type;
	this->id_ = textureLoader::loadTexture(path, type, flip);
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
///<para>albedo = (0.5, 0.5, 0.5)</para>
///<para>metallic = 0</para>
///<para>smoothness = 0</para>
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
///<para>albedo = (0.5, 0.5, 0.5)</para>
///<para>metallic = 0</para>
///<para>smoothness = 0</para>
///</summary>
void Material::setDefault()
{
	this->albedo_ = glm::vec3(0.5f, 0.5f, 0.5f);

	this->metallic_ = 0.0f;
	this->smoothness_ = 0.0f;
	this->surfaceHeight_ = 0.0f;
	
	if (this->textures_.size() > 0)
	{
		this->textures_.clear();
		std::vector<std::shared_ptr<Texture>>(this->textures_).swap(this->textures_);
	}
}

///<summary>Задаёт альбедо в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета (0 - 255).</param>
///<param name = 'green'>Зелёная компонента цвета (0 - 255).</param>
///<param name = 'blue'>Синяя компонента цвета (0 - 255).</param>
void Material::setAlbedo(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->albedo_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт альбедо через вектор.</summary>
///<param name = 'color'>Цвет (0 - 1).</param>
void Material::setAlbedo(const glm::vec3 color)
{
	this->albedo_ = color;
}

///<summary>Задаёт металличность поверхности.</summary>
///<param name = 'metallic'>Металличность.</param>
void Material::setMetallic(const float metallic)
{
	if (metallic > 1.0f) this->metallic_ = 1.0f;
	else
	{
		if (metallic < 0.0f) this->metallic_ = 0.0f;
		else this->metallic_ = metallic;
	}	
}

///<summary>Задаёт гладкость поверхности.</summary>
///<param name = 'smoothness'>Гладкость.</param>
void Material::setSmoothness(const float smoothness)
{
	if (smoothness > 1.0f) this->smoothness_ = 1.0f;
	else
	{
		if (smoothness < 0.0f) this->smoothness_ = 0.0f;
		else this->smoothness_ = smoothness;
	}
}

///<summary>Задаёт коэффициент высоты поверхности.</summary>
///<param name = 'surface_height'>Коэффициент высоты поверхности.</param>
void Material::setSurfaceHeight(const float surface_height)
{
	this->surfaceHeight_ = surface_height;
}

///<summary>Задаёт диффузную текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void Material::addTexture(std::shared_ptr<Texture> texture)
{
	this->textures_.push_back(texture);
}

///<summary>Возвращает альбедо в RGB формате.</summary>
glm::vec3 Material::getAlbedo() const
{
	return this->albedo_;
}

///<summary>Возвращает металличность поверхности.</summary>
float Material::getMetallic() const
{
	return this->metallic_;
}

///<summary>Возвращает гладкость поверхности.</summary>
float Material::getSmoothness() const
{
	return this->smoothness_;
}

///<summary>Возвращает коэффициент высоты поверхности.</summary>
float Material::getSurfaceHeight() const
{
	return this->surfaceHeight_;
}

///<summary>Возвращает список текстур.</summary>
std::vector<std::shared_ptr<Texture>> Material::getTextures() const
{
	return this->textures_;
}

///<summary>Проверяет список текстур на пустоту.</summary>
bool Material::isTexturesEmpty() const
{
	return this->textures_.empty();
}