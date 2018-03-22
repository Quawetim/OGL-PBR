#include "Mesh.h"

///<summary>Конструктор.</summary>
///<param name = 'name'>Имя меша.</param>
///<param name = 'vertices'>Вершины меша.</param>
///<param name = 'indices'>Индексы вершин.</param>
///<param name = 'textures'>Текстуры.</param>
Mesh::Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures)
{
    this->meshName = name;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->translationMatrix = glm::mat4(1.0f);
    this->rotationMatrix = glm::mat4(1.0f);
    this->scaleMatrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
    this->rotationAngle = 0.0f;
    this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->scale = glm::vec3(1.0f);

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
void Mesh::drawMesh(const Shader shader)
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

///<summary>Двигает меш в направлении оси с заданной скоростью.</summary>
///<param name = 'velocityX'>Скорость по оси x.</param>
///<param name = 'velocityY'>Скорость по оси y.</param>
///<param name = 'velocityZ'>Скорость по оси z.</param>
void Mesh::moveMesh(const float velocityX, const float velocityY, const float velocityZ)
{
    this->position += glm::vec3(velocityX * deltaTime, velocityY * deltaTime, velocityZ * deltaTime);
    this->translationMatrix = glm::translate(this->position);
}

///<summary>Вращает меш с заданной скоростью.</summary>
///<param name = 'angle'>Скорость поворота в градусах.</param>
///<param name = 'axis'>Ось вращения.</param>
void Mesh::rotateMesh(const double angle, const glm::vec3 axis)
{
    this->rotationAngle += angle * deltaTime;
    this->rotationAxis = axis;

    if (this->rotationAngle == 360) this->rotationAngle = 0;
    else
    {
        if (this->rotationAngle > 360) this->rotationAngle -= 360;
    }

    this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>Изменяет размер меша с заданной скоростью.</summary>
///<param name = 'scaleXYZ'>Скорость изменения размера по всем осям.</param>
void Mesh::scaleMesh(const float scaleXYZ)
{
    this->scale += glm::vec3(scaleXYZ * (float)deltaTime);

    if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Mesh::scaleMesh", QErrorType::error, "Scale = 0");
    else
    {
        if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Mesh::scaleMesh", QErrorType::warning, "Scale < 0");
    }

    this->scaleMatrix = glm::scale(this->scale);
}

///<summary>Изменяет размер меша с заданной скоростью.</summary>
///<param name = 'scaleX'>Скорость изменения размера по X.</param>
///<param name = 'scaleY'>Скорость изменения размера по Y.</param>
///<param name = 'scaleZ'>Скорость изменения размера по Z.</param>
void Mesh::scaleMesh(const float scaleX, const float scaleY, const float scaleZ)
{
    this->scale += glm::vec3(scaleX * deltaTime, scaleY * deltaTime, scaleZ * deltaTime);

    if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Mesh::scaleMesh", QErrorType::error, "Scale = 0");
    else
    {
        if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Mesh::scaleMesh", QErrorType::warning, "Scale < 0");
    }

    this->scaleMatrix = glm::scale(this->scale);
}

///<summary>Задаёт ambient цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Mesh::setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->ambientColor = glm::vec3(red / 255, green / 255, blue / 255);
}

///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Mesh::setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->diffuseColor = glm::vec3(red/255, green/255, blue/255);
}

///<summary>Задаёт specular цвет меша в RGB формате.</summary>
///<param name = 'red'>Красная компонента цвета.</param>
///<param name = 'green'>Зелёная компонента цвета.</param>
///<param name = 'blue'>Синяя компонента цвета.</param>
void Mesh::setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    this->specularColor = glm::vec3(red / 255, green / 255, blue / 255);
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
        case QTextureType::diffuse:     this->diffuseMap_flag = use; break;
        case QTextureType::specular:    this->specularMap_flag = use; break;
        case QTextureType::normal:      this->normalMap_flag = use; break;
    }
}

///<summary>Задаёт мешу тестовую текстуру.</summary>
///<param name = 'texture'>Текстура.</param>
void Mesh::setTestTexture(const QTexture texture)
{
    this->textures.push_back(texture);
}

///<summary>Задаёт позицию меша.</summary>
///<param name = 'position'>Позиция.</param>
void Mesh::setPosition(const glm::vec3 position)
{
    this->position = position;
    this->translationMatrix = glm::translate(this->position);
}

///<summary>Задаёт поворот меша.</summary>
///<param name = 'angle'>Угол поворота в градусах.</param>
///<param name = 'axis'>Ось поворота.</param>
void Mesh::setRotation(const double angle, const glm::vec3 axis)
{
    this->rotationAngle = angle;
    this->rotationAxis = axis;

    if (this->rotationAngle == 360) this->rotationAngle = 0;
    else
    {
        if (this->rotationAngle > 360) this->rotationAngle -= 360;
    }

    this->rotationMatrix = glm::rotate((float)glm::radians(this->rotationAngle), this->rotationAxis);
}

///<summary>Задаёт размер меша от исходного.</summary>
///<param name = 'scale'>Коэффициент размера.</param>
void Mesh::setScale(const glm::vec3 scale)
{
    this->scale = scale;

    if (this->scale.x == 0 || this->scale.y == 0 || this->scale.z == 0) logger.log("Mesh::setScale", QErrorType::error, "Scale = 0");
    else
    {
        if (this->scale.x < 0 || this->scale.y < 0 || this->scale.z < 0) logger.log("Mesh::setScale", QErrorType::warning, "Scale < 0");
    }

    this->scaleMatrix = glm::scale(this->scale);
}

///<summary>Возвращает имя меша.</summary>
std::string Mesh::getName() const
{
    return this->meshName;
}

///<summary>Возвращает матрицу модели.</summary>
glm::mat4 Mesh::getModelMatrix()const
{
    return this->translationMatrix * this->rotationMatrix * this->scaleMatrix;
}

///<summary>Возвращает позицию меша.</summary>
glm::vec3 Mesh::getPosition() const
{
    return this->position;
}