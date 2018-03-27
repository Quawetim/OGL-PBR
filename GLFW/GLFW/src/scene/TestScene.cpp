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
	vertices.push_back(v);
	v.position = glm::vec3(1.0f, -1.0f, 0.0f);
	vertices.push_back(v);
	v.position = glm::vec3(1.0f, 1.0f, 0.0f);
	vertices.push_back(v);
	v.position = glm::vec3(-1.0f, 1.0f, 0.0f);
	vertices.push_back(v);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	this->rectangle_ = Mesh("rectangle", vertices, indices, textures);
	this->rectangle_.setPosition(glm::vec3(0.0f, -8.0f, 0.0f));
	this->rectangle_.setRotation(45.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->rectangle_.setScale(glm::vec3(0.5f));
	
	this->nanosuit_ = Object("nanosuit", models[1]);
	this->nanosuit_.addModel(models[0]);
	this->nanosuit_.setPosition(glm::vec3(0.0f, -8.0f, 0.0f));
	this->nanosuit_.setRotation(30.0, glm::vec3(0.0f, 1.0f, 0.0f));
	this->nanosuit_.setScale(glm::vec3(0.5f));

	QTexture testTexture("resources/textures/test.png", QTextureType::diffuse);

	models[0]->useTestTexture(testTexture);
	//models[1]->useTestTexture(testTexture);
}

///<summary>Отрисовка сцены.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'projection_matrix'>Матрица проекции.</param>
///<param name = 'view_matrix'>Матрица вида.</param>
void TestScene::render(const Shader shader, const glm::mat4 projection_matrix, const glm::mat4 view_matrix)
{
	QMaterial material;

	shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->rectangle_.getModelMatrix());
	this->rectangle_.draw(shader, material);
	this->rectangle_.move(0.0f, 1.0f, 0.0f);
	this->rectangle_.rotate(45.0, glm::vec3(0.0f, 0.0f, 1.0f));
	this->rectangle_.scale(0.1f);

	shader.setProjectionViewModelMatrices(projection_matrix, view_matrix, this->nanosuit_.getModelMatrix());
	this->nanosuit_.draw(shader);
	this->nanosuit_.move(0.0f, 0.2f, 0.0f);
	this->nanosuit_.rotate(90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	this->nanosuit_.scale(0.01f);
}