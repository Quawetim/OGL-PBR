#include "TestScene.h"
#include "../renderer/Renderer.h"

///<summary>Деструктор.</summary>
TestScene::~TestScene()
{

}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void TestScene::init(std::vector<Model*> models)
{
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	VertexData v;
	v.position = glm::vec3(-1.0f, -1.0f, 0.0f);
	v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(v);
	v.position = glm::vec3(1.0f, -1.0f, 0.0f);
	v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(v);
	v.position = glm::vec3(1.0f, 1.0f, 0.0f);
	v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(v);
	v.position = glm::vec3(-1.0f, 1.0f, 0.0f);
	v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(v);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	
	this->objects_.push_back(new Object("nanosuit", models[0]));
	this->objects_[0]->setPosition(glm::vec3(0.0f, -4.0f, 0.0f));
	this->objects_[0]->setScale(glm::vec3(0.5f));

	this->objects_.push_back(new Object("deadpool", models[1]));
	this->objects_[1]->setPosition(glm::vec3(5.0f, -4.0f, 0.0f));
	this->objects_[1]->setRotation(-90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	this->objects_[1]->setScale(glm::vec3(0.04f));

	Material material;
	material.setDiffuseColor(110, 16, 7);
	material.setSpecularColor(156, 80, 73);

	this->objects_[1]->setMaterial(material);

	std::shared_ptr<Model> pointLight(new Model("resources/3dmodels/pointLight.obj"));
	std::shared_ptr<Shader> lightShader(new Shader("resources/shaders/lightShader.vs", "resources/shaders/lightShader.fs"));

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
void TestScene::render(Shader shader, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	for (size_t i = 0; i < this->objects_.size(); i++)
	{
		renderer->drawObject(this->objects_[i], shader, this->lights_, view_matrix, camera_position);
		this->objects_[i]->rotate(90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}	

	if (this->drawLights_)
	{
		for (size_t i = 0; i < this->lights_.size(); i++)
		{
			renderer->drawPointLight(this->lights_[i], this->lights_[i]->getShader(), view_matrix, camera_position);
		}
	}

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