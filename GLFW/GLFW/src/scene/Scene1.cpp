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

	this->lights_.clear();
	std::vector<std::shared_ptr<PointLight>>(this->lights_).swap(this->lights_);
}

///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
///<param name = 'models'>������ �������.</param>
void Scene1::init(std::vector<Model*> models)
{
	this->decrease_ = true;

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

	Texture texture;
	Material material;

	// ����
	texture = Texture("resources/textures/batman2.bmp", TextureType::smoothness);
	material.addTexture(texture);
	texture = Texture("resources/textures/batman2.bmp", TextureType::metallic);
	material.addTexture(texture);
	material.setAlbedo(255, 226, 155);
	material.setMetallic(1.0f);
	material.setSmoothness(1.0f);
	this->cubes_[0]->setScale(glm::vec3(0.5f));
	this->cubes_[0]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(255, 255, 255);
	material.setMetallic(1.0f);
	material.setSmoothness(1.0f);
	this->cubes_[1]->setMaterial(material);
	material.setDefault();

	material.setAlbedo(155, 97, 186);
	texture = Texture("resources/textures/batman1.bmp", TextureType::smoothness);
	material.addTexture(texture);
	texture = Texture("resources/textures/batman1.bmp", TextureType::metallic);
	material.addTexture(texture);
	material.setMetallic(0.3f);
	material.setSmoothness(0.3f);
	this->cubes_[2]->setMaterial(material);
	material.setDefault();

	texture = Texture("resources/textures/brick1/diffuse.bmp", TextureType::albedo);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick1/normal.bmp", TextureType::normal);
	material.addTexture(texture);
	material.setMetallic(0.0f);
	material.setSmoothness(0.0f);
	this->cubes_[3]->setMaterial(material);
	material.setDefault();

	// �����
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

	texture = Texture("resources/textures/brick2/diffuse.bmp", TextureType::albedo);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick2/normal.bmp", TextureType::normal);
	material.addTexture(texture);
	material.setMetallic(0.0f);
	material.setSmoothness(0.0f);
	this->spheres_[3]->setMaterial(material);
	material.setDefault();

	// ��������
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

	texture = Texture("resources/textures/brick3/diffuse.bmp", TextureType::albedo);
	material.addTexture(texture);
	texture = Texture("resources/textures/brick3/normal.bmp", TextureType::normal);
	material.addTexture(texture);
	material.setMetallic(0.0f);
	material.setSmoothness(0.0f);
	this->cylinders_[3]->setMaterial(material);
	material.setDefault();

	std::shared_ptr<Model> pointLight(new Model("resources/3dmodels/pointLight.obj"));
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

///<summary>��������� �����.</summary>
///<param name = 'view_matrix'>������� ����.</param>
///<param name = 'camera_position'>������� ������.</param>
void Scene1::render(float deltaTime, Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	bool move = false;
	
	// ����
	for (size_t i = 0; i < this->cubes_.size(); i++)
	{
		renderer->drawObject(this->cubes_[i], shader, this->lights_, view_matrix, camera_position);
	}

	if (move)
	{
		this->cubes_[0]->rotate(deltaTime, -90.0, glm::vec3(0.0f, 1.0f, 0.0f));
		this->cubes_[1]->rotate(deltaTime, -90.0, glm::vec3(0.0f, 0.0f, 1.0f));
		this->cubes_[2]->rotate(deltaTime, 90.0, glm::vec3(0.0f, 0.0f, 1.0f));
		this->cubes_[3]->rotate(deltaTime, -45.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// �����
	for (size_t i = 0; i < spheres_.size(); i++)
	{
		renderer->drawObject(this->spheres_[i], shader, this->lights_, view_matrix, camera_position);

		if (move)
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

	if (move) this->spheres_[3]->rotate(deltaTime, -40.0, glm::vec3(1.0f, 1.0f, 1.0f));

	// ��������
	for (size_t i = 0; i < this->cylinders_.size(); i++)
	{
		renderer->drawObject(this->cylinders_[i], shader, this->lights_, view_matrix, camera_position);
		
		if (move) this->cylinders_[i]->rotate(deltaTime, 10.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (this->drawLights_)
	{
		for (size_t i = 0; i < this->lights_.size(); i++)
		{
			renderer->drawPointLight(this->lights_[i], view_matrix, camera_position);
		}
	}	
	
	if (true)
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