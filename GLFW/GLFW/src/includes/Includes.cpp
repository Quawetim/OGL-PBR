#include "Includes.h"
#include "..\texture_loader\TextureLoader.h"
#include "..\renderer\Renderer.h"

///<summary>������. �������� ������ �� ����� � � ���� "log.txt".</summary>
Logger logger;

///<summary>��������.</summary>
Renderer* renderer;

///<summary>���������� �����.</summary>
InputHandle *InputHandle::inputHandle_;

///<summary>����� ������������ TextureType � � TextureKeys � �������.</summary>
const std::map<TextureType, TextureKeys> mapTextureType = 
{
	{ TextureType::albedo, {"albedoMaps", "useAlbedoMaps", "albedoMapsCount"} },
	{ TextureType::smoothness, {"smoothnessMaps", "useSmoothnessMaps", "smoothnessMapsCount" } },
	{ TextureType::metallic, { "metallicMaps", "useMetallicMaps", "metallicMapsCount" } },
	{ TextureType::ambientOcclusion, { "ambientOcclusionMaps", "useAmbientOcclusionMaps", "ambientOcclusionMapsCount" } },
	{ TextureType::normal, { "normalMaps", "useNormalMaps", "normalMapsCount" } },
	{ TextureType::height, { "heightMaps", "useHeightMaps", "heightMapsCount" } }
};

///<summary>�������� ����� ��� �����-���������.</summary>
float gamma = 2.2f;

////////////////////////////////////////////////////////////QTexture////////////////////////////////////////////////////////////

///<summary>�����������.</summary>
Texture::Texture()
{
	this->id_ = 0;
	this->path_ = "";
	this->type_ = TextureType::albedo;
}

///<summary>�����������.</summary>
///<param name = 'path'>���� � ��������.</param>
///<param name = 'type'>��� ��������.</param>
Texture::Texture(std::string path, TextureType type, const bool flip)
{
	this->path_ = path;
	this->type_ = type;
	this->id_ = textureLoader::loadTexture(path, type, flip);
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
///<para>albedo = (0.5, 0.5, 0.5)</para>
///<para>metallic = 0</para>
///<para>smoothness = 0</para>
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

///<summary>����� ������� � RGB �������.</summary>
///<param name = 'red'>������� ���������� ����� (0 - 255).</param>
///<param name = 'green'>������ ���������� ����� (0 - 255).</param>
///<param name = 'blue'>����� ���������� ����� (0 - 255).</param>
void Material::setAlbedo(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	this->albedo_ = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>����� ������� ����� ������.</summary>
///<param name = 'color'>���� (0 - 1).</param>
void Material::setAlbedo(const glm::vec3 color)
{
	this->albedo_ = color;
}

///<summary>����� ������������� �����������.</summary>
///<param name = 'metallic'>�������������.</param>
void Material::setMetallic(const float metallic)
{
	if (metallic > 1.0f) this->metallic_ = 1.0f;
	else
	{
		if (metallic < 0.0f) this->metallic_ = 0.0f;
		else this->metallic_ = metallic;
	}	
}

///<summary>����� ��������� �����������.</summary>
///<param name = 'smoothness'>���������.</param>
void Material::setSmoothness(const float smoothness)
{
	if (smoothness > 1.0f) this->smoothness_ = 1.0f;
	else
	{
		if (smoothness < 0.0f) this->smoothness_ = 0.0f;
		else this->smoothness_ = smoothness;
	}
}

///<summary>����� ����������� ������ �����������.</summary>
///<param name = 'surface_height'>����������� ������ �����������.</param>
void Material::setSurfaceHeight(const float surface_height)
{
	this->surfaceHeight_ = surface_height;
}

///<summary>����� ��������� ��������.</summary>
///<param name = 'texture'>��������.</param>
void Material::addTexture(std::shared_ptr<Texture> texture)
{
	this->textures_.push_back(texture);
}

///<summary>���������� ������� � RGB �������.</summary>
glm::vec3 Material::getAlbedo() const
{
	return this->albedo_;
}

///<summary>���������� ������������� �����������.</summary>
float Material::getMetallic() const
{
	return this->metallic_;
}

///<summary>���������� ��������� �����������.</summary>
float Material::getSmoothness() const
{
	return this->smoothness_;
}

///<summary>���������� ����������� ������ �����������.</summary>
float Material::getSurfaceHeight() const
{
	return this->surfaceHeight_;
}

///<summary>���������� ������ �������.</summary>
std::vector<std::shared_ptr<Texture>> Material::getTextures() const
{
	return this->textures_;
}

///<summary>��������� ������ ������� �� �������.</summary>
bool Material::isTexturesEmpty() const
{
	return this->textures_.empty();
}