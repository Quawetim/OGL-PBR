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
    std::string dir;

	///<summary>����, �� ������� ������� ������.</summary>
    std::vector<Mesh> meshes;
 
	///<summary>��� ����������� � ������ ��������.</summary>
    std::vector<QTexture> loaded_textures;

	///<summary>������������ ���� ���� ������.<./summary>
	bool model_colors_flag = false;

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
    ///<param name = 'textureType'>��� �������� � �������.</param>
    std::vector<QTexture> loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const QTextureType textureType);	

public:
	///<summary>�����������.</summary>
	Model() {};

    ///<summary>�����������.</summary>
    ///<param name = 'path'>���� � ������.</param>
    Model(std::string path);

	///<summary>��������� �������.
	///<para>���� ����� ���� "model_colors_flag", �� ��� ���� �������� � ������ ������, </para>
	///<para>����� - � �������� ������ ����.</para>
	///</summary>
	///<param name = 'shader'>������.</param>
	void draw(const Shader shader);

	///<summary>��������� ������ � ��������� �������.</summary>
	///<param name = 'shader'>������.</param>
	///<param name = 'ambientColor'>Ambient ����.</param>
	///<param name = 'diffuseColor'>Diffuse ����.</param>
	///<param name = 'specularColor'>Specular ����.</param>
	///<param name = 'shinePower'>���� (�������) �����.</param>
	void draw(const Shader shader, const glm::vec3 ambientColor, const glm::vec3 diffuseColor, const glm::vec3 specularColor, const float shinePower);

	///<summary>����� ���� ������������� ����� ���� ������ ��� ���� �����, 
	///<para>������������� ������.</para>
	///<para>��������� ���� ����� ������� � ���� ����� �������.</para>
	///</summary>
	void useModelColors();

    ///<summary>����� ���� ������������� �������� ���� name.</summary>
    ///<param name = 'mesh_name'>��� ����.</param>
    ///<param name = 'texture_type'>��� �������� �����.</param>
    ///<param name = 'use'>������������ �������� ��� ���.</param>
    void setTextureFlag(const std::string mesh_name, const QTextureType texture_type, const bool use);

    ///<summary>����� ���� ������������� �������� ���� ����� ������.</summary>
    ///<param name = 'type'>��� ��������.</param>
    ///<param name = 'use'>������������ �������� ��� ���.</param>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>����� ���� ����� �������� ��������.</summary>
    ///<param name = 'texture'>��������.</param>
    void setTestTexture(const QTexture texture);

	///<summary>���������� ���� ������������� ����� ���� ������ ��� ���� �����, 
	///<para>������������� ������.</para>
	///</summary>
	bool getModelColorsFlag() const;
};