#include "Mesh.h"

///<summary>Конструктор.</summary>
Mesh::Mesh()
{
	this->name_ = "not loaded";

	this->translationMatrix_ = glm::mat4(1.0f);
	this->rotationMatrix_ = glm::mat4(1.0f);
	this->scaleMatrix_ = glm::mat4(1.0f);
	this->position_ = glm::vec3(0.0f);
	this->rotationAngle_ = 0.0f;
	this->rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
	this->scaleCoeffs_ = glm::vec3(1.0f);

	this->useDiffuseMaps_ = true;
	this->useSpecularMaps_ = true;
	this->useNormalMaps_ = true;
}

///<summary>Конструктор.</summary>
///<param name = 'name'>Имя меша.</param>
///<param name = 'vertices'>Вершины меша.</param>
///<param name = 'indices'>Индексы вершин.</param>
///<param name = 'textures'>Текстуры.</param>
Mesh::Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures)
{
	this->name_ = name;
    this->vertices_ = vertices;
    this->indices_ = indices;
    this->textures_ = textures;

    this->translationMatrix_ = glm::mat4(1.0f);
    this->rotationMatrix_ = glm::mat4(1.0f);
    this->scaleMatrix_ = glm::mat4(1.0f);
    this->position_ = glm::vec3(0.0f);
    this->rotationAngle_ = 0.0f;
    this->rotationAxis_ = glm::vec3(0.0f, 1.0f, 0.0f);
    this->scaleCoeffs_ = glm::vec3(1.0f);

	this->useDiffuseMaps_ = true;
	this->useSpecularMaps_ = true;
	this->useNormalMaps_ = true;

    glGenVertexArrays(1, &this->VAO_);
    glGenBuffers(1, &this->VBO_);
    glGenBuffers(1, &this->EBO_);

    glBindVertexArray(this->VAO_);

    // Setup VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVertexData), &vertices[0], GL_STATIC_DRAW);

    // Setup EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO_);
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

	shader.activate();
	
	// Push material params
	shader.setVec3("material.ambientColor", material.getAmbientColor());
	shader.setVec3("material.diffuseColor", material.getDiffuseColor());
	shader.setVec3("material.specularColor", material.getSpecularColor());
	shader.setFloat("material.shininess", material.getShininess());

	// Push texture flags
	shader.setBool("diffuseMap1_flag", false);
	shader.setBool("specularMap1_flag", false);
	shader.setBool("normalMap1_flag", false);

	std::vector<QTexture> pointer;

	if (material.isTexturesEmpty()) pointer = textures_;
	else pointer = material.getTextures();

	// Push textures, i = texture unit
	for (size_t i = 0; i < pointer.size(); i++)
	{
		if (pointer[i].getType() == QTextureType::diffuse && !this->useDiffuseMaps_) continue;
		if (pointer[i].getType() == QTextureType::specular && !this->useSpecularMaps_) continue;
		if (pointer[i].getType() == QTextureType::normal && !this->useNormalMaps_) continue;

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

	glBindVertexArray(this->VAO_);
	glDrawElements(GL_TRIANGLES, this->indices_.size(), GL_UNSIGNED_INT, 0);
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
        case QTextureType::diffuse:     this->useDiffuseMaps_ = use; break;
        case QTextureType::specular:    this->useSpecularMaps_ = use; break;
        case QTextureType::normal:      this->useNormalMaps_ = use; break;
    }
}

///<summary>Задаёт мешу тестовую текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void Mesh::useTestTexture(const QTexture texture)
{
	this->textures_.clear();
	std::vector<QTexture>(this->textures_).swap(this->textures_);
	this->textures_.push_back(texture);
}