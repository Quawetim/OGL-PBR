#include "Scene3.h"
#include "../renderer/Renderer.h"

///<summary>Деструктор.</summary>
Scene3::~Scene3()
{
	this->objects_.clear();
	std::vector<std::shared_ptr<Object>>(this->objects_).swap(this->objects_);

	this->lights_.clear();
	std::vector<std::shared_ptr<PointLight>>(this->lights_).swap(this->lights_);
}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void Scene3::init(std::vector<std::shared_ptr<Model>> models)
{
	size_t number = 1;
	for (size_t i = 0; i < 4; i++)
	{
		std::shared_ptr<Object> obj;
		std::string name;

		for (size_t j = 0; j < 5; j++)
		{
			name = "material_ball_" + std::to_string(number);
			
			obj = std::shared_ptr<Object>(new Object(name, models[3]));
			
			obj->setPosition(glm::vec3(-20.0f + j * 10.0f, -4.0f, -40.0f + i * 10.0f));
			this->objects_.push_back(obj);

			number++;
		}
	}

	this->innerBall_ = this->objects_[0]->getModelByName("material_ball")->getMeshByName("inner_ball");
	this->outerBall_ = this->objects_[0]->getModelByName("material_ball")->getMeshByName("outer_ball");

	std::shared_ptr<Texture> texture;
	Material material;

	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/oak/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/oak/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/oak/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/oak/normal.png", TextureType::normal));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/oak/height.png", TextureType::height));
	material.addTexture(texture);
	material.setSurfaceHeight(0.01f);
	this->objects_[0]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/mahog/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/mahog/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/mahog/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/mahog/normal.png", TextureType::normal));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/wood/mahog/height.png", TextureType::height));
	material.addTexture(texture);
	material.setSurfaceHeight(0.01f);
	this->objects_[1]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/cobblestone-floor/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/cobblestone-floor/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/cobblestone-floor/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/cobblestone-floor/normal.png", TextureType::normal));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/cobblestone-floor/height.png", TextureType::height));
	material.addTexture(texture);
	material.setSurfaceHeight(0.08f);
	this->objects_[2]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/limestone/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/limestone/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/limestone/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/limestone/normal.png", TextureType::normal));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/limestone/height.png", TextureType::height));
	material.addTexture(texture);
	material.setSurfaceHeight(0.2f);
	this->objects_[3]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/moon/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/moon/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/moon/normal.png", TextureType::normal));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/moon/height.png", TextureType::height));
	material.addTexture(texture);
	material.setSurfaceHeight(0.2f);
	this->objects_[4]->setMaterial(material);
	material.setDefault();

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
	this->objects_[5]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tufted-leather/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tufted-leather/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tufted-leather/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tufted-leather/normal.png", TextureType::normal));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tufted-leather/height.png", TextureType::height));
	material.addTexture(texture);
	material.setSurfaceHeight(0.08f);
	this->objects_[6]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tile/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tile/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);	
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tile/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/tile/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[7]->setMaterial(material);
	material.setDefault();	

	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/scratched-paint/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/scratched-paint/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/scratched-paint/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/uncat/scratched-paint/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[8]->setMaterial(material);
	material.setDefault();	

	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/marble-streaked/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/marble-streaked/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/rock/marble-streaked/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[9]->setMaterial(material);
	material.setDefault();					

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/gold-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[10]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/aluminum-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/aluminum-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/aluminum-scuffed/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/aluminum-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[11]->setMaterial(material);
	material.setDefault();	

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/titanium-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/titanium-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/titanium-scuffed/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/titanium-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[12]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/metal-streaked/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/metal-streaked/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/metal-streaked/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	this->objects_[13]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-scuffed/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[14]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-rusted-streaks/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-rusted-streaks/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-rusted-streaks/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/iron-rusted-streaks/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[15]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-scuffed/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[16]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-oxidized/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-oxidized/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-oxidized/metallic.png", TextureType::metallic));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/metal/copper-oxidized/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[17]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-pattern/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-pattern/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-pattern/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[18]->setMaterial(material);
	material.setDefault();

	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/albedo.png", TextureType::albedo));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/smoothness.png", TextureType::smoothness));
	material.addTexture(texture);	
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/ao.png", TextureType::ambientOcclusion));
	material.addTexture(texture);
	texture = std::shared_ptr<Texture>(new Texture("pbr/plastic/plastic-scuffed/normal.png", TextureType::normal));
	material.addTexture(texture);
	this->objects_[19]->setMaterial(material);
	material.setDefault();	

	std::shared_ptr<Model> pointLight(new Model("pointLight.obj"));
	std::shared_ptr<Shader> lightShader(new Shader("lightShader"));

	std::shared_ptr<PointLight> light;
	
	light = std::shared_ptr<PointLight>(new PointLight(lightShader, pointLight));
	light->setPosition(glm::vec3(-15.0f, 5.0f, 10.0f));
	this->lights_.push_back(light);

	light = std::shared_ptr<PointLight>(new PointLight(lightShader, pointLight));
	light->setPosition(glm::vec3(0.0f, 5.0f, 10.0f));
	this->lights_.push_back(light);

	light = std::shared_ptr<PointLight>(new PointLight(lightShader, pointLight));
	light->setPosition(glm::vec3(15.0f, 5.0f, 10.0f));
	this->lights_.push_back(light);
}

///<summary>Отрисовка сцены.</summary>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void Scene3::render(float deltaTime, std::shared_ptr<Shader> shader, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	// Objects
	for (size_t i = 0; i < this->objects_.size(); i++)
	{
		renderer->drawObject(this->objects_[i], shader, this->lights_, view_matrix, camera_position);
	}

	if (this->objectsMoving_)
	{		
		if (this->innerBall_->getPosition().y > 0.0f && this->up_) this->up_ = false;
		if (this->innerBall_->getPosition().y < -0.1f && !this->up_) this->up_ = true;
		
		if (this->up_)
		{
			this->innerBall_->move(deltaTime, 0.0f, 0.05f, 0.0f);
			this->outerBall_->move(deltaTime, 0.0f, 0.05f, 0.0f);
		}
		else
		{
			this->innerBall_->move(deltaTime, 0.0f, -0.05f, 0.0f);
			this->outerBall_->move(deltaTime, 0.0f, -0.05f, 0.0f);
		}

		this->innerBall_->rotate(deltaTime, 15.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// Light
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
			
			if (position.z <= -50.0f && this->front_) this->front_ = false;
			if (position.z >= 10.0f && !this->front_) this->front_ = true;

			if (this->front_) position.z -= 8.0f * deltaTime;
			else position.z += 8.0f * deltaTime;

			this->lights_[i]->setPosition(position);
		}		
	}
}