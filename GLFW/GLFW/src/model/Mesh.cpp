#include "Mesh.h"

extern Logger logger;

///<summary>Конструктор.</summary>
///<param name = 'vertices'>Вершины меша.</param>
///<param name = 'indices'>Индексы вершин.</param>
///<param name = 'textures'>Текстуры.</param>
Mesh::Mesh(std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures)
{
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

    // Push textures, i = texture unit
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        switch (textures[i].type)
        {
            case QTextureType::diffuse:     
                {
                    number = std::to_string(diffuseNumber++);
                    name = "diffuse";
                    break;
                }
            case QTextureType::specular:    
                {
                    number = std::to_string(specularNumber++);
                    name = "specular";
                    break;
                }
            case QTextureType::normal:     
                {
                    number = std::to_string(normalNumber++);
                    name = "normal";
                    break;
                }
            default:
                {
                    logger.log("drawMesh", QErrorType::error, "Unexpected texture type");
                    logger.stop("drawMesh", true, "Unexpected texture type");
                    exit(Q_ERROR_UNEXPECTED_TEXTURE_TYPE);
                }
        }

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    // Draw mesh

    shader.activate();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}