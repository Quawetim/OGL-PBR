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
    ///<param name ='node'>���� assimp.</param>
    ///<param name ='scene'>����� assimp.</param>
    void handleNode(aiNode *node, const aiScene *scene);

    ///<summary>��������� ���� ������.</summary>
    ///<param name ='mesh'>��� assimp.</param>
    ///<param name ='scene'>����� assimp.</param>
    Mesh handleMesh(aiMesh *mesh, const aiScene *scene);

    ///<summary>�������� ������� ������.</summary>
    ///<param name ='material'>�������� assimp.</param>
    ///<param name ='type'>��� �������� assimp.</param>
    ///<param name ='textureType'>��� �������� � �������.</param>
    std::vector<QTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, QTextureType textureType);

public:
    ///<summary>�����������.</summary>
    ///<param name ='path'>���� � ������.</param>
    Model(std::string path);

    ///<summary>��������� ������.</summary>
    ///<param name ='shader'>������.</param>
    void drawModel(Shader shader);

    ///<summary>����� ambient ���� ���� ����� ������ � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� diffuse ���� ���� ����� ������ � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� specular ���� ���� ����� ������ � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� ���� (�������) ����� ���� ����� ������.</summary>
    ///<param name = 'value'>��������.</param>
    void setShinePower(const float value);

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
    void setTestTexture(QTexture texture);
};