#include "Scene1.h"

///<summary>Деструктор.</summary>
Scene1::~Scene1()
{
	this->cubes.clear();
	this->spheres.clear();
	this->cylinders.clear();
}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void Scene1::init(std::vector<Model*> models)
{
	glm::vec3 initialSpheresPos = glm::vec3(0.0f, 6.0f, 0.0f);
	glm::vec3 initialCylindersPos = glm::vec3(3.0f, 6.0f, 0.0f);

	for (size_t i = 0; i < 5; i++)
	{
		Object obj;
		std::string name;

		obj.setPosition(glm::vec3(1.0f));
		name = "cube" + std::to_string(i + 1);
		obj = Object(name, models[0]);
		obj.setPosition(glm::vec3(-3.0f, 6.0f - 3 * i, 0.0f));
		this->cubes.push_back(obj);

		name = "sphere" + std::to_string(i + 1);
		obj = Object(name, models[1]);
		obj.setPosition(glm::vec3(0.0f, 6.0f - 3 * i, 0.0f));
		this->spheres.push_back(obj);

		name = "cylinder" + std::to_string(i + 1);
		obj = Object(name, models[2]);
		obj.setPosition(glm::vec3(3.0f, 6.0f - 3 * i, 0.0f));
		this->cylinders.push_back(obj);
	}

	QTexture texture;
	QMaterial material;

	// Кубы
	texture = QTexture("resources/textures/batman_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	material.setDiffuseColor(230, 0, 128);
	cubes[0].setScale(glm::vec3(0.5f));		
	cubes[0].setMaterial(material);
	cubes[0].useObjectMaterial();
	material.reset();

	texture = QTexture("resources/textures/brick_diffuse.bmp", QTextureType::diffuse);
	material.addTexture(texture);
	texture = QTexture("resources/textures/brick_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	cubes[4].setMaterial(material);
	cubes[4].useObjectMaterial();
	material.reset();

	// Сферы
	material.setDiffuseColor(153, 77, 230);
	spheres[0].setMaterial(material);
	spheres[0].useObjectMaterial();
	material.reset();

	texture = QTexture("resources/textures/brick2_diffuse.bmp", QTextureType::diffuse);
	material.addTexture(texture);
	texture = QTexture("resources/textures/brick2_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	spheres[4].setMaterial(material);
	spheres[4].useObjectMaterial();
	material.reset();

	// Цилиндры
	material.setDiffuseColor(26, 230, 204);
	cylinders[0].setMaterial(material);
	cylinders[0].useObjectMaterial();
	material.reset();

	texture = QTexture("resources/textures/brick3_diffuse.bmp", QTextureType::diffuse);
	material.addTexture(texture);
	texture = QTexture("resources/textures/brick3_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	cylinders[4].setMaterial(material);
	cylinders[4].useObjectMaterial();
	material.reset();
}

///<summary>Отрисовка сцены.</summary>
///<param name = 'shader'></param>
void Scene1::render(const Shader shader, const glm::mat4 P, const glm::mat4 V)
{
	// Кубы
	for (size_t i = 0; i < cubes.size(); i++)
	{
		shader.setProjectionViewModelMatrices(P, V, cubes[i].getModelMatrix());
		cubes[i].draw(shader);		
	}

	cubes[0].rotate(-90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	cubes[1].rotate(90.0, glm::vec3(0.0f, 1.0f, 1.0f));
	cubes[2].rotate(-90.0, glm::vec3(0.0f, 0.0f, 1.0f));
	cubes[3].rotate(90.0, glm::vec3(0.0f, 0.0f, 1.0f));
	cubes[4].rotate(-30.0, glm::vec3(0.0f, 1.0f, 0.0f));

	// Сферы
	for (size_t i = 0; i < cubes.size(); i++)
	{
		shader.setProjectionViewModelMatrices(P, V, spheres[i].getModelMatrix());
		spheres[i].draw(shader);

		if (decrease)
		{
			if (spheres[i].getScale().x > 0.8f) spheres[i].scale(-0.2f);
			else decrease = false;
		}
		else
		{
			if (spheres[i].getScale().x < 1.0f) spheres[i].scale(0.2f);
			else decrease = true;
		}
	}

	spheres[4].rotate(-40.0, glm::vec3(1.0f, 1.0f, 1.0f));

	// Цилиндры
	for (size_t i = 0; i < cubes.size(); i++)
	{
		shader.setProjectionViewModelMatrices(P, V, cylinders[i].getModelMatrix());
		cylinders[i].draw(shader);
		cylinders[i].rotate(10.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}