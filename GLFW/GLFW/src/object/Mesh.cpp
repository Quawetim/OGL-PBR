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

    // Push normals to layout 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, normal));

    // Push texture coordinates to layout 2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, textureCoords));
    
    // Push tangent to layout 3
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, tangent));

    // Push bitangent to layout 4
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(QVertexData), (void*)offsetof(QVertexData, bitangent));

    glBindVertexArray(0);
}

///<summary>Отрисовка меша.</summary>
///<param name = 'shader'>Шейдер.</param>
void Mesh::draw(const Shader shader)
{
    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;
    unsigned int normalNumber = 1;

    std::string number, name;

    // Push material params
    shader.setVec3("material.ambientColor", this->ambientColor);
    shader.setVec3("material.diffuseColor", this->diffuseColor);
    shader.setVec3("material.specularColor", this->specularColor);
    shader.setFloat("material.shinePower", this->shinePower);

    // Push texture flags
    shader.setBool("diffuseMap1_flag", false);
    shader.setBool("specularMap1_flag", false);
    shader.setBool("normalMap1_flag", false);

    // Push textures, i = texture unit
    for (size_t i = 0; i < textures.size(); i++)
    {
        if (textures[i].type == QTextureType::diffuse && !this->use_diffuse_map_flag) continue;
        if (textures[i].type == QTextureType::specular && !this->use_specular_map_flag) continue;
        if (textures[i].type == QTextureType::normal && !this->use_normal_map_flag) continue;

        glActiveTexture(GL_TEXTURE0 + i);

        name = mapTextureType.find(textures[i].type)->second;

        switch (textures[i].type)
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
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    shader.activate();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

///<summary>Отрисовка меша с заданными цветами.</summary>
///<param name = 'shader'>Шейдер.</param>
///<param name = 'ambientColor'>Ambient цвет.</param>
///<param name = 'diffuseColor'>Diffuse цвет.</param>
///<param name = 'specularColor'>Specular цвет.</param>
///<param name = 'shinePower'>Сила (яркость) блика.</param>
void Mesh::draw(const Shader shader, const glm::vec3 ambientColor, const glm::vec3 diffuseColor, const glm::vec3 specularColor, const float shinePower)
{
	unsigned int diffuseNumber = 1;
	unsigned int specularNumber = 1;
	unsigned int normalNumber = 1;

	std::string number, name;

	// Push material params
	shader.setVec3("material.ambientColor", ambientColor);
	shader.setVec3("material.diffuseColor", diffuseColor);
	shader.setVec3("material.specularColor", specularColor);
	shader.setFloat("material.shinePower", shinePower);

	// Push texture flags
	shader.setBool("diffuseMap1_flag", false);
	shader.setBool("specularMap1_flag", false);
	shader.setBool("normalMap1_flag", false);

	// Push textures, i = texture unit
	for (size_t i = 0; i < textures.size(); i++)
	{
		if (textures[i].type == QTextureType::diffuse && !this->use_diffuse_map_flag) continue;
		if (textures[i].type == QTextureType::specular && !this->use_specular_map_flag) continue;
		if (textures[i].type == QTextureType::normal && !this->use_normal_map_flag) continue;

		glActiveTexture(GL_TEXTURE0 + i);

		name = mapTextureType.find(textures[i].type)->second;

		switch (textures[i].type)
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
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	shader.activate();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

///<summary>Задаёт ambient цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Mesh::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->ambientColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Mesh::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->diffuseColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт specular цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Mesh::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->specularColor = glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
}

///<summary>Задаёт силу (яркость) блика.</summary>
///<param name = 'value'>Значение.</param>
void Mesh::setShinePower(const float value)
{
    this->shinePower = value;
}

///<summary>Задаёт флаг использования текстуры меша.</summary>
///<param name = 'type'>Тип текстуры.</param>
///<param name = 'use'>Использовать текстуру или нет.</param>
void Mesh::setTextureFlag(const QTextureType type, const bool use)
{
    switch (type)
    {
        case QTextureType::diffuse:     this->use_diffuse_map_flag = use; break;
        case QTextureType::specular:    this->use_specular_map_flag = use; break;
        case QTextureType::normal:      this->use_normal_map_flag = use; break;
    }
}

///<summary>Задаёт мешу тестовую текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void Mesh::setTestTexture(const QTexture texture)
{
    this->textures.push_back(texture);
}