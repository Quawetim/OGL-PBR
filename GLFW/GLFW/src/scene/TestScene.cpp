#include "TestScene.h"

///<summary>Деструктор.</summary>
TestScene::~TestScene()
{

}

///<summary>Подготовка ресурсов для сцены. Создание и расстановка объектов.</summary>
///<param name = 'models'>Список моделей.</param>
void TestScene::init(std::vector<Model*> models)
{
	std::vector<QVertexData> vertices;
	std::vector<unsigned int> indices;
	std::vector<QTexture> textures;

	QVertexData v;
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

	this->rectangle_ = Mesh("rectangle", vertices, indices, textures);
	this->rectangle_.setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
	this->rectangle_.setRotation(45.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->rectangle_.setScale(glm::vec3(0.5f));
	
	this->objects_.push_back(Object("nanosuit", models[0]));
	this->objects_[0].setPosition(glm::vec3(0.0f, -4.0f, 0.0f));
	this->objects_[0].setScale(glm::vec3(0.5f));

	this->objects_.push_back(Object("deadpool", models[1]));
	this->objects_[1].setPosition(glm::vec3(5.0f, -4.0f, 0.0f));
	this->objects_[1].setRotation(-90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	this->objects_[1].setScale(glm::vec3(0.04f));

	QTexture testTexture("resources/textures/test.png", QTextureType::diffuse);

	//models[0]->useTestTexture(testTexture);
	models[1]->useTestTexture(testTexture);
}

///<summary>Отрисовка сцены.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'projection_matrix'>Матрица проекции.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
///<param name = 'camera_position'>Позиция камеры.</param>
void TestScene::render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::vec3 camera_position)
{
	QMaterial material;

	shader.activate();
	shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->rectangle_.getModelMatrix());
	shader.setVec3("cameraPosition", camera_position);

	this->rectangle_.draw(shader, material);
	this->rectangle_.rotate(45.0, glm::vec3(0.0f, 0.0f, 1.0f));

	for (size_t i = 0; i < this->objects_.size(); i++)
	{
		this->objects_[i].draw(shader, projection_matrix, view_matrix, camera_position);
		this->objects_[i].rotate(90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	}	
}