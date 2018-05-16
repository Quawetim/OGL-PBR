#include "Scene2.h"
#include "../renderer/Renderer.h"

///<summary>Деструктор.</summary>
Scene2::~Scene2()
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

	this->lights_.clear();
	std::vector<std::shared_ptr<PointLight>>(this->lights_).swap(this->lights_);
}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void Scene2::init(std::vector<Model*> models)
{
	for (size_t i = 0; i < 11; i++)
	{
		Object *obj;
		std::string name;

		for (size_t j = 0; j < 11; j++)
		{
			name = "sphere" + std::to_string(i + j + 1);
			obj = new Object(name, models[1]);
			obj->setPosition(glm::vec3(-10.0f + j * 2.0f, -10.0f + i * 2.0f, 0.0f));
			this->spheres_.push_back(obj);
		}
	}

	Texture texture;
	Material material;

	// Сферы
	float metallic = 0.0f;
	float smoothness = 0.0f;

	//material.setAlbedo(255, 255, 255);	//белый
	//material.setAlbedo(248, 188, 33);		//золото
	material.setAlbedo(75, 140, 237);		//голубой
	//material.setAlbedo(0, 0, 0);

	for (size_t i = 0; i < 11; i++)
	{
		material.setMetallic(metallic);

		for (size_t j = 0; j < 11; j++)
		{
			material.setSmoothness(smoothness);
			this->spheres_[11 * i + j]->setMaterial(material);

			smoothness += 0.1f;
		}

		smoothness = 0.0f;
		metallic += 0.1f;
	}

	std::shared_ptr<Model> pointLight(new Model("pointLight.obj"));
	std::shared_ptr<Shader> lightShader(new Shader("lightShader"));

	std::shared_ptr<PointLight> light;
	
	light = std::shared_ptr<PointLight>(new PointLight(lightShader, pointLight));
	light->setPosition(glm::vec3(-20.0f, 20.0f, 10.0f));
	this->lights_.push_back(light);

	light = std::shared_ptr<PointLight>(new PointLight(lightShader, pointLight));
	light->setPosition(glm::vec3(-20.0f, 0.0f, 10.0f));
	this->lights_.push_back(light);

	light = std::shared_ptr<PointLight>(new PointLight(lightShader, pointLight));
	light->setPosition(glm::vec3(-20.0f, -20.0f, 10.0f));
	this->lights_.push_back(light);
}

///<summary>Отрисовка сцены.</summary>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void Scene2::render(float deltaTime, Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	// Сферы
	for (size_t i = 0; i < spheres_.size(); i++)
	{
		renderer->drawObject(this->spheres_[i], shader, this->lights_, view_matrix, camera_position);
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
		for (size_t i = 0; i < this->lights_.size(); i++)
		{
			glm::vec3 position = this->lights_[i]->getPosition();
			
			if (position.x >= 20.0f && this->right_) this->right_ = false;
			if (position.x <= -20.0f && !this->right_) this->right_ = true;

			if (this->right_) position.x += 5.0f * deltaTime;
			else position.x -= 5.0f * deltaTime;

			this->lights_[i]->setPosition(position);
		}		
	}
}