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
};