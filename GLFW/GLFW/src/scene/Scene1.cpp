#include "Scene1.h"
#include "../renderer/Renderer.h"

///<summary>Деструктор.</summary>
Scene1::~Scene1()
{
	this->cubes_.clear();
	std::vector<std::shared_ptr<Object>>(this->cubes_).swap(this->cubes_);

	this->spheres_.clear();
	std::vector<std::shared_ptr<Object>>(this->spheres_).swap(this->spheres_);

	this->cylinders_.clear();
	std::vector<std::shared_ptr<Object>>(this->cylinders_).swap(this->cylinders_);

	this->objects_.clear();
	std::vector<std::shared_ptr<Object>>(this->objects_).swap(this->objects_);

	this->lights_.clear();
	std::vector<std::shared_ptr<PointLight>>(this->lights_).swap(this->lights_);
}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void Scene1::init(std::vector<std::shared_ptr<Model>> models)
{
	/*this->decrease_ = true;

	for (size_t i = 0; i < 4; i++)
	{
		Object *obj;
		std::string name;
	
		name = "cube" + std::to_string(i + 1);
		obj = new Object(name, models[0]);
		obj->setPosition(glm::vec3(-4.0f, 6.0f - 4 * i, 0.0f));
		this->cubes_.push_back(obj);

		name = "sphere" + std::to_string(i + 1);
		obj = new Object(name, models[1]);
		obj->setPosition(glm::vec3(0.0f, 6.0f - 4 * i, 0.0f));
		this->spheres_.push_back(obj);

		name = "cylinder" + std::to_string(i + 1);
		obj = new Object(name, models[2]);
		obj->setPosition(glm::vec3(4.0f, 6.0f - 4 * i, 0.0f));
		this->cylinders_.push_back(obj);
	}

	std::shared_ptr<Texture> texture;
	Material material;

	// Кубы
	material.setAlbedo(255, 226, 155);
	texture = std::shared_ptr<Texture>(new Texture("up.png", TextureType::albedo));
	material.addTexture(texture);
	this->cubes_[0]->setScale(glm::vec3(0.5f));
	this->cubes_[0]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(255, 255, 255);
	material.setMetallic(1.0f);
	material.setSmoothness(1.0f);
	this->cubes_[1]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(155, 97, 186);
	material.setMetallic(0.3f);
	material.setSmoothness(0.3f);
	this->cubes_[2]->setMaterial(material);
	material.setDefault();
	material.addTexture(texture);
	material.setMetallic(0.0f);
	material.setSmoothness(0.0f);

	this->cubes_[3]->setMaterial(material);
	material.setDefault();

	// Сферы
	material.setAlbedo(255, 226, 155);
	material.setMetallic(1.0f);
	material.setSmoothness(0.6f);
	this->spheres_[0]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(255, 255, 255);
	material.setMetallic(1.0f);
	material.setSmoothness(1.0f);
	this->spheres_[1]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(206, 120, 96);
	material.setMetallic(0.6f);
	material.setSmoothness(0.6f);
	this->spheres_[2]->setMaterial(material);
	material.setDefault();

	material.setMetallic(0.0f);
	material.setSmoothness(0.0f);
	material.addTexture(texture);
	this->spheres_[3]->setMaterial(material);
	material.setDefault();

	// Цилиндры
	material.setAlbedo(255, 226, 155);
	material.setMetallic(1.0f);
	material.setSmoothness(1.0f);
	this->cylinders_[0]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(255, 255, 255);
	material.setMetallic(1.0f);
	material.setSmoothness(1.0f);
	this->cylinders_[1]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(51, 16, 173);
	material.setMetallic(0.9f);
	material.setSmoothness(0.9f);
	this->cylinders_[2]->setMaterial(material);
	material.setDefault();

	material.setMetallic(0.0f);
	material.setSmoothness(0.0f);
	material.addTexture(texture);
	this->cylinders_[3]->setMaterial(material);
	material.setDefault();*/

	this->objects_.push_back(std::shared_ptr<Object>(new Object("material_ball", models[3])));
	this->objects_[0]->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));

	std::shared_ptr<Texture> texture;
	Material material;

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[0]->setMaterial(material);

	std::shared_ptr<Model> pointLight(new Model("pointLight.obj"));
	std::shared_ptr<Shader> lightShader(new Shader("lightShader"));

	std::shared_ptr<PointLight> light(new PointLight(lightShader, pointLight));

	light->setPosition(glm::vec3(0.0f, 0.0f, 15.0f));
	//light->setDiffuseColor(214, 68, 86);
	//light->setSpecularColor(214, 180, 176);

	this->lights_.push_back(light);

	light = std::shared_ptr<PointLight>(new PointLight(lightShader, pointLight));

	light->setPosition(glm::vec3(0.0f, 0.0f, -15.0f));
	//light->setDiffuseColor(97, 165, 203);
	//light->setSpecularColor(149, 192, 203);

	this->lights_.push_back(light);
}

///<summary>Отрисовка сцены.</summary>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void Scene1::render(float deltaTime, Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{	
	/*// Кубы
	for (size_t i = 0; i < this->cubes_.size(); i++)
	{
		renderer->drawObject(this->cubes_[i], shader, this->lights_, view_matrix, camera_position);
	}

	if (this->objectsMoving_)
	{
		this->cubes_[0]->rotate(deltaTime, -90.0, glm::vec3(0.0f, 1.0f, 0.0f));
		this->cubes_[1]->rotate(deltaTime, -90.0, glm::vec3(0.0f, 0.0f, 1.0f));
		this->cubes_[2]->rotate(deltaTime, 90.0, glm::vec3(0.0f, 0.0f, 1.0f));
		this->cubes_[3]->rotate(deltaTime, -45.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// Сферы
	for (size_t i = 0; i < spheres_.size(); i++)
	{
		renderer->drawObject(this->spheres_[i], shader, this->lights_, view_matrix, camera_position);

		if (this->objectsMoving_)
		{
			if (this->decrease_)
			{
				if (this->spheres_[i]->getScale().x > 0.8f) this->spheres_[i]->scale(deltaTime, -0.2f);
				else this->decrease_ = false;
			}
			else
			{
				if (this->spheres_[i]->getScale().x < 1.0f) this->spheres_[i]->scale(deltaTime, 0.2f);
				else this->decrease_ = true;
			}
		}
	}

	if (this->objectsMoving_) this->spheres_[3]->rotate(deltaTime, -40.0, glm::vec3(1.0f, 1.0f, 1.0f));

	// Цилиндры
	for (size_t i = 0; i < this->cylinders_.size(); i++)
	{
		renderer->drawObject(this->cylinders_[i], shader, this->lights_, view_matrix, camera_position);
		
		if (this->objectsMoving_) this->cylinders_[i]->rotate(deltaTime, 10.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}*/

	for (size_t i = 0; i < this->objects_.size(); i++)
	{
		renderer->drawObject(this->objects_[i], shader, this->lights_, view_matrix, camera_position);
	}

	

	if (this->lightsVisible_)
	{
		for (size_t i = 0; i < this->lights_.size(); i++)
		{
			renderer->drawPointLight(this->lights_[i], view_matrix, camera_position);
		}
	}
	
	if (this->lightsMoving_)
	{
		glm::vec3 pos, newPos;
		float r, angle;

		for (size_t i = 0; i < this->lights_.size(); i++)
		{
			pos = this->lights_[i]->getPosition();
			r = sqrt(pos.x * pos.x + pos.z * pos.z);
			angle = glm::degrees(acos(glm::dot(pos, glm::vec3(1.0f, 0.0f, 0.0f)) / r)) * getSign(pos.z);

			angle += 60.0f * deltaTime;
			while (abs(angle) >= 360.0f) angle -= 360.0f;

			newPos = glm::vec3(r * cos(glm::radians(angle)), pos.y, r * sin(glm::radians(angle)));
			this->lights_[i]->setPosition(newPos);
		}
	}
}