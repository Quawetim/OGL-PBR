#include "Mesh.h"

///<summary>�����������.</summary>
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
}

///<summary>�����������.</summary>
///<param name = 'name'>��� ����.</param>
///<param name = 'vertices'>������� ����.</param>
///<param name = 'indices'>������� ������.</param>
///<param name = 'textures'>��������.</param>
Mesh::Mesh(std::string name, std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
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

    glGenVertexArrays(1, &this->VAO_);
    glGenBuffers(1, &this->VBO_);
    glGenBuffers(1, &this->EBO_);

    glBindVertexArray(this->VAO_);

    // Setup VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);

    // Setup EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Push positions to layout 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glEnableVertexAttribArray(0);

    // Push normals to layout 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	glEnableVertexAttribArray(1);

    // Push texture coordinates to layout 2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, textureCoords));
	glEnableVertexAttribArray(2);
    
    // Push tangent to layout 3
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));
	glEnableVertexAttribArray(3);

    // Push bitangent to layout 4
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, bitangent));
	glEnableVertexAttribArray(4);

    glBindVertexArray(0);
}

///<summary>����������.</summary>
Mesh::~Mesh()
{
	for (size_t i = 0; i < this->textures_.size(); i++)
	{
		//textureLoader::deleteTexture(this->textures_[i].getID());
	}
}

///<summary>���������� ����� VAO.</summary>
unsigned int Mesh::getVAO() const
{
	return this->VAO_;
}

///<summary>���������� ������ ������� �������� ������.</summary>
unsigned int Mesh::getIndicesSize() const
{
	return this->indices_.size();
}

///<summary>���������� ��������, ����������� � ����.</summary>
const std::vector<Texture>& Mesh::getTextures() const
{
	return this->textures_;
}