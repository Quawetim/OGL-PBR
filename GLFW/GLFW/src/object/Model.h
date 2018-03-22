#pragma once
#include "..\includes\Includes.h"
#include "Mesh.h"

///<summary>Сложный 3D-объект, состоящий из нескольких простых. Контейнер мешей.</summary>
class Model
{
	friend class Object;
private: 
	///<summary>Имя модели.</summary>
    std::string modelName;
 
	///<summary>Директория в которой лежит модель.</summary>
    std::string dir;

	///<summary>Меши, из который состоит модель.</summary>
    std::vector<Mesh> meshes;
 
	///<summary>Уже загруженные в память текстуры.</summary>
    std::vector<QTexture> loaded_textures;    
 
	///<summary>Матрица перемещенния.</summary>
    glm::mat4 translationMatrix;
 
	///<summary>Матрица вращения.</summary>
    glm::mat4 rotationMatrix;

	///<summary>Матрица размера.</summary>
    glm::mat4 scaleMatrix;
 
	///<summary>Позиция модели.</summary>
    glm::vec3 position;
 
	///<summary>Угол поворота.</summary>
    double rotationAngle;
 
	///<summary>Ось поворота.</summary>
    glm::vec3 rotationAxis;
 
	///<summary>Коэффициент размера по каждой из осей.</summary>
    glm::vec3 scale;

    ///<summary>Обработка узла модели.</summary>
    ///<param name ='node'>Узел assimp.</param>
    ///<param name ='scene'>Сцена assimp.</param>
    void handleNode(const aiNode *node, const aiScene *scene);

    ///<summary>Обработка меша модели.</summary>
    ///<param name ='mesh'>Меш assimp.</param>
    ///<param name ='scene'>Сцена assimp.</param>
    Mesh handleMesh(const aiMesh *mesh, const aiScene *scene);

    ///<summary>Загрузка текстур модели.</summary>
    ///<param name ='material'>Материал assimp.</param>
    ///<param name ='type'>Тип текстуры assimp.</param>
    ///<param name ='textureType'>Тип текстуры в шейдере.</param>
    std::vector<QTexture> loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const QTextureType textureType);	

	///<summary>Отрисовка модели.</summary>
	///<param name ='shader'>Шейдер.</param>
	void drawModel(const Shader shader);

	///<summary>Двигает модель в направлении оси с заданной скоростью.</summary>
	///<param name = 'velocityX'>Скорость по оси x.</param>
	///<param name = 'velocityY'>Скорость по оси y.</param>
	///<param name = 'velocityZ'>Скорость по оси z.</param>
	void moveModel(const float velocityX, const float velocityY, const float velocityZ);

	///<summary>Вращает модель с заданной скоростью.</summary>
	///<param name = 'angle'>Скорость поворота в градусах.</param>
	///<param name = 'axis'>Ось вращения.</param>
	void rotateModel(const double angle, const glm::vec3 axis);

	///<summary>Изменяет размер модели с заданной скоростью.</summary>
	///<param name = 'scaleXYZ'>Скорость изменения размера по всем осям.</param>
	void scaleModel(const float scaleXYZ);

	///<summary>Изменяет размер модели с заданной скоростью.</summary>
	///<param name = 'scaleX'>Скорость изменения размера по X.</param>
	///<param name = 'scaleY'>Скорость изменения размера по Y.</param>
	///<param name = 'scaleZ'>Скорость изменения размера по Z.</param>
	void scaleModel(const float scaleX, const float scaleY, const float scaleZ);

public:
    ///<summary>Конструктор.</summary>
    ///<param name ='path'>Путь к модели.</param>
    Model(std::string path);  

    ///<summary>Задаёт ambient цвет всем мешам модели в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт diffuse цвет всем мешам модели в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт specular цвет всем мешам модели в RGB формате.</summary>
    ///<param name = 'red'>Красная компонента цвета.</param>
    ///<param name = 'green'>Зелёная компонента цвета.</param>
    ///<param name = 'blue'>Синяя компонента цвета.</param>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>Задаёт силу (яркость) блика всем мешам модели.</summary>
    ///<param name = 'value'>Значение.</param>
    void setShinePower(const float value);

    ///<summary>Задаёт флаг использования текстуры меша name.</summary>
    ///<param name = 'mesh_name'>Имя меша.</param>
    ///<param name = 'texture_type'>Тип текстуры флага.</param>
    ///<param name = 'use'>Использовать текстуру или нет.</param>
    void setTextureFlag(const std::string mesh_name, const QTextureType texture_type, const bool use);

    ///<summary>Задаёт флаг использования текстуры всех мешей модели.</summary>
    ///<param name = 'type'>Тип текстуры.</param>
    ///<param name = 'use'>Использовать текстуру или нет.</param>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>Задаёт всем мешам тестовую текстуру.</summary>
    ///<param name = 'texture'>Текстура.</param>
    void setTestTexture(const QTexture texture); 

	///<summary>Задаёт позицию модели.</summary>
	///<param name = 'position'>Позиция.</param>
	void setPosition(const glm::vec3 position);

	///<summary>Задаёт поворот модели.</summary>
	///<param name = 'angle'>Угол поворота в градусах.</param>
	///<param name = 'axis'>Ось поворота.</param>
	void setRotation(const double angle, const glm::vec3 axis);

	///<summary>Задаёт размер модели от исходного.</summary>
	///<param name = 'scale'>Коэффициент размера.</param>
	void setScale(const glm::vec3 scale);

    ///<summary>Возвращает имя модели.</summary>
    std::string getName() const;

    ///<summary>Возвращает матрицу модели.</summary>
    glm::mat4 getModelMatrix() const;

    ///<summary>Возвращает позицию модели.</summary>
    glm::vec3 getPosition() const;
};