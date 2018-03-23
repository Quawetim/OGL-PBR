#include "TestScene.h"

///<summary>����������.</summary>
TestScene::~TestScene()
{

}

///<summary>���������� �������� ��� �����. �������� � ����������� ��������.</summary>
///<param name = 'models'>������ �������.</param>
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

	unsigned int ind;
	ind = 0;
	indices.push_back(ind);
	ind = 1;
	indices.push_back(ind);
	ind = 2;
	indices.push_back(ind);
	ind = 0;
	indices.push_back(ind);
	ind = 2;
	indices.push_back(ind);
	ind = 3;
	indices.push_back(ind);

	rectangle = Mesh("rectangle", vertices, indices, textures);
	rectangle.setPosition(glm::vec3(0.0f, -8.0f, 0.0f));
	rectangle.setRotation(45.0, glm::vec3(0.0f, 0.0f, 1.0f));
	rectangle.setScale(glm::vec3(0.5f));
	
	Nanosuit = Object("nanosuit", models[1]);
	Nanosuit.addModel(models[0]);
	Nanosuit.setPosition(glm::vec3(0.0f, -8.0f, 0.0f));
	Nanosuit.setRotation(30.0, glm::vec3(0.0f, 1.0f, 0.0f));
	Nanosuit.setScale(glm::vec3(0.5f));

	QTexture testTexture;
	testTexture.type = QTextureType::diffuse;
	testTexture.path = "resources/textures/test.png";
	testTexture.id = TextureLoader::loadTexture(testTexture.path);

	//models[0]->setTestTexture(testTexture);
	//models[0]->setTextureFlag(QTextureType::diffuse, true);

	//models[1]->setTestTexture(testTexture);
}

///<summary>��������� �����.</summary>
///<param name = 'shader'></param>
void TestScene::render(const Shader shader, const glm::mat4 P, const glm::mat4 V)
{
	shader.setProjectionViewModelMatrices(P, V, rectangle.getModelMatrix());
	rectangle.draw(shader);
	rectangle.move(0.0f, 1.0f, 0.0f);
	rectangle.rotate(45.0, glm::vec3(0.0f, 0.0f, 1.0f));
	rectangle.scale(0.1f);

	shader.setProjectionViewModelMatrices(P, V, Nanosuit.getModelMatrix());
	Nanosuit.draw(shader);
	Nanosuit.move(0.0f, 0.2f, 0.0f);
	Nanosuit.rotate(90.0, glm::vec3(0.0f, 1.0f, 0.0f));
	Nanosuit.scale(0.01f);
}