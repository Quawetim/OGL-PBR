#pragma once
#include "..\includes\Includes.h"
#include "Mesh.h"

///<summary>Сложный 3D-объект, состоящий из нескольких простых. Контейнер мешей.</summary>
class Model : public IEntity
{
	friend class Object;
private:
	///<summary>Директория в которой лежит модель.</summary>
    std::string dir_;

	///<summary>Меши, из который состоит модель.</summary>
    std::vector<Mesh> meshes_;
 
	///<summary>Уже загруженные в память текстуры.</summary>
    std::vector<QTexture> loadedTextures_;

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
    ///<param name = 'texture_type'>Тип текстуры в шейдере.</param>
    std::vector<QTexture> loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const QTextureType texture_type);	

public:
	///<summary>Конструктор.</summary>
	Model();

    ///<summary>Конструктор.</summary>
    ///<param name = 'path'>Путь к модели.</param>
    Model(std::string path);

	///<summary>Возвращает меши, из которых состоит модель.</summary>
	const std::vector<Mesh>& getMeshes() const;
};