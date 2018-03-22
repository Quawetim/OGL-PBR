#pragma once
#include "..\includes\Includes.h"

///<summary>Простейший 3D-объект не содержит в себе никаких подобъектов.</summary>
class Mesh
{
	friend class Model;
private: 
	///<summary>Vertex array object.</summary>
    unsigned int VAO;
 
	///<summary>Vertex buffer object.</summary>
    unsigned int VBO;

	///<summary>Element buffer object.</summary>
    unsigned int EBO;
 
	///<summary>Имя меша.</summary>
    std::string meshName;

	///<summary>Вершины меша.</summary>
    std::vector<QVertexData> vertices;
 
	///<summary>Индексы вершин.</summary>
    std::vector<unsigned int> indices;
 
	///<summary>Текстуры.</summary>
    std::vector<QTexture> textures;
     
	///<summary>Матрица перемещенния.</summary>
    glm::mat4 translationMatrix;
 
	///<summary>Матрица вращения.</summary>
    glm::mat4 rotationMatrix;
 
	///<summary>Матрица размера.</summary>
    glm::mat4 scaleMatrix;
 
	///<summary>Позиция меша.</summary>
    glm::vec3 position;

	///<summary>Угол поворота.</summary>
    double rotationAngle;
 
	///<summary>Ось поворота.</summary>
    glm::vec3 rotationAxis;
 
	///<summary>Коэффициент размера по каждой из осей.</summary>
    glm::vec3 scale;
 
	///<summary>Флаг отрисовки diffuseMap.</summary>
    bool diffuseMap_flag = true;

	///<summary>Флаг отрисовки specularMap.</summary>
    bool specularMap_flag = true;
 
	///<summary>Флаг отрисовки normalMap.</summary>
    bool normalMap_flag = true;
 
	///<summary>Ambient цвет.</summary>
    glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
 
	///<summary>Diffuse цвет.</summary>
    glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
 
	///<summary>Specular цвет.</summary>
    glm::vec3 specularColor = glm::vec3(0.9f, 0.9f, 0.9f);
 
	///<summary>Сила (яркость) блика.</summary>
    float shinePower = 8.0f;

	///<summary>Отрисовка меша.</summary>
	///<param name = 'shader'>Шейдер.</param>
	void drawMesh(const Shader shader);

	///<summary>Двигает меш в направлении оси с заданной скоростью.</summary>
	///<param name = 'velocityX'>Скорость по оси x.</param>
	///<param name = 'velocityY'>Скорость по оси y.</param>
	///<param name = 'velocityZ'>Скорость по оси z.</param>
	void moveMesh(const float velocityX, const float velocityY, const float velocityZ);

	///<summary>Вращает меш с заданной скоростью.</summary>
	///<param name = 'angle'>Скорость поворота в градусах.</param>
	///<param name = 'axis'>Ось вращения.</param>
	void rotateMesh(const double angle, const glm::vec3 axis);

	///<summary>Изменяет размер меша с заданной скоростью.</summary>
	///<param name = 'scaleXYZ'>Скорость изменения размера по всем осям.</param>
	void scaleMesh(const float scaleXYZ);

	///<summary>Изменяет размер меша с заданной скоростью.</summary>
	///<param name = 'scaleX'>Скорость изменения размера по X.</param>
	///<param name = 'scaleY'>Скорость изменения размера по Y.</param>
	///<param name = 'scaleZ'>Скорость изменения размера по Z.</param>
	void scaleMesh(const float scaleX, const float scaleY, const float scaleZ);

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'name'>Имя меша.</param>
    ///<param name = 'vertices'>Вершины меша.</param>
    ///<param name = 'indices'>Индексы вершин.</param>
    ///<param name = 'textures'>Текстуры.</param>
    Mesh(std::string name, std::vector<QVertexData> vertices, std::vector<unsigned int> indices, std::vector<QTexture> textures);

    ///<summary>Задаёт ambient цвет меша в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт diffuse цвет меша в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт specular цвет меша в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт силу (яркость) блика.</summary>
    ///<param name = 'value'>Значение.</param>
    void setShinePower(const float value);

    ///<summary>Задаёт флаг использования текстуры меша.</summary>
    ///<param name = 'type'>Тип текстуры.</param>
    ///<param name = 'use'>Использовать текстуру или нет.</param>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>Задаёт мешу тестовую текстуру.</summary>
    ///<param name = 'texture'>Текстура.</param>
    void setTestTexture(const QTexture texture);

    ///<summary>Задаёт позицию меша.</summary>
    ///<param name = 'position'>Позиция.</param>
    void setPosition(const glm::vec3 position);

    ///<summary>Задаёт поворот меша.</summary>
    ///<param name = 'angle'>Угол поворота в градусах.</param>
    ///<param name = 'axis'>Ось поворота.</param>
    void setRotation(const double angle, const glm::vec3 axis);

    ///<summary>Задаёт размер меша от исходного.</summary>
    ///<param name = 'scale'>Коэффициент размера.</param>
    void setScale(const glm::vec3 scale);

    ///<summary>Возвращает имя меша.</summary>
    std::string getName() const;

    ///<summary>Возвращает матрицу модели.</summary>
    glm::mat4 getModelMatrix() const;

    ///<summary>Возвращает позицию меша.</summary>
    glm::vec3 getPosition() const;
};