#pragma once
#include "..\includes\Includes.h"
#include "Mesh.h"

class Model
{
private:
    // ����, �� ������� ������� ������.
    std::vector<Mesh> meshes;

    // ��� ����������� � ������ ��������
    std::vector<QTexture> loaded_textures;

    // ���������� � ������� ����� ������
    std::string dir;

    ///<summary>��������� ���� ������.</summary>
    ///<para name ='node'>���� assimp.</para>
    ///<para name ='scene'>����� assimp.</para>
    void handleNode(aiNode *node, const aiScene *scene);

    ///<summary>��������� ���� ������.</summary>
    ///<para name ='mesh'>��� assimp.</para>
    ///<para name ='scene'>����� assimp.</para>
    Mesh handleMesh(aiMesh *mesh, const aiScene *scene);

    ///<summary>�������� ������� ������.</summary>
    ///<para name ='material'>�������� assimp.</para>
    ///<para name ='type'>��� �������� assimp.</para>
    ///<para name ='textureType'>��� �������� � �������.</para>
    std::vector<QTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, QTextureType textureType);

public:
    ///<summary>�����������.</summary>
    ///<para name ='path'>���� � ������.</para>
    Model(std::string path);

    ///<summary>��������� ������.</summary>
    ///<para name ='shader'>������.</para>
    void drawModel(Shader shader);

    ///<summary>����� ambient ���� ���� ����� ������ � RGB �������.</summary>
    ///<para name = 'red'>������� ���������� �����.</para>
    ///<para name = 'green'>������ ���������� �����.</para>
    ///<para name = 'blue'>����� ���������� �����.</para>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� diffuse ���� ���� ����� ������ � RGB �������.</summary>
    ///<para name = 'red'>������� ���������� �����.</para>
    ///<para name = 'green'>������ ���������� �����.</para>
    ///<para name = 'blue'>����� ���������� �����.</para>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� specular ���� ���� ����� ������ � RGB �������.</summary>
    ///<para name = 'red'>������� ���������� �����.</para>
    ///<para name = 'green'>������ ���������� �����.</para>
    ///<para name = 'blue'>����� ���������� �����.</para>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� ���� (�������) ����� ���� ����� ������.</summary>
    ///<para name = 'value'>��������.</para>
    void setShinePower(const float value);

    ///<summary>����� ���� ������������� �������� ���� name.</summary>
    ///<para name = 'mesh_name'>��� ����.</para>
    ///<para name = 'texture_type'>��� �������� �����.</para>
    ///<para name = 'use'>������������ �������� ��� ���.</para>
    void setTextureFlag(const std::string mesh_name, const QTextureType texture_type, const bool use);

    ///<summary>����� ���� ������������� �������� ���� ����� ������.</summary>
    ///<para name = 'type'>��� ��������.</para>
    ///<para name = 'use'>������������ �������� ��� ���.</para>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>����� ���� ����� �������� ��������.</summary>
    ///<para name = 'texture'>��������.</para>
    void setTestTexture(QTexture texture);
};