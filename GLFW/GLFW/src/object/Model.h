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
    std::vector<std::shared_ptr<Mesh>> meshes_;
 
	///<summary>��� ����������� � ������ ��������.</summary>
    std::vector<std::shared_ptr<Texture>> loadedTextures_;

    ///<summary>��������� ���� ������.</summary>
    ///<param name = 'node'>���� assimp.</param>
    ///<param name = 'scene'>����� assimp.</param>
    void handleNode(const aiNode *node, const aiScene *scene);

    ///<summary>��������� ���� ������.</summary>
    ///<param name = 'mesh'>��� assimp.</param>
    ///<param name = 'scene'>����� assimp.</param>
	std::shared_ptr<Mesh> handleMesh(const aiMesh *mesh, const aiScene *scene);

    ///<summary>�������� ������� ������.</summary>
    ///<param name = 'material'>�������� assimp.</param>
    ///<param name = 'type'>��� �������� assimp.</param>
    ///<param name = 'texture_type'>��� �������� � �������.</param>
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const TextureType texture_type);

	///<summary>��������� ����������� � ������������� ��� ������ �� ������.</summary>
	///<param name = 'vertices'>�������.</param>
	void computeTangentsBitangents(std::vector<VertexData> &vertices);

public:
	///<summary>�����������.</summary>
	Model();

    ///<summary>�����������.</summary>
    ///<param name = 'path'>���� � ������.</param>
    Model(std::string path);

	///<summary>���������� ����, �� ������� ������� ������.</summary>
	const std::vector<std::shared_ptr<Mesh>> getMeshes() const;

	///<summary>
	///<para>���������� ��� �� �����.</para>
	///<para>���� �� ������, ���������� ������ ���.</para>
	///</summary>
	///<param name = 'name'>��� ����.</param>
	const std::shared_ptr<Mesh> getMeshByName(std::string name) const;
};