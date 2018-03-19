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
void Mesh::drawMesh(Shader shader)
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
        if (textures[i].type == QTextureType::diffuse && !this->diffuseMap_flag) continue;
        if (textures[i].type == QTextureType::specular && !this->specularMap_flag) continue;
        if (textures[i].type == QTextureType::normal && !this->normalMap_flag) continue;

        glActiveTexture(GL_TEXTURE0 + i);

        name = mapTextureType.find(textures[i].type)->second;

        switch (textures[i].type)
        {           
            case QTextureType::diffuse:     number = std::to_string(diffuseNumber++); break;
            case QTextureType::specular:    number = std::to_string(specularNumber++); break;
            case QTextureType::normal:      number = std::to_string(normalNumber++); break;
            default:
                {
                    logger.log("drawMesh", QErrorType::error, "Unexpected texture type");
                    logger.stop("drawMesh", true, "Unexpected texture type");
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
///<para name = 'red'>Красная компонента цвета.</para>
///<para name = 'green'>Зелёная компонента цвета.</para>
///<para name = 'blue'>Синяя компонента цвета.</para>
void Mesh::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->ambientColor = glm::vec3(red / 255, green / 255, blue / 255);
}

///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
///<para name = 'red'>Красная компонента цвета.</para>
///<para name = 'green'>Зелёная компонента цвета.</para>
///<para name = 'blue'>Синяя компонента цвета.</para>
void Mesh::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->diffuseColor = glm::vec3(red/255, green/255, blue/255);
}

///<summary>Задаёт specular цвет меша в RGB формате.</summary>
///<para name = 'red'>Красная компонента цвета.</para>
///<para name = 'green'>Зелёная компонента цвета.</para>
///<para name = 'blue'>Синяя компонента цвета.</para>
void Mesh::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->specularColor = glm::vec3(red / 255, green / 255, blue / 255);
}

///<summary>Задаёт силу (яркость) блика.</summary>
///<para name = 'value'>Значение.</para>
void Mesh::setShinePower(const float value)
{
    this->shinePower = value;
}

///<summary>Задаёт флаг использования текстуры меша.</summary>
///<para name = 'type'>Тип текстуры.</para>
///<para name = 'use'>Использовать текстуру или нет.</para>
void Mesh::setTextureFlag(const QTextureType type, const bool use)
{
    switch (type)
    {
        case QTextureType::diffuse:     this->diffuseMap_flag = use; break;
        case QTextureType::specular:    this->specularMap_flag = use; break;
        case QTextureType::normal:      this->normalMap_flag = use; break;
    }
}

///<summary>Задаёт мешу тестовую текстуру.</summary>
///<para name = 'texture'>Текстура.</para>
void Mesh::setTestTexture(QTexture texture)
{
    this->textures.push_back(texture);
}

///<summary>Возвращает имя меша.</summary>
std::string Mesh::getName() const
{
    return this->name;
}