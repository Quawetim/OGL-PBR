#pragma once
#include "..\includes\Includes.h"
#include "IEntity.h"
#include "Mesh.h"

///<summary>������� 3D-������, ��������� �� ���������� �������. ��������� �����.</summary>
class Model : public IEntity
{
	friend class Object;
private:
	///<summary>���������� � ������� ����� ������.</summary>
    std::string dir_;

	///<summary>����, �� ������� ������� ������.</summary>
    std::vector<Mesh> meshes_;
 
	///<summary>��� ����������� � ������ ��������.</summary>
    std::vector<QTexture> loadedTextures_;

    ///<summary>��������� ���� ������.</summary>
    ///<param name = 'node'>���� assimp.</param>
    ///<param name = 'scene'>����� assimp.</param>
    void handleNode(const aiNode *node, const aiScene *scene);

    ///<summary>��������� ���� ������.</summary>
    ///<param name = 'mesh'>��� assimp.</param>
    ///<param name = 'scene'>����� assimp.</param>
    Mesh handleMesh(const aiMesh *mesh, const aiScene *scene);

    ///<summary>�������� ������� ������.</summary>
    ///<param name = 'material'>�������� assimp.</param>
    ///<param name = 'type'>��� �������� assimp.</param>
    ///<param name = 'texture_type'>��� �������� � �������.</param>
    std::vector<QTexture> loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const QTextureType texture_type);	

public:
	///<summary>�����������.</summary>
	Model();

    ///<summary>�����������.</summary>
    ///<param name = 'path'>���� � ������.</param>
    Model(std::string path);

	///<summary>��������� ������.</summary>
	///<param name = 'shader'>������.</param>
	void draw(const Shader shader);

	///<summary>��������� ������ � �������� ����������.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'material'>��������.</param>
	void draw(const Shader shader, const QMaterial material);

    ///<summary>����� ���� ������������� �������� ���� name.</summary>
    ///<param name = 'mesh_name'>��� ����.</param>
    ///<param name = 'texture_type'>��� �������� �����.</param>
    ///<param name = 'use'>������������ �������� ��� ���.</param>
    void useTexture(const std::string mesh_name, const QTextureType texture_type, const bool use);

    ///<summary>����� ���� ����� �������� ��������.</summary>
    ///<param name = 'texture'>��������.</param>
    void useTestTexture(const QTexture texture);
};