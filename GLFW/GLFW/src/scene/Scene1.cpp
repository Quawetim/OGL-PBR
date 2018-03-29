#include "Scene1.h"

///<summary>Деструктор.</summary>
Scene1::~Scene1()
{
	this->cubes_.clear();
	std::vector<Object>(this->cubes_).swap(this->cubes_);

	this->spheres_.clear();
	std::vector<Object>(this->spheres_).swap(this->spheres_);

	this->cylinders_.clear();
	std::vector<Object>(this->cylinders_).swap(this->cylinders_);
}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void Scene1::init(std::vector<Model*> models)
{
	this->decrease_ = true;

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
		this->cubes_.push_back(obj);

		name = "sphere" + std::to_string(i + 1);
		obj = Object(name, models[1]);
		obj.setPosition(glm::vec3(0.0f, 6.0f - 3 * i, 0.0f));
		this->spheres_.push_back(obj);

		name = "cylinder" + std::to_string(i + 1);
		obj = Object(name, models[2]);
		obj.setPosition(glm::vec3(3.0f, 6.0f - 3 * i, 0.0f));
		this->cylinders_.push_back(obj);
	}

	QTexture texture;
	QMaterial material;

	// Кубы
	texture = QTexture("resources/textures/batman_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	material.setDiffuseColor(230, 0, 128);
	this->cubes_[0].setScale(glm::vec3(0.5f));
	this->cubes_[0].setMaterial(material);
	this->cubes_[0].useObjectMaterial();
	material.reset();

	texture = QTexture("resources/textures/brick_diffuse.bmp", QTextureType::diffuse);
	material.addTexture(texture);
	texture = QTexture("resources/textures/brick_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	this->cubes_[4].setMaterial(material);
	this->cubes_[4].useObjectMaterial();
	material.reset();

	// Сферы
	material.setDiffuseColor(153, 77, 230);
	this->spheres_[0].setMaterial(material);
	this->spheres_[0].useObjectMaterial();
	material.reset();

	texture = QTexture("resources/textures/brick2_diffuse.bmp", QTextureType::diffuse);
	material.addTexture(texture);
	texture = QTexture("resources/textures/brick2_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	this->spheres_[4].setMaterial(material);
	this->spheres_[4].useObjectMaterial();
	material.reset();

	// Цилиндры
	material.setDiffuseColor(26, 230, 204);
	this->cylinders_[0].setMaterial(material);
	this->cylinders_[0].useObjectMaterial();
	material.reset();

	texture = QTexture("resources/textures/brick3_diffuse.bmp", QTextureType::diffuse);
	material.addTexture(texture);
	texture = QTexture("resources/textures/brick3_specular.bmp", QTextureType::specular);
	material.addTexture(texture);
	this->cylinders_[4].setMaterial(material);
	this->cylinders_[4].useObjectMaterial();
	material.reset();
}

///<summary>Отрисовка сцены.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'projection_matrix'>Матрица проекции.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void Scene1::render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	// Кубы
	for (size_t i = 0; i < this->cubes_.size(); i++)
	{
		//shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->cubes_[i].getModelMatrix());
		this->cubes_[i].draw(shader, projection_matrix, view_matrix, camera_position);
	}

	this->cubes_[0].rotate(-90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	this->cubes_[1].rotate(90.0, glm::vec3(0.0f, 1.0f, 1.0f));
	this->cubes_[2].rotate(-90.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->cubes_[3].rotate(90.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->cubes_[4].rotate(-30.0, glm::vec3(0.0f, 1.0f, 0.0f));

	// Сферы
	for (size_t i = 0; i < cubes_.size(); i++)
	{
		//shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->spheres_[i].getModelMatrix());
		this->spheres_[i].draw(shader, projection_matrix, view_matrix, camera_position);

		if (this->decrease_)
		{
			if (this->spheres_[i].getScale().x > 0.8f) this->spheres_[i].scale(-0.2f);
			else this->decrease_ = false;
		}
		else
		{
			if (this->spheres_[i].getScale().x < 1.0f) this->spheres_[i].scale(0.2f);
			else this->decrease_ = true;
		}
	}

	this->spheres_[4].rotate(-40.0, glm::vec3(1.0f, 1.0f, 1.0f));

	// Цилиндры
	for (size_t i = 0; i < this->cubes_.size(); i++)
	{
		//shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->cylinders_[i].getModelMatrix());
		this->cylinders_[i].draw(shader, projection_matrix, view_matrix, camera_position);
		this->cylinders_[i].rotate(10.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}