#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"
#include "Mesh.h"

///<summary>Сложный 3D-объект, состоящий из нескольких простых. Контейнер мешей.</summary>
class Model : public IEntity
{
	friend class Object;
private:
	///<summary>Директория в которой лежит модель.</summary>
    std::string dir;

	///<summary>Меши, из который состоит модель.</summary>
    std::vector<Mesh> meshes;
 
	///<summary>Уже загруженные в память текстуры.</summary>
    std::vector<QTexture> loaded_textures;

	///<summary>Использовать цвет этой модели.<./summary>
	bool model_colors_flag = false;

    ///<summary>Обработка узла модели.</summary>
    ///<param name = 'node'>Узел assimp.</param>
    ///<param name = 'scene'>Сцена assimp.</param>
    void handleNode(const aiNode *node, const aiScene *scene);

    ///<summary>Обработка меша модели.</summary>
    ///<param name = 'mesh'>Меш assimp.</param>
    ///<param name = 'scene'>Сцена assimp.</param>
    Mesh handleMesh(const aiMesh *mesh, const aiScene *scene);

    ///<summary>Загрузка текстур модели.</summary>
    ///<param name = 'material'>Материал assimp.</param>
    ///<param name = 'type'>Тип текстуры assimp.</param>
    ///<param name = 'textureType'>Тип текстуры в шейдере.</param>
    std::vector<QTexture> loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const QTextureType textureType);	

public:
	///<summary>Конструктор.</summary>
	Model() {};

    ///<summary>Конструктор.</summary>
    ///<param name = 'path'>Путь к модели.</param>
    Model(std::string path);

	///<summary>Отрисовка объекта.
	///<para>Если задан флаг "model_colors_flag", то все меши рисуются с цветом модели, </para>
	///<para>иначе - с заданным цветом меша.</para>
	///</summary>
	///<param name = 'shader'>Шейдер.</param>
	void draw(const Shader shader);

	///<summary>Отрисовка модели с заданными цветами.</summary>
	///<param name = 'shader'>Шейдер.</param>
	///<param name = 'ambientColor'>Ambient цвет.</param>
	///<param name = 'diffuseColor'>Diffuse цвет.</param>
	///<param name = 'specularColor'>Specular цвет.</param>
	///<param name = 'shinePower'>Сила (яркость) блика.</param>
	void draw(const Shader shader, const glm::vec3 ambientColor, const glm::vec3 diffuseColor, const glm::vec3 specularColor, const float shinePower);

	///<summary>Задаёт флаг использования цвета этой модели для всех мешей, 
	///<para>принадлежащих модели.</para>
	///<para>Приоритет ниже флага текстур и ниже флага объекта.</para>
	///</summary>
	void useModelColors();

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

	///<summary>Возвращает флаг использования цвета этой модели для всех мешей, 
	///<para>принадлежащих модели.</para>
	///</summary>
	bool getModelColorsFlag() const;
};