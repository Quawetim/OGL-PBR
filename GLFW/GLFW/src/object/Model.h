#pragma once
#include "..\includes\Includes.h"
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

	///<summary>���������� ����, �� ������� ������� ������.</summary>
	const std::vector<Mesh>& getMeshes() const;
};