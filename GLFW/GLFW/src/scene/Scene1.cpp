#include "Scene1.h"
#include "../renderer/Renderer.h"

///<summary>Деструктор.</summary>
Scene1::~Scene1()
{
	this->objects_.clear();
	std::vector<std::shared_ptr<Object>>(this->objects_).swap(this->objects_);

	this->lights_.clear();
	std::vector<std::shared_ptr<PointLight>>(this->lights_).swap(this->lights_);
}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void Scene1::init(std::vector<std::shared_ptr<Model>> models)
{
	this->objects_.push_back(std::shared_ptr<Object>(new Object("material_ball_1", models[3])));
	this->objects_.push_back(std::shared_ptr<Object>(new Object("material_ball_2", models[3])));	
	this->objects_.push_back(std::shared_ptr<Object>(new Object("material_ball_3", models[3])));

	this->objects_[0]->setPosition(glm::vec3(-6.0f, 0.0f, 0.0f));
	this->objects_[0]->setRotation(-45.0, glm::vec3(0.0f, 1.0f, 0.0f));
	this->objects_[1]->setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	this->objects_[2]->setPosition(glm::vec3(6.0f, 0.0f, 0.0f));
	this->objects_[2]->setRotation(45.0, glm::vec3(0.0f, 1.0f, 0.0f));

	std::shared_ptr<Texture> texture;
	Material material;

	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/planet-surface/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/planet-surface/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/planet-surface/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/planet-surface/normal.png", TextureType::normal));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/planet-surface/height.png", TextureType::height));
	material.addTexture(texture);
	material.setSurfaceHeight(0.1f);
	this->objects_[0]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[1]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[2]->setMaterial(material);
	material.setDefault();

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
void Scene1::render(float deltaTime, std::shared_ptr<Shader> shader, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{	
	for (size_t i = 0; i < this->objects_.size(); i++)
	{
		renderer->drawObject(this->objects_[i], shader, this->lights_, view_matrix, camera_position);
	}

	if (this->objectsMoving_)
	{
		float v_y = 0.05f;

		std::shared_ptr<Mesh> innerBall = this->objects_[0]->getModelByName("material_ball")->getMeshByName("inner_ball");
		std::shared_ptr<Mesh> outerBall = this->objects_[0]->getModelByName("material_ball")->getMeshByName("outer_ball");

		if (this->up)
		{			
			if (innerBall->getPosition().y < 0.0f)
			{
				innerBall->move(deltaTime, 0.0f, v_y, 0.0f);
				outerBall->move(deltaTime, 0.0f, v_y, 0.0f);
			}
			else this->up = false;
		}
		else
		{
			if (innerBall->getPosition().y > -0.1f)
			{
				innerBall->move(deltaTime, 0.0f, -v_y, 0.0f);
				outerBall->move(deltaTime, 0.0f, -v_y, 0.0f);
			}
			else this->up = true;
		}

		innerBall->rotate(deltaTime, 15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}


	// Lights
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