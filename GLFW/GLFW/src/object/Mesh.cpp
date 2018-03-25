#include "Mesh.h"

///<summary>Конструктор.</summary>
///<param name = 'name'>Имя меша.</param>
///<param name = 'vertices'>Вершины меша.</param>
///<param name = 'indices'>Индексы вершин.</param>
///<param name = 'textures'>Текстуры.</param>
Mesh::Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures)
{
	this->name = name;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->translationMatrix = glm::mat4(1.0f);
    this->rotationMatrix = glm::mat4(1.0f);
    this->scaleMatrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
    this->rotationAngle = 0.0f;
    this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->scaleCoefficients = glm::vec3(1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Setup VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVertexData), &vertices[0], GL_STATIC_DRAW);

    // Setup EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Push positions to layout 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)0);
	glEnableVertexAttribArray(0);

    // Push normals to layout 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, normal));
	glEnableVertexAttribArray(1);

    // Push texture coordinates to layout 2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, textureCoords));
	glEnableVertexAttribArray(2);
    
    // Push tangent to layout 3
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, tangent));
	glEnableVertexAttribArray(3);

    // Push bitangent to layout 4
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, bitangent));
	glEnableVertexAttribArray(4);

    glBindVertexArray(0);
}

///<summary>Отрисовка меша.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'material'>Материал.</param>
void Mesh::draw(const Shader shader, const QMaterial material)
{
	unsigned int diffuseNumber = 1;
	unsigned int specularNumber = 1;
	unsigned int normalNumber = 1;

	std::string number, name;	
	
	// Push material params
	shader.setVec3("material.ambientColor", material.getAmbientColor());
	shader.setVec3("material.diffuseColor", material.getDiffuseColor());
	shader.setVec3("material.specularColor", material.getSpecularColor());
	shader.setFloat("material.shinePower", material.getShinePower());

	// Push texture flags
	shader.setBool("diffuseMap1_flag", false);
	shader.setBool("specularMap1_flag", false);
	shader.setBool("normalMap1_flag", false);

	std::vector<QTexture> pointer;

	if (material.noTextures()) pointer = textures;
	else pointer = material.getTextures();

	// Push textures, i = texture unit
	for (size_t i = 0; i < pointer.size(); i++)
	{
		if (pointer[i].getType() == QTextureType::diffuse && !this->use_diffuse_map) continue;
		if (pointer[i].getType() == QTextureType::specular && !this->use_specular_map) continue;
		if (pointer[i].getType() == QTextureType::normal && !this->use_normal_map) continue;

		glActiveTexture(GL_TEXTURE0 + i);

		name = mapTextureType.find(pointer[i].getType())->second;

		switch (pointer[i].getType())
		{
			case QTextureType::diffuse:     number = std::to_string(diffuseNumber++); break;
			case QTextureType::specular:    number = std::to_string(specularNumber++); break;
			case QTextureType::normal:      number = std::to_string(normalNumber++); break;
			default:
				{
					logger.log("Mesh::drawMesh", QErrorType::error, "Unexpected texture type");
					logger.stop("Mesh::drawMesh", true, "Unexpected texture type");
					exit(Q_ERROR_UNEXPECTED_TEXTURE_TYPE);
				}
		}

		shader.setBool(std::string(name + number + "_flag"), true);
		shader.setInt(std::string(name + number), i);
		glBindTexture(GL_TEXTURE_2D, pointer[i].getID());
	}

	glActiveTexture(GL_TEXTURE0);

	shader.activate();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

///<summary>Задаёт флаг использования текстуры меша.</summary>
///<param name = 'type'>Тип текстуры.</param>
///<param name = 'use'>Использовать текстуру или нет.</param>
void Mesh::useTexture(const QTextureType type, const bool use)
{
    switch (type)
    {
        case QTextureType::diffuse:     this->use_diffuse_map = use; break;
        case QTextureType::specular:    this->use_specular_map = use; break;
        case QTextureType::normal:      this->use_normal_map = use; break;
    }
}

///<summary>Задаёт мешу тестовую текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void Mesh::useTestTexture(const QTexture texture)
{
	this->textures.clear();
	std::vector<QTexture>(this->textures).swap(this->textures);
	this->textures.push_back(texture);
}