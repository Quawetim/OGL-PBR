#include "Scene1.h"
#include "../renderer/Renderer.h"

///<summary>����������.</summary>
Scene1::~Scene1()
{
	for (size_t i = 0; i < cubes_.size(); i++)
	{
		delete cubes_[i];
	}

	this->cubes_.clear();
	std::vector<Object*>(this->cubes_).swap(this->cubes_);

	for (size_t i = 0; i < spheres_.size(); i++)
	{
		delete spheres_[i];
	}

	this->spheres_.clear();
	std::vector<Object*>(this->spheres_).swap(this->spheres_);

	for (size_t i = 0; i < cylinders_.size(); i++)
	{
		delete cylinders_[i];
	}

	this->cylinders_.clear();
	std::vector<Object*>(this->cylinders_).swap(this->cylinders_);
}

///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
///<param name = 'models'>������ �������.</param>
void Scene1::init(std::vector<Model*> models)
{
	this->decrease_ = true;

	glm::vec3 initialSpheresPos = glm::vec3(0.0f, 6.0f, 0.0f);
	glm::vec3 initialCylindersPos = glm::vec3(3.0f, 6.0f, 0.0f);

	for (size_t i = 0; i < 5; i++)
	{
		Object *obj;
		std::string name;
	
		name = "cube" + std::to_string(i + 1);
		obj = new Object(name, models[0]);
		obj->setPosition(glm::vec3(-3.0f, 6.0f - 3 * i, 0.0f));
		this->cubes_.push_back(obj);

		name = "sphere" + std::to_string(i + 1);
		obj = new Object(name, models[1]);
		obj->setPosition(glm::vec3(0.0f, 6.0f - 3 * i, 0.0f));
		this->spheres_.push_back(obj);

		name = "cylinder" + std::to_string(i + 1);
		obj = new Object(name, models[2]);
		obj->setPosition(glm::vec3(3.0f, 6.0f - 3 * i, 0.0f));
		this->cylinders_.push_back(obj);
	}

	Texture texture;
	Material material;

	// ����
	texture = Texture("resources/textures/batman_specular.bmp", TextureType::specular);
	material.addTexture(texture);
	material.setDiffuseColor(130, 0, 72);
	this->cubes_[0]->setScale(glm::vec3(0.5f));
	this->cubes_[0]->setMaterial(material);
	material.reset();

	texture = Texture("resources/textures/brick1/diffuse.bmp", TextureType::diffuse);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick1/specular.bmp", TextureType::specular);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick1/normal.bmp", TextureType::normal);
	material.addTexture(texture);
	this->cubes_[4]->setMaterial(material);
	material.reset();

	// �����
	material.setDiffuseColor(90, 22, 160);
	this->spheres_[0]->setMaterial(material);
	material.reset();

	texture = Texture("resources/textures/brick2/diffuse.bmp", TextureType::diffuse);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick2/specular.bmp", TextureType::specular);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick2/normal.bmp", TextureType::normal);
	material.addTexture(texture);
	this->spheres_[4]->setMaterial(material);
	material.reset();

	// ��������
	material.setDiffuseColor(18, 158, 140);
	this->cylinders_[0]->setMaterial(material);
	material.reset();

	texture = Texture("resources/textures/brick3/diffuse.bmp", TextureType::diffuse);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick3/specular.bmp", TextureType::specular);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick3/normal.bmp", TextureType::normal);
	material.addTexture(texture);
	this->cylinders_[4]->setMaterial(material);
	material.reset();
}

///<summary>��������� �����.</summary>
///<param name = 'view_matrix'>������� ����.</param>
///<param name = 'camera_position'>������� ������.</param>
void Scene1::render(Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	// ����
	for (size_t i = 0; i < this->cubes_.size(); i++)
	{
		renderer->drawObject(this->cubes_[i], shader, view_matrix, camera_position);
	}

	this->cubes_[0]->rotate(-90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	this->cubes_[1]->rotate(90.0, glm::vec3(0.0f, 1.0f, 1.0f));
	this->cubes_[2]->rotate(-90.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->cubes_[3]->rotate(90.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->cubes_[4]->rotate(-30.0, glm::vec3(0.0f, 1.0f, 0.0f));

	// �����
	for (size_t i = 0; i < cubes_.size(); i++)
	{
		renderer->drawObject(this->spheres_[i], shader, view_matrix, camera_position);

		if (this->decrease_)
		{
			if (this->spheres_[i]->getScale().x > 0.8f) this->spheres_[i]->scale(-0.2f);
			else this->decrease_ = false;
		}
		else
		{
			if (this->spheres_[i]->getScale().x < 1.0f) this->spheres_[i]->scale(0.2f);
			else this->decrease_ = true;
		}
	}

	this->spheres_[4]->rotate(-40.0, glm::vec3(1.0f, 1.0f, 1.0f));

	// ��������
	for (size_t i = 0; i < this->cubes_.size(); i++)
	{
		renderer->drawObject(this->cylinders_[i], shader, view_matrix, camera_position);
		this->cylinders_[i]->rotate(10.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}